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
  
  POWER_12V_EN                   = 0x00000001,
  POWER_5V_EN                    = 0x00000002,
  POWER_12V_HEAD_RK_EN           = 0x00000004,
  POWER_RESERVE_1                = 0x00000008,
  
  POWER_12V_CHEST_RK_EN          = 0x00000010,
  POWER_RESERVE_2                = 0x00000020,
  POWER_24V_LEFT_HAND_MOTER_EN   = 0x00000040,
  POWER_24V_RIGHT_HAND_MOTOR_EN  = 0x00000080,
  
  POWER_24V_HEAD_MOTOR_EN        = 0x00000100,
  POWER_12V_SWITCH_EN            = 0x00000200,
  POWER_12V_ROUTER_EN            = 0x00000400,
  POWER_12V_CARD_READER_EN       = 0x00000800,
  
  POWER_12V_AUDIO_PA_EN          = 0x00001000,
  POWER_5V_LEFT_HAND_MOTOR_EN    = 0x00002000,
  POWER_5V_RIGHT_HAND_MOTOR_EN   = 0x00004000,
  POWER_5V_HEAD_MOTOR_EN         = 0x00008000,
  
  
  POWER_5V_RESERVE_EN            = 0x00010000,
  POWER_5V_LEDX2_EN              = 0x00020000,
  POWER_5V_REPAIR_EN             = 0x00040000,
  POWER_5V_TOUCH_EN              = 0x00080000,
  
  POWER_5V_KEYBOARD_EN           = 0x00100000,
  POWER_5V_CARD_READER_EN        = 0x00200000,
  POWER_5V_CODE_EN               = 0x00400000,
  POWER_5V_CAMERA_EN             = 0x00800000,
  
  POWER_5V_HD_CAMERA_EN          = 0x01000000,
  POWER_5V_HEAD_LED_EN           = 0x02000000,
  POWER_24V_HD_CAMERA            = 0x04000000,
  POWER_24V_CAMERA               = 0x08000000,
  
  POWER_5V_HEAD_TOUCH            = 0x10000000,
  
  POWER_ALL                      = 0x1FFFFFFF,
  
} PowerEnable_TypeDef;



typedef enum
{
    MODULE_5V_EN = 0,      
    MODULE_12V_HEAD_RK_EN,
    MODULE_RESERVE_1,

    MODULE_12V_CHEST_RK_EN,
    MODULER_RESERVE_2,
    MODULE_24V_LEFT_HAND_MOTER_EN,
    MODULE_24V_RIGHT_HAND_MOTOR_EN,

    MODULE_24V_HEAD_MOTOR_EN,
    MODULE_12V_SWITCH_EN,
    MODULE_12V_ROUTER_EN,
    MODULE_12V_CARD_READER_EN,

    MODULE_12V_AUDIO_PA_EN,
    MODULE_5V_LEFT_HAND_MOTOR_EN,
    MODULE_5V_RIGHT_HAND_MOTOR_EN,
    MODULE_5V_HEAD_MOTOR_EN,


    MODULE_5V_RESERVE_EN,
    MODULE_5V_LEDX2_EN,
    MODULE_5V_REPAIR_EN,
    MODULE_5V_TOUCH_EN,

    MODULE_5V_KEYBOARD_EN,
    MODULE_5V_CARD_READER_EN,
    MODULE_5V_CODE_EN,
    MODULE_5V_CAMERA_EN,

    MODULE_5V_HD_CAMERA_EN,
    MODULE_5V_HEAD_LED_EN,
    MODULE_24V_HD_CAMERA,
    MODULE_24V_CAMERA,

    MODULE_5V_HEAD_TOUCH,
    MODULE_ALL,  
}module_t;

typedef enum
{
#if 1
    POWER_HEAD_RK  = 0x01,
    POWER_CHEST_RK = 0x02,
    POWER_ALL_RK   = 0x0F
#endif
} PowerControl_TypeDef;
typedef enum
{
    POWER_ON_ALREADY = 1,
    POWER_ON_PROCESSING,
    POWER_OFF_ALREADY,
    POWER_OFF_PROCESSING,
    POWER_ERR_STATE
}PowerControlState_TypeDef;

uint32_t        BSP_GetVersion(void);

void            Serial_Leds_Init( void );

void            BSP_SW_Init(Switch_TypeDef Switch, SwitchMode_TypeDef SwitchMode);
uint32_t        BSP_SW_GetState(Switch_TypeDef Switch);

void     BSP_GPIO_Init( void );
void     BSP_Power_OnOff(PowerEnable_TypeDef PowerEn, PowerOnOff_TypeDef OnOff);
uint32_t GetModulePowerState(PowerEnable_TypeDef PowerEn);
void     BSP_Control_Sigal(PowerControl_TypeDef PowerCon, ControlSignal_TypeDef isHold);

uint8_t SetRkState(PowerControl_TypeDef rk, uint8_t on_off);
PowerControlState_TypeDef GetRkState(PowerControl_TypeDef  select);

extern void halEnterSleepMode( void );

extern void halWakeupFormSLeep( void );

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1XX_POWERBOARD_H */

