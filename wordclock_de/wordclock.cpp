#include "wordclock.h"

WordClock::WordClock(CRGB *leds, unsigned int numLEDs)
{
    this->leds_ = leds;
    this->numLEDs_ = numLEDs;
}

WordClock::~WordClock()
{
}

void WordClock::turnOnLED(uint8_t ledNumber, const Color &color)
{
    leds_[ledNumber].setRGB(color.green, color.red, color.blue);
}

void WordClock::turnOffLED(uint8_t ledNumber)
{
    leds_[ledNumber].setRGB(0, 0, 0);
}

void WordClock::turnOnLED(uint8_t from, uint8_t to, const Color &color)
{
    for (uint8_t i = from; i <= to; i++)
    {
        turnOnLED(i, color);
    }
}

void WordClock::turnOffLED(uint8_t from, uint8_t to)
{
    for (uint8_t i = from; i <= to; i++)
    {
        turnOffLED(i);
    }
}

void WordClock::turnOffAllLEDs()
{
    turnOffLED(0, numLEDs_ - 1);
}

void WordClock::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    // Turn off all LEDs
    turnOffAllLEDs();

    // Save the state to internal variables
    clockHour_ = hour; // We are using 24 hours time format
    clockMinute_ = minute;
    clockSecond_ = second;

    if (showClock_)
    {
        // Set the backround color
        turnOnLED(0, 109, colorBackground_);

        // Get the meridiem (AM or PM)
        if (hour >= 12)
        {
            setPm();
        }
        else
        {
            setAm();
        }

        if ((clockMinute_ >= 25) || (clockMode_ == INFORMAL && clockMinute_ >= 15 && clockMinute_ < 20))
        {
            setHour((clockHour_ % 12) + 1);
        }
        else
        {
            setHour(clockHour_ % 12);
        }

        setMinute(minute);
        setMinutePrecision(minute);
        setSecond(second);
        setWordItIs();
        if (clockMinute_ < 5)
        {
            setWordOclock();
        }
    }

    FastLED.show();
}

void WordClock::setHour(uint8_t hour)
{
    switch (hour)
    {
    case 0:
    {
        setHourTwelve();
        break;
    }
    case 1:
    {
        setHourOne();
        break;
    }
    case 2:
    {
        setHourTwo();
        break;
    }
    case 3:
    {
        setHourThree();
        break;
    }
    case 4:
    {
        setHourFour();
        break;
    }
    case 5:
    {
        setHourFive();
        break;
    }
    case 6:
    {
        setHourSix();
        break;
    }
    case 7:
    {
        setHourSeven();
        break;
    }
    case 8:
    {
        setHourEight();
        break;
    }
    case 9:
    {
        setHourNine();
        break;
    }
    case 10:
    {
        setHourTen();
        break;
    }
    case 11:
    {
        setHourEleven();
        break;
    }
    case 12:
    {
        setHourTwelve();
        break;
    }
    }
}

/*
 * ----------------------------------------------------------------------------
 *  Possible Times:
 *      01:00 --> Ein Uhr
 *      01:05 --> Fünf nach Eins
 *      01:10 --> Zehn nach Eins
 *      01:15 --> Viertel nach Eins / Viertel Zwei
 *      01:20 --> Zwanzig nach Eins
 *      01:25 --> Fünf vor halb Zwei
 *      01:30 --> Halb Zwei
 *      01:35 --> Fünf nach halb Zwei
 *      01:40 --> Zwanzig vor Zwei
 *      01:45 --> Viertel vor Zwei / Drei Viertel Zwei
 *      01:50 --> Zehn vor Zwei
 *      01:55 --> Fünf vor Zwei
 * ============================================================================
 */
void WordClock::setMinute(uint8_t minute)
{
    if (minute >= 55)
    {
        setMinuteFive();
        setBefore();
    }
    else if (minute >= 50)
    {
        setMinuteTen();
        setBefore();
    }
    else if (minute >= 45)
    {
        setMinuteQuarter();
        setBefore();
    }
    else if (minute >= 40)
    {
        setMinuteTwenty();
        setBefore();
    }
    else if (minute >= 35)
    {
        setMinuteFive();
        setMinuteHalf();
        setAfter();
    }
    else if (minute >= 30)
    {
        setMinuteHalf();
    }
    else if (minute >= 25)
    {
        setMinuteFive();
        setMinuteHalf();
        setBefore();
    }
    else if (minute >= 20)
    {
        setMinuteTwenty();
        setAfter();
    }
    else if (minute >= 15)
    {
        setMinuteQuarter();
        setAfter();
    }
    else if (minute >= 10)
    {
        setMinuteTen();
        setAfter();
    }
    else if (minute >= 5)
    {
        setMinuteFive();
        setAfter();
    }
    else if (minute >= 0)
    {
        // DO NOTHING
    }
}

void WordClock::setSecond(uint8_t second)
{
    // Get the second in 10s interval
    // Using modulo the second will be defined from 0-10
    uint8_t secondInterval = second % 10;
    turnOffLED(110, 119);

    switch (secondInterval)
    {
    case 0:
    {
        turnOnLED(110, colorSecond_);
        break;
    }
    case 1:
    {
        turnOnLED(111, colorSecond_);
        break;
    }
    case 2:
    {
        turnOnLED(112, colorSecond_);
        break;
    }
    case 3:
    {
        turnOnLED(113, colorSecond_);
        break;
    }
    case 4:
    {
        turnOnLED(114, colorSecond_);
        break;
    }
    case 5:
    {
        turnOnLED(115, colorSecond_);
        break;
    }
    case 6:
    {
        turnOnLED(116, colorSecond_);
        break;
    }

    case 7:
    {
        turnOnLED(117, colorSecond_);
        break;
    }

    case 8:
    {
        turnOnLED(118, colorSecond_);
        break;
    }

    case 9:
    {
        turnOnLED(119, colorSecond_);
        break;
    }
    }
}

void WordClock::setAm()
{
    // Turn Off PM
    turnOffLED(70, 71);

    // Turn On AM
    turnOnLED(59, 60, colorAmPm_);
}

void WordClock::setPm()
{
    // Turn Off AM
    turnOffLED(59, 60);

    // Turn On PM
    turnOnLED(70, 71, colorAmPm_);
}

void WordClock::setBefore()
{
    // Turn Off After
    turnOffLED(33, 36);

    // Turn On Before
    turnOnLED(41, 43, colorActiveWord_);
}

void WordClock::setAfter()
{
    // Turn Off Before
    turnOffLED(41, 43);

    // Turn On After
    turnOnLED(33, 36, colorActiveWord_);
}

// Function to turn on "ES IST"
void WordClock::setWordItIs()
{
    turnOnLED(0, 1, colorActiveWord_);
    turnOnLED(3, 5, colorActiveWord_);
}

// Function to turn on "UHR"
void WordClock::setWordOclock()
{
    turnOnLED(99, 101, colorActiveWord_);
}

// ---------- LEDs for Hours
void WordClock::setHourOne()
{
    if (clockMinute_ < 5) // "EIN"
    {
        turnOnLED(63, 65, colorActiveWord_);
    }
    else // "EINS"
    {
        turnOnLED(62, 65, colorActiveWord_);
    }
}

void WordClock::setHourTwo()
{
    turnOnLED(55, 58, colorActiveWord_);
}

void WordClock::setHourThree()
{
    turnOnLED(66, 69, colorActiveWord_);
}

void WordClock::setHourFour()
{
    turnOnLED(73, 76, colorActiveWord_);
}

void WordClock::setHourFive()
{
    turnOnLED(51, 54, colorActiveWord_);
}

void WordClock::setHourSix()
{
    turnOnLED(83, 87, colorActiveWord_);
}

void WordClock::setHourSeven()
{
    turnOnLED(88, 93, colorActiveWord_);
}

void WordClock::setHourEight()
{
    turnOnLED(77, 80, colorActiveWord_);
}

void WordClock::setHourNine()
{
    turnOnLED(103, 106, colorActiveWord_);
}

void WordClock::setHourTen()
{
    turnOnLED(106, 109, colorActiveWord_);
}

void WordClock::setHourEleven()
{
    turnOnLED(49, 51, colorActiveWord_);
}

void WordClock::setHourTwelve()
{
    turnOnLED(94, 98, colorActiveWord_);
}

// ---------- LEDs for Minutes
void WordClock::setMinutePrecision(uint8_t minute)
{
    uint8_t minutePrecision = minute % 5;

    switch (minutePrecision)
    {
    case 0:
    {
        turnOffLED(120, 123);
        break;
    }
    case 1:
    {
        turnOnLED(122, colorMinute_);
        break;
    }
    case 2:
    {
        turnOnLED(122, colorMinute_);
        turnOnLED(123, colorMinute_);
        break;
    }
    case 3:
    {
        turnOnLED(122, colorMinute_);
        turnOnLED(123, colorMinute_);
        turnOnLED(120, colorMinute_);
        break;
    }
    case 4:
    {
        turnOnLED(120, 123, colorMinute_);
        break;
    }
    }
}

void WordClock::setMinuteFive()
{
    turnOnLED(7, 10, colorActiveWord_);
}

void WordClock::setMinuteTen()
{
    turnOnLED(18, 21, colorActiveWord_);
}

void WordClock::setMinuteQuarter()
{
    turnOnLED(26, 32, colorActiveWord_);
}

void WordClock::setMinuteTwenty()
{
    turnOnLED(11, 17, colorActiveWord_);
}

void WordClock::setMinuteHalf()
{
    turnOnLED(44, 47, colorActiveWord_);
}

void WordClock::setMinuteThreeQuarter()
{
    turnOnLED(22, 32, colorActiveWord_);
}

// ---------- Feature functions

void WordClock::setBrightness(uint8_t brightnessValue)
{
    FastLED.setBrightness(brightnessValue);
}

void WordClock::showClock(bool val)
{
    showClock_ = val;
}

void WordClock::setColorActive(uint8_t red, uint8_t green, uint8_t blue)
{
    colorActiveWord_.red = red;
    colorActiveWord_.green = green;
    colorActiveWord_.blue = blue;
}

void WordClock::setColorBackground(uint8_t red, uint8_t green, uint8_t blue)
{
    colorBackground_.red = red;
    colorBackground_.green = green;
    colorBackground_.green = green;
}

void WordClock::setColorAmPm(uint8_t red, uint8_t green, uint8_t blue)
{
    colorAmPm_.red = red;
    colorAmPm_.green = green;
    colorAmPm_.blue = blue;
}

void WordClock::setColorSecond(uint8_t red, uint8_t green, uint8_t blue)
{
    colorSecond_.red = red;
    colorSecond_.green = green;
    colorSecond_.blue = blue;
}

void WordClock::setColorMinute(uint8_t red, uint8_t green, uint8_t blue)
{
    colorMinute_.red = red;
    colorMinute_.green = green;
    colorMinute_.blue = blue;
}