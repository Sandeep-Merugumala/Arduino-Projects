#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Hardware Pins
#define ALERT_LED 2
#define EMERGENCY_BUTTON 15
#define NURSE_CALL_BUTTON 19
#define NURSE_RESPONSE_LED 21

// Task Handles
TaskHandle_t tempMonitorTaskHandle;
TaskHandle_t emergencyTaskHandle;
TaskHandle_t resourceManagementTaskHandle;
TaskHandle_t nurseCallTaskHandle;
TaskHandle_t heartRateMonitorTaskHandle;

// Function Prototypes
void tempMonitorTask(void *pvParameters);
void emergencyTask(void *pvParameters);
void resourceManagementTask(void *pvParameters);
void nurseCallTask(void *pvParameters);

void setup() {
    Serial.begin(115200);

    // Initialize hardware
    pinMode(ALERT_LED, OUTPUT);
    pinMode(NURSE_RESPONSE_LED, OUTPUT);
    pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
    pinMode(NURSE_CALL_BUTTON, INPUT_PULLUP);

    // Create FreeRTOS Tasks
    xTaskCreatePinnedToCore(tempMonitorTask, "Temperature Monitor", 2048, NULL, 2, &tempMonitorTaskHandle, 0);
    xTaskCreatePinnedToCore(emergencyTask, "Emergency Alert", 1024, NULL, 3, &emergencyTaskHandle, 1);
    xTaskCreatePinnedToCore(resourceManagementTask, "Resource Management", 2048, NULL, 2, &resourceManagementTaskHandle, 0);
    xTaskCreatePinnedToCore(nurseCallTask, "Nurse Call System", 1024, NULL, 2, &nurseCallTaskHandle, 1);
    xTaskCreatePinnedToCore(heartRateMonitorTask, "Heart Rate Monitor", 2048, NULL, 2, &heartRateMonitorTaskHandle, 0);
}

void loop() {
    // FreeRTOS handles the tasks
}

// **1️⃣ Simulated Temperature Monitoring Task**
void tempMonitorTask(void *pvParameters) {
    while (1) {
        int temp = random(35, 41); // Simulating temperature between 35°C and 40°C
        Serial.print("Simulated Temperature: "); Serial.print(temp); Serial.println(" °C");

        if (temp > 38) { // High Fever Alert
            Serial.println("⚠️ High Temperature Detected! Alerting Staff...");
            digitalWrite(ALERT_LED, HIGH);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        } else {
            digitalWrite(ALERT_LED, LOW);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS); // Check every 5 seconds
    }
}

void heartRateMonitorTask(void *pvParameters) {
    while (1) {
        int heartRate = random(50, 120); // Simulate heart rate between 50 and 120 BPM
        Serial.print("❤️ Simulated Heart Rate: ");
        Serial.print(heartRate);
        Serial.println(" BPM");

        if (heartRate < 60 || heartRate > 100) { // Check for abnormal heart rate
            Serial.println("⚠️ Abnormal Heart Rate Detected! Alerting Staff...");
            digitalWrite(ALERT_LED, HIGH);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
        } else {
            digitalWrite(ALERT_LED, LOW);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS); // Check every 5 seconds
    }
}


// **2️⃣ Emergency Button Task**
void emergencyTask(void *pvParameters) {
    while (1) {
        if (digitalRead(EMERGENCY_BUTTON) == LOW) { // Button Pressed
            Serial.println("🚨 Emergency Button Pressed! Alerting Staff...");
            digitalWrite(ALERT_LED, HIGH);
        } else { // Button Released
            digitalWrite(ALERT_LED, LOW);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Short delay to prevent bouncing issues
    }
}

// **3️⃣ Task Scheduling & Resource Management Task**
void resourceManagementTask(void *pvParameters) {
    int totalBeds = 10;
    int occupiedBeds = 0;
    
    while (1) {
        int action = random(1, 3); // Randomly choose between check-in (1) and discharge (2)

        if (action == 1 && occupiedBeds < totalBeds) { // Patient Check-In
            occupiedBeds++;
            Serial.print("🛏️ Patient Checked In. Beds Occupied: ");
            Serial.print(occupiedBeds);
            Serial.print("/");
            Serial.println(totalBeds);
        } else if (action == 2 && occupiedBeds > 0) { // Patient Discharge
            occupiedBeds--;
            Serial.print("✅ Patient Discharged. Beds Occupied: ");
            Serial.print(occupiedBeds);
            Serial.print("/");
            Serial.println(totalBeds);
        }

        vTaskDelay(4000 / portTICK_PERIOD_MS); // Manage resources every 4 seconds
    }
}

// **4️⃣ Nurse Call Task**
void nurseCallTask(void *pvParameters) {
    while (1) {
        if (digitalRead(NURSE_CALL_BUTTON) == LOW) {
            Serial.println("🛎️ Nurse Call Button Pressed! Calling Nurse...");
            for (int i = 0; i < 5; i++) { // Blink Nurse Response LED
                digitalWrite(NURSE_RESPONSE_LED, HIGH);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                digitalWrite(NURSE_RESPONSE_LED, LOW);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
