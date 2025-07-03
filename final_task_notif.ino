#include <Arduino.h>

TaskHandle_t consumerTaskHandle = NULL; // Task handle for consumer

// Producer Task
void producerTask(void *pvParameters) {
    while (1) {
        Serial.println("Producer: Sending notification...");
        
        // Notify the consumer task
        xTaskNotifyGive(consumerTaskHandle);

        // Wait before producing the next item
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds delay
    }
}

// Consumer Task
void consumerTask(void *pvParameters) {
    while (1) {
        // Wait indefinitely for a notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Process the received notification
        Serial.println("Consumer: Received and processing notification.");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000); // Allow serial monitor to initialize

    // Create Consumer Task first
    xTaskCreate(
        consumerTask,       // Task function
        "ConsumerTask",     // Task name
        2048,               // Stack size
        NULL,               // Parameters
        2,                  // Priority
        &consumerTaskHandle // Store task handle
    );

    // Create Producer Task
    xTaskCreate(
        producerTask,       // Task function
        "ProducerTask",     // Task name
        2048,               // Stack size
        NULL,               // Parameters
        1,                  // Priority
        NULL                // Task handle
    );
}

void loop() {
    // Nothing in loop, FreeRTOS manages tasks
}
