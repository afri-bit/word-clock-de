#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

typedef enum
{
    AM = 0,
    PM = 1
} Meridiem;

typedef enum
{
    FORMAL = 0,
    INFORMAL = 1
} ClockMode;

/**
 * @brief Data container for user defined date and time
 * 
 */
struct DateTime
{
    uint8_t hour;
    uint8_t min;
    uint8_t second;
    uint8_t day_week;
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

#endif /* TYPES_H_ */