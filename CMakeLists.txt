cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD solderparty_rp2040_stamp)
set(FAMILY rp2040)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(rp2040_msc)

set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(rp2040_msc
        src/main.c
        src/fatfs_port.c
        src/diskio.c
        src/ff.c
        src/ffunicode.c
        )

add_custom_target(build-time-make-directory ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory cmake-build-debug)

target_link_libraries(rp2040_msc PUBLIC
        pico_stdlib
        pico_bootsel_via_double_reset
        hardware_rtc
        hardware_sync
        hardware_flash)

target_include_directories(rp2040_msc
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/includes/
        )

pico_add_extra_outputs(rp2040_msc)