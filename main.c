/*
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pico-inverter.elf verify reset exit"
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "LCD_1in3.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include "Infrared.h"
uint16_t buff[5];
uint16_t tbuff[5];
int sinfeq = 30;
bool enable =0;
bool stup = 0;
bool bouncec =0;
bool bouncel =0;
bool bouncer =0;
int step =1;
bool reserved_addr(uint8_t addr) {
return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void cre1()
{
    DEV_Delay_ms(100);
    if(DEV_Module_Init()!=0){
        return;
    }
    DEV_SET_PWM(50);
    LCD_1IN3_Init(HORIZONTAL);
    LCD_1IN3_Clear(WHITE);
    UDOUBLE Imagesize = LCD_1IN3_HEIGHT*LCD_1IN3_WIDTH*2;
    UWORD *BlackImage;
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        exit(0);
    }
    Paint_NewImage((UBYTE *)BlackImage,LCD_1IN3.WIDTH,LCD_1IN3.HEIGHT, 180, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_180);
    Paint_Clear(WHITE);
/*
up=14
down=20
left=16
right=21
ctrl=15
*/

    uint8_t up = 20;
	uint8_t down = 14;
	uint8_t left = 21;
	uint8_t right = 16;
	uint8_t ctrl = 15;
	SET_Infrared_PIN(up);
    SET_Infrared_PIN(down);
    SET_Infrared_PIN(left);
    SET_Infrared_PIN(right);
    SET_Infrared_PIN(ctrl);

    Paint_Clear(WHITE);
    while(true)
    {
        Paint_Clear(WHITE);
        Paint_DrawRectangle(60, 60, 91, 90, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(60, 150, 91, 180, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(15, 105, 46, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(105, 105, 136, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(60, 105, 91, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawNum(0,0,sinfeq,&Font20,0, 0x000f, 0xfff0);
        Paint_DrawNum(0,20,step,&Font20,0, 0x000f, 0xfff0);
        if (enable) Paint_DrawRectangle(90, 0, 110, 20, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
        else
        {
            Paint_DrawRectangle(90, 0, 110, 20, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            if(DEV_Digital_Read(up ) == 0){
                Paint_DrawRectangle(60, 60, 90, 90, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                if ((sinfeq+step)<1000) sinfeq=sinfeq+step;
                else sinfeq = 1000;
                DEV_Delay_ms(100);
            }
            if(DEV_Digital_Read(down ) == 0){
                Paint_DrawRectangle(60, 150, 90, 180, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                if ((sinfeq-step)>10) sinfeq=sinfeq-step;
                else sinfeq=10;
                DEV_Delay_ms(100);
            }
            if(DEV_Digital_Read(left ) == 0){
                Paint_DrawRectangle(15, 105, 45, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                if (!bouncel)if (step<100) step=step*10;
                bouncel=1;
            }
            else bouncel=0;
            if(DEV_Digital_Read(right ) == 0){
                Paint_DrawRectangle(105, 105, 135, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                if (!bouncer)if (step>1) step=step/10;
                bouncer=1;
            }
            else bouncer=0;
        }
        if(DEV_Digital_Read(ctrl ) == 0){
            Paint_DrawRectangle(60, 105, 90, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            if (!bouncec)enable = !enable;
            bouncec=1;
        }
        else bouncec=0;
        LCD_1IN3_Display(BlackImage);
    }
}

void buffprepsy()
{
    buff[0]=(0x2000);
}

void buffprepsi(int feq,int pha)
{
    uint32_t binfeq = (int)(round((((2<<27))/(1000000))*(feq)));
    uint16_t binpha = (int)((4096)-(round((((pha)*(0.333333))*(4096)))));
    buff[0]=(0x2100);
    buff[1]=((0x4000)|((binfeq)&(0x3fff)));
    buff[2]=((0x4000)|((binfeq>>14)&(0x3fff)));  
    buff[3]=((0xc000)|((binpha)&(0x0fff)));
}

void wrtodds(int sinfeq)
{
        //program DDS individually
        buffprepsi(sinfeq,0);
        gpio_clr_mask(0x00000002);
        spi_write16_blocking(spi0,buff,4);
        gpio_set_mask(0x00020022);

        buffprepsi(sinfeq,1);
        gpio_clr_mask(0x00000020);
        spi_write16_blocking(spi0,buff,4);
        gpio_set_mask(0x00020022);

        buffprepsi(sinfeq,2);
        gpio_clr_mask(0x00020000);
        spi_write16_blocking(spi0,buff,4);
        gpio_set_mask(0x00020022);
        //enable all DDS at ones to achieve synchronisation
        buffprepsy();
        gpio_clr_mask(0x00020022);
        spi_write16_blocking(spi0,buff,1);
        gpio_set_mask(0x00020022);
}

int main()
{   
    gpio_init(0);
    gpio_set_dir(0,true);
    gpio_set_outover(0,GPIO_OVERRIDE_LOW);

    gpio_init(1);
    gpio_set_dir(1,true);
    gpio_set_function( 2, GPIO_FUNC_SPI);
    gpio_set_function( 3, GPIO_FUNC_SPI);
    gpio_init(5);
    gpio_set_dir(5,true);
    gpio_set_function( 6, GPIO_FUNC_SPI);
    gpio_set_function( 7, GPIO_FUNC_SPI);
    gpio_init(17);
    gpio_set_dir(17,true);
    gpio_set_function(18, GPIO_FUNC_SPI);
    gpio_set_function(19, GPIO_FUNC_SPI);

    gpio_set_mask(0x00020022);

    spi_init(spi0,200000);
    spi_set_format(spi0,16,1,0,1);
    multicore_launch_core1(cre1);
    sleep_ms(500);
/*
    sleep_ms(1000);
    wrtodds(sinfeq);
    sleep_ms(500);
    gpio_put(0,true);
//*/
    while (true)
    {
        if ((enable)&&(!stup))
        {
            gpio_set_outover(0,GPIO_OVERRIDE_LOW);
            wrtodds(sinfeq);
            sleep_ms(500);
            gpio_set_outover(0,GPIO_OVERRIDE_HIGH);
            stup = true;
            continue;
        }
        else if ((!enable)&&(stup))
        {
            gpio_set_outover(0,GPIO_OVERRIDE_LOW);
            stup = false;
            continue;
        }
        else if (enable&&stup)
        {
            gpio_set_outover(0,GPIO_OVERRIDE_HIGH);
            continue;
        }
        else if ((!enable)&&(!stup))
        {
            gpio_set_outover(0,GPIO_OVERRIDE_LOW);
        }
        
        tight_loop_contents();
    }
}