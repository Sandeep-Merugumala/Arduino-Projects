#include <Arduino.h>

void taskFunction(void *pvParameters) {
    char *message = (char *)pvParameters;  // Correct cast

    while (1) {
        Serial.println(message);  // Print the received message
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);  // Allow time for Serial Monitor to initialize

    // Debug message
    Serial.println("Starting FreeRTOS task...");

    // Define a message
    static char message[] = "Hello from FreeRTOS Task!";
    
    // Create the task and pass the message as a parameter
    BaseType_t result = xTaskCreate(
        taskFunction,    // Task function
        "PrintTask",     // Name of the task
        2048,           // Increased stack size
        (void *)message, // Parameter to pass
        1,              // Priority
        NULL            // Task handle
    );

    // Check if task creation failed
    if (result != pdPASS) {
        Serial.println("Task creation failed!");
    } else {
        Serial.println("Task created successfully!");
    }
}

void loop() {
    // Nothing in loop, FreeRTOS is handling tasks
}
