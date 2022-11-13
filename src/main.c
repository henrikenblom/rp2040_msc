#include <malloc.h>
#include <hardware/flash.h>
#include "ff.h"
#include "device/usbd.h"

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
    printf("");
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    printf("");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    (void) itf;
    (void) rts;

    // TODO set some indicator
    if (dtr) {
        // Terminal connected
    } else {
        // Terminal disconnected
    }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf) {
    (void) itf;
}


int main() {
    FATFS *fs;
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    BYTE work[FLASH_SECTOR_SIZE]; /* Work area (larger is better for processing time) */
    DWORD nclst;
    TCHAR label;
    DWORD vsn;
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */

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