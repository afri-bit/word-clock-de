#include <FastLED.h>

/* ========== Define parameters for the whole system */
#define NUM_LEDS 124
#define MAX_BRIGHTNESS 30

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3

// This is an array of leds. One item for each led in your strip.
CRGB leds[NUM_LEDS];
