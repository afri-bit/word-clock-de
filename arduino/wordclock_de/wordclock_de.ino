/* ****************************************************************************
 * Components:
 *  - LED WS2812B 
 *  
 * ============================================================================
 * 
 * 
 * ****************************************************************************
 */

#include "settings.h"
#include "wordclock.h"
#include "word_led_display.h"

#include "DS1302.h"

// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = 7;  // Chip Enable
const int kIoPin   = 5;  // Input/Output
const int kSclkPin = 2;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

WordClock myWordClock{leds, 124};



/**
 * @brief This function sets up the ledsand tells the controller about them
 * 
 */
void setup()
{
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);

    // Init the LEDs (type and assign the pin number)
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical

    // Initialize a new chip by turning off write protection and clearing the
    // clock halt flag. These methods needn't always be called. See the DS1302
    // datasheet for details.
    rtc.writeProtect(false);
    rtc.halt(false);

    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

    // Make a new time object to set the date and time.
    // Sunday, September 22, 2013 at 01:38:50.
    // Time t(2020, 11, 18, 22, 40, 00, Time::kTuesday);

    // Set the time and date on the chip.
    // rtc.time(t);
}

/**
 * @brief This function runs over and over, and is where you do the magic to light
 *        your leds.
 * 
 */
int incomingByte = 0; // for incoming serial data
void loop()
{
    // // send data only when you receive data:
    // if (Serial.available() > 0)
    // {
    //     uint8_t payload[3];
    //     incomingByte = Serial.readBytes(payload, 3);

    //     uint8_t hour = payload[0];
    //     uint8_t minute = payload[1];
    //     uint8_t second = payload[2];
    //     myWordClock.turnOffAllLEDs();
    //     myWordClock.setBrightness(50);
    //     myWordClock.setClock(hour, minute, second);
    //     FastLED.show();
    //     delay(1000);
    // }

    // Get the current time and date from the chip.
    Time t = rtc.time();
    myWordClock.turnOffAllLEDs();
    myWordClock.setBrightness(20);
    myWordClock.setClock(t.hr, t.min, t.sec);
    Serial.println(t.hr);
    Serial.println(t.min);
    Serial.println(t.sec);
    Serial.println("==================================");
    
    FastLED.show();


    delay(100);
}
