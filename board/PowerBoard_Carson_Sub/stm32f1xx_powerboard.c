/* 
*  Author: Adam Huang 
*  Date:2016/6/4
*/

#include "stm32f1xx_powerboard.h"
#include "Mico.h"
#include "app_platform.h"
uint32_t BSP_SW_GetState(Switch_TypeDef Switch)
{
  (void)Switch;    
  return MicoGpioInputGet( GPIO_PWR_KEY );
}

void BSP_Power_OnOff(PowerEnable_TypeDef PowerEn, PowerOnOff_TypeDef OnOff)
{
  if( POWER_ON == OnOff )
  { 
    if( PowerEn & POWER_12V_EN )
    {
      MicoGpioOutputLow( GPIO_12V_EN );
    }
    if( PowerEn & POWER_5V_EN )
    {
      MicoGpioOutputLow( GPIO_5V_EN );
    }   
    if( PowerEn & POWER_12V_HEAD_RK_EN )
    {
      MicoGpioOutputLow( GPIO_12V_HEAD_RK_EN );
    }   
    if( PowerEn & POWER_RESERVE_1 )
    {
      //MicoGpioOutputLow( GPIO_5V_EN );
    }
    
  

    if( PowerEn & POWER_12V_CHEST_RK_EN )
    {
      MicoGpioOutputLow( GPIO_12V_CHEST_RK_EN );
    }
    if( PowerEn & POWER_RESERVE_2 )
    {
      //MicoGpioOutputLow( MICO_GPIO_24V_EN );
    }

    if( PowerEn & POWER_24V_LEFT_HAND_MOTER_EN )
    {
      MicoGpioOutputLow( GPIO_24V_LEFT_HAND_MOTOR_EN );
    }
    
    if( PowerEn & POWER_24V_RIGHT_HAND_MOTOR_EN )
    {
      MicoGpioOutputLow( GPIO_24V_RIGHT_HAND_MOTOR_EN );
    }
  
    
    
      
    
    if( PowerEn & POWER_24V_HEAD_MOTOR_EN )
    {
      MicoGpioOutputLow( GPIO_24V_HEAD_MOTOR_EN );
    }
    
  
    if( PowerEn & POWER_12V_SWITCH_EN )
    {
      MicoGpioOutputLow( GPIO_12V_SWITCH_EN );
    }
  
    if( PowerEn & POWER_12V_ROUTER_EN )
    {
      MicoGpioOutputLow( GPIO_12V_ROUTER_EN );
    }
    
    if( PowerEn & POWER_12V_CARD_READER_EN )
    {
      MicoGpioOutputLow( GPIO_12V_CARD_READER_EN );
    }
      

    if( PowerEn & POWER_12V_AUDIO_PA_EN )
    {
      MicoGpioOutputLow( GPIO_12V_AUDIO_PA_EN );
    }
  
    if( PowerEn & POWER_5V_LEFT_HAND_MOTOR_EN )
    {
      MicoGpioOutputLow( GPIO_5V_LEFT_HAND_MOTOR_EN );
    }
    
    
    if( PowerEn & POWER_5V_RIGHT_HAND_MOTOR_EN )
    {
      MicoGpioOutputLow( GPIO_5V_RIGHT_HAND_MOTOR_EN );
    }
    if( PowerEn & POWER_5V_HEAD_MOTOR_EN )
    {
      MicoGpioOutputLow( GPIO_5V_HEAD_MOTOR_EN );
    }
      
 
      
    if( PowerEn & POWER_5V_RESERVE_EN )
    {
      MicoGpioOutputLow( GPIO_5V_RESERVE_EN );
    }
    if( PowerEn & POWER_5V_LEDX2_EN )
    {
      MicoGpioOutputLow( GPIO_5V_LEDX2_EN );
    }
    
    if( PowerEn & POWER_5V_REPAIR_EN )
    {
      MicoGpioOutputLow( GPIO_5V_REPAIR_BOARD_EN );
    }
    
    if( PowerEn & POWER_5V_TOUCH_EN )
    {
      MicoGpioOutputLow( GPIO_5V_TOUCH_EN );
    }

    
    if( PowerEn & POWER_5V_KEYBOARD_EN )
    {
      MicoGpioOutputLow( GPIO_5V_KEYBORAD_EN );
    }

    if( PowerEn & POWER_5V_CARD_READER_EN )
    {
      MicoGpioOutputLow( GPIO_5V_CARD_READER_EN );
    }
    
    if( PowerEn & POWER_5V_CODE_EN )
    {
      MicoGpioOutputLow( GPIO_5V_CODE_EN );
    }
    
    if( PowerEn & POWER_5V_CAMERA_EN )
    {
      MicoGpioOutputLow( GPIO_5V_CAMERA_EN );
    }
     
  
    if( PowerEn & POWER_5V_HD_CAMERA_EN )
    {
      MicoGpioOutputLow( GPIO_5V_HD_CAMERA_EN );
    }
    
    if( PowerEn & POWER_5V_HEAD_LED_EN )
    {
      MicoGpioOutputLow( GPIO_5V_HEAD_LED_EN );
    }
    
    if( PowerEn & POWER_24V_HD_CAMERA )
    {
      MicoGpioOutputLow( GPIO_24V_HD_CAMERA_EN );
    }
    if( PowerEn & POWER_24V_CAMERA )
    {
      MicoGpioOutputLow( GPIO_24V_CAMERA_EN );
    }
    
    
    if( PowerEn & POWER_5V_HEAD_TOUCH )
    {
      MicoGpioOutputLow( MICO_GPIO_AIUI_EN );
    }
    
      
  }
    
      
  else if( POWER_OFF == OnOff )
  {
    if( PowerEn & POWER_12V_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_EN );
    }
    if( PowerEn & POWER_5V_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_EN );
    }
    if( PowerEn & POWER_12V_HEAD_RK_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_HEAD_RK_EN );
    }
    if( PowerEn & POWER_RESERVE_1 )
    {
      //MicoGpioOutputHigh( GPIO_5V_EN );
    }
    

    if( PowerEn & POWER_12V_CHEST_RK_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_CHEST_RK_EN );
    }
    if( PowerEn & POWER_RESERVE_2 )
    {
      //MicoGpioOutputHigh( MICO_GPIO_24V_EN );
    }
    if( PowerEn & POWER_24V_LEFT_HAND_MOTER_EN )
    {
      MicoGpioOutputHigh( GPIO_24V_LEFT_HAND_MOTOR_EN );
    }
    if( PowerEn & POWER_24V_RIGHT_HAND_MOTOR_EN )
    {
      MicoGpioOutputHigh( GPIO_24V_RIGHT_HAND_MOTOR_EN );
    }
   
    
    if( PowerEn & POWER_24V_HEAD_MOTOR_EN )
    {
      MicoGpioOutputHigh( GPIO_24V_HEAD_MOTOR_EN );
    }
    if( PowerEn & POWER_12V_SWITCH_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_SWITCH_EN );
    }
    if( PowerEn & POWER_12V_ROUTER_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_ROUTER_EN );
    }
    if( PowerEn & POWER_12V_CARD_READER_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_CARD_READER_EN );
    }
      
      
    if( PowerEn & POWER_12V_AUDIO_PA_EN )
    {
      MicoGpioOutputHigh( GPIO_12V_AUDIO_PA_EN );
    }
    if( PowerEn & POWER_5V_LEFT_HAND_MOTOR_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_LEFT_HAND_MOTOR_EN );
    }
    if( PowerEn & POWER_5V_RIGHT_HAND_MOTOR_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_RIGHT_HAND_MOTOR_EN );
    }
    if( PowerEn & POWER_5V_HEAD_MOTOR_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_HEAD_MOTOR_EN );
    }
      
     
      
    if( PowerEn & POWER_5V_RESERVE_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_RESERVE_EN );
    }
    if( PowerEn & POWER_5V_LEDX2_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_LEDX2_EN );
    }
    if( PowerEn & POWER_5V_REPAIR_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_REPAIR_BOARD_EN );
    }
    if( PowerEn & POWER_5V_TOUCH_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_TOUCH_EN );
    }
    
    
    
    if( PowerEn & POWER_5V_KEYBOARD_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_KEYBORAD_EN );
    }
    if( PowerEn & POWER_5V_CARD_READER_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_CARD_READER_EN );
    }
    if( PowerEn & POWER_5V_CODE_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_CODE_EN );
    }
    
    if( PowerEn & POWER_5V_CAMERA_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_CAMERA_EN );
    }

    
    if( PowerEn & POWER_5V_HD_CAMERA_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_HD_CAMERA_EN );
    }
    
    if( PowerEn & POWER_5V_HEAD_LED_EN )
    {
      MicoGpioOutputHigh( GPIO_5V_HEAD_LED_EN );
    }
    
    if( PowerEn & POWER_24V_HD_CAMERA )
    {
      MicoGpioOutputHigh( GPIO_24V_HD_CAMERA_EN );
    }
    if( PowerEn & POWER_24V_CAMERA )
    {
      MicoGpioOutputHigh( GPIO_24V_CAMERA_EN );
    }
    
    
    if( PowerEn & POWER_5V_HEAD_TOUCH )
    {
      MicoGpioOutputHigh( MICO_GPIO_AIUI_EN );
    }
  }
}

uint32_t GetModulePowerState( PowerEnable_TypeDef PowerEn )
{
    uint32_t pinState;
    
    pinState = (uint32_t)0;

    if( PowerEn & POWER_12V_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_EN  ) )
      {
        pinState |= POWER_12V_EN ;
      }
    }
    
    if( PowerEn & POWER_5V_EN  )
    {
      if( !MicoGpioInputGet( GPIO_5V_EN  ) )
      {
        pinState |= POWER_5V_EN ;
      }
    }
    
    if( PowerEn & POWER_12V_HEAD_RK_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_HEAD_RK_EN  ) )
      {
        pinState |= POWER_12V_HEAD_RK_EN ;
      }
    }
    /*
    if( PowerEn & POWER_RESERVE_1  )
    {
      if( !MicoGpioInputGet( MICO_GPIO_LEDS_EN ) )
      {
        pinState |= POWER_RESERVE_1 ;
      }
    }
    */
    
    if( PowerEn & POWER_12V_CHEST_RK_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_CHEST_RK_EN  ) )
      {
        pinState |= POWER_12V_CHEST_RK_EN ;
      }
    }
    /*
    if( PowerEn & POWER_RESERVE_2  )
    {
      if( !MicoGpioInputGet( GPIO_12V_ROUTER_EN ) )
      {
        pinState |= POWER_RESERVE_2 ;
      }
    }
    */
    
    
    if( PowerEn & POWER_24V_LEFT_HAND_MOTER_EN  )
    {
      if( !MicoGpioInputGet( GPIO_24V_LEFT_HAND_MOTOR_EN  ) )
      {
        pinState |= POWER_24V_LEFT_HAND_MOTER_EN ;
      }
    }
    
    
    if( PowerEn & POWER_24V_RIGHT_HAND_MOTOR_EN  )
    {
      if( !MicoGpioInputGet( GPIO_24V_RIGHT_HAND_MOTOR_EN  ) )
      {
        pinState |= POWER_24V_RIGHT_HAND_MOTOR_EN ;
      }
    }
    
    if( PowerEn & POWER_24V_HEAD_MOTOR_EN  )
    {
      if( !MicoGpioInputGet( GPIO_24V_HEAD_MOTOR_EN  ) )
      {
        pinState |= POWER_24V_HEAD_MOTOR_EN ;
      }
    }
    
    if( PowerEn & POWER_12V_SWITCH_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_SWITCH_EN  ) )
      {
        pinState |= POWER_12V_SWITCH_EN ;
      }
    }
    
    if( PowerEn & POWER_12V_ROUTER_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_ROUTER_EN  ) )
      {
        pinState |= POWER_12V_ROUTER_EN ;
      }
    }
    if( PowerEn & POWER_12V_CARD_READER_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_CARD_READER_EN  ) )
      {
        pinState |= POWER_12V_CARD_READER_EN ;
      }
    }
    if( PowerEn & POWER_12V_AUDIO_PA_EN  )
    {
      if( !MicoGpioInputGet( GPIO_12V_AUDIO_PA_EN  ) )
      {
        pinState |= POWER_12V_AUDIO_PA_EN ;
      }
    }
    if( PowerEn & POWER_5V_LEFT_HAND_MOTOR_EN  )
    {
      if( !MicoGpioInputGet( GPIO_5V_LEFT_HAND_MOTOR_EN  ) )
      {
        pinState |= POWER_5V_LEFT_HAND_MOTOR_EN ;
      }
    }
    
    if( PowerEn & POWER_5V_RIGHT_HAND_MOTOR_EN  )
    {
      if( !MicoGpioInputGet( GPIO_5V_RIGHT_HAND_MOTOR_EN   ) )
      {
        pinState |= POWER_5V_RIGHT_HAND_MOTOR_EN ;
      }
    }
        if( PowerEn & POWER_5V_HEAD_MOTOR_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_HEAD_MOTOR_EN   ) )
      {
        pinState |= POWER_5V_HEAD_MOTOR_EN  ;
      }
    }
        if( PowerEn & POWER_5V_RESERVE_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_RESERVE_EN   ) )
      {
        pinState |= POWER_5V_RESERVE_EN  ;
      }
    }
        if( PowerEn & POWER_5V_LEDX2_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_LEDX2_EN   ) )
      {
        pinState |= POWER_5V_LEDX2_EN  ;
      }
    }
        if( PowerEn & POWER_5V_REPAIR_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_REPAIR_BOARD_EN   ) )
      {
        pinState |= POWER_5V_REPAIR_EN  ;
      }
    }
    if( PowerEn & POWER_5V_TOUCH_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_TOUCH_EN   ) )
      {
        pinState |= POWER_5V_TOUCH_EN  ;
      }
    }
    if( PowerEn & POWER_5V_KEYBOARD_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_KEYBORAD_EN   ) )
      {
        pinState |= POWER_5V_KEYBOARD_EN  ;
      }
    }
    if( PowerEn & POWER_5V_CARD_READER_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_CARD_READER_EN   ) )
      {
        pinState |= POWER_5V_CARD_READER_EN  ;
      }
    }
    if( PowerEn & POWER_5V_CODE_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_CODE_EN   ) )
      {
        pinState |= POWER_5V_CODE_EN  ;
      }
    }
    if( PowerEn & POWER_5V_CAMERA_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_CAMERA_EN   ) )
      {
        pinState |= POWER_5V_CAMERA_EN  ;
      }
    }
    if( PowerEn & POWER_5V_HD_CAMERA_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_HD_CAMERA_EN   ) )
      {
        pinState |= POWER_5V_HD_CAMERA_EN  ;
      }
    }
    if( PowerEn & POWER_5V_HEAD_LED_EN   )
    {
      if( !MicoGpioInputGet( GPIO_5V_HEAD_LED_EN   ) )
      {
        pinState |= POWER_5V_HEAD_LED_EN  ;
      }
    }
    
    if( PowerEn & POWER_24V_HD_CAMERA   )
    {
      if( !MicoGpioInputGet( GPIO_24V_HD_CAMERA_EN   ) )
      {
        pinState |= POWER_24V_HD_CAMERA  ;
      }
    }
    
    
    if( PowerEn & POWER_24V_CAMERA   )
    {
      if( !MicoGpioInputGet( GPIO_24V_CAMERA_EN   ) )
      {
        pinState |= POWER_24V_CAMERA  ;
      }
    }
    
    
    if( PowerEn & POWER_5V_HEAD_TOUCH   )
    {
      if( !MicoGpioInputGet( GPIO_5V_HEAD_TOUCH_EN   ) )
      {
        pinState |= POWER_5V_HEAD_TOUCH  ;
      }
    }
    
    
                
    return pinState;
}


void SetModulePowerOnOff( uint8_t module, uint8_t onoff )
{
  switch( module )
  {
  case MODULE_5V_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_HEAD_RK_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_HEAD_RK_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_HEAD_RK_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_CHEST_RK_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_CHEST_RK_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_CHEST_RK_EN, POWER_OFF );
    }
    break;
  case MODULE_24V_LEFT_HAND_MOTER_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_LEFT_HAND_MOTER_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_LEFT_HAND_MOTER_EN, POWER_OFF );
    }
    break;
  case MODULE_24V_RIGHT_HAND_MOTOR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_RIGHT_HAND_MOTOR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_RIGHT_HAND_MOTOR_EN, POWER_OFF );
    }
    break;
  case MODULE_24V_HEAD_MOTOR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_HEAD_MOTOR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_HEAD_MOTOR_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_SWITCH_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_SWITCH_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_SWITCH_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_ROUTER_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_ROUTER_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_ROUTER_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_CARD_READER_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_CARD_READER_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_CARD_READER_EN, POWER_OFF );
    }
    break;
  case MODULE_12V_AUDIO_PA_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_AUDIO_PA_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_AUDIO_PA_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_LEFT_HAND_MOTOR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_LEFT_HAND_MOTOR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_LEFT_HAND_MOTOR_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_RIGHT_HAND_MOTOR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_RIGHT_HAND_MOTOR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_RIGHT_HAND_MOTOR_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_HEAD_MOTOR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_HEAD_MOTOR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_HEAD_MOTOR_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_RESERVE_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_RESERVE_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_RESERVE_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_LEDX2_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_LEDX2_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_LEDX2_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_REPAIR_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_REPAIR_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_REPAIR_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_TOUCH_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_TOUCH_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_TOUCH_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_KEYBOARD_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_KEYBOARD_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_KEYBOARD_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_CARD_READER_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_CARD_READER_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_CARD_READER_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_CODE_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_CODE_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_CODE_EN, POWER_OFF );
    }
    break;
    
    case MODULE_5V_CAMERA_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_CAMERA_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_CAMERA_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_HD_CAMERA_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_HD_CAMERA_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_HD_CAMERA_EN, POWER_OFF );
    }
    break;
  case MODULE_5V_HEAD_LED_EN:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_HEAD_LED_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_HEAD_LED_EN, POWER_OFF );
    }
    break;
  case MODULE_24V_HD_CAMERA:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_HD_CAMERA, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_HD_CAMERA, POWER_OFF );
    }
    break;
  case MODULE_24V_CAMERA:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_CAMERA, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_CAMERA, POWER_OFF );
    }
    break;
    case MODULE_5V_HEAD_TOUCH:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_HEAD_TOUCH, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_HEAD_TOUCH, POWER_OFF );
    }
    break;
    }
}

void BSP_Control_Sigal(PowerControl_TypeDef PowerCon, ControlSignal_TypeDef isHold)
{
  if( CONTROL_RELEASE == isHold )
  {
#if 1
    switch( PowerCon )
    {
    case POWER_CHEST_RK:
      MicoGpioOutputLow( GPIO_12V_CHEST_RK_SIGNAL ); 
      break;
    case POWER_HEAD_RK:
      MicoGpioOutputLow( GPIO_12V_HEAD_RK_SIGNAL ); 
      break;
    default:
      break;
    }
#else
    if( PowerCon & POWER_CHEST_RK )
    {
      MicoGpioOutputLow( GPIO_12V_CHEST_RK_SIGNAL ); 
    }
    if( PowerCon & POWER_HEAD_RK )
    {
      MicoGpioOutputLow( GPIO_12V_HEAD_RK_SIGNAL ); 
    }
#endif
  }
  else if( CONTROL_HOLD == isHold )
  {
#if 1
    switch( PowerCon )
    {
    case POWER_CHEST_RK:
      MicoGpioOutputHigh( GPIO_12V_CHEST_RK_SIGNAL ); 
      break;
    case POWER_HEAD_RK:
      MicoGpioOutputHigh( GPIO_12V_HEAD_RK_SIGNAL ); 
      break;
    default:
      break;
    }
#else
    if( PowerCon & POWER_CHEST_RK )
    {
      MicoGpioOutputHigh( GPIO_12V_CHEST_RK_SIGNAL ); 
    }
    if( PowerCon & POWER_HEAD_RK )
    {
      MicoGpioOutputHigh( GPIO_12V_HEAD_RK_SIGNAL ); 
    }
#endif
  }
}


PowerControlState_TypeDef GetRkState(PowerControl_TypeDef  select)
{
    if(select == POWER_CHEST_RK)
    {
        if(chest_rk_control_signal->isDeviceProcessOver == NO)
        {
            if( (DEVICE_POWER_OFF == chest_rk_control_signal->deviceOnOff) /*&& (chest_rk_control_signal->startTime != 0)*/ )
            {
                return POWER_ON_PROCESSING;
            }
            if( (DEVICE_POWER_ON == chest_rk_control_signal->deviceOnOff) /*&& (chest_rk_control_signal->startTime != 0)*/ )
            {
                return POWER_OFF_PROCESSING;
            }
        }
        else
        {
            if(DEVICE_POWER_ON == chest_rk_control_signal->deviceOnOff)
            {
                return POWER_ON_ALREADY;
            }
            if(DEVICE_POWER_OFF == chest_rk_control_signal->deviceOnOff)
            {
                return POWER_OFF_ALREADY;
            }
        }
    }
      
    if(select == POWER_HEAD_RK)
    {
        if(head_rk_control_signal->isDeviceProcessOver == NO)
        {
            if( (DEVICE_POWER_OFF == head_rk_control_signal->deviceOnOff)/* && (head_rk_control_signal->startTime != 0)*/ )
            {
                return POWER_ON_PROCESSING;
            }
            if( (DEVICE_POWER_ON == head_rk_control_signal->deviceOnOff) /*&& (head_rk_control_signal->startTime != 0)*/ )
            {
                return POWER_OFF_PROCESSING;
            }
        }
        else
        {
            if(DEVICE_POWER_ON == head_rk_control_signal->deviceOnOff)
            {
                return POWER_ON_ALREADY;
            }
            if(DEVICE_POWER_OFF == head_rk_control_signal->deviceOnOff)
            {
                return POWER_OFF_ALREADY;
            }
        }
    }
    return POWER_ERR_STATE;   
}

#define RK_STATE_PARA_ERR       0x06
uint8_t SetRkState(PowerControl_TypeDef rk, uint8_t on_off)
{
    if(rk == POWER_HEAD_RK)
    {
        if(on_off == 1)
        {
            PowerOnDevices(POWER_HEAD_RK);
            return 1;
        }
        if(on_off == 0)
        {
            PowerOffDevices(POWER_HEAD_RK);
            return 0;
        } 
        return RK_STATE_PARA_ERR;
    }
    if(rk == POWER_CHEST_RK)
    {
        if(on_off == 1)
        {
            PowerOnDevices(POWER_CHEST_RK);
            return 1;
        }
        if(on_off == 0)
        {
            PowerOffDevices(POWER_CHEST_RK);
            return 0;
        }    
        
    }
    return RK_STATE_PARA_ERR;
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

