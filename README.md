# MAX7219_CH32 Library

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/me-RK/MAX7219_CH32)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Arduino-orange.svg)](https://www.arduino.cc/)
[![CH32V003](https://img.shields.io/badge/CH32V003-Optimized-brightgreen.svg)](https://www.wch-ic.com/products/CH32V003.html)
[![Arduino Uno](https://img.shields.io/badge/Arduino%20Uno-Supported-brightgreen.svg)](https://store.arduino.cc/products/arduino-uno-rev3)
[![Arduino Nano](https://img.shields.io/badge/Arduino%20Nano-Supported-brightgreen.svg)](https://store.arduino.cc/products/arduino-nano)
[![Arduino Mega](https://img.shields.io/badge/Arduino%20Mega-Supported-brightgreen.svg)](https://store.arduino.cc/products/arduino-mega-2560-rev3)
[![ESP32](https://img.shields.io/badge/ESP32-Supported-brightgreen.svg)](https://www.espressif.com/)
[![ESP8266](https://img.shields.io/badge/ESP8266-Supported-brightgreen.svg)](https://www.espressif.com/)

![MAX7219_CH32 Library](assets/title-image.png)

A lightweight Arduino library for controlling MAX7219 LED driver ICs with 7-segment displays. Optimized for memory-constrained microcontrollers like CH32V003 and other Arduino-compatible boards.

## 🎯 Primary Purpose

**The main objective of this library is to provide a simple, efficient, and memory-optimized interface for controlling MAX7219 7-segment displays on resource-constrained microcontrollers.**

This library is specifically designed for developers working with limited flash and RAM, such as the CH32V003 microcontroller, while maintaining compatibility with standard Arduino boards. It offers a clean API that handles both hardware (BCD decode) and software (no-decode) display modes, making it easy to display numbers, text, time, and custom patterns.

### Key Benefits

- **Memory Efficient**: Optimized for microcontrollers with limited flash and RAM
- **Easy to Use**: Simple, intuitive API for common display tasks
- **Flexible**: Supports both hardware and software display modes
- **Feature Rich**: Includes animations, alignment options, and special formats
- **Well Documented**: Comprehensive [Doxygen](https://me-rk.github.io/MAX7219_CH32/) documentation
- **Production Ready**: Follows Arduino library best practices

## Features

- 🎯 **Dual Display Modes**: BCD decode mode (hardware) and no-decode mode (software)
- 📐 **Flexible Alignment**: Left, right, and center alignment options
- 🔢 **Multiple Formats**: Numbers (int, long, float), text, time (HH:MM, HH:MM:SS), hex, binary
- 🎬 **Animations**: Blink, fade, scroll, chase, and count animations
- 🎛️ **Brightness Control**: 16-level brightness control (0-15)
- ⚡ **Power Management**: Turn display on/off programmatically
- 🔧 **Low-Level Control**: Direct segment and raw digit manipulation
- 📖 **Well Documented**: Comprehensive API documentation with examples
- 💾 **Memory Optimized**: Designed for memory-constrained microcontrollers
- 🎯 **Production Ready**: Tested and optimized for CH32V003

## Platform Compatibility

The MAX7219_CH32 library is compatible with the following platforms:

- ✅ **CH32V003**: Tested and optimized (primary target platform)
- ✅ **Arduino Uno/Nano**: Full support with standard Arduino pins
- ✅ **Arduino Mega**: Full support with standard Arduino pins
- ✅ **ESP32**: Compatible (may require pin adjustments)
- ✅ **ESP8266**: Compatible (may require pin adjustments)
- ✅ **Other Arduino-compatible boards**: Should work with proper pin configuration

## Installation

### Arduino Library Manager (Recommended)

1. Open Arduino IDE
2. Go to **Tools** → **Manage Libraries**
3. Search for "MAX7219_CH32"
4. Click **Install**

### Manual Installation

1. Download or clone this repository
2. Copy the `MAX7219_CH32` folder to your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **macOS**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Hardware Requirements

![Hardware Setup](assets/hardware-image.png)

- MAX7219 LED driver IC
- 7-segment display module (common cathode)
- Arduino-compatible microcontroller
- 3 digital pins for SPI communication (DIN, CLK, CS)
- 5V power supply

## Wiring

### Pin Connections

| MAX7219 Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| DIN         | Digital pin | Data input (MOSI) |
| CLK         | Digital pin | Clock (SCK) |
| CS          | Digital pin | Chip select (CS/SS) |
| VCC         | 5V          | Power supply |
| GND         | GND         | Ground |

### Example Wiring (CH32V003)

```
MAX7219 DIN → PC6
MAX7219 CLK → PC5
MAX7219 CS  → PC4
MAX7219 VCC → 5V
MAX7219 GND → GND
```

### Example Wiring (Standard Arduino)

```
MAX7219 DIN → Pin 12
MAX7219 CLK → Pin 11
MAX7219 CS  → Pin 10
MAX7219 VCC → 5V
MAX7219 GND → GND
```

## Quick Start

### 1. Basic Setup

```cpp
#include <MAX7219_CH32.h>

// Create display instance: DIN, CLK, CS, number of digits
MAX7219 display(12, 11, 10, 8);

void setup() {
  // Initialize display in BCD decode mode
  display.begin(MAX7219_MODE_DECODE);
  display.setBrightness(8);  // Medium brightness (0-15)
}

void loop() {
  // Display a number
  display.display(12345);
  delay(1000);
  
  // Display text (switch to no-decode mode)
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display("HELLO");
  delay(1000);
}
```

### 2. Display Modes

```cpp
#include <MAX7219_CH32.h>

MAX7219 display(12, 11, 10, 8);

void setup() {
  display.begin();
  
  // BCD decode mode (hardware handles 0-9)
  display.setMode(MAX7219_MODE_DECODE);
  display.display(12345678);  // Perfect for numbers
  
  delay(2000);
  
  // No-decode mode (software handles display)
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.display("HELLO");   // Perfect for text and letters
}
```

### 3. Alignment Options

```cpp
#include <MAX7219_CH32.h>

MAX7219 display(12, 11, 10, 8);

void setup() {
  display.begin(MAX7219_MODE_DECODE);
  
  // Right alignment (default)
  display.setAlignment(MAX7219_ALIGN_RIGHT);
  display.display(12345);
  
  delay(2000);
  
  // Left alignment
  display.setAlignment(MAX7219_ALIGN_LEFT);
  display.display(12345);
  
  delay(2000);
  
  // Center alignment
  display.setAlignment(MAX7219_ALIGN_CENTER);
  display.display(12345);
}
```

### 4. Time Display

```cpp
#include <MAX7219_CH32.h>

MAX7219 display(12, 11, 10, 8);

void setup() {
  display.begin(MAX7219_MODE_DECODE);
  display.setBrightness(8);
}

void loop() {
  // Display time in HH:MM format
  display.displayTime(12, 34);
  delay(2000);
  
  // Display time in HH:MM:SS format
  display.displayTime(12, 34, 56);
  delay(2000);
}
```

### 5. Animations

```cpp
#include <MAX7219_CH32.h>

MAX7219 display(12, 11, 10, 8);

void setup() {
  display.begin(MAX7219_MODE_DECODE);
  display.setBrightness(8);
}

void loop() {
  // Blink animation
  display.display("BLINK");
  display.blink(3, 500);
  
  // Scroll text
  display.setMode(MAX7219_MODE_NO_DECODE);
  display.scroll("SCROLLING TEXT", 100);
  
  // Chase animation
  display.setMode(MAX7219_MODE_DECODE);
  display.chase(100);
  
  // Count up
  display.countUp(0, 9999, 50);
}
```

## API Reference

### Constructor

```cpp
MAX7219(uint8_t dinPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits = 8)
```

Creates a MAX7219 display instance.

**Parameters:**
- `dinPin`: Data input pin (MOSI)
- `clkPin`: Clock pin (SCK)
- `csPin`: Chip select pin (CS/SS)
- `numDigits`: Number of digits (1-8, default: 8)

**Example:**
```cpp
MAX7219 display(12, 11, 10, 8);  // 8-digit display
MAX7219 display(12, 11, 10, 4);  // 4-digit display
```

### Initialization Methods

#### `begin()`
Initialize the display with default mode (BCD decode).

```cpp
display.begin();
```

#### `begin(uint8_t mode)`
Initialize the display with specific mode.

```cpp
display.begin(MAX7219_MODE_DECODE);      // BCD decode mode
display.begin(MAX7219_MODE_NO_DECODE);   // No-decode mode
```

### Configuration Methods

#### `setMode(uint8_t mode)`
Set the display decode mode.

```cpp
display.setMode(MAX7219_MODE_DECODE);     // Hardware decode (0-9)
display.setMode(MAX7219_MODE_NO_DECODE);  // Software decode (text/patterns)
```

#### `getMode()`
Get the current display mode.

```cpp
uint8_t mode = display.getMode();
```

#### `setAlignment(uint8_t alignment)`
Set text/number alignment.

```cpp
display.setAlignment(MAX7219_ALIGN_LEFT);    // Left-aligned
display.setAlignment(MAX7219_ALIGN_RIGHT);   // Right-aligned (default)
display.setAlignment(MAX7219_ALIGN_CENTER);  // Center-aligned
```

#### `getAlignment()`
Get the current alignment setting.

```cpp
uint8_t alignment = display.getAlignment();
```

#### `setBrightness(uint8_t level)`
Set display brightness (0-15, where 0 is dimmest and 15 is brightest).

```cpp
display.setBrightness(8);   // Medium brightness
display.setBrightness(15);  // Maximum brightness
```

#### `setPower(bool on)`
Turn display on or off.

```cpp
display.setPower(true);   // Turn on
display.setPower(false);  // Turn off (power save mode)
```

#### `clear()`
Clear all digits on the display.

```cpp
display.clear();
```

### Display Methods

#### `display(const char* text)`
Display a text string.

```cpp
display.display("HELLO");
display.display("12345");  // In decode mode, tries to parse as number
```

#### `display(int number)`
Display an integer.

```cpp
display.display(12345);
display.display(-1234);  // Negative numbers supported
```

#### `display(long number)`
Display a long number.

```cpp
display.display(12345678L);
```

#### `display(double number, uint8_t decimals = 2)`
Display a floating point number.

```cpp
display.display(12.34, 2);    // 12.34
display.display(123.456, 3);  // 123.456
```

#### `displayAt(uint8_t position, uint8_t value, bool dp = false)`
Display a value at a specific position.

```cpp
display.displayAt(0, 5);           // Display 5 at position 0
display.displayAt(3, 7, true);     // Display 7 with decimal point at position 3
```

#### `displayAt(uint8_t position, char character, bool dp = false)`
Display a character at a specific position.

```cpp
display.displayAt(0, 'H');
display.displayAt(1, 'E');
display.displayAt(2, 'L');
```

### Special Format Methods

#### `displayTime(uint8_t hours, uint8_t minutes)`
Display time in HH:MM format.

```cpp
display.displayTime(12, 34);  // Displays "12:34"
```

#### `displayTime(uint8_t hours, uint8_t minutes, uint8_t seconds)`
Display time in HH:MM:SS format.

```cpp
display.displayTime(12, 34, 56);  // Displays "12:34:56"
```

#### `displayHex(uint32_t value)`
Display a value in hexadecimal format.

```cpp
display.displayHex(0x1234);  // Displays "1234" in hex
```

#### `displayBinary(uint8_t value)`
Display a value in binary format (requires 8 digits).

```cpp
display.displayBinary(0b10101010);  // Displays "10101010"
```

### Animation Methods

#### `blink(uint8_t times = 3, uint16_t delayMs = 500)`
Blink the display.

```cpp
display.blink(3, 500);      // Blink 3 times with 500ms delay
display.blink(5, 200);      // Blink 5 times with 200ms delay
```

#### `fadeIn(uint16_t delayMs = 50)`
Fade in animation (brightness 0 to current).

```cpp
display.fadeIn(50);  // Fade in with 50ms steps
```

#### `fadeOut(uint16_t delayMs = 50)`
Fade out animation (current to 0).

```cpp
display.fadeOut(50);  // Fade out with 50ms steps
```

#### `scroll(const char* text, uint16_t delayMs = 200)`
Scroll text across the display.

```cpp
display.scroll("SCROLLING TEXT", 100);  // Scroll with 100ms delay
```

#### `scroll(long number, uint16_t delayMs = 200)`
Scroll a number across the display.

```cpp
display.scroll(123456, 100);
```

#### `chase(uint16_t delayMs = 100)`
Chase animation (digit-by-digit).

```cpp
display.chase(100);  // Chase with 100ms delay
```

#### `countUp(long from, long to, uint16_t delayMs = 100)`
Count up animation.

```cpp
display.countUp(0, 9999, 50);  // Count from 0 to 9999 with 50ms delay
```

#### `countDown(long from, long to, uint16_t delayMs = 100)`
Count down animation.

```cpp
display.countDown(9999, 0, 50);  // Count from 9999 to 0 with 50ms delay
```

### Low-Level Control Methods

#### `setSegments(uint8_t position, uint8_t segments)`
Set raw segment pattern at a specific position.

```cpp
display.setSegments(0, 0b00110111);  // Display 'H' (segment pattern)
```

#### `setRawDigit(uint8_t position, uint8_t value, bool dp = false)`
Set raw digit value at a specific position.

```cpp
display.setRawDigit(0, 5);        // Display 5
display.setRawDigit(2, 7, true);  // Display 7 with decimal point
```

## Constants

### Display Modes

- `MAX7219_MODE_DECODE` - BCD decode mode (hardware handles 0-9 display)
- `MAX7219_MODE_NO_DECODE` - No decode mode (software handles display)

### Alignment Options

- `MAX7219_ALIGN_LEFT` - Left-aligned (position 0 = leftmost)
- `MAX7219_ALIGN_RIGHT` - Right-aligned (position 0 = rightmost, default)
- `MAX7219_ALIGN_CENTER` - Center-aligned (proportional to content size)

## Examples

The library includes several example sketches:

### Getting Started Examples
- **BasicExample**: Simple demonstration showing basic number and text display
- **AdvancedExample**: Comprehensive demonstration of all library features including alignment, modes, animations, and special formats

### BasicExample Walkthrough

The **BasicExample** is perfect for beginners and demonstrates the most common usage:

1. **Display Numbers**: Shows how to display integers
2. **Display Text**: Shows how to display text strings
3. **Mode Switching**: Demonstrates switching between decode and no-decode modes
4. **Clear Display**: Shows how to clear the display

This example is ideal for:
- Learning the basics of the library
- Testing your hardware setup
- Understanding the fundamental display functions
- Quick start guide for new users

### AdvancedExample Walkthrough

![AdvancedExample Demo](assets/advanced-example-demo.gif)

**Watch the demo video above to see all features in action!**

The **AdvancedExample** demonstrates all major features of the MAX7219_CH32 library:

1. **Alignment**: Shows left, right, and center alignment options
2. **Display Modes**: Demonstrates BCD decode and no-decode modes
3. **Number Display**: Shows integer, negative, and floating point numbers
4. **Text Display**: Demonstrates character and string display
5. **Position Display**: Shows how to display at specific positions
6. **Time Display**: Demonstrates HH:MM and HH:MM:SS formats
7. **Hex & Binary**: Shows hexadecimal and binary display
8. **Brightness Control**: Demonstrates brightness adjustment
9. **Power Control**: Shows display on/off functionality
10. **Animations**: Demonstrates blink, scroll, chase, and count animations
11. **Low-Level Control**: Shows direct segment and digit manipulation

This example is ideal for:
- Learning all library features
- Testing your hardware setup
- Understanding different display modes
- Verifying library installation
- Seeing practical usage examples

## Memory Usage

This library is optimized for memory-constrained microcontrollers:

- **Flash**: ~6-8 KB (varies by compiler and optimization settings)
- **RAM**: ~200-300 bytes (depends on display configuration and number of digits)

### Memory Optimization Tips

1. Use BCD decode mode when displaying only numbers (more efficient)
2. Use no-decode mode only when displaying text or custom patterns
3. Avoid using floating point if not necessary
4. Use appropriate number of digits for your display

## Troubleshooting

### Display shows nothing

- **Check wiring connections**: Verify DIN, CLK, and CS pins are correctly connected
- **Verify power supply**: Ensure 5V is properly connected and stable
- **Check CS pin**: Ensure chip select pin is correctly connected
- **Increase brightness**: Try `display.setBrightness(15)` to ensure it's not too dim
- **Verify initialization**: Ensure `begin()` is called in `setup()`

### Display shows incorrect characters

- **Check mode**: Verify you're using the correct mode (DECODE vs NO_DECODE)
- **Verify alignment**: Check alignment settings match your expectations
- **Check digit count**: Ensure number of digits matches your display configuration
- **Verify data**: Check that the data you're sending is valid

### Display flickers or is unstable

- **Check power supply**: Ensure stable 5V power supply with adequate current
- **Verify SPI timing**: Library uses software SPI - ensure no interference
- **Reduce brightness**: Try reducing brightness to `display.setBrightness(8)`
- **Check connections**: Ensure all connections are solid and properly soldered

### Display shows garbled characters

- **Check mode switching**: Ensure you set the correct mode before displaying
- **Verify text format**: In no-decode mode, only A-Z, 0-9, and some special chars are supported
- **Check alignment**: Incorrect alignment can make text appear garbled

## Future Enhancements

The library is designed to be extensible. Future versions may include:

- **Multiple Display Support**: Cascade multiple MAX7219 chips
- **Custom Characters**: User-defined character patterns
- **Display Modes**: Additional display modes and effects
- **Performance**: Further memory and speed optimizations
- **SPI Hardware Support**: Optional hardware SPI for faster communication
- **Interrupt Support**: Non-blocking animations

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

**Rithik Krisna M** ([@me-RK](https://github.com/me-RK))

## Version History

- **v1.0.0** (2025): 
  ```
  🎉 Initial release
  ✅ Basic display functionality with BCD decode and no-decode modes
  📐 Alignment options (left, right, center)
  🎬 Animation support (blink, fade, scroll, chase, count)
  🔢 Multiple display formats (numbers, text, time, hex, binary)
  💾 Memory optimized for CH32V003 and other constrained microcontrollers
  📖 Comprehensive Doxygen documentation
  🎯 Production ready with Arduino library best practices
  ```

## Documentation

API documentation can be generated using Doxygen. See [DOCUMENTATION.md](DOCUMENTATION.md) for instructions on generating the documentation.

To generate the documentation:
```bash
doxygen Doxyfile
```

The documentation will be available in the `docs/html/` directory.

Online documentation is available at: [https://me-rk.github.io/MAX7219_CH32/](https://me-rk.github.io/MAX7219_CH32/)

## Support

If you encounter any issues or have questions:

1. Check the [examples](examples/) folder
2. Review the [documentation](DOCUMENTATION.md)
3. Open an [issue](https://github.com/me-RK/MAX7219_CH32/issues)
4. Check the [Arduino Forum](https://forum.arduino.cc/)

---

**Note**: This library is designed for educational and development purposes. For production applications, ensure proper testing and validation for your specific use case.
