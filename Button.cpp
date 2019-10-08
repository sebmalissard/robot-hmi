#include "Button.h"

typedef enum button_cmd_t
{
    STATUS      = 0x01,
} button_cmd_t;


status_t Button::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((button_cmd_t) cmd)
    {
        case STATUS:
            *size = 1;
            return getStatus(&arg[0]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Button get command");
    }

    return INVALID_COMMAND;
}

status_t Button::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    // No set command
    (void)(arg);
    (void)(size);
    
    switch ((button_cmd_t) cmd)
    {
        default:
            SERIAL_DEBUG.println("Unsupported Button set command");
    }
    
    return INVALID_COMMAND;
}



status_t Button::getStatus(uint8_t *status)
{
    if (!status)
    {
        return STATUS_ERROR;
    }
    
    _status = digitalRead(_pin);
    
    *status = _status;
    
    return STATUS_OK;
}
