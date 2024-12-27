/*
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pico-inverter.elf verify reset exit"
*/


#include "LCD_1in3.h"

#include "DEV_Config.h"
#include "GUI_Paint.h"


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
#define uppin 14
#define cenpin 15
#define leftpin 16
#define downpin 20
#define rightpin 21
int sinfeq = 30;
bool enpwr =0;
bool dispwr = 1;
uint16_t buff[5];
uint16_t tbuff[5];
UWORD *BlackImage;
UDOUBLE Imagesize = LCD_1IN3_HEIGHT*LCD_1IN3_WIDTH*2;

void core1()
{
    if (gpio_get(downpin)) sinfeq--;
    if (gpio_get(uppin)) sinfeq++;
    if (gpio_get(cenpin)) 
    {
        enpwr=dispwr;
        dispwr=!dispwr;
        crft_img();
        LCD_1IN3_Display(BlackImage);
        sleep_ms(1000);        
    }
    crft_img();
    LCD_1IN3_Display(BlackImage);
    DEV_Delay_ms(50);

}
void crft_img()
{
    Paint_Clear(WHITE);
    Paint_DrawNum(0,0,sinfeq,&Font12,1,BLACK,WHITE);
    Paint_DrawNum(0,20,!dispwr,&Font12,1,BLACK,WHITE);
}

void buffprepsn()
{
    buff[0]=(0x2100);
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
    DEV_Module_Init();
    LCD_1IN3_Init(HORIZONTAL);
    LCD_1IN3_Clear(WHITE);
    multicore_launch_core1(core1);
    
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((UBYTE *)BlackImage,LCD_1IN3.WIDTH,LCD_1IN3.HEIGHT, 0, WHITE);

    gpio_init(0);
    gpio_set_dir(0,true);
    gpio_put(0,false);

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

    sleep_ms(1000);
    buffprepsn();
    gpio_clr_mask(0x00020022);
    spi_write16_blocking(spi0,buff,1);
    gpio_set_mask(0x00020022);
    

    while (true)
    {
        if (enpwr)
        {
            gpio_put(0,false);
            wrtodds(sinfeq);
            sleep_ms(100);
            gpio_put(0,true);
            dispwr=0;
            enpwr=0;
        }
        
        if (dispwr)
        {
            gpio_put(0,false);
        }
        /*
        gpio_put(0,false);
        wrtodds(sinfeq);
        sleep_ms(500);
        gpio_put(0,true);
        //*/
        tight_loop_contents();
    }
}
