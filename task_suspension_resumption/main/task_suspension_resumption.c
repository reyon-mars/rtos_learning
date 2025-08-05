#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2
#define TAG      "TASK_CTRL"

TaskHandle_t led_task_handle = NULL;

void led_task( void* pv_parameteres ){

    gpio_set_direction( LED_GPIO, GPIO_MODE_OUTPUT );

    while ((1))
    {
        gpio_set_level( LED_GPIO, 1 );
        vTaskDelay( pdMS_TO_TICKS(500) );
        gpio_set_level( LED_GPIO, 0 );
        vTaskDelay( pdMS_TO_TICKS(500) );
    }
    
}

void controller_task( void* pv_parameters ){
    while( 1 ){
        vTaskDelay( pdMS_TO_TICKS(5000) );

        if( eTaskGetState( led_task_handle ) != eSuspended ){
            ESP_LOGI( TAG , "Suspending LED task....");
            vTaskSuspend( led_task_handle );
        }

        vTaskDelay( pdMS_TO_TICKS( 5000 ));

        if( eTaskGetState( led_task_handle ) == eSuspended ){
            ESP_LOGI( TAG, "Resuming LED task....");
            vTaskResume( led_task_handle );
        }

    }
}

void app_main(void)
{
    xTaskCreate( led_task, "LED_TASK", 2048, NULL, 1, &led_task_handle );
    xTaskCreate( controller_task, "CONTROLLER_TASK", 2048, NULL, 1, NULL );

}
