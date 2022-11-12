#include <hardware/sync.h>
#include <memory.h>
#include "diskio.h"
#include "ff.h"
#include "hardware/flash.h"

typedef void *bdev_t;
#define FLASH_STORAGE_BYTES (8 * 1024 * 1024)
#define BLOCK_SIZE          (FLASH_SECTOR_SIZE)
#define BLOCK_COUNT         (FLASH_STORAGE_BYTES / BLOCK_SIZE)
#define FLASH_BASE_ADDR     (256 * 1024)
#define FLASH_MMAP_ADDR     (XIP_BASE + FLASH_BASE_ADDR)

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
        bdev_t pdrv,      /* Physical drive number (0..) */
        BYTE *buff,        /* Data buffer to store read data */
        DWORD sector,    /* Sector address (LBA) */
        UINT count        /* Number of sectors to read (1..128) */
) {
    (void) pdrv;
    
    memcpy(buff, (void *) (FLASH_MMAP_ADDR + sector * BLOCK_SIZE), count * BLOCK_SIZE);
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(
        bdev_t pdrv,          /* Physical drive nmuber (0..) */
        const BYTE *buff,    /* Data to be written */
        DWORD sector,        /* Sector address (LBA) */
        UINT count            /* Number of sectors to write (1..128) */
) {
    (void) pdrv;

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_ADDR + sector * BLOCK_SIZE, count * BLOCK_SIZE);
    flash_range_program(FLASH_BASE_ADDR + sector * BLOCK_SIZE, buff, count * BLOCK_SIZE);
    restore_interrupts(ints);
    return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
        bdev_t pdrv,      /* Physical drive nmuber (0..) */
        BYTE cmd,        /* Control code */
        void *buff        /* Buffer to send/receive control data */
) {
    (void) pdrv;

    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;

        case GET_SECTOR_COUNT: {
            *((DWORD *) buff) = BLOCK_COUNT;
            return RES_OK;
        }

        case GET_SECTOR_SIZE: {
            *((DWORD *) buff) = BLOCK_SIZE;
            return RES_OK;
        }

        case GET_BLOCK_SIZE:
            *((DWORD *) buff) = 1;
            return RES_OK;

        case IOCTL_INIT:
        case IOCTL_STATUS: {
            *((DSTATUS *) buff) = 0;
            return RES_OK;
        }

        default:
            return RES_PARERR;
    }
}