#ifndef _IRQ_H
#define _IRQ_H

#include "Device.h"

extern uint8_t irq_reg;
extern uint8_t irq_mask;

class Irq : public Device
{
    public:
        Irq(uint8_t pin);
        
        void setIrq(bool value);
        
        status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size);
        status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size);
        
        status_t getRegister(uint8_t *reg);
        
        status_t getMask(uint8_t *mask);
        status_t setMask(uint8_t mask);
    
    private:
        uint8_t     _pin;
};

#endif /* _IRQ_H */
