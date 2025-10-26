#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2
#define TAG      "LED_TIMER"

static TimerHandle_t led_timer_handle;
static bool led_state = true;

void timer_callback( TimerHandle_t xTimer )
{
    gpio_set_level( LED_GPIO, led_state );
    led_state = !led_state;
    ESP_LOGI( TAG, "Timer fired. LED is now %s", led_state ? "ON" : "OFF" );
}


void app_main(void)
{

    gpio_config_t gpio_conf = {
        .mode = GPIO_MODE_OUTPUT, 
        .pin_bit_mask = 1ULL << LED_GPIO,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config( &gpio_conf );

    ESP_LOGI( TAG, "Creating Timer....." );
    
    led_timer_handle = xTimerCreate( "LED_TIMER", pdMS_TO_TICKS( 500 ), pdTRUE, NULL, timer_callback );
    
    if( led_timer_handle == NULL )
    {
        ESP_LOGI( TAG, "Failed to create timer..... " );
        return;
    }

    ESP_LOGI( TAG, "Starting Timer.... ");
    xTimerStart( led_timer_handle, 0 );

}
