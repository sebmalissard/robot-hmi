#include "Irq.h"

typedef enum irq_cmd_t
{
    REGISTER    = 0x01,
    MASK        = 0x02,
} irq_cmd_t;

Irq::Irq(uint8_t pin): _pin(pin)
{
    pinMode(pin, OUTPUT);
}


void Irq::setIrq(bool value)
{
    static bool last_value = 0;

    SERIAL_DEBUG.println("set_irq");
    if (last_value != value)
    {
        last_value = value;
        SERIAL_DEBUG.println("SET:");
        SERIAL_DEBUG.println(value);
        digitalWrite(_pin, value);
    }
}

status_t Irq::commandGet(uint8_t cmd, uint8_t *arg, size_t *size)
{
    switch ((irq_cmd_t) cmd)
    {
        case REGISTER:
            *size = 1;
            return getRegister(&arg[0]);
            
        case MASK:
            *size = 1;
            return getMask(&arg[0]);
            
        default:
            SERIAL_DEBUG.println("Invalid IRQ get command.");
    }

    return STATUS_INVALID_COMMAND;
}

status_t Irq::commandSet(uint8_t cmd, const uint8_t *arg, size_t size)
{
    switch ((irq_cmd_t) cmd)
    {            
        case MASK:
            if (size == 1)
                return setMask(arg[0]);
            break;
        
        default:
            SERIAL_DEBUG.println("Invalid IRQ set command.");
    }
    
    return STATUS_INVALID_COMMAND;
}

status_t Irq::getRegister(uint8_t *reg)
{
    if (!reg)
    {
        return STATUS_ERROR;
    }
    
    *reg = irq_reg;

    irq_reg = 0x00;
    setIrq(LOW);

SERIAL_DEBUG.println("Read register value =");
SERIAL_DEBUG.println(*reg);
    
    return STATUS_OK;
}

status_t Irq::getMask(uint8_t *mask)
{
    if (!mask)
    {
        return STATUS_ERROR;
    }
    
    *mask = irq_mask;
    
    return STATUS_OK;
}

status_t Irq::setMask(uint8_t mask)
{
    irq_mask = mask;
    
    return STATUS_OK;
}
