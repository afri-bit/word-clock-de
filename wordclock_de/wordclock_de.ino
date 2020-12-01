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

// Create a DS1302 object.
DS1302 rtc(RTC_PIN_ENABLE, RTC_PIN_IO, RTC_PIN_CLOCK);

WordClock wordClock{leds, LED_COUNT};

static uint32_t ctr_motion_detn = 0;

/**
 * @brief This function sets up the ledsand tells the controller about them
 * 
 */
void setup()
{
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);

    // Init the LEDs (type and assign the pin number)
    FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); // GRB ordering is typical

    // Init DIGITAL PINS
    pinMode(MOTION_DTC_PIN_DATA, INPUT);

    // Initialize a new chip by turning off write protection and clearing the
    // clock halt flag. These methods needn't always be called. See the DS1302
    // datasheet for details.
    rtc.writeProtect(false);
    rtc.halt(false);

    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

    // TODO: Uncomment following code to set the RTC initially (Debug purpose)
    // Make a new time object to set the date and time.
    // Sunday, September 22, 2013 at 01:38:50.
    // Time t(2020, 11, 30, 23, 34, 00, Time::kMonday);

    // Set the time and date on the chip.
    // rtc.time(t);
}

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
    //     wordClock.turnOffAllLEDs();
    //     wordClock.setBrightness(50);
    //     wordClock.setClock(hour, minute, second);
    //     FastLED.show();
    //     delay(1000);
    // }

    int motionDetected = digitalRead(MOTION_DTC_PIN_DATA);
    if (motionDetected == HIGH) // Motion detected
    {
        // Reset the counter
        ctr_motion_detn = 0;
        wordClock.showClock(true);
    }
    else // No motion detected
    {
        ctr_motion_detn++;

        if (ctr_motion_detn >= step_motion_detn)
        {
            ctr_motion_detn = 0;
            wordClock.showClock(false);
        }
    }

    // Get the current time and date from the chip.
    Time t = rtc.time();
    if (t.date != 0 && t.mon != 0 && t.yr != 2000)
    {
        wordClock.setTime(t.hr, t.min, t.sec);
    }

    // else
    // {
    //     Serial.println(t.hr);
    //     Serial.println(t.min);
    //     Serial.println(t.sec);
    //     Serial.println(t.date);
    //     Serial.println(t.mon);
    //     Serial.println(t.yr);
    //     Serial.println("==================================");
    // }

    wordClock.setBrightness(30);
    

    // With the following delay, the application can be controlled using certain execution time
    // This makes easier to implement counters/timers for the application
    delay(step_time);
}
