/*
 * MAX7219_CH32.h - MAX7219 7-Segment Display Library for Arduino
 * 
 * A lightweight library for controlling MAX7219 LED driver ICs with 7-segment displays.
 * Optimized for memory-constrained microcontrollers like CH32V003.
 * 
 * Features:
 * - BCD decode mode (hardware handles 0-9 display)
 * - No-decode mode (software handles numbers, letters, patterns)
 * - Configurable alignment (left, right, center)
 * - Animation support (blink, fade, scroll, chase, count)
 * - Simple display functions for numbers, text, time, hex, binary
 * 
 * Author: Rithik Krisna M
 * License: MIT License
 * Version: 1.0.0
 * Created: 2025
 */

#ifndef MAX7219_CH32_H
#define MAX7219_CH32_H

#include <Arduino.h>

// ========================================
// Display Modes
// ========================================
#define MAX7219_MODE_DECODE     0xFF  // BCD decode for all digits (hardware mode)
#define MAX7219_MODE_NO_DECODE  0x00  // No decode (software handles display)

// ========================================
// Alignment Options
// ========================================
#define MAX7219_ALIGN_LEFT      0     // Left-aligned (position 0 = leftmost)
#define MAX7219_ALIGN_RIGHT     1     // Right-aligned (position 0 = rightmost)
#define MAX7219_ALIGN_CENTER    2     // Center-aligned (proportional to content size)

// ========================================
// MAX7219 Register Addresses
// ========================================
#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT1      0x02
#define MAX7219_REG_DIGIT2      0x03
#define MAX7219_REG_DIGIT3      0x04
#define MAX7219_REG_DIGIT4      0x05
#define MAX7219_REG_DIGIT5      0x06
#define MAX7219_REG_DIGIT6      0x07
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODE      0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// ========================================
// MAX7219 Class
// ========================================
/**
 * @class MAX7219
 * @brief Main class for controlling MAX7219 7-segment displays
 */
class MAX7219 {
public:
  /**
   * @brief Constructor for MAX7219 display
   * @param dinPin Data input pin (MOSI)
   * @param clkPin Clock pin (SCK)
   * @param csPin Chip select pin (CS/SS)
   * @param numDigits Number of digits (1-8, default: 8)
   */
  MAX7219(uint8_t dinPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits = 8);
  
  // ========================================
  // Initialization
  // ========================================
  /**
   * @brief Initialize the display with default mode
   */
  void begin();
  
  /**
   * @brief Initialize the display with specific mode
   * @param mode Display mode (MAX7219_MODE_DECODE or MAX7219_MODE_NO_DECODE)
   */
  void begin(uint8_t mode);
  
  // ========================================
  // Configuration
  // ========================================
  /**
   * @brief Set decode mode
   * @param mode Display mode (MAX7219_MODE_DECODE or MAX7219_MODE_NO_DECODE)
   */
  void setMode(uint8_t mode);
  
  /**
   * @brief Get current mode
   * @return Current display mode
   */
  uint8_t getMode();
  
  /**
   * @brief Set text alignment
   * @param alignment Alignment mode (MAX7219_ALIGN_LEFT, MAX7219_ALIGN_RIGHT, or MAX7219_ALIGN_CENTER)
   */
  void setAlignment(uint8_t alignment);
  
  /**
   * @brief Get current alignment
   * @return Current alignment mode
   */
  uint8_t getAlignment();
  
  /**
   * @brief Set display brightness
   * @param level Brightness level (0-15, where 0 is dimmest and 15 is brightest)
   */
  void setBrightness(uint8_t level);
  
  /**
   * @brief Turn display on or off
   * @param on true to turn on, false to turn off
   */
  void setPower(bool on);
  
  /**
   * @brief Clear all digits
   */
  void clear();
  
  // ========================================
  // Display Functions (Mode-Aware)
  // ========================================
  /**
   * @brief Display text string
   * @param text Text to display (null-terminated string)
   */
  void display(const char* text);
  
  /**
   * @brief Display integer
   * @param number Integer value to display
   */
  void display(int number);
  
  /**
   * @brief Display long number
   * @param number Long value to display
   */
  void display(long number);
  
  /**
   * @brief Display floating point number
   * @param number Floating point value to display
   * @param decimals Number of decimal places (default: 2)
   */
  void display(double number, uint8_t decimals = 2);
  
  /**
   * @brief Display value at specific position
   * @param position Digit position (0-based)
   * @param value Numeric value (0-9)
   * @param dp true to show decimal point (default: false)
   */
  void displayAt(uint8_t position, uint8_t value, bool dp = false);
  
  /**
   * @brief Display character at specific position
   * @param position Digit position (0-based)
   * @param character Character to display
   * @param dp true to show decimal point (default: false)
   */
  void displayAt(uint8_t position, char character, bool dp = false);
  
  // ========================================
  // Special Formats
  // ========================================
  /**
   * @brief Display time in HH:MM format
   * @param hours Hours (0-99)
   * @param minutes Minutes (0-59)
   */
  void displayTime(uint8_t hours, uint8_t minutes);
  
  /**
   * @brief Display time in HH:MM:SS format
   * @param hours Hours (0-99)
   * @param minutes Minutes (0-59)
   * @param seconds Seconds (0-59)
   */
  void displayTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
  
  /**
   * @brief Display value in hexadecimal format
   * @param value Value to display in hex
   */
  void displayHex(uint32_t value);
  
  /**
   * @brief Display value in binary format
   * @param value Value to display in binary (requires 8 digits)
   */
  void displayBinary(uint8_t value);
  
  // ========================================
  // Animations
  // ========================================
  /**
   * @brief Blink the display
   * @param times Number of blinks (default: 3)
   * @param delayMs Delay between blinks in milliseconds (default: 500)
   */
  void blink(uint8_t times = 3, uint16_t delayMs = 500);
  
  /**
   * @brief Fade in animation
   * @param delayMs Delay between brightness steps in milliseconds (default: 50)
   */
  void fadeIn(uint16_t delayMs = 50);
  
  /**
   * @brief Fade out animation
   * @param delayMs Delay between brightness steps in milliseconds (default: 50)
   */
  void fadeOut(uint16_t delayMs = 50);
  
  /**
   * @brief Scroll text across display
   * @param text Text to scroll
   * @param delayMs Delay between scroll steps in milliseconds (default: 200)
   */
  void scroll(const char* text, uint16_t delayMs = 200);
  
  /**
   * @brief Scroll number across display
   * @param number Number to scroll
   * @param delayMs Delay between scroll steps in milliseconds (default: 200)
   */
  void scroll(long number, uint16_t delayMs = 200);
  
  /**
   * @brief Chase animation (digit-by-digit)
   * @param delayMs Delay between steps in milliseconds (default: 100)
   */
  void chase(uint16_t delayMs = 100);
  
  /**
   * @brief Count up animation
   * @param from Starting value
   * @param to Ending value
   * @param delayMs Delay between counts in milliseconds (default: 100)
   */
  void countUp(long from, long to, uint16_t delayMs = 100);
  
  /**
   * @brief Count down animation
   * @param from Starting value
   * @param to Ending value
   * @param delayMs Delay between counts in milliseconds (default: 100)
   */
  void countDown(long from, long to, uint16_t delayMs = 100);
  
  // ========================================
  // Low-Level Control
  // ========================================
  /**
   * @brief Set raw segment pattern at position
   * @param position Digit position (0-based)
   * @param segments Bit pattern for segments (DP-A-B-C-D-E-F-G)
   */
  void setSegments(uint8_t position, uint8_t segments);
  
  /**
   * @brief Set raw digit value at position
   * @param position Digit position (0-based)
   * @param value Raw digit value
   * @param dp true to show decimal point (default: false)
   */
  void setRawDigit(uint8_t position, uint8_t value, bool dp = false);
  
private:
  // Pin configuration
  uint8_t _dinPin;
  uint8_t _clkPin;
  uint8_t _csPin;
  
  // Display configuration
  uint8_t _numDigits;
  uint8_t _mode;
  uint8_t _alignment;
  uint8_t _brightness;
  
  // Internal functions
  void sendCommand(uint8_t address, uint8_t data);
  void sendByte(uint8_t data);
  void sendBit(uint8_t bit);
  uint8_t getPhysicalPosition(uint8_t logicalPosition);
  uint8_t charToSegments(char c);
  uint8_t digitToSegments(uint8_t digit);
  void displayNumberInternal(long number, bool leadingZeros = false);
  void displayFloatInternal(double number, uint8_t decimals);
  void displayTextInternal(const char* text);
};

#endif // MAX7219_CH32_H

