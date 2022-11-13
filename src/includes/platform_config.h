#include "hardware/flash.h"

#define FLASH_STORAGE_BYTES (1408 * 1024)
#define BLOCK_SIZE          (FLASH_SECTOR_SIZE)
#define BLOCK_COUNT         (FLASH_STORAGE_BYTES / FLASH_SECTOR_SIZE)
#define FLASH_BASE_ADDR     (PICO_FLASH_SIZE_BYTES - FLASH_STORAGE_BYTES)
#define FLASH_MMAP_ADDR     (XIP_BASE + FLASH_BASE_ADDR)

#define BOARD_TUD_RHPORT      0

#ifndef MICROPY_HW_USB_VID
#define MICROPY_HW_USB_VID (0x2E8A) // Raspberry Pi
#endif
#ifndef MICROPY_HW_USB_PID
#define MICROPY_HW_USB_PID (0x0005) // RP2 MicroPython
#endif

#define MICROPY_FATFS_MAX_SS                    (FLASH_SECTOR_SIZE)