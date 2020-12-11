#include <FastLED.h>

/* ========== Define parameters for the whole system */

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define LED_MAX_BRIGHTNESS 30
#define LED_PIN 3
#define LED_COUNT 124

// This is an array of leds. One item for each led in your strip.
CRGB leds[LED_COUNT];

// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
#define RTC_PIN_ENABLE 7 // Chip Enable
#define RTC_PIN_IO 5 // Input/Output
#define RTC_PIN_CLOCK 2 // Serial Clock

// Pin for motion detection sensor data
#define MOTION_DTC_PIN_DATA 12

static uint32_t step_time = 10; // Unit in ms
static uint32_t timeout_motion_detn = 60000; // Unit in ms
static uint32_t step_motion_detn = timeout_motion_detn / step_time;  

