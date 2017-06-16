#ifndef __LED_CTRL_H
#define __LED_CTRL_H

#include "Common.h"

typedef enum
{
    LED1,
    LED2,
}LED_TypeDef;

typedef struct
{   
    uint32_t frq;
    uint8_t pecent;
    uint8_t on_off;
#define ON  1
#define OFF 0
}PWM_LED_Typedef;


uint8_t SetLedBrightness(LED_TypeDef led, uint8_t brightness);
uint8_t GetLedBrightness(LED_TypeDef led);
#endif