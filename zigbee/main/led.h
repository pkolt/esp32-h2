#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

extern const led_color_t LED_COLOR_RED;
extern const led_color_t LED_COLOR_GREEN;
extern const led_color_t LED_COLOR_BLUE;
extern const led_color_t LED_COLOR_WHITE;
extern const led_color_t LED_COLOR_PURPLE;
extern const led_color_t LED_COLOR_YELLOW;

void led_init(gpio_num_t gpio_pin);
void led_set_color(led_color_t color);
void led_on(void);
void led_off(void);
void led_set_state(bool state);

#endif // LED_H