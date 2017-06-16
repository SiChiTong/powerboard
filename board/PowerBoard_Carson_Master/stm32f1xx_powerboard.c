/* 
*  Author: Adam Huang 
*  Date:2016/6/4
*/

#include "stm32f1xx_powerboard.h"
#include "Mico.h"
 
uint32_t BSP_SW_GetState(Switch_TypeDef Switch)
{
  (void)Switch;    
  return MicoGpioInputGet( MICO_GPIO_PWRKEY );
}

void BSP_Power_OnOff(PowerEnable_TypeDef PowerEn, PowerOnOff_TypeDef OnOff)
{
  if( POWER_ON == OnOff )
  {
    if( PowerEn & POWER_5V_RES )
    {
      MicoGpioOutputLow( MICO_GPIO_5V_RES_EN );
    }
    if( PowerEn & POWER_24V_ICCARD )
    {
      MicoGpioOutputLow( MICO_GPIO_24V_ICCARD_EN );
    }
    if( PowerEn & POWER_24V_RES )
    {
      MicoGpioOutputLow( MICO_GPIO_24V_RES_EN );
    }
    if( PowerEn & POWER_5V_EN )
    {
      MicoGpioOutputLow( MICO_GPIO_5V_EN );
    }
    if( PowerEn & POWER_12V_EN )
    {
      MicoGpioOutputLow( MICO_GPIO_12V_EN );
    }
    if( PowerEn & POWER_24V_EN )
    {
      MicoGpioOutputLow( MICO_GPIO_24V_EN );
    }
    if( PowerEn & POWER_24V_PRINTER )
    {
      MicoGpioOutputLow( MICO_GPIO_24V_PRINTER_EN );
    }
    if( PowerEn & POWER_12V_PC )
    {
      MicoGpioOutputLow( MICO_GPIO_12V_PC_EN );
    }
    if( PowerEn & POWER_BAT_NV )
    {
      MicoGpioOutputLow( MICO_GPIO_BAT_NV_EN );
    }
    if( PowerEn & POWER_12V_RES )
    {
      MicoGpioOutputLow( MICO_GPIO_12V_RES_EN );
    }
  }
  else if( POWER_OFF == OnOff )
  {
    if( PowerEn & POWER_5V_RES )
    {
      MicoGpioOutputHigh( MICO_GPIO_5V_RES_EN );
    }
    if( PowerEn & POWER_24V_ICCARD )
    {
      MicoGpioOutputHigh( MICO_GPIO_24V_ICCARD_EN );
    }
    if( PowerEn & POWER_24V_RES )
    {
      MicoGpioOutputHigh( MICO_GPIO_24V_RES_EN );
    }
    if( PowerEn & POWER_5V_EN )
    {
      MicoGpioOutputHigh( MICO_GPIO_5V_EN );
    }
    if( PowerEn & POWER_12V_EN )
    {
      MicoGpioOutputHigh( MICO_GPIO_12V_EN );
    }
    if( PowerEn & POWER_24V_EN )
    {
      MicoGpioOutputHigh( MICO_GPIO_24V_EN );
    }
    if( PowerEn & POWER_24V_PRINTER )
    {
      MicoGpioOutputHigh( MICO_GPIO_24V_PRINTER_EN );
    }
    if( PowerEn & POWER_12V_PC )
    {
      MicoGpioOutputHigh( MICO_GPIO_12V_PC_EN );
    }
    if( PowerEn & POWER_BAT_NV )
    {
      MicoGpioOutputHigh( MICO_GPIO_BAT_NV_EN );
    }
    if( PowerEn & POWER_12V_RES )
    {
      MicoGpioOutputHigh( MICO_GPIO_12V_RES_EN );
    }
  }
}

uint32_t getModulePowerState( PowerEnable_TypeDef PowerEn )
{
    uint32_t pinState;
    
    pinState = (uint32_t)0;

    if( PowerEn & POWER_5V_EN )
    {
      if( !MicoGpioInputGet( MICO_GPIO_5V_EN ) )
      {
        pinState |= POWER_5V_EN;
      }
    }
    if( PowerEn & POWER_12V_EN )
    {
      if( !MicoGpioInputGet( MICO_GPIO_12V_EN ) )
      {
        pinState |= POWER_12V_EN;
      }
    }
    if( PowerEn & POWER_24V_EN )
    {
      if( !MicoGpioInputGet( MICO_GPIO_24V_EN ) )
      {
        pinState |= POWER_24V_EN;
      }
    }
    if( PowerEn & POWER_5V_RES )
    {
      if( !MicoGpioInputGet( MICO_GPIO_5V_RES_EN ) )
      {
        pinState |= POWER_5V_RES;
      }
    }
    if( PowerEn & POWER_24V_ICCARD )
    {
      if( !MicoGpioInputGet( MICO_GPIO_24V_ICCARD_EN ) )
      {
        pinState |= POWER_24V_ICCARD;
      }
    }
    if( PowerEn & POWER_24V_RES )
    {
      if( !MicoGpioInputGet( MICO_GPIO_24V_RES_EN ) )
      {
        pinState |= POWER_24V_RES;
      }
    }
    if( PowerEn & POWER_24V_PRINTER )
    {
      if( !MicoGpioInputGet( MICO_GPIO_24V_PRINTER_EN ) )
      {
        pinState |= POWER_24V_PRINTER;
      }
    }
    if( PowerEn & POWER_12V_PC )
    {
      if( !MicoGpioInputGet( MICO_GPIO_12V_PC_EN ) )
      {
        pinState |= POWER_12V_PC;
      }
    }
    if( PowerEn & POWER_BAT_NV )
    {
      if( !MicoGpioInputGet( MICO_GPIO_BAT_NV_EN ) )
      {
        pinState |= POWER_BAT_NV;
      }
    }
    if( PowerEn & POWER_12V_RES )
    {
      if( !MicoGpioInputGet( MICO_GPIO_12V_RES_EN ) )
      {
        pinState |= POWER_12V_RES;
      }
    }
    return pinState;
}

void BSP_Control_Sigal(PowerControl_TypeDef PowerCon, ControlSignal_TypeDef isHold)
{
  if( CONTROL_RELEASE == isHold )
  {
    switch( PowerCon )
    {
    case POWER_CON_NV:
      MicoGpioOutputLow( MICO_GPIO_PWR_NV ); 
      break;
    case POWER_CON_PC:
      MicoGpioOutputLow( MICO_GPIO_PWR_PC ); 
      break;
    default:
      break;
    }
  }
  else if( CONTROL_HOLD == isHold )
  {
    switch( PowerCon )
    {
    case POWER_CON_NV:
      MicoGpioOutputHigh( MICO_GPIO_PWR_NV ); 
      break;
    case POWER_CON_PC:
      MicoGpioOutputHigh( MICO_GPIO_PWR_PC ); 
      break;
    default:
      break;
    }
  }
}

#ifndef BOOTLOADER
void halEnterSleepMode( void )
{
//  BSP_LED_Off( LED_SYS );
//  HAL_PWR_EnterSTANDBYMode();
}

void halWakeupFormSLeep( void )
{
//  init_clocks();
}
#endif

