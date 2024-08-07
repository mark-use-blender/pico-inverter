// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --- //
// ph0 //
// --- //

#define ph0_wrap_target 0
#define ph0_wrap 9

static const uint16_t ph0_program_instructions[] = {
            //     .wrap_target
    0xa02b, //  0: mov    x, !null                   
    0x2003, //  1: wait   0 gpio, 3                  
    0x2082, //  2: wait   1 gpio, 2                  
    0x2083, //  3: wait   1 gpio, 3                  
    0x0046, //  4: jmp    x--, 6                     
    0x0007, //  5: jmp    7                          
    0x00c4, //  6: jmp    pin, 4                     
    0xa029, //  7: mov    x, !x                      
    0x4020, //  8: in     x, 32                      
    0x8000, //  9: push   noblock                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program ph0_program = {
    .instructions = ph0_program_instructions,
    .length = 10,
    .origin = -1,
};

static inline pio_sm_config ph0_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ph0_wrap_target, offset + ph0_wrap);
    return c;
}

// Helper function (for use in C program) to initialize this PIO program
void ph0_program_init(PIO pio, uint sm, uint offset, float div) {
    // Sets up state machine and wrap target. This function is automatically
    pio_sm_config c = ph0_program_get_default_config(offset);
    sm_config_set_fifo_join (&c, 2);
    sm_config_set_jmp_pin(&c, 2);
    // Set the clock divider for the state machine
    sm_config_set_clkdiv(&c, div);
    // Load configuration and jump to start of the program
    pio_sm_init(pio, sm, offset, &c);
}

#endif

