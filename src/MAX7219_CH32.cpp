/*
 * MAX7219_CH32.cpp - MAX7219 7-Segment Display Library Implementation
 * 
 * Implementation file for MAX7219_CH32 library
 * 
 * Author: Rithik Krisna M
 * License: MIT License
 * Version: 1.0.0
 * Created: 2025
 */

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "MAX7219_CH32.h"

// ========================================
// Segment Pattern Tables
// ========================================
// 7-segment bit pattern: DP-A-B-C-D-E-F-G
//     A
//    ---
//  F| G |B
//    ---
//  E|   |C
//    ---  DP
//     D

// Digit patterns for no-decode mode
const uint8_t SEGMENT_DIGITS[10] = {
  0b01111110,  // 0
  0b00110000,  // 1
  0b01101101,  // 2
  0b01111001,  // 3
  0b00110011,  // 4
  0b01011011,  // 5
  0b01011111,  // 6
  0b01110000,  // 7
  0b01111111,  // 8
  0b01111011   // 9
};

// Letter patterns (A-Z)
const uint8_t SEGMENT_LETTERS[26] = {
  0b01110111,  // A
  0b00011111,  // b
  0b01001110,  // C
  0b00111101,  // d
  0b01001111,  // E
  0b01000111,  // F
  0b01011110,  // G
  0b00110111,  // H
  0b00000110,  // I
  0b00111100,  // J
  0b01010111,  // K
  0b00001110,  // L
  0b01010100,  // M
  0b00010101,  // n
  0b01111110,  // O
  0b01100111,  // P
  0b01110011,  // q
  0b00000101,  // r
  0b01011011,  // S
  0b00001111,  // t
  0b00111110,  // U
  0b00011100,  // v
  0b00101010,  // W
  0b00110111,  // X
  0b00111011,  // y
  0b01101101   // Z
};

// Hex digits (A-F)
const uint8_t SEGMENT_HEX[6] = {
  0b01110111,  // A
  0b00011111,  // b
  0b01001110,  // C
  0b00111101,  // d
  0b01001111,  // E
  0b01000111   // F
};

// Special characters
#define SEGMENT_BLANK   0b00000000
#define SEGMENT_MINUS   0b00000001
#define SEGMENT_DOT     0b10000000

// ========================================
// Constructor
// ========================================
MAX7219::MAX7219(uint8_t dinPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits) {
  _dinPin = dinPin;
  _clkPin = clkPin;
  _csPin = csPin;
  _numDigits = constrain(numDigits, 1, 8);
  _mode = MAX7219_MODE_DECODE;
  _alignment = MAX7219_ALIGN_RIGHT;
  _brightness = 8;
}

// ========================================
// Initialization
// ========================================
void MAX7219::begin() {
  begin(_mode);
}

void MAX7219::begin(uint8_t mode) {
  pinMode(_dinPin, OUTPUT);
  pinMode(_clkPin, OUTPUT);
  pinMode(_csPin, OUTPUT);
  
  digitalWrite(_csPin, HIGH);
  digitalWrite(_clkPin, LOW);
  digitalWrite(_dinPin, LOW);
  
  delay(10);
  
  _mode = mode;
  
  sendCommand(MAX7219_REG_SHUTDOWN, 0x01);           // Normal operation
  sendCommand(MAX7219_REG_SCANLIMIT, _numDigits - 1); // Set digit count
  sendCommand(MAX7219_REG_DECODE, _mode);            // Set decode mode
  sendCommand(MAX7219_REG_INTENSITY, _brightness);   // Set brightness
  sendCommand(MAX7219_REG_DISPLAYTEST, 0x00);        // Normal mode
  
  clear();
}

// ========================================
// Configuration Functions
// ========================================
void MAX7219::setMode(uint8_t mode) {
  _mode = mode;
  sendCommand(MAX7219_REG_DECODE, mode);
  clear();
}

uint8_t MAX7219::getMode() {
  return _mode;
}

void MAX7219::setAlignment(uint8_t alignment) {
  _alignment = alignment;
}

uint8_t MAX7219::getAlignment() {
  return _alignment;
}

void MAX7219::setBrightness(uint8_t level) {
  _brightness = constrain(level, 0, 15);
  sendCommand(MAX7219_REG_INTENSITY, _brightness);
}

void MAX7219::setPower(bool on) {
  sendCommand(MAX7219_REG_SHUTDOWN, on ? 0x01 : 0x00);
}

void MAX7219::clear() {
  for (uint8_t i = 0; i < _numDigits; i++) {
    if (_mode == MAX7219_MODE_DECODE) {
      sendCommand(MAX7219_REG_DIGIT0 + i, 0x0F);  // Blank in decode mode
    } else {
      sendCommand(MAX7219_REG_DIGIT0 + i, 0x00);  // All segments off
    }
  }
}

// ========================================
// Display Functions (Mode-Aware)
// ========================================
void MAX7219::display(const char* text) {
  if (_mode == MAX7219_MODE_DECODE) {
    // In decode mode, try to parse as number
    long num = atol(text);
    displayNumberInternal(num, false);
  } else {
    // In no-decode mode, display as text
    displayTextInternal(text);
  }
}

void MAX7219::display(int number) {
  displayNumberInternal((long)number, false);
}

void MAX7219::display(long number) {
  displayNumberInternal(number, false);
}

void MAX7219::display(double number, uint8_t decimals) {
  displayFloatInternal((float)number, decimals);
}

void MAX7219::displayAt(uint8_t position, uint8_t value, bool dp) {
  if (position >= _numDigits) return;
  
  uint8_t physPos = getPhysicalPosition(position);
  
  if (_mode == MAX7219_MODE_DECODE) {
    // BCD mode: hardware handles display
    uint8_t data = constrain(value, 0, 9);
    if (dp) data |= 0x80;
    sendCommand(MAX7219_REG_DIGIT0 + physPos, data);
  } else {
    // No-decode mode: use segment patterns
    uint8_t segments = (value <= 9) ? digitToSegments(value) : SEGMENT_BLANK;
    if (dp) segments |= SEGMENT_DOT;
    sendCommand(MAX7219_REG_DIGIT0 + physPos, segments);
  }
}

void MAX7219::displayAt(uint8_t position, char character, bool dp) {
  if (position >= _numDigits) return;
  
  uint8_t physPos = getPhysicalPosition(position);
  
  if (_mode == MAX7219_MODE_DECODE) {
    // BCD mode: only supports digits
    if (character >= '0' && character <= '9') {
      uint8_t data = character - '0';
      if (dp) data |= 0x80;
      sendCommand(MAX7219_REG_DIGIT0 + physPos, data);
    } else if (character == '-') {
      sendCommand(MAX7219_REG_DIGIT0 + physPos, 0x0A);
    } else {
      sendCommand(MAX7219_REG_DIGIT0 + physPos, 0x0F);  // Blank
    }
  } else {
    // No-decode mode: supports all characters
    uint8_t segments = charToSegments(character);
    if (dp) segments |= SEGMENT_DOT;
    sendCommand(MAX7219_REG_DIGIT0 + physPos, segments);
  }
}

// ========================================
// Special Format Functions
// ========================================
void MAX7219::displayTime(uint8_t hours, uint8_t minutes) {
  if (_numDigits < 4) return;
  
  hours = constrain(hours, 0, 99);
  minutes = constrain(minutes, 0, 59);
  
  uint8_t startPos;
  if (_alignment == MAX7219_ALIGN_LEFT) {
    startPos = 0;
  } else if (_alignment == MAX7219_ALIGN_CENTER) {
    startPos = (_numDigits - 4) / 2;
  } else {  // RIGHT
    startPos = _numDigits - 4;
  }
  
  displayAt(startPos + 0, (uint8_t)(hours / 10));
  displayAt(startPos + 1, (uint8_t)(hours % 10), true);  // DP as colon
  displayAt(startPos + 2, (uint8_t)(minutes / 10));
  displayAt(startPos + 3, (uint8_t)(minutes % 10));
}

void MAX7219::displayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  if (_numDigits < 6) return;
  
  hours = constrain(hours, 0, 99);
  minutes = constrain(minutes, 0, 59);
  seconds = constrain(seconds, 0, 59);
  
  uint8_t startPos;
  if (_alignment == MAX7219_ALIGN_LEFT) {
    startPos = 0;
  } else if (_alignment == MAX7219_ALIGN_CENTER) {
    startPos = (_numDigits - 6) / 2;
  } else {  // RIGHT
    startPos = _numDigits - 6;
  }
  
  displayAt(startPos + 0, (uint8_t)(hours / 10));
  displayAt(startPos + 1, (uint8_t)(hours % 10), true);
  displayAt(startPos + 2, (uint8_t)(minutes / 10));
  displayAt(startPos + 3, (uint8_t)(minutes % 10), true);
  displayAt(startPos + 4, (uint8_t)(seconds / 10));
  displayAt(startPos + 5, (uint8_t)(seconds % 10));
}

void MAX7219::displayHex(uint32_t value) {
  if (_mode == MAX7219_MODE_DECODE) {
    setMode(MAX7219_MODE_NO_DECODE);  // Switch to no-decode for hex
  }
  
  for (int i = _numDigits - 1; i >= 0; i--) {
    uint8_t digit = value & 0xF;
    uint8_t physPos = getPhysicalPosition(i);
    
    if (digit < 10) {
      sendCommand(MAX7219_REG_DIGIT0 + physPos, SEGMENT_DIGITS[digit]);
    } else {
      sendCommand(MAX7219_REG_DIGIT0 + physPos, SEGMENT_HEX[digit - 10]);
    }
    
    value >>= 4;
    if (value == 0 && i > 0) {
      // Clear leading zeros
      for (int j = i - 1; j >= 0; j--) {
        uint8_t pos = getPhysicalPosition(j);
        sendCommand(MAX7219_REG_DIGIT0 + pos, SEGMENT_BLANK);
      }
      break;
    }
  }
}

void MAX7219::displayBinary(uint8_t value) {
  if (_numDigits < 8) return;
  
  for (int i = 0; i < 8; i++) {
    uint8_t bit = (value >> (7 - i)) & 1;
    displayAt(i, bit);
  }
}

// ========================================
// Animation Functions
// ========================================
void MAX7219::blink(uint8_t times, uint16_t delayMs) {
  for (uint8_t i = 0; i < times; i++) {
    setPower(false);
    delay(delayMs);
    setPower(true);
    delay(delayMs);
  }
}

void MAX7219::fadeIn(uint16_t delayMs) {
  uint8_t oldBrightness = _brightness;
  for (uint8_t b = 0; b <= 15; b++) {
    setBrightness(b);
    delay(delayMs);
  }
  setBrightness(oldBrightness);
}

void MAX7219::fadeOut(uint16_t delayMs) {
  uint8_t oldBrightness = _brightness;
  for (int b = _brightness; b >= 0; b--) {
    setBrightness(b);
    delay(delayMs);
  }
  setBrightness(oldBrightness);
}

void MAX7219::scroll(const char* text, uint16_t delayMs) {
  int len = strlen(text);
  
  for (int offset = _numDigits - 1; offset >= -len; offset--) {
    clear();
    
    for (int i = 0; i < len; i++) {
      int pos = offset + i;
      if (pos >= 0 && pos < _numDigits) {
        displayAt(pos, text[i]);
      }
    }
    
    delay(delayMs);
  }
}

void MAX7219::scroll(long number, uint16_t delayMs) {
  char buffer[16];
  sprintf(buffer, "%ld", number);
  scroll(buffer, delayMs);
}

void MAX7219::chase(uint16_t delayMs) {
  for (uint8_t i = 0; i < _numDigits; i++) {
    clear();
    displayAt(i, (uint8_t)8);  // Display "8" to light all segments
    delay(delayMs);
  }
  
  for (int i = _numDigits - 1; i >= 0; i--) {
    clear();
    displayAt(i, (uint8_t)8);
    delay(delayMs);
  }
}

void MAX7219::countUp(long from, long to, uint16_t delayMs) {
  if (from > to) return;
  
  for (long i = from; i <= to; i++) {
    display(i);
    delay(delayMs);
  }
}

void MAX7219::countDown(long from, long to, uint16_t delayMs) {
  if (from < to) return;
  
  for (long i = from; i >= to; i--) {
    display(i);
    delay(delayMs);
  }
}

// ========================================
// Low-Level Control
// ========================================
void MAX7219::setSegments(uint8_t position, uint8_t segments) {
  if (position >= _numDigits) return;
  uint8_t physPos = getPhysicalPosition(position);
  sendCommand(MAX7219_REG_DIGIT0 + physPos, segments);
}

void MAX7219::setRawDigit(uint8_t position, uint8_t value, bool dp) {
  if (position >= _numDigits) return;
  uint8_t physPos = getPhysicalPosition(position);
  uint8_t data = value;
  if (dp) data |= 0x80;
  sendCommand(MAX7219_REG_DIGIT0 + physPos, data);
}

// ========================================
// Internal Helper Functions
// ========================================
void MAX7219::sendCommand(uint8_t address, uint8_t data) {
  digitalWrite(_csPin, LOW);
  delayMicroseconds(1);
  sendByte(address);
  sendByte(data);
  delayMicroseconds(1);
  digitalWrite(_csPin, HIGH);
  delayMicroseconds(1);
}

void MAX7219::sendByte(uint8_t data) {
  for (int i = 7; i >= 0; i--) {
    sendBit((data >> i) & 0x01);
  }
}

void MAX7219::sendBit(uint8_t bit) {
  digitalWrite(_dinPin, bit ? HIGH : LOW);
  delayMicroseconds(1);
  digitalWrite(_clkPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_clkPin, LOW);
}

uint8_t MAX7219::getPhysicalPosition(uint8_t logicalPosition) {
  if (_alignment == MAX7219_ALIGN_LEFT) {
    return _numDigits - 1 - logicalPosition;  // Left-to-right
  } else if (_alignment == MAX7219_ALIGN_RIGHT) {
    return logicalPosition;  // Right-to-left
  } else {  // CENTER
    // For center alignment, treat logical positions same as left alignment
    return _numDigits - 1 - logicalPosition;
  }
}

uint8_t MAX7219::charToSegments(char c) {
  if (c >= '0' && c <= '9') {
    return SEGMENT_DIGITS[c - '0'];
  } else if (c >= 'A' && c <= 'Z') {
    return SEGMENT_LETTERS[c - 'A'];
  } else if (c >= 'a' && c <= 'z') {
    return SEGMENT_LETTERS[c - 'a'];
  } else if (c == '-') {
    return SEGMENT_MINUS;
  } else if (c == ' ') {
    return SEGMENT_BLANK;
  }
  return SEGMENT_BLANK;
}

uint8_t MAX7219::digitToSegments(uint8_t digit) {
  if (digit <= 9) {
    return SEGMENT_DIGITS[digit];
  }
  return SEGMENT_BLANK;
}

void MAX7219::displayNumberInternal(long number, bool leadingZeros) {
  bool negative = number < 0;
  if (negative) number = -number;
  
  // Clear display first
  clear();
  
  // Collect digits in reverse order (least significant first)
  uint8_t digits[8];
  uint8_t numDigits = 0;
  
  if (number == 0) {
    digits[0] = 0;
    numDigits = 1;
  } else {
    while (number > 0 && numDigits < 8) {
      digits[numDigits++] = number % 10;
      number /= 10;
    }
  }
  
  // Display digits in correct order
  uint8_t lastPos = 0;
  bool hasShownDigit = false;
  
  // Calculate starting position based on alignment
  uint8_t startPos;
  if (_alignment == MAX7219_ALIGN_RIGHT) {
    // Right align: fill from rightmost positions
    startPos = 0;
  } else if (_alignment == MAX7219_ALIGN_CENTER) {
    // Center align: calculate center position
    startPos = (_numDigits - numDigits) / 2;
  } else {  // LEFT
    // Left align: fill from leftmost positions
    startPos = 0;
  }
  
  // Display digits in correct order
  for (int i = numDigits - 1; i >= 0; i--) {
    uint8_t pos;
    if (_alignment == MAX7219_ALIGN_RIGHT) {
      // Right align: most significant (i=4, digit 1) goes to logical position 4, least (i=0, digit 5) to logical 0
      pos = i;  // i=4→pos=4, i=3→pos=3, ..., i=0→pos=0
    } else if (_alignment == MAX7219_ALIGN_CENTER) {
      // Center align: most significant digit at startPos, then increment
      pos = startPos + (numDigits - 1 - i);
    } else {  // LEFT
      // Left align: most significant (i=4, digit 1) goes to logical position 0, least (i=0, digit 5) to logical 4
      pos = numDigits - 1 - i;  // i=4→pos=0, i=3→pos=1, ..., i=0→pos=4
    }
    
    if (!leadingZeros && !hasShownDigit && digits[i] == 0 && i > 0) {
      // Skip leading zeros
        uint8_t physPos = getPhysicalPosition(pos);
        if (_mode == MAX7219_MODE_DECODE) {
          sendCommand(MAX7219_REG_DIGIT0 + physPos, 0x0F);
        } else {
          sendCommand(MAX7219_REG_DIGIT0 + physPos, SEGMENT_BLANK);
      }
    } else {
      displayAt(pos, digits[i]);
      hasShownDigit = true;
      lastPos = pos;
    }
  }
  
  // Display negative sign at the leftmost position if needed
  if (negative && hasShownDigit) {
    uint8_t leftmostPos;
    if (_alignment == MAX7219_ALIGN_LEFT) {
      leftmostPos = 0;
    } else if (_alignment == MAX7219_ALIGN_CENTER) {
      // For center, place negative sign one position before the number
      leftmostPos = (startPos > 0) ? startPos - 1 : 0;
    } else {  // RIGHT
      leftmostPos = _numDigits - 1;
    }
    if (leftmostPos != lastPos && leftmostPos < _numDigits) {
      displayAt(leftmostPos, '-');
    }
  }
}

void MAX7219::displayFloatInternal(double number, uint8_t decimals) {
  if (decimals >= _numDigits) decimals = _numDigits - 1;
  
  // Clear display first
  clear();
  
  long multiplier = 1;
  for (uint8_t i = 0; i < decimals; i++) multiplier *= 10;
  
  long intValue = (long)(number * multiplier);
  bool negative = intValue < 0;
  if (negative) intValue = -intValue;
  
  // Collect digits in reverse order (least significant first)
  uint8_t digits[8];
  uint8_t numDigits = 0;
  
  if (intValue == 0) {
    digits[0] = 0;
    numDigits = 1;
  } else {
    while (intValue > 0 && numDigits < 8) {
      digits[numDigits++] = intValue % 10;
      intValue /= 10;
    }
  }
  
  // Display digits in correct order
  uint8_t lastPos = 0;
  bool hasShownDigit = false;
  
  // Calculate starting position based on alignment
  uint8_t startPos;
  if (_alignment == MAX7219_ALIGN_RIGHT) {
    startPos = 0;
  } else if (_alignment == MAX7219_ALIGN_CENTER) {
    startPos = (_numDigits - numDigits) / 2;
  } else {  // LEFT
    startPos = 0;
  }
  
  for (int i = numDigits - 1; i >= 0; i--) {
    uint8_t pos;
    if (_alignment == MAX7219_ALIGN_RIGHT) {
      pos = i;
    } else if (_alignment == MAX7219_ALIGN_CENTER) {
      pos = startPos + (numDigits - 1 - i);
    } else {  // LEFT
      pos = numDigits - 1 - i;
    }
    bool showDP = (numDigits - 1 - i == decimals);
    displayAt(pos, digits[i], showDP);
    hasShownDigit = true;
    lastPos = pos;
  }
  
  // Display negative sign at the leftmost position if needed
  if (negative && hasShownDigit) {
    uint8_t leftmostPos;
    if (_alignment == MAX7219_ALIGN_LEFT) {
      leftmostPos = 0;
    } else if (_alignment == MAX7219_ALIGN_CENTER) {
      // For center, place negative sign one position before the number
      leftmostPos = (startPos > 0) ? startPos - 1 : 0;
    } else {  // RIGHT
      leftmostPos = _numDigits - 1;
    }
    if (leftmostPos != lastPos && leftmostPos < _numDigits) {
      displayAt(leftmostPos, '-');
    }
  }
}

void MAX7219::displayTextInternal(const char* text) {
  int len = strlen(text);
  int textStartIdx = 0;
  int logicalStartPos = 0;
  
  clear();
  
  if (_alignment == MAX7219_ALIGN_RIGHT) {
    // For right alignment: text starts at logical position 0 (rightmost)
    if (len <= _numDigits) {
      // Text fits: display from rightmost positions
      logicalStartPos = 0;
      textStartIdx = 0;
      // Display text in reverse order so first char appears on left, last char on right
      for (int i = 0; i < len && i < _numDigits; i++) {
        displayAt(len - 1 - i, text[i]);
      }
    } else {
      // Text is longer: show last _numDigits characters
      textStartIdx = len - _numDigits;
      for (int i = 0; i < _numDigits; i++) {
        displayAt(_numDigits - 1 - i, text[textStartIdx + i]);
      }
    }
  } else if (_alignment == MAX7219_ALIGN_CENTER) {
    // For center alignment: calculate center position
    if (len <= _numDigits) {
      logicalStartPos = (_numDigits - len) / 2;
      textStartIdx = 0;
      for (int i = 0; i < len && i < _numDigits; i++) {
        displayAt(logicalStartPos + i, text[textStartIdx + i]);
      }
    } else {
      // Text is longer: show last _numDigits characters centered
      logicalStartPos = 0;
      textStartIdx = len - _numDigits;
      for (int i = 0; i < _numDigits; i++) {
        displayAt(logicalStartPos + i, text[textStartIdx + i]);
      }
    }
  } else {  // LEFT
    // For left alignment: text starts at logical position 0 (leftmost)
    logicalStartPos = 0;
    textStartIdx = 0;
    for (int i = 0; i < _numDigits && (textStartIdx + i) < len; i++) {
      displayAt(logicalStartPos + i, text[textStartIdx + i]);
    }
  }
  
  // For left alignment with short text, remaining positions are already blank (from clear())
  // For right alignment with short text, clear() already blanked everything, then we display
  // starting from logicalStartPos which handles the padding
}

