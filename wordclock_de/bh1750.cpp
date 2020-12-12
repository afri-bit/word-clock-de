#include "bh1750.h"

void BH1750::init()
{
    Wire.beginTransmission(addr);
    Wire.write(0x10); // 1lx resolution 120ms
    Wire.endTransmission();
}

uint16_t BH1750::getIntensity() //
{
    int i = 0;
    uint16_t val = 0;

    Wire.beginTransmission(addr);
    Wire.requestFrom(addr, 2);
    while (Wire.available()) //
    {
        buff[i] = Wire.read(); // receive one byte
        i++;
    }
    Wire.endTransmission();

    if (i == 2)
    {
        val = ((buff[0] << 8) | buff[1]) / 1.2;
        // Serial.print(val, DEC);
        // Serial.print(" lux");
    }

    return val;
}