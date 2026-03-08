#include "led_driver.h"

#include "hardware/gpio.h"
#include "hardware/clocks.h"

#ifdef PICO_DEFAULT_LED_PIN
#ifndef PICO_DEFAULT_LED_PIN_INVERTED
#define PICO_DEFAULT_LED_PIN_INVERTED 0
#endif

void led_driver_init(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, PICO_DEFAULT_LED_PIN_INVERTED);
}

void led_driver_set(bool on) {
    gpio_put(PICO_DEFAULT_LED_PIN, on ? !PICO_DEFAULT_LED_PIN_INVERTED : PICO_DEFAULT_LED_PIN_INVERTED);
}

#elif defined(PICO_DEFAULT_WS2812_PIN)

#include "hardware/pio.h"
#include "ws2812.pio.h"

static PIO pio;
static uint sm;
static uint offset;

void led_driver_init(void) {
    pio = pio0;
    sm = pio_claim_unused_sm(pio, true);
    offset = pio_add_program(pio, &ws2812_program);

    pio_gpio_init(pio, PICO_DEFAULT_WS2812_PIN);
    pio_sm_set_consecutive_pindirs(pio, sm, PICO_DEFAULT_WS2812_PIN, 1, true);

    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, PICO_DEFAULT_WS2812_PIN);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    int cycles_per_bit = ws2812_T1 + ws2812_T2 + ws2812_T3;
    float div = clock_get_hz(clk_sys) / (800000.0f * cycles_per_bit);
    sm_config_set_clkdiv(&c, div);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void led_driver_set(bool on) {
#ifndef REMAPPER_WS2812_COLOR_RGB
#define REMAPPER_WS2812_COLOR_RGB 0x00ff00u  /* default green (R,G,B) */
#endif
#ifndef REMAPPER_WS2812_BRIGHTNESS_PERCENT
#define REMAPPER_WS2812_BRIGHTNESS_PERCENT 100
#endif
    uint32_t rgb = on ? (uint32_t)REMAPPER_WS2812_COLOR_RGB : 0u;
    unsigned pct = (unsigned)REMAPPER_WS2812_BRIGHTNESS_PERCENT;
    if (pct > 100u) pct = 100u;
    if (on && pct < 100u) {
        unsigned r = (rgb >> 16) & 0xffu, g = (rgb >> 8) & 0xffu, b = rgb & 0xffu;
        r = (r * pct) / 100u;
        g = (g * pct) / 100u;
        b = (b * pct) / 100u;
        rgb = (r << 16) | (g << 8) | b;
    }
    /* LED expects RGB byte order (first byte R, then G, then B) */
    pio_sm_put_blocking(pio, sm, rgb << 8u);
}

#else

void led_driver_init(void) {}
void led_driver_set(bool on) { (void)on; }

#endif
