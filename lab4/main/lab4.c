#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"

        #define LDR_INPUT   ADC1_CHANNEL_6   // ใช้ขา GPIO34
#define LED_OUTPUT  GPIO_NUM_25      // ใช้ขา GPIO25
#define LIGHT_LIMIT 2000             // ค่ากำหนดแสง (0-4095)

static const char *LOG_TAG = "LIGHT_SENSOR";

void app_main(void)
{
    // ตั้งค่า ADC ให้กับ LDR
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LDR_INPUT, ADC_ATTEN_DB_11);

    // ตั้งค่า GPIO สำหรับ LED
    gpio_reset_pin(LED_OUTPUT);
    gpio_set_direction(LED_OUTPUT, GPIO_MODE_OUTPUT);

    while (1) {
        int sensor_value = adc1_get_raw(LDR_INPUT);
        ESP_LOGI(LOG_TAG, "Current LDR: %d", sensor_value);

        if (sensor_value > LIGHT_LIMIT) {
            gpio_set_level(LED_OUTPUT, 1);   // เปิด LED
            ESP_LOGI(LOG_TAG, "LED is ON (Dark environment)");
        } else {
            gpio_set_level(LED_OUTPUT, 0);   // ปิด LED
            ESP_LOGI(LOG_TAG, "LED is OFF (Bright environment)");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // หน่วงเวลา 1 วินาที
    }
}
