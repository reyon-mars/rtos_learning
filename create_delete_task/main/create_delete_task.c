#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_random.h"

#define LED_GPIO GPIO_NUM_2

TaskHandle_t task1_handle = NULL;

void task1(void* p) {

    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    
    int led_state = 0;
    int count = *((int*)p);

    while (1) {
        printf("The current loop iteration is %d\n", count++);
        gpio_set_level(LED_GPIO, led_state);
        led_state = !led_state;

        vTaskDelay(pdMS_TO_TICKS(1000));
        
        if( count > 100 ){
            vTaskDelete( task1_handle );
        }
    }
}

void app_main(void) {
    static int arg_value;
    arg_value = esp_random() % 10;

    xTaskCreate(&task1, "task1", 2048, &arg_value, 1, &task1_handle);

}
