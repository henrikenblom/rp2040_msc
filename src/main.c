#include <malloc.h>
#include <hardware/flash.h>
#include "ff.h"
#include "device/usbd.h"
#include "msc_control.h"

FATFS *fs;

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
    f_unmount("");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    set_msc_ready_to_attach();
    FIL fil;            /* File object */
    char line[100]; /* Line buffer */
    f_mount(fs, "", FA_READ | FA_WRITE);
    f_open(&fil, "message.txt", FA_READ);
    while (f_gets(line, sizeof line, &fil)) {
        printf("%s", line);
    }
    f_close(&fil);
}

int main() {
    BYTE work[FLASH_SECTOR_SIZE]; /* Work area (larger is better for processing time) */
    fs = malloc(sizeof(FATFS));
    if (f_mount(fs, "", FA_READ | FA_WRITE) != FR_OK) {
        f_mkfs("", 0, work, sizeof(work));
        f_setlabel("RP2040SID");
        f_mount(fs, "", FA_READ | FA_WRITE);
    }
    f_unmount("");

    tud_init(BOARD_TUD_RHPORT);

    while (true) {
        tud_task();
    }
}