##########################################################################
# "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware
# license):
# <dev@layer128.net> wrote this file. As long as you retain this notice
# you can do whatever you want with this stuff. If we meet some day, and
# you think this stuff is worth it, you can buy me a be(ve)er(age) in
# return. (I don't like beer much.)
#
# Matthias Kleemann
##########################################################################

#include(../generic-gcc-avr.cmake)
#include_directories(print_3x7 print_5x7 print_6x14 print_10x14)

add_subdirectory(print_5x7)
add_subdirectory(print_3x7)
add_subdirectory(print_6x14)
add_subdirectory(print_10x14)

link_libraries(
        i2c_bitbang-${AVR_MCU}
        print_5x7-${AVR_MCU}
        print_3x7-${AVR_MCU}
        print_6x14-${AVR_MCU}
        print_10x14-${AVR_MCU}
)
add_avr_library(
        ssd1306xled
        ssd1306xled.c
        ssd1306xled.h
)
