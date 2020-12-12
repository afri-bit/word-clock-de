
#include "system.h"
#include "helper.h"
#include <Wire.h>
#include <FastLED.h>

#include "wordclock.h"
#include "ds1302.h"
#include "bh1750.h"

CRGB leds[LED_COUNT];
WordClock wordClock{leds, LED_COUNT};
DS1302 rtc(RTC_PIN_ENABLE, RTC_PIN_IO, RTC_PIN_CLOCK);
BH1750 brightnessSensor;

uint32_t step_motion_detn = TIMEOUT_MTN_DETN / step_time;
uint32_t ctr_motion_detn = 0;
int motion_detected_prev = LOW;

/**
 * @brief Function to check if the message is valid. The key to prove that the
 *        message is valid to check the last byte of the message frame, which
 *        is called Checksum byte. The method to be used to create and prove 
 *        the checksum is XOR method.
 * 
 * @param msg Pointer to message frame array
 * @param size Size of the message
 * @return true Message is valid
 * @return false Message is invalid
 */
bool message_valid(uint8_t *msg, int size)
{
    uint8_t tmp = 0;
    uint8_t chksum = msg[size - 1];

    for (int i = 0; i < size - 1; i++)
    {
        // XOR operation
        tmp ^= msg[i];
    }

    bool ret = tmp == chksum ? true : false;

    return ret;
}

void process_message(uint8_t *msg, int size)
{
    // Process the incoming message if the length is sufficient
    // Ignore message length below 10: 4 bytes id, 4 bytes length,
    // at least 1 byte payload, 1 byte checksum
    if (size >= 10)
    {
        if (message_valid(msg, size))
        {
            // Get the size based on represented bytes
            int size_payload = bytes_to_int(&(msg[4]), 4);

            // Start to process the message, if the informed size is
            // equal to payload size
            if (size - 9 == size_payload)
            {
                int msg_id = bytes_to_int(msg, 4);
                switch (msg_id)
                {
                case 0x550: // Background Color
                {
                    // Set the color with the WordClock object
                    wordClock.setColorBackground(msg[8], msg[9], msg[10]);
                    break;
                }
                case 0x551: // Active Letter Color
                {
                    // Set the color with the WordClock object
                    wordClock.setColorActive(msg[8], msg[9], msg[10]);
                    break;
                }
                case 0x552: // AM/PM Color
                {
                    // Set the color with the WordClock object
                    wordClock.setColorAmPm(msg[8], msg[9], msg[10]);
                    break;
                }
                case 0x553: // Minutes Color (Corner LED)
                {
                    // Set the color with the WordClock object
                    wordClock.setColorMinute(msg[8], msg[9], msg[10]);
                    break;
                }
                case 0x554: // Secondes Color
                {
                    // Set the color with the WordClock object
                    wordClock.setColorSecond(msg[8], msg[9], msg[10]);
                    break;
                }
                case 0x555: // Timeout for Motion Detection
                {
                    // Set the timeout
                    int16_t val; // in second
                    memcpy(&val, &msg[8], 2);
                    TIMEOUT_MTN_DETN = static_cast<int32_t>(val) * 1000; // Convert to millisecond
                    break;
                }
                case 0x700: // Date Time for Synchronization
                {
                    DateTime dateTime;
                    memcpy(&dateTime, &(msg[8]), 8);

                    Time t(dateTime.year, dateTime.month, dateTime.day,
                           dateTime.hour, dateTime.min, dateTime.second,
                           static_cast<Time::Day>(dateTime.day_week));

                    // Update the time and date on the chip, to reset the precision
                    rtc.time(t);

                    break;
                }
                }
            }
        }
    }

    return;
}

void reset_brightness_counter()
{
}

void process_brightness()
{
    if (AUTO_BRIGHTNESS)
    {
        uint16_t lux = brightnessSensor.getIntensity();
        if (lux >= 250) // 100%
            LED_BRIGHTNESS = 1 * LED_MAX_BRIGHTNESS;
        else if (lux >= 220) // 90 %
            LED_BRIGHTNESS = 0.9 * LED_MAX_BRIGHTNESS;
        else if (lux >= 200) // 80 %
            LED_BRIGHTNESS = 0.8 * LED_MAX_BRIGHTNESS;
        else if (lux >= 160) // 70 %
            LED_BRIGHTNESS = 0.7 * LED_MAX_BRIGHTNESS;
        else if (lux >= 130) // 60 %
            LED_BRIGHTNESS = 0.6 * LED_MAX_BRIGHTNESS;
        else if (lux >= 100) // 50 %
            LED_BRIGHTNESS = 0.5 * LED_MAX_BRIGHTNESS;
        else if (lux >= 60) // 40 %
            LED_BRIGHTNESS = 0.4 * LED_MAX_BRIGHTNESS;
        else if (lux >= 30) // 30 %
            LED_BRIGHTNESS = 0.3 * LED_MAX_BRIGHTNESS;
        else if (lux >= 10) // 20 %
            LED_BRIGHTNESS = 0.2 * LED_MAX_BRIGHTNESS;
        else if (lux > 0) // 10 %
            LED_BRIGHTNESS = 0.1 * LED_MAX_BRIGHTNESS;
    }

    FastLED.setBrightness(LED_BRIGHTNESS);
}

// =============================================================================

/**
 * @brief This function sets up the ledsand tells the controller about them
 * 
 */
void setup()
{
    // Sanity check delay - allows reprogramming if accidently blowing power with leds
    delay(2000);

    Wire.begin();

    // Init the LEDs (type and assign the pin number)
    FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); // GRB ordering is typical

    brightnessSensor.init();

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

void loop()
{
    // Receiving data from serial to be processed
    if (Serial.available() > 0)
    {
        uint8_t rx_msg[30];
        int incoming_bytes = Serial.readBytes(rx_msg, sizeof(rx_msg));
        process_message(rx_msg, incoming_bytes);
    }

    if (TIMEOUT_MTN_DETN < 1000)
    {
        wordClock.showClock(false);
    }
    else if (TIMEOUT_MTN_DETN > 3600000)
    {
        wordClock.showClock(true);
    }
    else
    {
        int motion_detected = digitalRead(MOTION_DTC_PIN_DATA);
        if (motion_detected_prev == LOW && motion_detected == HIGH) // Motion detected, but only rising edge
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
        // Set current state to previous state
        motion_detected_prev = motion_detected;
    }

    // Get the current time and date from the chip.
    Time t = rtc.time();
    if (t.date != 0 && t.mon != 0 && t.yr != 2000)
    {
        wordClock.setTime(t.hr, t.min, t.sec);
    }

    process_brightness();

    // With the following delay, the application can be controlled using certain execution time
    // This makes easier to implement counters/timers for the application
    delay(step_time);
}
