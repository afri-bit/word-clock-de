#ifndef SYSTEM_H_
#define SYSTEM_H_

// System Timing Parameter
static uint32_t step_time = 10; // Unit in ms

// ========== LEDS
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define LED_PIN 3

// ========== REALTIME CLOCK
// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
// http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
#define RTC_PIN_ENABLE 7 // Chip Enable
#define RTC_PIN_IO 5 // Input/Output
#define RTC_PIN_CLOCK 2 // Serial Clock

// ========== MOTION DETECTOR
#define MOTION_DTC_PIN_DATA 12

const uint32_t LED_COUNT = 124;

// System Configuration Parameter
uint8_t LED_MAX_BRIGHTNESS = 50;
uint8_t LED_BRIGHTNESS = 30;
bool AUTO_BRIGHTNESS = true;
int32_t TIMEOUT_MTN_DETN = 30000; // Unit in ms

#endif /* SYSTEM_H_ */