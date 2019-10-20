#ifndef _DEVICE_H
#define _DEVICE_H

#include "common.h"

class Device
{
    public:
        virtual void loop() { return; };
        virtual bool irq_polling() { return false; };
        
        virtual status_t commandGet(uint8_t cmd, uint8_t *arg, size_t *size) = 0;
        virtual status_t commandSet(uint8_t cmd, const uint8_t *arg, size_t size) = 0;
};

#endif /* _DEVICE_H */
