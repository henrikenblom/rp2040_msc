#include <malloc.h>
#include <hardware/flash.h>
#include "ff.h"

int main() {
    FATFS *fs;
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    BYTE work[FLASH_SECTOR_SIZE]; /* Work area (larger is better for processing time) */
    DWORD nclst;
    TCHAR label;
    DWORD vsn;

    fs = malloc(sizeof(FATFS));
    if (f_mount(fs) != FR_OK) {
        assert(f_mkfs(fs, FM_FAT | FM_SFD, 0, work, sizeof work) == FR_OK);
        f_mount(fs);
    }

//    f_open(fs, &fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
//    f_write(&fil, "Hello, World!\r\n", 15, &bw);
//    f_close(&fil);
    f_getfree(fs, &nclst);
    f_umount(fs);

    while (true) {

    }
}