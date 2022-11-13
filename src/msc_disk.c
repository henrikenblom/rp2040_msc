#include "tusb.h"
#include "platform_config.h"

static bool ejected = false;

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    const char vid[] = "Micropy";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";

    strncpy((char *) vendor_id, vid, 8);
    strncpy((char *) product_id, pid, 16);
    strncpy((char *) product_rev, rev, 4);
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    if (ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }
    return true;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    *block_size = BLOCK_SIZE;
    *block_count = BLOCK_COUNT;
    printf("%d", BLOCK_SIZE);
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    if (load_eject) {
        if (start) {
            // load disk storage
            ejected = false;
        } else {
            // unload disk storage
            ejected = true;
        }
    }
    return true;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    uint32_t count = bufsize / BLOCK_SIZE;
    memcpy(buffer, (void *) (FLASH_MMAP_ADDR + lba * BLOCK_SIZE), count * BLOCK_SIZE);
    if (count == 0) {
        return -1;
    } else {
        return count * BLOCK_SIZE;
    }
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    uint32_t count = bufsize / BLOCK_SIZE;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_ADDR + lba * BLOCK_SIZE, count * BLOCK_SIZE);
    flash_range_program(FLASH_BASE_ADDR + lba * BLOCK_SIZE, buffer, count * BLOCK_SIZE);
    restore_interrupts(ints);
    return count * BLOCK_SIZE;
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    int32_t resplen = 0;
    switch (scsi_cmd[0]) {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            // Sync the logical unit if needed.
            break;

        default:
            // Set Sense = Invalid Command Operation
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            // negative means error -> tinyusb could stall and/or response with failed status
            resplen = -1;
            break;
    }
    return resplen;
}