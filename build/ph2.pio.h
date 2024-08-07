// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --- //
// ph2 //
// --- //

#define ph2_wrap_target 3
#define ph2_wrap 11

static const uint16_t ph2_program_instructions[] = {
    0xa02b, //  0: mov    x, !null                   
    0x2002, //  1: wait   0 gpio, 2                  
    0x2082, //  2: wait   1 gpio, 2                  
            //     .wrap_target
    0xa02b, //  3: mov    x, !null                   
    0x00c6, //  4: jmp    pin, 6                     
    0x0044, //  5: jmp    x--, 4                     
    0x00c8, //  6: jmp    pin, 8                     
    0x0009, //  7: jmp    9                          
    0x0046, //  8: jmp    x--, 6                     
    0xa029, //  9: mov    x, !x                      
    0x4020, // 10: in     x, 32                      
    0x8000, // 11: push   noblock                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program ph2_program = {
    .instructions = ph2_program_instructions,
    .length = 12,
    .origin = -1,
};

static inline pio_sm_config ph2_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ph2_wrap_target, offset + ph2_wrap);
    return c;
}

// Helper function (for use in C program) to initialize this PIO program
void ph2_program_init(PIO pio, uint sm, uint offset, float div, uint pin) {
    // Sets up state machine and wrap target. This function is automatically
    pio_sm_config c = ph2_program_get_default_config(offset);
    sm_config_set_fifo_join (&c, 2);
     sm_config_set_set_pins(&c,pin,2);
    sm_config_set_jmp_pin(&c, 2);
    // Set the clock divider for the state machine
    sm_config_set_clkdiv(&c, div);
    // Load configuration and jump to start of the program
    pio_sm_init(pio, sm, offset, &c);
}

#endif

