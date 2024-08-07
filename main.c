/*
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pico-inverter.elf verify reset exit"
*/

/*
tab = []
while i<9:
     tab.append(int(sin(pi*i/9)*7))
     i=i+1

tab

[0.0, 10.452846326765346, 20.79116908177593, 30.901699437494738, 40.67366430758001, 49.99999999999999, 58.778525229247315, 66.91306063588583, 74.31448254773942, 80.90169943749474, 86.60254037844386, 91.35454576426008, 95.10565162951535, 97.81476007338057, 99.45218953682733, 100.0, 99.45218953682733, 97.81476007338057, 95.10565162951536, 91.3545457642601, 86.60254037844388, 80.90169943749474, 74.31448254773944, 66.91306063588584, 58.77852522924732, 49.99999999999999, 40.673664307580005, 30.901699437494752, 20.79116908177593, 10.452846326765329]

[0, 2, 4, 6, 6, 6, 6, 4, 2]

[0, 10, 20, 30, 40, 49, 58, 66, 74, 80, 86, 91, 95, 97, 99, 100,
 99, 97, 95, 91, 86, 80, 74, 66, 58, 49, 40, 30, 20, 10]



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
#include "ph0.pio.h"
#include "ph1.pio.h"
#include "ph2.pio.h"
#include "ui.pio.h"
#include "src/pico_servo.h"

#define PI 3.14
#define sindev 18
#define tdev 16
#define sft_tab 4
float cldiv = 1;
int feq = 1;
int phoff ;
uint32_t s_tab;
int phpin0 = 2;
int phpin1 = 4;
int phpin2 = 6;
int rotpin = 8;
void iniclkpram()
{
    s_tab = 0;
    for (int i=1;i<sindev/2;i++)
    {
        s_tab = s_tab<<sft_tab;
        s_tab = s_tab|(uint32_t)nearbyint(sinf(PI*i/sindev/2)*tdev);
    }
    cldiv = fmaxf(1.0,(125000000/sindev/tdev/2/feq));
    phoff = sindev*2/3;
    return;
}




int main()
{
    iniclkpram();
    

    PIO pio00 = pio0;
    PIO pio01 = pio1;
    // Get state machine in PIO 
    uint sm1 = pio_claim_unused_sm(pio00, true);
    uint sm2 = pio_claim_unused_sm(pio00, true);
    uint sm3 = pio_claim_unused_sm(pio01, true);
    uint sm4 = pio_claim_unused_sm(pio01, true);
    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset1 = pio_add_program(pio00, &ph0_program);
    uint offset2 = pio_add_program(pio00, &ph1_program);
    uint offset3 = pio_add_program(pio01, &ph2_program);
    uint offset4 = pio_add_program(pio01, &ui_program);
    // Initialize the program using the helper function in our .pio file
    ph0_program_init(pio00, sm1, offset1, cldiv,phpin2);
    ui_program_init(pio01, sm4, offset4, 1,rotpin);
    // Start running our PIO program in the state machine
    // pio_sm_set_enabled(pio00, sm1, true);
    // pio_sm_set_enabled(pio00, sm2, true);
    // pio_sm_set_enabled(pio01, sm3, true);
    pio_sm_set_enabled(pio01, sm4, true);
    ph0_program_init(pio00, sm2, offset2, cldiv, phpin0);
    ph1_program_init(pio00, sm2, offset2, cldiv, phpin1);
    ph2_program_init(pio01, sm3, offset3, cldiv, phpin2);



}