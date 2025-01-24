cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)
#include(~/pico-sdk/tools/CMakeLists.txt)
project(pico-invrerter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

# add compiled files subdirectory
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/Config)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/Infrared)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/OLED)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/Icm20948)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/LCD)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/Fonts)
add_subdirectory(lcd/Pico_code/Pico_code/c/lib/GUI)
add_subdirectory(lcd/Pico_code/Pico_code/c/examples)



# add header file directory
include_directories(lcd/Pico_code/Pico_code/c/examples)
include_directories(./lcd/Pico_code/Pico_code/c/lib/Config)
include_directories(./lcd/Pico_code/Pico_code/c/lib/GUI)
include_directories(./lcd/Pico_code/Pico_code/c/lib/Icm20948)
include_directories(./lcd/Pico_code/Pico_code/c/lib/LCD)
include_directories(./lcd/Pico_code/Pico_code/c/lib/Infrared)
include_directories(./lcd/Pico_code/Pico_code/c/lib/OLED)

add_executable(pico-invrerter
  src/pico_servo.c
  src/pico_servo.h
  main.c
)
pico_generate_pio_header(pico-invrerter
        ./../ph0.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../ph1.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../ph2.pio
        
)
pico_generate_pio_header(pico-invrerter
        ./../ui.pio
        
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
  hardware_dma
  hardware_sync
  hardware_timer
  hardware_irq
  pico_multicore
  hardware_pio
  hardware_spi
  LCD 
  OLED 
  Infrared 
  Icm20948 
  GUI 
  Fonts 
  Config

)








