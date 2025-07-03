#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Task Handles
TaskHandle_t highPriorityTaskHandle = NULL;
TaskHandle_t lowPriorityTaskHandle = NULL;

// Mutex Handle
SemaphoreHandle_t resourceMutex;

// Low-Priority Task (Holds Resource)
void LowPriorityTask(void *pvParameters) {
    while (1) {
        // Take the Mutex (Lock the Resource)
        if (xSemaphoreTake(resourceMutex, portMAX_DELAY)) {
            Serial.println("LowPriorityTask: Acquired resource. Holding for 5 seconds...");
            vTaskDelay(pdMS_TO_TICKS(5000)); // Simulating long processing

            Serial.println("LowPriorityTask: Releasing resource.");
            xSemaphoreGive(resourceMutex); // Release the resource
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to allow other tasks to run
    }
}

// High-Priority Task (Needs Resource)
void HighPriorityTask(void *pvParameters) {
    while (1) {
        Serial.println("HighPriorityTask: Waiting for resource...");

        // Try to take the mutex
        if (xSemaphoreTake(resourceMutex, portMAX_DELAY)) {
            Serial.println("HighPriorityTask: Acquired resource and processing...");
            vTaskDelay(pdMS_TO_TICKS(2000)); // Simulate processing

            Serial.println("HighPriorityTask: Releasing resource.");
            xSemaphoreGive(resourceMutex); // Release the resource
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Short delay before trying again
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000); // Allow serial monitor to initialize

    // Create a mutex with priority inheritance enabled
    resourceMutex = xSemaphoreCreateMutex();

    // Create Low-Priority Task
    xTaskCreate(
        LowPriorityTask, "LowPriorityTask",
        2048, NULL, 1, &lowPriorityTaskHandle // Low priority (1)
    );

    // Create High-Priority Task
    xTaskCreate(
        HighPriorityTask, "HighPriorityTask",
        2048, NULL, 2, &highPriorityTaskHandle // High priority (2)
    );
}

void loop() {
    // Nothing here, FreeRTOS handles tasks
}