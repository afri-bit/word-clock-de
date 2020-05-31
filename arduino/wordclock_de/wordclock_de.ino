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

    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

/**
 * @brief This function runs over and over, and is where you do the magic to light
 *        your leds.
 * 
 */
int incomingByte = 0; // for incoming serial data
void loop()
{
    // send data only when you receive data:
    if (Serial.available() > 0)
    {
        uint8_t payload[3];
        incomingByte = Serial.readBytes(payload, 3);

        uint8_t hour = payload[0];
        uint8_t minute = payload[1];
        uint8_t second = payload[2];
        myWordClock.turnOffAllLEDs();
        myWordClock.setBrightness(80);
        myWordClock.setClock(hour, minute, second);
        FastLED.show();
        delay(1000);
    }
}
