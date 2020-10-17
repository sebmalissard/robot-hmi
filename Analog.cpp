#include "Analog.h"

typedef enum led_cmd_t
{
    VOLTAGE    = 0x01,
} analog_cmd_t;

Analog::Analog(uint8_t pin): _pin(pin)
{
    pinMode(pin, INPUT_PULLUP);
}


status_t Analog::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((analog_cmd_t) cmd)
    {
        case VOLTAGE:
            *size = 2;
            return getVoltage(&arg[1], &arg[0]); // FIXME order msb and lsb
            
        default:
            SERIAL_DEBUG.println("Unsupported Analog get command.");
    }

    return STATUS_INVALID_COMMAND;
}

status_t Analog::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((analog_cmd_t) cmd)
    {
        default:
            SERIAL_DEBUG.println("Unsupported Analog set command.");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t Analog::getVoltage(uint8_t *voltage_msb, uint8_t *voltage_lsb)
{
    int32_t val = 0;
    
    if (!voltage_msb)
    {
        return STATUS_ERROR;
    }
    
    val = analogRead(_pin);

    //SERIAL_DEBUG.print("Analog value : ");
    //SERIAL_DEBUG.print(val);
    //SERIAL_DEBUG.print(" pin ");
    //SERIAL_DEBUG.print(_pin);
    //SERIAL_DEBUG.print(" voltage mV : ");
    
    // Do computation on 32 bits
    val = val * 5 * 1000 / 1023;
    
    *voltage_msb = val & 0xFF;
    *voltage_lsb = (val >> 8) & 0xFF;

    //SERIAL_DEBUG.print(*voltage_msb, HEX);
    //SERIAL_DEBUG.print(" lsb: ");
    //SERIAL_DEBUG.println(*voltage_lsb, HEX);
    
    return STATUS_OK;
}
