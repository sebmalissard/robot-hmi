#ifndef _MAX7219_H
#define _MAX7219_H

#include <stdint.h>

#define SPI_SS          10
#define SERIAL_DEBUG    Serial

// 7 segment controler
class Max7219
{
    public:
        Max7219(unsigned int digitNum);
        
        // Range max for 4 digits: -999 to 9999
        void printInteger(const long int integer);
        
        // See table for all character supported
        void printString(const char *string);
        
        // Print raw, expected tab size didigNum
        void printRaw(const uint8_t dig[]);
        
        // 0 to 255, but only 16 level available
        void setIntensity(const uint8_t intensity);
        
        // Clear the display
        void clear();
        
    private:
        unsigned int digitNum;
        uint8_t asciiDigit[256] = {0};
        
        void regWrite(uint8_t reg, uint8_t val);
};

#endif /* _MAX7219_H */
