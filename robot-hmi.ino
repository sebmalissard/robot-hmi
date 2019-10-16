#include <Wire.h>

#include "common.h"
#include "Max7219.h"
#include "Led.h"
#include "LedRgb.h"
#include "ButtonSwitch.h"
#include "ButtonRocker.h"
#include "Display7seg4dig.h"
#include "Irq.h"

status_t status = STATUS_OK;
device_t device = SYSTEM;
uint8_t  id = 0;

bool    read_request = false;
uint8_t request_msg[16] = {0};
size_t  request_size = 3;

uint8_t irq_reg  = 0x00;
uint8_t irq_mask = 0xff;

Led *led[LED_NUMBER];
LedRgb *led_rgb[LED_RGB_NUMBER];
ButtonSwitch *button_switch[BUTTON_SWITCH_NUMBER];
ButtonRocker *button_rocker[BUTTON_ROCKER_NUMBER];
Display7seg4dig *display_7seg_4dig[DISPLAY_7SEG_4DIG_NUMBER];
Irq * irq[IRQ_NUMBER];

void receive_command(int nb);
void request_status();

void irq_button_loop()
{
    static unsigned long prev_time = 0;
    unsigned long current_time = millis();
    
    if (current_time - prev_time > LOOP_POLLING_BUTTON)
    {
        unsigned int i;
        uint8_t state;

        prev_time = current_time;
// TODO fixme
//        for (i=0; i<BUTTON_NUMBER; i++)
//        {
//            if ((irq_mask >> i) & 1)
//            {
//                button[i]->getState(&state);
//                
//                if (state)
//                {
//                    irq_reg |= (1 << i);
//                    irq[0]->setIrq(HIGH);
//                }
//            }
//        }
    }
}

#define SET_INVALID_STATUS_PACKET(status)       \
    request_msg[2] = status;                    \
    request_size = 3 

#define COMMAND(device)                                                               \
    if (read_request)                                                                 \
    {                                                                                 \
        size_t arg_size = 0;                                                          \
        request_msg[2] = device[id]->commandGet(command, &request_msg[3], &arg_size); \
        if (request_msg[2] == STATUS_OK)                                              \
            request_size = 3 + arg_size;                                              \
        else                                                                          \
            request_size = 3;                                                         \
    }                                                                                 \
    else                                                                              \
    {                                                                                 \
        request_msg[2] = device[id]->commandSet(command, &packet[3], nb - 3);         \
        request_size = 3;                                                             \
    }

#define LOOP(device, nb)                \
    for (unsigned int i=0; i<nb; i++)   \
    {                                   \
        device[i]->loop();              \
    }

void setup()
{
    SETUP_PINMUX();
        
    SERIAL_DEBUG.begin(9600);
    
    Wire.begin(I2C_ADDR);
    Wire.onReceive(receive_command);
    Wire.onRequest(request_status);
}

void loop()
{
    LOOP(led, LED_NUMBER);
    LOOP(led_rgb, LED_RGB_NUMBER);
    LOOP(display_7seg_4dig, DISPLAY_7SEG_4DIG_NUMBER);
    
    irq_button_loop();
   
    delay(LOOP_PERIOD);
}

void receive_command(int nb)
{
    uint8_t command = 0;
    uint8_t packet[nb];
    
    for (int i=0; i<nb; i++)
    {
        packet[i] = Wire.read();
    }
    
    if (nb < 3)
    {
        SERIAL_DEBUG.println("Receive incomplete packet.");
        return;
    }
    
    device  = (device_t) packet[0];
    id      = packet[1];
    command = packet[2];
    
    request_msg[0] = device;
    request_msg[1] = id;
    
    if (nb == 3)
    {
        read_request = true;
    }
    else
    {
        read_request = false;
    }
    
    switch (device)
    {
        case LED:
            if (id < LED_NUMBER)
            {
                COMMAND(led);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid LED ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
        
        case LED_RGB:
            if (id < LED_RGB_NUMBER)
            {
                COMMAND(led_rgb);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid LED RGB ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
        
        case BUTTON_SWITCH:
            if (id < BUTTON_SWITCH_NUMBER)
            {
                COMMAND(button_switch);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid BUTTON SWITCH ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;

        case BUTTON_ROCKER:
            if (id < BUTTON_ROCKER_NUMBER)
            {
                COMMAND(button_rocker);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid BUTTON ROCKER ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
            
        case DISPLAY_7SEG_4DIG:
            if (id < DISPLAY_7SEG_4DIG_NUMBER)
            {
                COMMAND(display_7seg_4dig);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid Display 7 segments 4 digits ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
        
        case IRQ:
            if (id == 0)
            {
                COMMAND(irq);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid IRQ ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
        
        default:
            SERIAL_DEBUG.println("Receive invalid device command.");
            SET_INVALID_STATUS_PACKET(STATUS_INVALID_DEVICE);
    }
}

void request_status()
{
    /* 
     *  Workarround to fix hardware Raspberry Pi issue (BCM2835): 
     *  https://github.com/raspberrypi/linux/issues/254
     *  http://www.advamation.com/knowhow/raspberrypi/rpi-i2c-bug.html
     *  http://www.advamation.com/knowhow/raspberrypi/i2c-read2_zoom.gif
     *  If this function is too fast a bad clock is generated by
     *  the Raspberry Pi. In this case, the first bit sent can be 
     *  misinterpreted by the Raspberry Pi. 
     *  Example: Arduino send: 0x01 --> Raspberry Pi receive: 0x81.
     */
    delayMicroseconds(6);
    
    Wire.write(request_msg, request_size);
}
