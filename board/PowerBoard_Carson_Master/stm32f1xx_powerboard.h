/* 
*  Author: Adam Huang 
*  Date:2016/6/4
*/
#ifndef __STM32F1XX_POWERBOARD_H
#define __STM32F1XX_POWERBOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"

    
typedef enum
{
  POWER_OFF = 0,
  POWER_ON = 1
} PowerOnOff_TypeDef;

typedef enum {
  CONTROL_RELEASE = 0,
  CONTROL_HOLD = 1
} ControlSignal_TypeDef;

typedef enum 
{
  LED_SYS = 0,
  
} Led_TypeDef;

typedef enum 
{  
  SWITCH_USER = 0,
} Switch_TypeDef;

typedef enum 
{  
  SWITCH_MODE_GPIO = 0,
  SWITCH_MODE_EXTI = 1
} SwitchMode_TypeDef; 

typedef enum
{
  POWER_SWITCH          = 0x00000001,
  POWER_5V_RES          = 0x00000002,
  POWER_24V_ICCARD      = 0x00000004,
  POWER_24V_RES         = 0x00000008,
  POWER_5V_EN           = 0x00000010,
  POWER_12V_EN          = 0x00000020,
  POWER_24V_EN          = 0x00000040,
  POWER_24V_PRINTER     = 0x00000080,
  POWER_12V_PC          = 0x00000100,
  POWER_BAT_NV          = 0x00000200,
  POWER_12V_RES         = 0x00000400,
  POWER_ALL             = 0x001FFFFF,
} PowerEnable_TypeDef;

typedef enum
{
  POWER_CON_NV  = 0x01,
  POWER_CON_PC  = 0x02,
  POWER_CON_ALL = 0x1F
} PowerControl_TypeDef;

uint32_t        BSP_GetVersion(void);

void            Serial_Leds_Init( void );

void            BSP_SW_Init(Switch_TypeDef Switch, SwitchMode_TypeDef SwitchMode);
uint32_t        BSP_SW_GetState(Switch_TypeDef Switch);

void     BSP_GPIO_Init( void );
void     BSP_Power_OnOff(PowerEnable_TypeDef PowerEn, PowerOnOff_TypeDef OnOff);
uint32_t getModulePowerState(PowerEnable_TypeDef PowerEn);
void     BSP_Control_Sigal(PowerControl_TypeDef PowerCon, ControlSignal_TypeDef isHold);

extern void halEnterSleepMode( void );

extern void halWakeupFormSLeep( void );

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1XX_POWERBOARD_H */

