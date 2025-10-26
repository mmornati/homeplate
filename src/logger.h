#pragma once

#include <Arduino.h>
#include "homeplate.h"

/**
 * Logging system with different severity levels
 * Provides structured logging for better debugging and monitoring
 */

enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_CRITICAL = 4
};

// Global log level - can be changed at runtime
#ifndef GLOBAL_LOG_LEVEL
    #ifdef DEBUG
        #define GLOBAL_LOG_LEVEL LOG_LEVEL_DEBUG
    #else
        #define GLOBAL_LOG_LEVEL LOG_LEVEL_INFO
    #endif
#endif

class Logger {
private:
    static LogLevel currentLevel;
    
    static const char* getLevelString(LogLevel level) {
        switch (level) {
            case LOG_LEVEL_DEBUG:    return "DEBUG";
            case LOG_LEVEL_INFO:     return "INFO";
            case LOG_LEVEL_WARNING:  return "WARN";
            case LOG_LEVEL_ERROR:    return "ERROR";
            case LOG_LEVEL_CRITICAL: return "CRIT";
            default:                 return "UNKNOWN";
        }
    }
    
    static void log(LogLevel level, const char* tag, const char* format, va_list args) {
        if (level < currentLevel) {
            return;
        }
        
        // Format: [LEVEL][TAG] message
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), format, args);
        Serial.printf("[%s][%s] %s\n", getLevelString(level), tag, buffer);
    }

public:
    static void setLevel(LogLevel level) {
        currentLevel = level;
    }
    
    static LogLevel getLevel() {
        return currentLevel;
    }
    
    static void debug(const char* tag, const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LOG_LEVEL_DEBUG, tag, format, args);
        va_end(args);
    }
    
    static void info(const char* tag, const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LOG_LEVEL_INFO, tag, format, args);
        va_end(args);
    }
    
    static void warning(const char* tag, const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LOG_LEVEL_WARNING, tag, format, args);
        va_end(args);
    }
    
    static void error(const char* tag, const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LOG_LEVEL_ERROR, tag, format, args);
        va_end(args);
    }
    
    static void critical(const char* tag, const char* format, ...) {
        va_list args;
        va_start(args, format);
        log(LOG_LEVEL_CRITICAL, tag, format, args);
        va_end(args);
    }
};

// Declare static member (defined in logger.cpp)

/**
 * Convenience macros for logging
 * 
 * Usage:
 *   LOG_DEBUG("WIFI", "Connecting to %s", ssid);
 *   LOG_INFO("MQTT", "Connected to broker");
 *   LOG_WARNING("BATTERY", "Low battery: %d%%", percent);
 *   LOG_ERROR("DISPLAY", "Failed to render image");
 *   LOG_CRITICAL("SYSTEM", "Out of memory!");
 */
#define LOG_DEBUG(tag, ...) Logger::debug(tag, __VA_ARGS__)
#define LOG_INFO(tag, ...) Logger::info(tag, __VA_ARGS__)
#define LOG_WARNING(tag, ...) Logger::warning(tag, __VA_ARGS__)
#define LOG_ERROR(tag, ...) Logger::error(tag, __VA_ARGS__)
#define LOG_CRITICAL(tag, ...) Logger::critical(tag, __VA_ARGS__)

// Helper macro for deprecation warnings
#define LOG_DEPRECATED(tag, old_func, new_func) \
    LOG_WARNING(tag, "%s is deprecated, use %s instead", old_func, new_func)
