cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)
#include(~/pico-sdk/tools/CMakeLists.txt)
project(pico-invrerter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(pico-invrerter
  src/pico_servo.c
  src/pico_servo.h
  main.c
)
pico_generate_pio_header(pico-invrerter
        ./../off1.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../off2.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../spe1.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../spe2.pio
        
)

pico_add_extra_outputs(pico-invrerter)
target_link_libraries(
  pico-invrerter 
  pico_stdlib 
  hardware_clocks
  hardware_gpio 
  hardware_adc 
  hardware_pwm 
  hardware_uart 
  hardware_flash 
  hardware_sync 
  hardware_timer
  hardware_irq
  pico_multicore
  hardware_pio
)








