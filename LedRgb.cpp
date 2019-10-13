#include "LedRgb.h"

typedef enum led_rgb_cmd_t
{
    POWER       = 0x01,
    COLOR       = 0x02,
    PWM         = 0x03,
    BLINK       = 0x04,
} led_rgb_cmd_t;

typedef enum led_rgb_color_t
{
    NONE                = 0x00,
    RED                 = 0x01,
    YELLOW              = 0x02,
    GREEN               = 0x03,
    CYAN                = 0x04,
    BLUE                = 0x05,
    MAGENTA             = 0x06,
    WHITE               = 0x07,
    COLOR_MAX           = 0x08,
} led_rgb_color_t;

uint8_t led_rgb_pwm_tab[COLOR_MAX][3] =
{
    {0x00, 0x00, 0x00},
    {0xFF, 0x00, 0x00},
    {0xFF, 0xFF, 0x00},
    {0x00, 0xFF, 0x00},
    {0x00, 0xFF, 0xFF},
    {0x00, 0x00, 0xFF},
    {0xFF, 0x00, 0xFF},
    {0xFF, 0xFF, 0xFF},
};

LedRgb::LedRgb(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue, bool is_common_cathode): 
    _pin_red(pin_red), _pin_green(pin_green), _pin_blue(pin_blue), _is_common_cathode(is_common_cathode),
    _power(LOW), _pwm_red(0), _pwm_green(0), _pwm_blue(0), _blink_period(0), _blink_duty_cycle(0)
{
    pinMode(pin_red,   OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue,  OUTPUT);

    ledSetValue(_power);
}

void LedRgb::loop()
{
    static uint16_t i = 0;
    static bool last_value = LOW;

    if (_power)
    {
        if (i < DIV_ROUND_CLOSEST((uint32_t)_blink_period * _blink_duty_cycle / 255, LOOP_PERIOD))
        {
            if (last_value != HIGH)
            {
                ledSetValue(HIGH);
                last_value = HIGH;
            }
        }
        else
        {
            if (last_value != LOW)
            {
                ledSetValue(LOW);
                last_value = LOW;
            }
        }
        
        i++;
        
        if (i >= _blink_period / LOOP_PERIOD)
        {
            i = 0;
        }
    }
}

status_t LedRgb::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((led_rgb_cmd_t) cmd)
    {
        case POWER:
            *size = 1;
            return getPower(&arg[0]);
            
        case COLOR:
            *size = 1;
            return getColor(&arg[0]);
            
        case PWM:
            *size = 3;
            return getPwm(&arg[0], &arg[1], &arg[2]);
            
        case BLINK:
            *size = 3;
            return getBlink((uint16_t*) &arg[0], &arg[2]);
            
        default:
            SERIAL_DEBUG.println("Unsupported Led RGB get command.");
    }

    return STATUS_INVALID_COMMAND;
}

status_t LedRgb::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((led_rgb_cmd_t) cmd)
    {
        case POWER:
             if (size == 1)
                return setPower(arg[0]);
             break;
            
        case COLOR:
            if (size == 1)
                return setColor(arg[0]);
            break;
            
        case PWM:
            if (size == 3)
                return setPwm(arg[0], arg[1], arg[2]);
            break;
            
        case BLINK:
            if (size == 3)
                return setBlink((arg[0] << 8) + arg[1], arg[2]);
            break;
        
        default:
            SERIAL_DEBUG.println("Unsupported Led RGB set command.");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t LedRgb::getPower(uint8_t *power)
{
    if (!power)
    {
        return STATUS_ERROR;
    }
    
    *power = _power;
    
    return STATUS_OK;
}

status_t LedRgb::setPower(uint8_t power)
{
    _power = power ? true : false;

    ledSetValue(_power);
    
    return STATUS_OK;
}

status_t LedRgb::getColor(uint8_t *color)
{
    if (!color)
    {
        return STATUS_ERROR;
    }
    
    for (unsigned int i=0; i<COLOR_MAX; i++)
    {
        if (_pwm_red == led_rgb_pwm_tab[i][0] && 
            _pwm_green == led_rgb_pwm_tab[i][1] && 
            _pwm_blue == led_rgb_pwm_tab[i][2])
        {
            *color = i;
        }
    }
    
    *color = NONE;

    return STATUS_OK;
}

status_t LedRgb::setColor(uint8_t color)
{
    if (color > COLOR_MAX)
    {
        return STATUS_ERROR;
    }
    
    _pwm_red   = led_rgb_pwm_tab[color][0];
    _pwm_green = led_rgb_pwm_tab[color][1];
    _pwm_blue  = led_rgb_pwm_tab[color][2];

    ledSetValue(_power);
    
    return STATUS_OK;
}

status_t LedRgb::getPwm(uint8_t *pwm_red, uint8_t *pwm_green, uint8_t *pwm_blue)
{
    if (!pwm_red || !pwm_green || !pwm_blue)
    {
        return STATUS_ERROR;
    }
    
    *pwm_red   = _pwm_red;
    *pwm_green = _pwm_green;
    *pwm_blue  = _pwm_blue;
    
    return STATUS_OK;
}

status_t LedRgb::setPwm(uint8_t pwm_red, uint8_t pwm_green, uint8_t pwm_blue)
{
    _pwm_red   = pwm_red;
    _pwm_green = pwm_green;
    _pwm_blue  = pwm_blue;
    
    ledSetValue(_power);
    
    return STATUS_OK;
}

status_t LedRgb::getBlink(uint16_t *period, uint8_t *duty_cycle)
{
    if (!period || !duty_cycle)
    {
        return STATUS_ERROR;
    }
    
    *period = _blink_period;
    *duty_cycle = _blink_duty_cycle;
    
    return STATUS_OK;
}

status_t LedRgb::setBlink(uint16_t period, uint8_t duty_cycle)
{
    _blink_period = period;
    _blink_duty_cycle = duty_cycle;
    
    ledSetValue(_power);
    
    return STATUS_OK;
}

void LedRgb::ledSetValue(bool value)
{
    if (_is_common_cathode)
    {
        analogWrite(_pin_red,   value ? _pwm_red   : 0);
        analogWrite(_pin_green, value ? _pwm_green : 0);
        analogWrite(_pin_blue,  value ? _pwm_blue  : 0);
    }
    else
    {
        analogWrite(_pin_red,   value ? 255 - _pwm_red   : 255);
        analogWrite(_pin_green, value ? 255 - _pwm_green : 255);
        analogWrite(_pin_blue,  value ? 255 - _pwm_blue  : 255);
    }
}
