#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED1_GPIO GPIO_NUM_2
#define LED2_GPIO GPIO_NUM_10
#define LED3_GPIO GPIO_NUM_12
#define TAG       "MULTI_BLINK"

void blink_task( void* param ){
    const uint32_t led_gpio = ((uint32_t*)param)[0];
    const TickType_t delay_ticks = ((uint32_t*)param)[1];

    gpio_config_t io_conf = {
        .pin_bit_mask = ( 1ULL << led_gpio ),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config( &io_conf );
    
    while( 1 ){
        gpio_set_level( led_gpio, 1 );
        vTaskDelay( delay_ticks );
        gpio_set_level( led_gpio, 0 );
        vTaskDelay( delay_ticks );
    }
}

void app_main(void)
{
    ESP_LOGI( TAG, "Starting Multi-LED Blink with FreeRTOS. ");
    static uint32_t led1_args[2] = { LED1_GPIO, pdMS_TO_TICKS( 100 )};
    static uint32_t led2_args[2] = { LED2_GPIO, pdMS_TO_TICKS( 100 )};
    static uint32_t led3_args[2] = { LED3_GPIO, pdMS_TO_TICKS( 100 )};
    
    xTaskCreate( blink_task, "task1", 4096, led1_args, 1, NULL );
    xTaskCreate( blink_task, "task2", 4096, led2_args, 1, NULL );
    xTaskCreate( blink_task, "task3", 4096, led3_args, 1, NULL );

    
}
