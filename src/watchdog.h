#pragma once

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "homeplate.h"

/**
 * Watchdog Timer Management
 * 
 * The watchdog timer helps prevent the system from hanging indefinitely.
 * If a task doesn't reset the watchdog within the timeout period,
 * the system will automatically reboot.
 * 
 * This is crucial for embedded devices that need to run autonomously.
 */

// Watchdog timeout in seconds
#ifndef WDT_TIMEOUT_SEC
#define WDT_TIMEOUT_SEC 120  // 2 minutes - adjust based on your needs
#endif

class WatchdogManager {
private:
    static bool initialized;
    static bool enabled;

public:
    /**
     * Initialize the watchdog timer
     * @param timeout_sec Timeout in seconds (default: WDT_TIMEOUT_SEC)
     * @param panic_on_timeout If true, system panics on timeout; if false, just resets
     * @return true if successful, false otherwise
     */
    static bool init(uint32_t timeout_sec = WDT_TIMEOUT_SEC, bool panic_on_timeout = true) {
        if (initialized) {
            Serial.println("[WDT] Already initialized");
            return true;
        }

        esp_err_t result = esp_task_wdt_init(timeout_sec, panic_on_timeout);
        
        if (result == ESP_OK) {
            initialized = true;
            enabled = true;
            Serial.printf("[WDT] Watchdog initialized with %u second timeout\n", timeout_sec);
            return true;
        } else {
            Serial.printf("[WDT] Failed to initialize watchdog: %d\n", result);
            return false;
        }
    }

    /**
     * Subscribe the current task to the watchdog
     * Must be called from each task that should be monitored
     */
    static bool subscribe() {
        if (!initialized) {
            Serial.println("[WDT] Cannot subscribe: watchdog not initialized");
            return false;
        }

        esp_err_t result = esp_task_wdt_add(NULL);
        
        if (result == ESP_OK) {
            Serial.printf("[WDT] Task '%s' subscribed to watchdog\n", pcTaskGetName(NULL));
            return true;
        } else {
            Serial.printf("[WDT] Failed to subscribe task: %d\n", result);
            return false;
        }
    }

    /**
     * Unsubscribe the current task from the watchdog
     */
    static bool unsubscribe() {
        if (!initialized) {
            return false;
        }

        esp_err_t result = esp_task_wdt_delete(NULL);
        
        if (result == ESP_OK) {
            Serial.printf("[WDT] Task '%s' unsubscribed from watchdog\n", pcTaskGetName(NULL));
            return true;
        } else {
            Serial.printf("[WDT] Failed to unsubscribe task: %d\n", result);
            return false;
        }
    }

    /**
     * Reset (feed) the watchdog timer
     * Should be called periodically in each monitored task
     */
    static void reset() {
        if (initialized && enabled) {
            esp_task_wdt_reset();
        }
    }

    /**
     * Disable the watchdog (use with caution)
     */
    static void disable() {
        if (initialized) {
            enabled = false;
            Serial.println("[WDT] Watchdog disabled");
        }
    }

    /**
     * Re-enable the watchdog
     */
    static void enable() {
        if (initialized) {
            enabled = true;
            Serial.println("[WDT] Watchdog enabled");
        }
    }

    /**
     * Check if watchdog is initialized
     */
    static bool isInitialized() {
        return initialized;
    }

    /**
     * Check if watchdog is enabled
     */
    static bool isEnabled() {
        return enabled;
    }

    /**
     * Deinitialize the watchdog
     */
    static void deinit() {
        if (initialized) {
            esp_task_wdt_deinit();
            initialized = false;
            enabled = false;
            Serial.println("[WDT] Watchdog deinitialized");
        }
    }
};

// Declare static members (defined in watchdog.cpp)

/**
 * Usage example:
 * 
 * In setup():
 *   WatchdogManager::init(120);  // 2 minute timeout
 * 
 * In long-running task:
 *   void myTask(void* param) {
 *       WatchdogManager::subscribe();
 *       
 *       while(true) {
 *           // Do work...
 *           
 *           WatchdogManager::reset();  // Feed the watchdog
 *           vTaskDelay(1000 / portTICK_PERIOD_MS);
 *       }
 *   }
 * 
 * For OTA updates or other long operations:
 *   WatchdogManager::disable();
 *   // ... perform OTA update ...
 *   WatchdogManager::enable();
 */
