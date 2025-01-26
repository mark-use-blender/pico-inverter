/*
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pico-inverter.elf verify reset exit"
*/


#include "LCD_1in3.h"

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


bool reserved_addr(uint8_t addr) {
return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}
int sinfeq = 30;
bool enable =0;
bool bouncec =0;
bool bouncel =0;
bool bouncer =0;
int step =1;
void cre1()
{
    DEV_Delay_ms(100);
    //printf("LCD_1in3_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return;
    }
    DEV_SET_PWM(50);
    /* LCD Init */
    //printf("1.3inch LCD demo...\r\n");
    LCD_1IN3_Init(HORIZONTAL);
    LCD_1IN3_Clear(WHITE);
    
    //LCD_SetBacklight(1023);
    UDOUBLE Imagesize = LCD_1IN3_HEIGHT*LCD_1IN3_WIDTH*2;
    UWORD *BlackImage;
    if((BlackImage = (UWORD *)malloc(Imagesize)) == NULL) {
        //printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((UBYTE *)BlackImage,LCD_1IN3.WIDTH,LCD_1IN3.HEIGHT, 180, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_180);
    Paint_Clear(WHITE);


/*
    uint8_t up = 20;
	uint8_t down = 14;
	uint8_t left = 21;
	uint8_t right = 16;
	uint8_t ctrl = 15;
//*/

//*
    uint8_t up = 18;
	uint8_t down = 2;
	uint8_t left = 20;
	uint8_t right = 16;
	uint8_t ctrl = 3;
//*/

		 
	SET_Infrared_PIN(up);
    SET_Infrared_PIN(down);
    SET_Infrared_PIN(left);
    SET_Infrared_PIN(right);
    SET_Infrared_PIN(ctrl);

    
    Paint_Clear(WHITE);

    // Paint_DrawRectangle(60, 60, 91, 90, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(60, 150, 91, 180, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(15, 105, 46, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(105, 105, 136, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(60, 105, 91, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    // LCD_1IN3_Display(BlackImage);
    while(true)
    {
        Paint_Clear(WHITE);
        Paint_DrawRectangle(60, 60, 91, 90, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(60, 150, 91, 180, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(15, 105, 46, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(105, 105, 136, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawRectangle(60, 105, 91, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
        Paint_DrawNum(0,0,sinfeq,&Font20,0, 0x000f, 0xfff0);
        // LCD_1IN3_DisplayWindows(0, 0,90, 20,BlackImage);
        Paint_DrawNum(0,20,step,&Font20,0, 0x000f, 0xfff0);
        // LCD_1IN3_DisplayWindows(0, 20,90, 40,BlackImage);
        // LCD_1IN3_Display(BlackImage);
        if (enable){
            Paint_DrawRectangle(90, 0, 110, 20, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            //LCD_1IN3_DisplayWindows(90, 0, 110, 20,BlackImage);
        }
        else
        {
            Paint_DrawRectangle(90, 0, 110, 20, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            //LCD_1IN3_DisplayWindows(90, 0, 110, 20,BlackImage);
            if(DEV_Digital_Read(up ) == 0){
                Paint_DrawRectangle(60, 60, 90, 90, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(60, 60, 90, 90,BlackImage);
                printf("gpio =%d\r\n",up);
                if ((sinfeq+step)<1000) sinfeq=sinfeq+step;
                else sinfeq = 1000;
                DEV_Delay_ms(100);
            }
            else{
                Paint_DrawRectangle(60, 60, 90, 90, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(60, 60, 90, 90,BlackImage);
            }

            if(DEV_Digital_Read(down ) == 0){
                Paint_DrawRectangle(60, 150, 90, 180, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(60, 150, 90, 180,BlackImage);
                printf("gpio =%d\r\n",down);
                if ((sinfeq-step)>10) sinfeq=sinfeq-step;
                else sinfeq=10;
                DEV_Delay_ms(100);
            }
            else{
                Paint_DrawRectangle(60, 150, 90, 180, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(60, 150, 90, 180,BlackImage);
            }
            
            if(DEV_Digital_Read(left ) == 0){
                Paint_DrawRectangle(15, 105, 45, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(15, 105, 45, 135,BlackImage);
                printf("gpio =%d\r\n",left);
                if (!bouncel)if (step<100) step=step*10;
                bouncel=1;
            }
            else{
                Paint_DrawRectangle(15, 105, 45, 135, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(15, 105, 45, 135,BlackImage);
                bouncel=0;
            }
                
            if(DEV_Digital_Read(right ) == 0){
                Paint_DrawRectangle(105, 105, 135, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(105, 105, 135, 135,BlackImage);
                printf("gpio =%d\r\n",right);
                if (!bouncer)if (step>1) step=step/10;
                bouncer=1;
            }
            else{
                Paint_DrawRectangle(105, 105, 135, 135, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
                //LCD_1IN3_DisplayWindows(105, 105, 135, 135,BlackImage);
                bouncer=0;
            }
            // LCD_1IN3_Display(BlackImage);
        }
        if(DEV_Digital_Read(ctrl ) == 0){
            Paint_DrawRectangle(60, 105, 90, 135, 0xF800, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            //LCD_1IN3_DisplayWindows(60, 105, 90, 135,BlackImage);
            printf("gpio =%d\r\n",ctrl);
            if (!bouncec)enable = !enable;
            bouncec=1;
        }
        else{
            Paint_DrawRectangle(60, 105, 90, 135, WHITE, DOT_PIXEL_2X2,DRAW_FILL_FULL);
            //LCD_1IN3_DisplayWindows(60, 105, 90, 135,BlackImage);
            bouncec=0;
        }
        LCD_1IN3_Display(BlackImage);
    }

}
void main()
{
    multicore_launch_core1(cre1);
    while(true)
    {
        // sinfeq--;
        sleep_ms(500);
    }

}