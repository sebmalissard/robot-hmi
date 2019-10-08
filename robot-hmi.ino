#include <Wire.h>

#include "common.h"
#include "Max7219.h"
#include "Led.h"
#include "Button.h"

#define LED_NUMBER      3
#define BUTTON_NUMBER   6


typedef enum device_t {
    SYSTEM          = 0x00,
    GPIO            = 0x01,
    LED             = 0x02,
    LED_RGB         = 0x02,
    BUTTON          = 0x04,
    SERVO           = 0x05,
    AX12            = 0x06,
    SEVEN_SEG_DIG   = 0x07,
    MOTOR           = 0x08,
    IRQ             = 0xFF
} device_t;

status_t status = OK;
device_t device = SYSTEM;
uint8_t  id = 0;

bool    read_request = false;
uint8_t request_msg[16] = {0};
size_t  request_size = 0;


Max7219 display(4);

Led *led[LED_NUMBER];
Button *button[BUTTON_NUMBER];


void reveive_command(int nb);

void setup()
{
    led[0] = new Led(A0, false);
    led[1] = new Led(A1, false);
    led[2] = new Led(A2, false);
    /* MAX LED_NUMBER */

//    test.setPower(0x01);
    
    Wire.begin(0x61);
//    Wire.onReceive(receive_command);
   // Wire.onRequest(request_status);

}

void loop()
{
    display.printString("pret");
    led[0]->setPower(1);
    delay(2000);
    display.setIntensity(255);
    delay(1000);
    display.setIntensity(0);
    led[0]->setPower(0);
    delay(1000);
    display.setIntensity(60);
    delay(1000);
    display.setIntensity(128);

    
}


#define COMMAND(device)                                                               \
    if (read_request)                                                                 \
    {                                                                                 \
        size_t arg_size = 0;                                                          \
        request_msg[2] = device[id]->commandGet(command, &request_msg[3], &arg_size);  \
        if (request_msg[2] == OK)                                                     \
            request_size = 3 + arg_size;                                              \
        else                                                                          \
            request_size = 3;                                                         \
    }                                                                                 \
    else                                                                              \
        request_msg[2] = device[id]->commandSet(command, &packet[3], nb - 3);

void reveive_command(int nb)
{
    uint8_t command = 0;
    uint8_t packet[nb];
    
    for (int i=0; i<nb; i++)
    {
        packet[i] = Wire.read();
    }
    
    if (nb < 3)
    {
        SERIAL_DEBUG.println("Receive incomplete packet");
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
                SERIAL_DEBUG.println("ERROR: Invalid LED ID");
                // status = INVALID;
            }
            break;
        
        case BUTTON:
            if (id < BUTTON_NUMBER)
            {
                COMMAND(button);
            }
            else
            {
                SERIAL_DEBUG.println("ERROR: Invalid BUTTON ID");
                // status = INVALID;
            }
            break;
        
        
        
        
    }
}
