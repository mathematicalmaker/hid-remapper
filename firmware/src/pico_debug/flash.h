
#ifndef __FLASH_H
#define __FLASH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int rp2040_add_flash_bit(uint32_t offset, const uint8_t *src, int size);

/** Reboot the SWD-connected RP2040 target (e.g. B board) so it runs newly flashed firmware. */
void rp2040_reboot_target(void);

#ifdef __cplusplus
}
#endif

#endif
