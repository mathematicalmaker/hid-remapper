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
#ifndef REMAPPER_WS2812_COLOR_GRB
#define REMAPPER_WS2812_COLOR_GRB 0xff0000u  /* default green */
#endif
    uint32_t grb = on ? (uint32_t)REMAPPER_WS2812_COLOR_GRB : 0u;
    pio_sm_put_blocking(pio, sm, grb << 8u);
}

#else

void led_driver_init(void) {}
void led_driver_set(bool on) { (void)on; }

#endif
