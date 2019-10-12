#include <SPI.h>

#include "Max7219.h"

#define MAX7219_REG_NO_OP           0x00
#define MAX7219_REG_DIGIT_0         0x01
#define MAX7219_REG_DIGIT_1         0x02
#define MAX7219_REG_DIGIT_2         0x03
#define MAX7219_REG_DIGIT_3         0x04
#define MAX7219_REG_DIGIT_4         0x05
#define MAX7219_REG_DIGIT_5         0x06
#define MAX7219_REG_DIGIT_6         0x07
#define MAX7219_REG_DIGIT_7         0x08
#define MAX7219_REG_DECODE_MODE     0x09
#define MAX7219_REG_INTENSITY       0x0A
#define MAX7219_REG_SCAN_LIMIT      0x0B
#define MAX7219_REG_SHUTDOWN        0x0C
#define MAX7219_REG_DISPLAY_TEST    0x0F

// MAX7219_REG_DECODE_MODE
#define MAX7219_DECODE_MODE_DISABLE 0x00

// MAX7219_REG_SHUTDOWN
#define MAX7219_SHUTDOWN_ENABLE     0x00
#define MAX7219_SHUTDOWN_DISABLE    0x01

// MAX7219_REG_DISPLAY_TEST
#define MAX7219_DISPLAY_TEST_OFF    0x00
#define MAX7219_DISPLAY_TEST_ON     0x01

// MAX7219_REG_DIGIT_*
#define MAX7219_DIGIT_SEG_G         0x01
#define MAX7219_DIGIT_SEG_F         0x02
#define MAX7219_DIGIT_SEG_E         0x04
#define MAX7219_DIGIT_SEG_D         0x08
#define MAX7219_DIGIT_SEG_C         0x10
#define MAX7219_DIGIT_SEG_B         0x20
#define MAX7219_DIGIT_SEG_A         0x40
#define MAX7219_DIGIT_SEG_DP        0x80

Max7219::Max7219(unsigned int digitNum) : digitNum(digitNum)
{
    SERIAL_DEBUG.begin(9600);
    
    if (digitNum > 8)
    {
        SERIAL_DEBUG.print("ERROR: Invalid number of digits, max 8, current: ");
        SERIAL_DEBUG.println(digitNum);
        digitNum = 8;
    }

    // Initialize ascii 7 segment table
    asciiDigit[' '] = 0b00000000;
    asciiDigit['!'] = 0b10110000;
    asciiDigit['"'] = 0b00100010;
    asciiDigit['\'']= 0b00100000;
    asciiDigit['('] = 0b01001110; // [; C
    asciiDigit[')'] = 0b01111000; // ]
    asciiDigit['-'] = 0b00000001;
    asciiDigit['.'] = 0b10000000;
    asciiDigit['0'] = 0b01111110;
    asciiDigit['1'] = 0b00110000;
    asciiDigit['2'] = 0b01101101;
    asciiDigit['3'] = 0b01111001;
    asciiDigit['4'] = 0b00110011;
    asciiDigit['5'] = 0b01011011;
    asciiDigit['6'] = 0b01011111;
    asciiDigit['7'] = 0b01110000;
    asciiDigit['8'] = 0b01111111;
    asciiDigit['9'] = 0b01111011;
    asciiDigit['='] = 0b00001001;
    asciiDigit['?'] = 0b01100101;
    asciiDigit['A'] = 0b01110111;
    asciiDigit['B'] = 0b00011111; // b
    asciiDigit['C'] = 0b01001110;
    asciiDigit['D'] = 0b00111101; // d
    asciiDigit['E'] = 0b01001111;
    asciiDigit['F'] = 0b01000111;
    asciiDigit['G'] = 0b01111011; // g; same as '9'
    asciiDigit['H'] = 0b00110111;
    asciiDigit['I'] = 0b00110000; // same as '1'
    asciiDigit['J'] = 0b00111100;
    asciiDigit['K'] = 0b00000000; // no define
    asciiDigit['L'] = 0b00001110;
    asciiDigit['M'] = 0b00000000; // no define
    asciiDigit['N'] = 0b00010101; // n
    asciiDigit['O'] = 0b01111110; // same as '0'
    asciiDigit['P'] = 0b01100111;
    asciiDigit['Q'] = 0b01110011; // q
    asciiDigit['R'] = 0b00000101; // r
    asciiDigit['S'] = 0b01011011; // same as '5'
    asciiDigit['T'] = 0b00001111; // t
    asciiDigit['U'] = 0b00111110;
    asciiDigit['V'] = 0b00000000; // no define
    asciiDigit['W'] = 0b00000000; // no define
    asciiDigit['X'] = 0b00000000; // no define
    asciiDigit['Y'] = 0b00111011; // y
    asciiDigit['Z'] = 0b00000000; // no define
    asciiDigit['['] = 0b01001110; // C
    asciiDigit[']'] = 0b01111000;
    asciiDigit['_'] = 0b00001000;
    asciiDigit['`'] = 0b00000010;
    asciiDigit['a'] = 0b01110111; // A
    asciiDigit['b'] = 0b00011111;
    asciiDigit['c'] = 0b00001101;
    asciiDigit['d'] = 0b00111101;
    asciiDigit['e'] = 0b01001111; // E
    asciiDigit['f'] = 0b01000111; // F
    asciiDigit['g'] = 0b01111011; // same as '9'
    asciiDigit['h'] = 0b00010111;
    asciiDigit['i'] = 0b00010000;
    asciiDigit['j'] = 0b00111100; // J
    asciiDigit['k'] = 0b00000000; // no define
    asciiDigit['l'] = 0b00000110;
    asciiDigit['m'] = 0b00000000; // no define
    asciiDigit['n'] = 0b00010101;
    asciiDigit['o'] = 0b00011101;
    asciiDigit['p'] = 0b01100111; // P
    asciiDigit['q'] = 0b01110011;
    asciiDigit['r'] = 0b00000101;
    asciiDigit['s'] = 0b01011011; // S; same as '5'
    asciiDigit['t'] = 0b00001111;
    asciiDigit['u'] = 0b00011100;
    asciiDigit['v'] = 0b00000000; // no define
    asciiDigit['w'] = 0b00000000; // no define
    asciiDigit['x'] = 0b00000000; // no define
    asciiDigit['y'] = 0b00111011;
    asciiDigit['z'] = 0b00000000; // no define
    asciiDigit[0xC2] = 0b01100011; // °; FIXME take 2 character

    
    SPI.begin();
    pinMode(SPI_SS, OUTPUT);
    
    regWrite(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_DISABLE);
    regWrite(MAX7219_REG_DISPLAY_TEST, MAX7219_DISPLAY_TEST_OFF);
    regWrite(MAX7219_REG_SCAN_LIMIT, digitNum);
    regWrite(MAX7219_REG_DECODE_MODE, MAX7219_DECODE_MODE_DISABLE);
    clear();
    setIntensity(128);
}

void Max7219::printInteger(const long int integer)
{
    char str[digitNum+1] = {0};
    
    snprintf(str, digitNum+1, "%ld", integer);
    
    printString(str);
}

void Max7219::printString(const char *string)
{
    unsigned int i, j;
    
    for (i=0; i<digitNum; i++)
    {
        if (string[i] == '\0')
        {
            break;
        }
        SERIAL_DEBUG.print("Value: ");
        SERIAL_DEBUG.println(string[i], BIN);
        regWrite(MAX7219_REG_DIGIT_0 + i, asciiDigit[(uint8_t) string[i]]);
    }

    for (j=i; j<digitNum; j++)
    {
        regWrite(MAX7219_REG_DIGIT_0 + j, 0x00);
    }
}

void Max7219::printRaw(const uint8_t dig[])
{
    unsigned int i;
    
    for (i=0; i<digitNum; i++)
    {
        regWrite(MAX7219_REG_DIGIT_0 + i, dig[i]);
    }
}

void Max7219::setIntensity(const uint8_t intensity)
{
    regWrite(MAX7219_REG_INTENSITY, intensity/16);
}

void Max7219::clear()
{
    for (unsigned int i=0; i<digitNum; i++)
    {
        regWrite(MAX7219_REG_DIGIT_0 + i, 0x00);
    }
}

void Max7219::regWrite(uint8_t reg, uint8_t val)
{
    SERIAL_DEBUG.print(reg, HEX);
    SERIAL_DEBUG.print(" ");
    SERIAL_DEBUG.println(val, BIN);
    
    digitalWrite(SPI_SS, LOW);
    SPI.transfer(reg);
    SPI.transfer(val);
    digitalWrite(SPI_SS, HIGH);
}
