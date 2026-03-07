#include "activity_led.h"
#include "led_driver.h"

#include <hardware/timer.h>

static bool led_state = false;
static uint64_t turn_led_off_after = 0;
static bool driver_initialized = false;

static void ensure_init() {
    if (!driver_initialized) {
        led_driver_init();
        driver_initialized = true;
    }
}

void activity_led_on() {
    ensure_init();
    led_state = true;
    led_driver_set(true);
    turn_led_off_after = time_us_64() + 50000;
}

void activity_led_off_maybe() {
    if (led_state && (time_us_64() > turn_led_off_after)) {
        ensure_init();
        led_state = false;
        led_driver_set(false);
    }
}
