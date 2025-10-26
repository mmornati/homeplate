#pragma once

#include <Arduino.h>
#include "homeplate.h"

/**
 * RAII (Resource Acquisition Is Initialization) guards for mutex management
 * These classes automatically acquire locks in constructor and release in destructor
 * This prevents forgetting to release locks and handles exceptions gracefully
 */

class I2CLock {
public:
    I2CLock() { 
        xSemaphoreTake(mutexI2C, portMAX_DELAY);
    }
    
    ~I2CLock() { 
        xSemaphoreGive(mutexI2C);
    }
    
    // Prevent copying
    I2CLock(const I2CLock&) = delete;
    I2CLock& operator=(const I2CLock&) = delete;
};

class SPILock {
public:
    SPILock() { 
        xSemaphoreTake(mutexSPI, portMAX_DELAY);
    }
    
    ~SPILock() { 
        xSemaphoreGive(mutexSPI);
    }
    
    // Prevent copying
    SPILock(const SPILock&) = delete;
    SPILock& operator=(const SPILock&) = delete;
};

class DisplayLock {
public:
    DisplayLock() { 
        xSemaphoreTake(mutexDisplay, portMAX_DELAY);
    }
    
    ~DisplayLock() { 
        xSemaphoreGive(mutexDisplay);
    }
    
    // Prevent copying
    DisplayLock(const DisplayLock&) = delete;
    DisplayLock& operator=(const DisplayLock&) = delete;
};

/**
 * Combined locks for operations that need multiple resources
 * Always acquire locks in the same order to prevent deadlocks
 */
class I2CDisplayLock {
public:
    I2CDisplayLock() {
        xSemaphoreTake(mutexI2C, portMAX_DELAY);
        xSemaphoreTake(mutexDisplay, portMAX_DELAY);
    }
    
    ~I2CDisplayLock() {
        xSemaphoreGive(mutexDisplay);
        xSemaphoreGive(mutexI2C);
    }
    
    // Prevent copying
    I2CDisplayLock(const I2CDisplayLock&) = delete;
    I2CDisplayLock& operator=(const I2CDisplayLock&) = delete;
};

/**
 * Usage example:
 * 
 * void someFunction() {
 *     I2CLock lock;  // Acquires I2C lock
 *     // ... do I2C operations ...
 *     // Lock automatically released when function exits or exception thrown
 * }
 * 
 * void displayOperation() {
 *     I2CDisplayLock lock;  // Acquires both I2C and Display locks
 *     // ... do display operations ...
 *     // Both locks released automatically
 * }
 */
