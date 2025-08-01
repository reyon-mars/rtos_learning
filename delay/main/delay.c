#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#define LED1_GPIO GPIO_NUM_10
#define LED2_GPIO GPIO_NUM_2
#define LED3_GPIO GPIO_NUM_12


#define BLINK_PERIOD_MS 1000
#define TAG "BLINK_TEST"

void init_gpio_output( gpio_num_t gpio_num ){
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << gpio_num,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config( &config );
}

void vtaskdelay_blink( void* param ){
    const gpio_num_t pin = (*(int*) param );
    int led_state = 1;
    while( 1 ){
        gpio_set_level( pin, led_state );
        ESP_LOGI( TAG, "vTaskDelay() LED ON" );
        vTaskDelay( pdMS_TO_TICKS( BLINK_PERIOD_MS / 2 ));
        led_state = !led_state;

        gpio_set_level( pin, led_state );
        ESP_LOGI( TAG, "vTaskDelay() LED OFF" );
        vTaskDelay( pdMS_TO_TICKS( BLINK_PERIOD_MS / 2 ));
        led_state = !led_state;
    }
}

void vtaskdelayuntil_blink( void* param ){
    const gpio_num_t pin = (*(int*) param );
    int led_state = 1;
    TickType_t x_last_wake_time = xTaskGetTickCount();

    while( 1 ){
        gpio_set_level( pin, led_state );
        ESP_LOGI( TAG, "vTaskDelayUntil() LED ON" );
        vTaskDelayUntil( &x_last_wake_time, pdMS_TO_TICKS( BLINK_PERIOD_MS / 2 ));
        led_state = !led_state;

        gpio_set_level( pin, led_state );
        ESP_LOGI( TAG, "vTaskDelayUntil() LED OFF");
        vTaskDelayUntil( &x_last_wake_time, pdMS_TO_TICKS( BLINK_PERIOD_MS / 2 ));
        led_state = !led_state;
    }
}

void led_timer_callback( TimerHandle_t xTimer ){
    static int timer_led_state = 1;
    static bool initialized = false;
    if( !initialized ){
        gpio_set_level( LED3_GPIO, 0 );
        initialized = true;
    }
    timer_led_state = !timer_led_state;
    gpio_set_level( LED3_GPIO, timer_led_state );
    ESP_LOGI( TAG, "[%lu] xTimer LED %s", esp_log_timestamp(), timer_led_state ? "ON" : "OFF" );
}

void app_main( void ){
    init_gpio_output( LED1_GPIO );
    init_gpio_output( LED2_GPIO );
    init_gpio_output( LED3_GPIO );

    static gpio_num_t LED1_PIN = LED1_GPIO;
    static gpio_num_t LED2_PIN = LED2_GPIO;

    xTaskCreate( vtaskdelay_blink, "vtaskdelay_blink", 2048,(void*)&LED1_PIN, 0, NULL );
    xTaskCreate( vtaskdelayuntil_blink, "vtaskdelayuntil_blink", 2048, (void*)&LED2_PIN, 0, NULL );

    TimerHandle_t led_timer = xTimerCreate(
                                                "LED3_Timer",
                                                pdMS_TO_TICKS( BLINK_PERIOD_MS / 2 ),
                                                pdTRUE,
                                                NULL,
                                                led_timer_callback
    );

    xTimerStart( led_timer, 0 );


}