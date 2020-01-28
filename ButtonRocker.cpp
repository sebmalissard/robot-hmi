#include "ButtonRocker.h"

typedef enum button_cmd_t
{
    STATE      = 0x01,
} button_cmd_t;

ButtonRocker::ButtonRocker(uint8_t pin): _pin(pin), _irq_polling_last_state(LOW)
{
    pinMode(pin, INPUT_PULLUP);
}

bool ButtonRocker::irq_polling()
{
    uint8_t state;
    
    getState(&state);
    
    if (_irq_polling_last_state != state)
    {
        _irq_polling_last_state = state;
        
        return true;
    }
    
    return false;
}

status_t ButtonRocker::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((button_cmd_t) cmd)
    {
        case STATE:
            *size = 1;
            return getState(&arg[0]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Button Rocker get command");
    }

    return STATUS_INVALID_COMMAND;
}

status_t ButtonRocker::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    // No set command
    (void)(arg);
    (void)(size);
    
    switch ((button_cmd_t) cmd)
    {
        default:
            SERIAL_DEBUG.println("Unsupported Button Rocker set command");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t ButtonRocker::getState(uint8_t *state)
{
    if (!state)
    {
        return STATUS_ERROR;
    }
    
    _state = !digitalRead(_pin);
    
    *state = _state;
    
    return STATUS_OK;
}
