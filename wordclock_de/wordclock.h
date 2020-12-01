
/*
 * ============================================================================
 * Words and LEDs Descriptions
 *  o[122]                                                                o[123]
 *
 *  E[000] S[001] K[002] I[003] S[004] T[005] A[006] F[007] Ü[008] N[009] F[010]
 *  Z[021] E[020] H[019] N[018] Z[017] W[016] A[015] N[014] Z[013] I[012] G[011]
 *  D[022] R[023] E[024] I[025] V[026] I[027] E[028] R[029] T[030] E[031] L[032]
 *  V[043] O[042] R[041] F[040] U[039] N[038] K[037] N[036] A[035] C[034] H[033]
 *  H[044] A[045] L[046] B[047] A[048] E[049] L[050] F[051] Ü[052] N[053] F[054]
 *  E[065] I[064] N[063] S[062] X[061] A[060] M[059] Z[058] W[057] E[056] I[055]
 *  D[066] R[067] E[068] I[069] P[070] M[071] J[072] V[073] I[074] E[075] R[076]
 *  S[087] E[086] C[085] H[084] S[083] N[082] L[081] A[080] C[079] H[078] T[077]
 *  S[088] I[089] E[090] B[091] E[092] N[093] Z[094] W[095] Ö[096] L[097] F[098]
 *  Z[109] E[108] H[107] N[106] E[105] U[104] N[103] K[102] U[101] H[100] R[099]
 * 
 *     o[110] o[111] o[112] o[113] o[114] o[115] o[116] o[117] o[118] o[119]    
 *   
 *  o[121]                                                                o[120]
 * ----------------------------------------------------------------------------
 *  Possible Times and the german informal Grammar:
 *      01:00 --> Ein Uhr
 *      01:05 --> Fünf nach Eins
 *      01:10 --> Zehn nach Eins
 *      01:15 --> Viertel nach Eins
 *      01:20 --> Zwanzig nach Eins
 *      01:25 --> Fünf vor halb Zwei
 *      01:30 --> Halb Zwei
 *      01:35 --> Fünf nach halb Zwei
 *      01:40 --> Zwanzig vor Zwei
 *      01:45 --> Viertel vor Zwei
 *      01:50 --> Zehn vor Zwei
 *      01:55 --> Fünf vor Zwei
 * ============================================================================
 */

#ifndef WORDCLOCK_H_
#define WORDCLOCK_H_

#include <stdint.h>

#include <FastLED.h>

#include "color.h"
#include "clock_mode.h"

class WordClock
{
private:
    CRGB *leds_;
    unsigned int numLEDs_;

    uint8_t clockHour_ = 0;
    uint8_t clockMinute_ = 0;
    uint8_t clockSecond_ = 0;

    Color colorActiveWord_{255, 255, 255};
    Color colorInactiveWord_{0, 0, 0};
    Color colorAmPm_{255, 0, 0};
    Color colorSecond_{240, 145, 20};
    Color colorMinutePrecision_{255, 0, 255};

    ClockMode clockMode_ = FORMAL;

    bool showClock_ = true;

    uint8_t maxBrightness_;
    uint8_t brightness_;

    // General LEDs function
    void turnOffLED(uint8_t from, uint8_t to);
    void turnOffLED(uint8_t ledNumber);
    void turnOffAllLEDs();
    
    void turnOnLED(uint8_t ledNumber, const Color &color);
    void turnOnLED(uint8_t from, uint8_t to, const Color &color);

    // Set the LEDs based on the hour, minute and second
    void setHour(uint8_t hour);
    void setMinute(uint8_t minute);
    void setSecond(uint8_t second);

    // Function to turn on AM / PM time
    void setAm();
    void setPm();

    // Function to turn on "VOR" and "NACH"
    void setBefore();
    void setAfter();

    // Function to turn on "ES IST"
    void setWordItIs();

    // Function to turn on "UHR"
    void setWordOclock();

    // ---------- Hours LED Method
    void setHourOne();
    void setHourTwo();
    void setHourThree();
    void setHourFour();
    void setHourFive();
    void setHourSix();
    void setHourSeven();
    void setHourEight();
    void setHourNine();
    void setHourTen();
    void setHourEleven();
    void setHourTwelve();

    void setMinutePrecision(uint8_t minute);
    void setMinuteFive();
    void setMinuteTen();
    void setMinuteQuarter();
    void setMinuteTwenty();
    void setMinuteHalf();
    void setMinuteThreeQuarter();

public:
    /**
     * @brief Construct a new Word Clock object
     * 
     * @param leds 
     * @param numLEDs 
     */
    WordClock(CRGB *leds, unsigned int numLEDs);

    /**
     * @brief Destroy the Word Clock object
     * 
     */
    ~WordClock();

    /**
     * @brief Algorithm function to set the clock based on input from the outside source
     * 
     * @param hour 
     * @param minute 
     * @param second 
     */
    void setTime(uint8_t hour, uint8_t minute, uint8_t second);

    /**
     * @brief Set the Brightness object
     * 
     * @param brightnessValue 
     */
    void setBrightness(uint8_t brightnessValue);

    /**
     * @brief Set the Color Active object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setColorActive(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Set the Color In Active object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setColorInActive(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Set the Color Am Pm object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setColorAmPm(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Set the Color Second object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setColorSecond(uint8_t red, uint8_t green, uint8_t blue);

    void showClock(bool val); 

};

#endif /* WORDCLOCK_H_ */
