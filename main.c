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
int sinfeq = 3000;
uint16_t buff[5];



void buffprep(int feq,int pha)
{
    uint32_t binfeq = (int)(round((((2<<27))/(1000000))*(feq)));
    uint16_t binpha = (int)(round(pha*0.33*4096));
    buff[0]=(0x3000);
    buff[1]=((0x4000)+((binfeq)&(0x3fff)));
    buff[2]=((0x4000)+((binfeq>>14)&(0x3fff)));
    buff[3]=(0x3000);
    buff[4]=((0xc000)+((binpha)&(0x0fff)));
    



}


int main()
{
    
    gpio_set_function( 1, GPIO_FUNC_SPI);
    gpio_set_function( 2, GPIO_FUNC_SPI);
    gpio_set_function( 3, GPIO_FUNC_SPI);
    gpio_set_function( 5, GPIO_FUNC_SPI);
    gpio_set_function( 6, GPIO_FUNC_SPI);
    gpio_set_function( 7, GPIO_FUNC_SPI);
    gpio_set_function(17, GPIO_FUNC_SPI);
    gpio_set_function(18, GPIO_FUNC_SPI);
    gpio_set_function(19, GPIO_FUNC_SPI);

    //gpio_init(0);
    gpio_set_outover (1, GPIO_OVERRIDE_HIGH);
    //gpio_init(4);
    gpio_set_outover (5, GPIO_OVERRIDE_HIGH);
    //gpio_init(16);
    gpio_set_outover (17, GPIO_OVERRIDE_HIGH);

    

    spi_init(spi0,20000);
    spi_set_format(spi0,16,1,0,1);
    sleep_ms(1000);
    buffprep(sinfeq,0);
    gpio_set_outover (1, GPIO_OVERRIDE_LOW);
    spi_write16_blocking(spi0,buff,5);
    gpio_set_outover (1, GPIO_OVERRIDE_HIGH);
    buffprep(sinfeq,1);
    gpio_set_outover (5, GPIO_OVERRIDE_LOW);
    spi_write16_blocking(spi0,buff,5);
    gpio_set_outover (5, GPIO_OVERRIDE_HIGH);
    buffprep(sinfeq,2);
    gpio_set_outover (17, GPIO_OVERRIDE_LOW);
    spi_write16_blocking(spi0,buff,5);
    gpio_set_outover (17, GPIO_OVERRIDE_HIGH);
    while (true)
    {
        //*
        buffprep(sinfeq,0);
        gpio_set_outover (1, GPIO_OVERRIDE_LOW);
        spi_write16_blocking(spi0,buff,5);
        gpio_set_outover (1, GPIO_OVERRIDE_HIGH);
        buffprep(sinfeq,1);
        gpio_set_outover (5, GPIO_OVERRIDE_LOW);
        spi_write16_blocking(spi0,buff,5);
        gpio_set_outover (5, GPIO_OVERRIDE_HIGH);
        //*/
        buffprep(sinfeq,2);
        gpio_set_outover (17, GPIO_OVERRIDE_LOW);
        spi_write16_blocking(spi0,buff,5);
        gpio_set_outover (17, GPIO_OVERRIDE_HIGH);
        sleep_ms(1000);
        //*/
        tight_loop_contents();
        
        
    }








}