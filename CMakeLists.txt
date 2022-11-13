cmake_minimum_required(VERSION 3.13)

set(TINYUSB_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/tinyusb)
set(PICO_BOARD pico)
set(BOARD pico_sdk)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
include(${TINYUSB_PATH}/hw/bsp/family_support.cmake)

project(rp2040_msc)

pico_sdk_init()

add_executable(rp2040_msc
        src/main.c
        src/fatfs_port.c
        src/diskio.c
        src/ff.c
        src/ffunicode.c
        src/usb_descriptors.c
        src/msc_disk.c
        src/ffsystem.c
        )

add_custom_target(build-time-make-directory ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory cmake-build-debug)

target_link_libraries(rp2040_msc PUBLIC
        pico_stdlib
        pico_bootsel_via_double_reset
        pico_multicore
        pico_unique_id
        tinyusb_common
        tinyusb_device
        hardware_rtc
        hardware_sync
        hardware_flash
        )

target_include_directories(rp2040_msc
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/includes/
        ${TINYUSB_PATH}/src/
        ${TINYUSB_PATH}/src/common/
        ${TINYUSB_PATH}/hw/
        )

target_compile_definitions(rp2040_msc PUBLIC
        CFG_TUH_MSC=1
        CFG_TUH_DEVICE_MAX=1
        CFG_TUD_MSC=1
        PICO_NO_BI_STDIO_UART=1
        PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1
        )

pico_enable_stdio_uart(rp2040_msc 1)
pico_enable_stdio_usb(rp2040_msc 0)

pico_add_extra_outputs(rp2040_msc)