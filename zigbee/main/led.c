#include "led.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "LED";
static led_strip_handle_t led_strip;
static led_color_t current_color;

const led_color_t LED_COLOR_RED = {255, 0, 0};
const led_color_t LED_COLOR_GREEN = {0, 255, 0};
const led_color_t LED_COLOR_BLUE = {0, 0, 255};
const led_color_t LED_COLOR_WHITE = {255, 255, 255};
const led_color_t LED_COLOR_PURPLE = {255, 0, 255};
const led_color_t LED_COLOR_YELLOW = {255, 255, 0};

void led_init(gpio_num_t gpio_pin) {
    ESP_LOGI(TAG, "Setup LED");

    led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_pin,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB,
        .max_leds = 1,
    };

    led_strip_rmt_config_t rmt_config = {
        // 10MHz вызовет предупреждение "ESP-IDF: W (274) rmt: channel resolution loss, real=10666666"
        // 96MHz / 10MHz = 9.6, число не целое, поэтому будет выбран ближайший делитель 10.6~10666666Hz
        // 96Mhz / 16Mhz = 6 (делитель подходит для RMT)
        .resolution_hz = 16 * 1000 * 1000, // 16MHz
        .flags.with_dma = false,
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    ESP_ERROR_CHECK(led_strip_clear(led_strip));
}

void led_set_color(led_color_t color) {
    current_color = color;
}

void led_on(void) {
    ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, current_color.r, current_color.g, current_color.b));
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
}

void led_off(void) {
    ESP_ERROR_CHECK(led_strip_clear(led_strip));
}

void led_set_state(bool state) {
    if (state) {
        led_on();
    } else {
        led_off();
    }
}