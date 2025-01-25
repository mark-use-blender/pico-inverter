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
int sinfeq = 30;
uint16_t buff[5];
uint16_t tbuff[5];

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
    wrtodds(sinfeq);
    sleep_ms(500);
    gpio_put(0,true);

    while (true)
    {
        /*
        if (enable|!stup)
        {
            gpio_put(0,false);
            wrtodds(sinfeq);
            sleep_ms(500);
            gpio_put(0,true);
            stup = true;
        }
        if (!enable|stup)
        {
            gpio_put(0,false);
            stup = false;
        }
        //*/
        tight_loop_contents();
        
        
    }








}