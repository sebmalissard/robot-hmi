#include <Wire.h>

#include "common.h"
#include "Max7219.h"
#include "Led.h"
#include "LedRgb.h"
#include "ButtonSwitch.h"
#include "ButtonRocker.h"
#include "Ax12.h"
#include "Display7seg4dig.h"
#include "Irq.h"

uint8_t request_msg[16] = {0};
size_t  request_size = 3;

uint8_t irq_reg  = 0x00;
uint8_t irq_mask = 0xff;
Device  *irq_callback[IRQ_REG_SIZE] = {0};

Led               *led[LED_NUMBER];
LedRgb            *led_rgb[LED_RGB_NUMBER];
ButtonSwitch      *button_switch[BUTTON_SWITCH_NUMBER];
ButtonRocker      *button_rocker[BUTTON_ROCKER_NUMBER];
Ax12              *ax12[AX12_NUMBER];
Display7seg4dig   *display_7seg_4dig[DISPLAY_7SEG_4DIG_NUMBER];
Irq               *irq[IRQ_NUMBER];

typedef struct device_list_t {
  Device  **dev;
  size_t  size;
} device_list_t;

device_list_t devices[] =
{
    {(Device**)led,               LED_NUMBER},
    {(Device**)led_rgb,           LED_RGB_NUMBER},
    {(Device**)button_switch,     BUTTON_SWITCH_NUMBER},
    {(Device**)button_rocker,     BUTTON_ROCKER_NUMBER},
    {(Device**)ax12,              AX12_NUMBER},
    {(Device**)display_7seg_4dig, DISPLAY_7SEG_4DIG_NUMBER},
    {(Device**)irq,               IRQ_NUMBER},
};

void devices_loop();
void devices_irq_polling();
void set_invalid_packet(status_t status);
void command_packet(Device *dev[], uint8_t dev_size,  uint8_t *packet, uint8_t nb);
void receive_command(int nb);
void request_status();

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
    devices_loop();
    devices_irq_polling();
    
    delay(LOOP_PERIOD);
}

void devices_loop()
{
    unsigned int i, j;

    for (i=0; i<sizeof(devices)/sizeof(devices[0]); i++)
    {
        for (j=0; j<devices[i].size; j++)
        {
            devices[i].dev[j]->loop();
        }
    }
}

void devices_irq_polling()
{
    static unsigned long prev_time = 0;
    unsigned long current_time = millis();
    
    if (current_time - prev_time > LOOP_POLLING_BUTTON)
    {
        unsigned int i;

        prev_time = current_time;

        for (i=0; i<IRQ_REG_SIZE; i++)
        {
            if ((irq_callback[i] != NULL) && ((irq_mask >> i) & 1) && (irq_callback[i]->irq_polling()))
            {
                irq_reg |= (1 << i);
                irq[0]->setIrq(HIGH);
            }
        }
    }
}

void set_invalid_packet(status_t status)
{
    request_msg[2] = status;
    request_size = 3;
}

void command_packet(Device *dev[], uint8_t dev_size,  uint8_t *packet, uint8_t nb)
{
    uint8_t device  = packet[0];
    uint8_t id      = packet[1];
    uint8_t command = packet[2];
    
    if (id < dev_size)
    {
        if (nb == 3) // read request
        {
            size_t arg_size = 0;
            
            request_msg[2] = dev[id]->commandGet(command, &request_msg[3], &arg_size);
            request_size = 3;
            
            if (request_msg[2] == STATUS_OK)
            {
                request_size += arg_size;
            }
        }
        else // write request
        {
            request_msg[2] = dev[id]->commandSet(command, &packet[3], nb - 3);
            request_size = 3;
        }
    }
    else
    {
        SERIAL_DEBUG.print("Receive invalid ID (");
        SERIAL_DEBUG.print(id, HEX);
        SERIAL_DEBUG.print("):");
        SERIAL_DEBUG.println(deviceToString((device_t) device));
        set_invalid_packet(STATUS_INVALID_ID);
    }
}

void receive_command(int nb)
{
    uint8_t packet[nb];
    uint8_t device;
    uint8_t id;
    
    for (int i=0; i<nb; i++)
    {
        packet[i] = Wire.read();
    }
    
    if (nb < 3)
    {
        SERIAL_DEBUG.println("Receive incomplete packet.");
        request_msg[0] = SYSTEM;
        request_msg[1] = 0x00;
        set_invalid_packet(STATUS_INVALID_PACKET);
        return;
    }
    
    device = packet[0];
    id     = packet[1];
    
    request_msg[0] = device;
    request_msg[1] = id;
    
    switch ((device_t) device)
    {
        case LED:               command_packet((Device**)led,               LED_NUMBER,               packet, nb); break;
        case LED_RGB:           command_packet((Device**)led_rgb,           LED_RGB_NUMBER,           packet, nb); break;
        case BUTTON_SWITCH:     command_packet((Device**)button_switch,     BUTTON_SWITCH_NUMBER,     packet, nb); break;
        case BUTTON_ROCKER:     command_packet((Device**)button_rocker,     BUTTON_ROCKER_NUMBER,     packet, nb); break;
        case AX12:              command_packet((Device**)ax12,              AX12_NUMBER,              packet, nb); break;
        case DISPLAY_7SEG_4DIG: command_packet((Device**)display_7seg_4dig, DISPLAY_7SEG_4DIG_NUMBER, packet, nb); break;
        case IRQ:               command_packet((Device**)irq,               IRQ_NUMBER,               packet, nb); break;
        default:
            SERIAL_DEBUG.print("Receive invalid device (");
            SERIAL_DEBUG.print(device, HEX);
            SERIAL_DEBUG.print("):");
            set_invalid_packet(STATUS_INVALID_DEVICE); 
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
