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
include_directories(../)
#add_subdirectory(../ssd1306xled "${CMAKE_BINARY_DIR}/lib")
link_libraries(ssd1306xled-${AVR_MCU} i2c_bitbang-${AVR_MCU})

add_avr_library(
        print_10x14
        print_10x14.c
        print_10x14.h
        ../font5x7.h
)