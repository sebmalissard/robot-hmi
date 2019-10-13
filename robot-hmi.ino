#include <Wire.h>

#include "common.h"
#include "Max7219.h"
#include "Led.h"
#include "LedRgb.h"
#include "Button.h"
#include "Display7seg4dig.h"
#include "Irq.h"

#define LED_NUMBER                  3
#define LED_RGB_NUMBER              1
#define BUTTON_NUMBER               6
#define DISPLAY_7SEG_4DIG_NUMBER    1
#define IRQ_NUMBER                  1

typedef enum device_t {
    SYSTEM              = 0x00,
    GPIO                = 0x01,
    LED                 = 0x02,
    LED_RGB             = 0x03,
    BUTTON              = 0x04,
    SERVO               = 0x05,
    AX12                = 0x06,
    DISPLAY_7SEG_4DIG   = 0x07,
    MOTOR               = 0x08,
    IRQ                 = 0xFF
} device_t;

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
Button *button[BUTTON_NUMBER];
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
        
        for (i=0; i<BUTTON_NUMBER; i++)
        {
            if ((irq_mask >> i) & 1)
            {
                button[i]->getState(&state);
                
                if (state)
                {
                    irq_reg |= (1 << i);
                    irq[0]->setIrq(HIGH);
                }
            }
        }
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
        

void setup()
{
    led[0] = new Led(A0, false);
    led[1] = new Led(A1, false);
    led[2] = new Led(A2, false);
    /* MAX LED_NUMBER */
    
    led_rgb[0] = new LedRgb(5, 6, 9, false);
    /* MAX LED_RGB_NUMBER */
    
    button[0] = new Button(7);
    button[1] = new Button(0);
    button[2] = new Button(1);
    button[3] = new Button(4);
    button[4] = new Button(A3);
    button[5] = new Button(14);
    /* MAX BUTTON_NUMBER */
    
    display_7seg_4dig[0] = new Display7seg4dig();
    /* MAX DISPLAY_7SEG_4DIG_NUMBER */

    irq[0] = new  Irq(8);
    /* MAX IRQ_NUMBER */
     
//    test.setPower(0x01);
    
    Wire.begin(0x61);
    Wire.onReceive(receive_command);
    Wire.onRequest(request_status);

    display_7seg_4dig[0]->setPower(0);
    display_7seg_4dig[0]->setIntensity(255);
    display_7seg_4dig[0]->setPower(1);
    //display_7seg_4dig[0]->setRaw(16, 32, 64, 128);
    display_7seg_4dig[0]->setChar('h', 'o', 'l', 'o');
    //display_7seg_4dig[0]->setInteger(8888);
    display_7seg_4dig[0]->setBlink(1000, 128);
}

void loop()
{

    //led[0]->setPower(1);
    //led[0]->setBlink(10, 50);
    led[0]->loop();
    led_rgb[0]->loop();
    display_7seg_4dig[0]->loop();
    
    irq_button_loop();
   

    delay(LOOP_PERIOD);
    
//    display.printString("pret");
//    led[0]->setPower(1);
//    delay(2000);
//    display.setIntensity(255);
//    led[0]->setIntensity(100);
//    delay(1000);
//    display.setIntensity(0);
//    led[0]->setBlink(200, 126);
//    delay(1000);
//    display.setIntensity(60);
//    delay(1000);
//    display.setIntensity(128);

    
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
        
        case BUTTON:
            if (id < BUTTON_NUMBER)
            {
                COMMAND(button);
            }
            else
            {
                SERIAL_DEBUG.println("Invalid BUTTON ID.");
                SET_INVALID_STATUS_PACKET(STATUS_INVALID_ID);
            }
            break;
        
        case DISPLAY_7SEG_4DIG:
            if (id < DISPLAY_7SEG_4DIG_NUMBER)
            {
                //COMMAND(display_7seg_4dig);
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
