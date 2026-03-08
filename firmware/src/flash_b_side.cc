#include <hardware/watchdog.h>

#include "dual_b_binary.h"

extern "C" {
#include "adi.h"
#include "flash.h"
#include "swd.h"
}

int main() {
    if (swd_init() != 0) {
        watchdog_reboot(0, 0, 0);
        while (true) __asm__ volatile("wfi");
    }
    if (dp_init() != 0) {
        /* SWD not connected or wrong wiring: A GP2->B GP27 (SWCLK), A GP3->B GP26 (SWDIO), GND. Try swapping A2/A3 if needed. */
        watchdog_reboot(0, 0, 0);
        while (true) __asm__ volatile("wfi");
    }

    core_select(0);
    core_reset_halt();
    core_select(1);
    core_reset_halt();
    core_select(0);

    rp2040_add_flash_bit(0, dual_b_binary, dual_b_binary_length);
    rp2040_add_flash_bit(0xffffffff, NULL, 0);

    rp2040_reboot_target();

    watchdog_reboot(0, 0, 0);
    while (true) {
        __asm__ volatile("wfi");
    }

    return 0;
}
