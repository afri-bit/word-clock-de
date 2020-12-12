#ifndef BH1750_H_
#define BH1750_H_

#include <math.h>
#include <string.h>
#include <Wire.h>


class BH1750
{
public:
    uint16_t getIntensity();
    void init();

private:
    uint8_t addr = 0x23; //i2c address
    uint8_t buff[2];
};

#endif /* BH1750_H_ */