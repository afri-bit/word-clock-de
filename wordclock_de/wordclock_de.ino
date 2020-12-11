
#include "settings.h"
#include "wordclock.h"
#include "word_led_display.h"

#include "DS1302.h"

#include "data.h"
#include "helper.h"

// Global objects and variables for the system
DS1302 rtc(RTC_PIN_ENABLE, RTC_PIN_IO, RTC_PIN_CLOCK);

// WordClock object declaration
WordClock wordClock{leds, LED_COUNT};

// Counter for motion detection
static uint32_t ctr_motion_detn = 0;
static int motion_detected_prev = LOW;

// ============================================================================
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
                    // TODO: Set the timeout
                    break;
                }
                case 0x700: // Date Time for Synchronization
                {
                    DateTime dateTime;
                    memcpy(&dateTime, &(msg[8]), 8);

                    Time t(dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.min, dateTime.second, static_cast<Time::Day>(dateTime.day_week));

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

// =============================================================================

/**
 * @brief This function sets up the ledsand tells the controller about them
 * 
 */
void setup()
{
    // Sanity check delay - allows reprogramming if accidently blowing power with leds
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

void loop()
{
    // send data only when you receive data:
    if (Serial.available() > 0)
    {
        uint8_t rx_msg[30];
        int incoming_bytes = Serial.readBytes(rx_msg, sizeof(rx_msg));
        process_message(rx_msg, incoming_bytes);
    }

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

    // Get the current time and date from the chip.
    Time t = rtc.time();
    if (t.date != 0 && t.mon != 0 && t.yr != 2000)
    {
        wordClock.setTime(t.hr, t.min, t.sec);
    }

    wordClock.setBrightness(30);

    // With the following delay, the application can be controlled using certain execution time
    // This makes easier to implement counters/timers for the application
    delay(step_time);
}
