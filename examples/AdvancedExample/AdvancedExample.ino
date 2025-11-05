/*
  AdvancedExample
  
  Comprehensive demonstration of the MAX7219_CH32 library.
  Shows all features including display modes, alignment options, animations, 
  special formats, and low-level control.
  
  Circuit:
  - MAX7219 DIN pin connected to PC6 (CH32V003) or pin 12 (Arduino)
  - MAX7219 CLK pin connected to PC5 (CH32V003) or pin 11 (Arduino)
  - MAX7219 CS pin connected to PC4 (CH32V003) or pin 10 (Arduino)
  - MAX7219 VCC connected to 5V
  - MAX7219 GND connected to GND
  - 7-segment display connected to MAX7219
  
  Created: 2025
  By: Rithik Krisna M
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
  // Initialize the display in BCD decode mode
  // MAX7219_MODE_DECODE: Hardware handles 0-9 digit display (more efficient for numbers)
  // MAX7219_MODE_NO_DECODE: Software handles display (required for text/letters)
  display.begin(MAX7219_MODE_DECODE);
  
  // Set display brightness (0-15)
  // 0 = dimmest, 15 = brightest
  // Recommended: 8 (medium brightness)
  display.setBrightness(8);
  
  // Small delay to ensure display is ready
  delay(500);
}

void loop() {
  // ========================================
  // DEMO 1: Text and Number Alignment
  // ========================================
  // The library supports three alignment options:
  // - LEFT: Text/numbers start from the leftmost position
  // - RIGHT: Text/numbers start from the rightmost position (default)
  // - CENTER: Text/numbers are centered on the display
  
  // Right alignment (default)
  display.setAlignment(MAX7219_ALIGN_RIGHT);
  display.display("RIGHT");  // Text will be right-aligned
  delay(1500);
  display.display(12345);    // Numbers will be right-aligned
  delay(1500);
  
  // Left alignment
  display.setAlignment(MAX7219_ALIGN_LEFT);
  display.display("LEFT");   // Text will be left-aligned
  delay(1500);
  display.display(12345);    // Numbers will be left-aligned
  delay(1500);
  
  // Center alignment
  display.setAlignment(MAX7219_ALIGN_CENTER);
  display.display("CENTER"); // Text will be centered
  delay(1500);
  display.display(12345);    // Numbers will be centered
  delay(1500);
  
  // ========================================
  // DEMO 2: Display Modes
  // ========================================
  // There are two display modes:
  // - DECODE: Hardware BCD decode (0-9 only, more efficient)
  // - NO_DECODE: Software decode (A-Z, 0-9, special chars)
  
  display.setAlignment(MAX7219_ALIGN_LEFT);
  
  // BCD decode mode - best for numbers
  display.setMode(MAX7219_MODE_DECODE);
  display.display(12345678);  // Display 8-digit number
  delay(1500);
  
  // No-decode mode - required for text
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display("HELLO");   // Display text string
  delay(1500);
  
  // ========================================
  // DEMO 3: Displaying Different Number Types
  // ========================================
  
  // Positive integers
  display.setMode(MAX7219_MODE_DECODE);
  display.display(12345);
  delay(1000);
  
  // Negative numbers (shows minus sign)
  display.display(-1234);
  delay(1000);
  
  // Floating point numbers (requires no-decode mode)
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display(12.34, 2);  // Display 12.34 with 2 decimal places
  delay(1000);
  
  // ========================================
  // DEMO 4: Displaying Text
  // ========================================
  // Text display requires NO_DECODE mode
  // Supported characters: A-Z, 0-9, and some special characters
  
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display("TEST");  // Display text string
  delay(1000);
  display.display("ABCD");  // Display another text string
  delay(1000);
  
  // ========================================
  // DEMO 5: Displaying at Specific Positions
  // ========================================
  // You can display characters or numbers at specific digit positions
  
  // Display numbers at specific positions (decode mode)
  display.setMode(MAX7219_MODE_DECODE);
  display.clear();           // Clear all digits first
  display.displayAt(0, (uint8_t)1);  // Display '1' at position 0
  display.displayAt(1, (uint8_t)2);  // Display '2' at position 1
  display.displayAt(2, (uint8_t)3);  // Display '3' at position 2
  display.displayAt(3, (uint8_t)4);  // Display '4' at position 3
  delay(1500);
  
  // Display number with decimal point
  display.clear();
  display.displayAt(3, (uint8_t)5, true);  // Display '5' with decimal point at position 3
  delay(1000);
  
  // Display characters at specific positions (no-decode mode)
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.clear();
  display.displayAt(0, 'H');  // Display 'H' at position 0
  display.displayAt(1, 'E');  // Display 'E' at position 1
  display.displayAt(2, 'L');  // Display 'L' at position 2
  display.displayAt(3, 'L');  // Display 'L' at position 3
  display.displayAt(4, 'O');  // Display 'O' at position 4
  delay(1500);
  
  // ========================================
  // DEMO 6: Time Display
  // ========================================
  // Special function for displaying time in HH:MM or HH:MM:SS format
  // Decimal points are used as colons between hours, minutes, and seconds
  
  display.setMode(MAX7219_MODE_DECODE);
  
  // Display time in HH:MM format (requires at least 4 digits)
  display.displayTime(12, 34);  // Display "12:34"
  delay(1500);
  
  // Display time in HH:MM:SS format (requires at least 6 digits)
  display.displayTime(12, 34, 56);  // Display "12:34:56"
  delay(1500);
  
  // ========================================
  // DEMO 7: Hexadecimal and Binary Display
  // ========================================
  
  // Display hexadecimal value
  // Automatically switches to no-decode mode if needed
  display.displayHex(0x1234);  // Display "1234" in hexadecimal
  delay(1500);
  
  // Display binary value (requires 8 digits)
  display.setMode(MAX7219_MODE_DECODE);
  display.displayBinary(0b10101010);  // Display "10101010" in binary
  delay(1500);
  
  // ========================================
  // DEMO 8: Brightness Control
  // ========================================
  // Brightness can be adjusted from 0 (dimmest) to 15 (brightest)
  
  display.setMode(MAX7219_MODE_DECODE);
  display.display("BRIGHT");  // Display text to show brightness effect
  
  // Fade in: Gradually increase brightness from 0 to 15
  for (uint8_t b = 0; b <= 15; b++) {
    display.setBrightness(b);
    delay(50);  // Small delay between brightness steps
  }
  
  // Fade out: Gradually decrease brightness from 15 to 0
  for (int b = 15; b >= 0; b--) {
    display.setBrightness(b);
    delay(50);  // Small delay between brightness steps
  }
  
  // Restore medium brightness
  display.setBrightness(8);
  delay(500);
  
  // ========================================
  // DEMO 9: Power Control
  // ========================================
  // Turn display on/off programmatically
  // Useful for power saving or visual effects
  
  display.display("POWER");
  delay(1000);
  
  display.setPower(false);  // Turn display off (power save mode)
  delay(500);
  
  display.setPower(true);   // Turn display back on
  delay(500);
  
  // ========================================
  // DEMO 10: Animations
  // ========================================
  // The library provides several built-in animation functions
  
  // Blink animation: Flash the display on and off
  display.display("BLINK");
  display.blink(2, 200);  // Blink 2 times with 200ms delay
  delay(500);
  
  // Scroll animation: Scroll text across the display
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.scroll("SCROLL", 80);  // Scroll text with 80ms delay between steps
  delay(500);
  
  // Chase animation: Light up each digit one by one
  display.setMode(MAX7219_MODE_DECODE);
  display.chase(100);  // Chase animation with 100ms delay
  delay(500);
  
  // Count up animation: Count from one number to another
  display.countUp(0, 50, 30);  // Count from 0 to 50 with 30ms delay
  delay(500);
  
  // ========================================
  // DEMO 11: Low-Level Control
  // ========================================
  // Advanced functions for direct segment and digit control
  
  // Direct segment control: Set raw segment patterns
  // Each segment is controlled by a bit pattern
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.clear();
  display.setSegments(0, 0b00110111);  // Display 'H' using segment pattern
  display.setSegments(1, 0b01001110);  // Display 'C' using segment pattern
  display.setSegments(2, 0b00000110);  // Display 'I' using segment pattern
  delay(1500);
  
  // Raw digit control: Direct control of digit values
  display.setMode(MAX7219_MODE_DECODE);
  display.clear();
  display.setRawDigit(0, 1);          // Set digit 0 to '1'
  display.setRawDigit(2, 2, true);    // Set digit 2 to '2' with decimal point
  display.setRawDigit(4, 3);          // Set digit 4 to '3'
  delay(1500);
  
  // ========================================
  // Final Display
  // ========================================
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display("DONE");        // Display completion message
  delay(1500);
  display.blink(2, 300);          // Blink 2 times
  display.clear();                // Clear display
  delay(1000);
  
  // The loop will repeat from the beginning, showing all demos again
}
