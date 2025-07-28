/**
 * Программа переключает заданные цвета по порядку
 * (c) 2025 Pavel Koltyshev
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "APP";

#define LED_GPIO 8

// Структура данных цвет
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    const char *name;
} color_t;

// Цвета
const color_t color_red = {255, 0, 0, "Red"};
const color_t color_green = {0, 255, 0, "Green"};
const color_t color_blue = {0, 0, 255, "Blue"};
const color_t color_white = {255, 255, 255, "White"};
const color_t color_purple = {255, 0, 255, "Purple"};
const color_t color_yellow = {255, 255, 0, "Yellow"};

// Последовательность цветов для переключения
const color_t colors[] = {
    color_red,
    color_green,
    color_blue,
    color_white,
    color_purple,
    color_yellow
};

// Вычисляем размер последовательности цветов (чтобы не хардкодить руками)
const size_t colors_size = sizeof(colors) / sizeof(colors[0]);

// Задает цвет LED
static void set_color_led(led_strip_handle_t led_strip, color_t color) {
    ESP_LOGI(TAG, "Set color = %s", color.name);
    ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, color.r, color.g, color.b));
}

// Устанавливает индекс следующего цвета в последовательности
static void set_next_color_index(size_t *current_color_index) {
    *current_color_index = (*current_color_index + 1) % colors_size;
}

// Устанавливаем состояние LED (включен/выключен)
static void set_state_led(led_strip_handle_t led_strip, bool value) {
    ESP_LOGI(TAG, "LED is %s", value ? "ON" : "OFF");
    if (value) {
        ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    } else {
        ESP_ERROR_CHECK(led_strip_clear(led_strip));
    }
}

// Настройка LED
static void configure_led(led_strip_handle_t *led_strip) {
    ESP_LOGI(TAG, "Setup LED");

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
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

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, led_strip));
    ESP_ERROR_CHECK(led_strip_clear(*led_strip));
}

// Точка запуска программы
void app_main(void) {
    led_strip_handle_t led_strip;
    size_t current_color_index = 0;

    configure_led(&led_strip);

    while (1) {
        set_color_led(led_strip, colors[current_color_index]);

        set_state_led(led_strip, true);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        set_state_led(led_strip, false);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        set_next_color_index(&current_color_index);
    }
}
