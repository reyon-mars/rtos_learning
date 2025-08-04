#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED1_GPIO GPIO_NUM_10
#define LED2_GPIO GPIO_NUM_2
#define TAG       "MULTITASKING LED"

void configure_gpio_output( gpio_num_t pin ){
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << pin, 
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config( &io_conf );
}

void blink_task1( void* param ){
    gpio_num_t pin = *(gpio_num_t*) param;
    while( 1 ){
        gpio_set_level( pin, 1 );
        vTaskDelay( pdMS_TO_TICKS( 500 ));
        gpio_set_level( pin, 0 );
        vTaskDelay( pdMS_TO_TICKS( 500 ));
    }
}

void blink_task2( void* param ){
    gpio_num_t pin = *(gpio_num_t*) param;
    while( 1 ){
        gpio_set_level( pin, 1 );
        vTaskDelay( pdMS_TO_TICKS( 1000 ));
        gpio_set_level( pin, 0 );
        vTaskDelay ( pdMS_TO_TICKS( 1000));
    }
}


void app_main(void)
{
    configure_gpio_output( LED1_GPIO );
    configure_gpio_output( LED2_GPIO );
    int led1 = LED1_GPIO;
    int led2 = LED2_GPIO;

    xTaskCreate( blink_task1, "blink1", 2048, &led1, 5, NULL );
    xTaskCreate( blink_task2, "blink2", 2048, &led2, 5, NULL );


}
