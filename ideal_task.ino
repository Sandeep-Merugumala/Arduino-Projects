#include <Arduino.h>


// 🔹 Idle counter to track CPU idle time
volatile uint32_t idleCounter = 0;

// 🔹 Implement FreeRTOS Idle Hook
void vApplicationIdleHook() {
    idleCounter++;  // Increment idle counter when system is idle
    delay(50);  // Simulate background processing (Avoid excessive CPU usage)
}

// 🔹 Task to monitor CPU utilization
void cpuMonitorTask(void *pvParameters) {
    uint32_t prevIdleCount = 0;

    while (1) {
        uint32_t idleTime = idleCounter - prevIdleCount;  // Calculate idle time
        prevIdleCount = idleCounter;

        // 🔹 Print CPU Utilization Data
        Serial.print("Idle Task executed: ");
        Serial.print(idleTime);
        Serial.println(" times in last second.");

        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay 1 second
    }
}

// 🔹 Normal task simulating CPU load
void busyTask(void *pvParameters) {
    while (1) {
        Serial.println("Busy Task Running...");
        vTaskDelay(pdMS_TO_TICKS(500));  // Simulate work, then yield
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);  // Allow Serial Monitor to initialize

    // 🔹 Create CPU Utilization Monitoring Task
    xTaskCreate(
        cpuMonitorTask,  // Task function
        "CPU Monitor",   // Task name
        4096,            // Stack size
        NULL,            // Parameters
        1,               // Priority
        NULL             // Task handle
    );

    // 🔹 Create a busy task that consumes CPU time
    xTaskCreate(
        busyTask,   // Task function
        "Busy Task", // Task name
        2048,       // Stack size
        NULL,       // Parameters
        1,          // Priority (Higher than idle)
        NULL        // Task handle
    );
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(100));  // Prevent CPU starvation
}
