#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TASK1 "TASK1"
#define TASK2 "TASK2"

TaskHandle_t task1_handle = NULL;
TaskHandle_t task2_handle = NULL;

void task1( void* p ){

    int count = 0;
    
    while( 1 ){
        ESP_LOGI( TASK1, "Hello, World ! %d ", count++ );
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2( void* p ){
    while(true){
        vTaskDelay( pdMS_TO_TICKS( 5000 ) );
        vTaskSuspend( task1_handle );
        vTaskDelay( pdMS_TO_TICKS( 5000 ) );
        vTaskResume( task1_handle );
    }
}


void app_main(void)
{
    xTaskCreate( task1, "task1", 2048, NULL, 0, &task1_handle );
    xTaskCreate( task2, "task2", 2048, NULL, 0, &task2_handle );

}
