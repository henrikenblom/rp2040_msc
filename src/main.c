#include <malloc.h>
#include <hardware/flash.h>
#include "ff.h"
#include "device/usbd.h"
#include "bsp/board.h"

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
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


//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void) {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_connected() )
    {
        // connected and there are data available
        if (tud_cdc_available()) {
            // read datas
            char buf[64];
            uint32_t count = tud_cdc_read(buf, sizeof(buf));
            (void) count;

            // Echo back
            // Note: Skip echo by commenting out write() and write_flush()
            // for throughput test e.g
            //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
            tud_cdc_write(buf, count);
            tud_cdc_write_flush();
        }
    }
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
    f_mkfs(fs, FM_FAT | FM_SFD, 0, work, sizeof work);

    f_mount(fs);

    f_open(fs, &fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    f_write(&fil, "Hello, World!\r\n", 15, &bw);
    f_close(&fil);
    fr = f_open(fs, &fil, "hello.txt", FA_READ);
    if (fr) return (int)fr;
    f_close(&fil);
    f_umount(fs);
    tud_init(BOARD_TUD_RHPORT);

    while (true) {
        tud_task();
    }
}