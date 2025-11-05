/*
  BasicExample
  
  Simple demonstration of the MAX7219_CH32 library.
  This example shows the most basic usage - displaying numbers and text.
  
  Circuit:
  - MAX7219 DIN pin connected to PC6 (CH32V003) or pin 12 (Arduino)
  - MAX7219 CLK pin connected to PC5 (CH32V003) or pin 11 (Arduino)
  - MAX7219 CS pin connected to PC4 (CH32V003) or pin 10 (Arduino)
  - MAX7219 VCC connected to 5V
  - MAX7219 GND connected to GND
  - 7-segment display connected to MAX7219
  
  Created: 2025
  By: Rithik Krisna M <me-RK>
*/

#include <MAX7219_CH32.h>

// ========================================
// Pin Configuration
// ========================================
// Create display instance: DIN, CLK, CS, Number of digits
// 
// For CH32V003 (Recommended):
//   DIN = PC6
//   CLK = PC5
//   CS  = PC4
//
// For Standard Arduino:
//   DIN = pin 12
//   CLK = pin 11
//   CS  = pin 10
//
// Number of digits: 1-8 (default: 8)
MAX7219 display(PC6, PC5, PC4, 8);  // CH32V003 pins (change to 12, 11, 10 for Arduino)

void setup() {
  // Initialize the display
  // begin() initializes the display with default BCD decode mode
  // This mode is optimized for displaying numbers (0-9)
  display.begin();
  
  // Set display brightness (0-15)
  // 0 = dimmest, 15 = brightest
  // Recommended: 8 (medium brightness)
  display.setBrightness(8);
}

void loop() {
  // ========================================
  // Display a Number
  // ========================================
  // Display an integer number
  // The display will automatically use BCD decode mode for numbers
  display.display(12345);
  delay(2000);  // Wait 2 seconds
  
  // ========================================
  // Display Text
  // ========================================
  // Switch to no-decode mode for displaying text and letters
  // This mode allows displaying characters A-Z, 0-9, and some special characters
  display.setMode(MAX7219_MODE_NO_DECODE);
  
  // Display a text string
  // Only characters A-Z, 0-9, and some special chars are supported
  display.display("HELLO");
  delay(2000);  // Wait 2 seconds
  
  // ========================================
  // Display Another Number
  // ========================================
  // Switch back to decode mode for displaying numbers
  // Decode mode is more efficient for numbers and uses hardware BCD decoding
  display.setMode(MAX7219_MODE_DECODE);
  
  // Display another number
  display.display(9876);
  delay(2000);  // Wait 2 seconds
  
  // ========================================
  // Clear Display
  // ========================================
  // Clear all digits on the display
  // All segments will be turned off
  display.clear();
  delay(1000);  // Wait 1 second before repeating
  
  // The loop will repeat from the beginning
}
