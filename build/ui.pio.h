// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// -- //
// ui //
// -- //

#define ui_wrap_target 3
#define ui_wrap 10
#define ui_pio_version 0

static const uint16_t ui_program_instructions[] = {
    0xa02b, //  0: mov    x, !null                   
    0x2003, //  1: wait   0 gpio, 3                  
    0x2083, //  2: wait   1 gpio, 3                  
            //     .wrap_target
    0xa02b, //  3: mov    x, !null                   
    0x00c7, //  4: jmp    pin, 7                     
    0x00c8, //  5: jmp    pin, 8                     
    0x0045, //  6: jmp    x--, 5                     
    0x0044, //  7: jmp    x--, 4                     
    0xa029, //  8: mov    x, !x                      
    0x4020, //  9: in     x, 32                      
    0x8000, // 10: push   noblock                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program ui_program = {
    .instructions = ui_program_instructions,
    .length = 11,
    .origin = -1,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x1
#endif
};

static inline pio_sm_config ui_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ui_wrap_target, offset + ui_wrap);
    return c;
}

// Helper function (for use in C program) to initialize this PIO program
void ui_program_init(PIO pio, uint sm, uint offset, float div, uint pin) {
    // Sets up state machine and wrap target. This function is automatically
    pio_sm_config c = ui_program_get_default_config(offset);
    sm_config_set_fifo_join (&c, 2);
     sm_config_set_set_pins(&c,pin,2);
    sm_config_set_jmp_pin(&c, 3);
    // Set the clock divider for the state machine
    sm_config_set_clkdiv(&c, div);
    // Load configuration and jump to start of the program
    pio_sm_init(pio, sm, offset, &c);
}

#endif

