/* 
*  Author: Adam Huang
*  Date:2016/6/4
*/
#include "app_platform.h"
#include <stdlib.h>
#include "board_init.h"
#include "protocol.h"
#include "upgrade_flash.h"

//#define JOY_TEST

#define platform_log(M, ...) custom_log("Platform", M, ##__VA_ARGS__)
#define platform_log_trace() custom_log_trace("Platform")

static switch_t ramSwitch_user;
switch_t *switch_user = &ramSwitch_user;

static boardStatus_t ramBoardStatus;
boardStatus_t *boardStatus = &ramBoardStatus;

//static controlSignal_t ramDLP_ControlSignal, ramPAD_ControlSignal ;
//static controlSignal_t ramNV_ControlSignal;

//controlSignal_t *DLP_ControlSignal = &ramDLP_ControlSignal;
//controlSignal_t *PAD_ControlSignal = &ramPAD_ControlSignal;

static controlSignal_t ram_head_rk_ontrolSignal;
static controlSignal_t ram_chest_rk_controlSignal;

controlSignal_t *head_rk_control_signal = &ram_head_rk_ontrolSignal;
controlSignal_t *chest_rk_control_signal = &ram_chest_rk_controlSignal;

//controlSignal_t *NV_ControlSignal = &ramNV_ControlSignal;

//static void Switch_Tick( void );
static void ControlSignal_Tick( void );
static void BoardStatus_Tick( void );

OSStatus Platform_Init( void )
{ 
  OSStatus err = kNoErr;
  if( switch_user )
  {
    switch_user->switchOnOff = SWITCH_OFF;
    switch_user->debounceTime = SWITCH_DEBOUNCE_TIME;
    switch_user->getSwitchState = BSP_SW_GetState;
    switch_user->preIOState = switch_user->getSwitchState( SWITCH_USER );
  }
  
  require_action( boardStatus, exit, err = kNoMemoryErr );
  memset(boardStatus, 0x0, sizeof(boardStatus_t));
  if( boardStatus )
  {
    boardStatus->isPowerOffFinish = YES;
    boardStatus->isPowerOnFinish = YES;
    boardStatus->devicesOnOff = DEVICES_OFF;
    boardStatus->setPowerOnoff = BSP_Power_OnOff;
    //boardStatus->irled_duty = 50;
  }

  require_action( head_rk_control_signal, exit, err = kNoMemoryErr );
  memset(head_rk_control_signal, 0x0, sizeof(controlSignal_t));
  if( head_rk_control_signal )
  {
    head_rk_control_signal->powerOnDelayTime = HEAD_RK_POWER_ON_DELAY_TIME;
    head_rk_control_signal->powerOffDelayTime = HEAD_RK_POWER_OFF_DELAY_TIME;
    head_rk_control_signal->powerOnHoldTime = HEAD_RK_POWER_ON_HOLD_TIME;
    head_rk_control_signal->powerOffHoldTime = HEAD_RK_POWER_OFF_HOLD_TIME;
    head_rk_control_signal->setControlSigal = BSP_Control_Sigal;
    head_rk_control_signal->isDeviceProcessOver = YES;
  }

  require_action( chest_rk_control_signal, exit, err = kNoMemoryErr );
  memset(chest_rk_control_signal, 0x0, sizeof(controlSignal_t));
  if( chest_rk_control_signal )
  {
    chest_rk_control_signal->powerOnDelayTime = CHEST_RK_POWER_ON_DELAY_TIME;
    chest_rk_control_signal->powerOffDelayTime = CHEST_RK_POWER_OFF_DELAY_TIME;
    chest_rk_control_signal->powerOnHoldTime = CHEST_RK_POWER_ON_HOLD_TIME;
    chest_rk_control_signal->powerOffHoldTime = CHEST_RK_POWER_OFF_HOLD_TIME;
    chest_rk_control_signal->setControlSigal = BSP_Control_Sigal;
    chest_rk_control_signal->isDeviceProcessOver = YES;
  }
 
exit:
  if( err != kNoErr )
  {
    platform_log("platform init error: no enough memory!");
  }
  else
  {
    platform_log("platform init success!");
  }
  return err;
}

void PowerOnDevices( uint8_t select )
{
  //if( DEVICES_OFF == boardStatus->devicesOnOff )
  {
    //if( (YES == boardStatus->isPowerOffFinish) && (YES == boardStatus->isPowerOnFinish) )
    {
      
      
      if(select == POWER_CHEST_RK)
      {
          if(chest_rk_control_signal->isDeviceProcessOver == YES)
          {
                chest_rk_control_signal->isDeviceProcessOver = NO;
                chest_rk_control_signal->startTime = os_get_time();
                platform_log("CHEST RK start to boot");         
          }
          else
          {
              printf("\r\nShould Wait CHEST RK processing is Over First! !\r\n");
              goto exit;
          }
      }
      
      if(select == POWER_HEAD_RK)
      {
          if(head_rk_control_signal->isDeviceProcessOver == YES)
          {
                head_rk_control_signal->isDeviceProcessOver = NO;      
                head_rk_control_signal->startTime = os_get_time();
                platform_log("HEAD RK start to boot");
          }
          else
          {
              printf("\r\nShould Wait HEAD RK processing is Over First! !\r\n");
              goto exit;
          }
      } 
      
      boardStatus->startTime = os_get_time();
      boardStatus->isPowerOnFinish = NO;
      
      boardStatus->setPowerOnoff(POWER_12V_EN, POWER_ON);
      
      boardStatus->sysStatus &= ~(STATE_RUN_BITS);
      boardStatus->sysStatus |= STATE_IS_POWER_ON;  
    }
  }
exit:
  return;
}

void PowerOffDevices( uint8_t select )
{
  //if( DEVICES_ON == boardStatus->devicesOnOff )
  {
    //if( (YES == boardStatus->isPowerOnFinish) && (YES == boardStatus->isPowerOffFinish) )
    {
      
      
      if(select == POWER_CHEST_RK)
      {
            if(chest_rk_control_signal->isDeviceProcessOver == YES)
            {
                chest_rk_control_signal->isDeviceProcessOver = NO;
                chest_rk_control_signal->startTime = os_get_time();
                platform_log("CHEST RK start to shutdown");
            }
            else
            {
                printf("\r\nShould Wait CHEST RK processing is Over First! !\r\n");
                goto exit;
            }
            
      }
      if(select == POWER_HEAD_RK)
      {
            if(head_rk_control_signal->isDeviceProcessOver == YES)
            {
                head_rk_control_signal->isDeviceProcessOver = NO;      
                head_rk_control_signal->startTime = os_get_time();
                platform_log("HEAD RK start to shutdown");
            }
            else
            {
                printf("\r\nShould Wait HEAD RK processing is Over First! !\r\n");
                goto exit;
            }
            
      }
      
      boardStatus->startTime = os_get_time();
      boardStatus->isPowerOffFinish = NO;
      
      boardStatus->sysStatus &= ~(STATE_RUN_BITS);
      boardStatus->sysStatus |= STATE_IS_POWER_OFF;
      //setCurLedsMode(LIGHTS_MODE_GOODEBYE);
    }
  }
exit:
  return;
}

void PowerOn(void)
{
    PowerOnDevices(POWER_HEAD_RK);
    PowerOnDevices(POWER_CHEST_RK);
    BSP_Power_OnOff(POWER_ALL, POWER_ON);
    
}
void PowerOff(void)
{
    PowerOffDevices(POWER_HEAD_RK);
    PowerOffDevices(POWER_CHEST_RK);
    //BSP_Power_OnOff(POWER_ALL & (~(POWER_12V_EN | POWER_12V_EN)), POWER_OFF);
    
}
static void Switch_Tick( void )
{
  if( /*(YES != switch_user->isSwitchOver) && */(switch_user->getSwitchState != NULL) )
  {
    if( (switch_user->startTime != 0) && ((os_get_time() - switch_user->startTime) >= SWITCH_DEBOUNCE_TIME) )
    {
      switch_user->startTime = 0;
      
      if( switch_user->preIOState != switch_user->getSwitchState( SWITCH_USER ) )
      {
        switch_user->preIOState = switch_user->getSwitchState( SWITCH_USER );
      }
      else
      {
        return;
      }

      switch_user->isSwitchOver = YES;
      if( (STATE_POWER_OFF == (boardStatus->sysStatus & STATE_RUN_BITS))/* && (voltageConvert->bat_voltage >= VBAT_POWER_ON_LEVEL)*/ )
      {
          switch_user->switchOnOff = SWITCH_ON;
          platform_log("confirm to ON stable");
          if( boardStatus->sysStatus & STATE_IS_LOW_POWER )
          {
            boardStatus->sysStatus &= ~STATE_IS_LOW_POWER;
          }
         
          PowerOn();
      }
      if( ( STATE_POWER_ON == (boardStatus->sysStatus & STATE_RUN_BITS) ) )
      {
          switch_user->switchOnOff = SWITCH_OFF;
          platform_log("confirm to OFF stable");
          PowerOff();
      }
    }
  }
}
void setModulePowerSignalOnOff( uint8_t module, uint8_t onoff )
{
  switch( module )
  {
  case MODULE_HEAD_RK:
    if( onoff )
    {
      if( head_rk_control_signal->deviceOnOff == DEVICE_POWER_OFF )
      {
        head_rk_control_signal->isDeviceProcessOver = NO;
        head_rk_control_signal->startTime = os_get_time();
      }
    }
    else
    {
      if( head_rk_control_signal->deviceOnOff == DEVICE_POWER_ON )
      {
        head_rk_control_signal->isDeviceProcessOver = NO;
        head_rk_control_signal->startTime = os_get_time();
      }
    }
    break;

  case MODULE_CHEST_RK:
    if( onoff )
    {
      if( chest_rk_control_signal->deviceOnOff == DEVICE_POWER_OFF )
      {
        chest_rk_control_signal->isDeviceProcessOver = NO;
        chest_rk_control_signal->startTime = os_get_time();
      }
    }
    else
    {
      if( chest_rk_control_signal->deviceOnOff == DEVICE_POWER_ON )
      {
        chest_rk_control_signal->isDeviceProcessOver = NO;
        chest_rk_control_signal->startTime = os_get_time();
      }
    }
    break;
  default:
    break;
  }
}

static void ControlSignal_Tick( void )
{
/*  */
  if( (NO == head_rk_control_signal->isDeviceProcessOver) && (head_rk_control_signal->setControlSigal != NULL) )
  {
    if( (DEVICE_POWER_OFF == head_rk_control_signal->deviceOnOff) && (head_rk_control_signal->startTime != 0) )
    {
      switch( head_rk_control_signal->period )
      {
      case SIGNAL_DELAY:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_ON_DELAY_TIME )
        {
          head_rk_control_signal->startTime = os_get_time();
          head_rk_control_signal->setControlSigal(POWER_HEAD_RK, CONTROL_HOLD);
          head_rk_control_signal->period = SIGNAL_HOLD;
          platform_log("HEAD RK boot signal hold on");
        }
        break;
      case SIGNAL_HOLD:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_ON_HOLD_TIME )
        {
          head_rk_control_signal->startTime = os_get_time();
          head_rk_control_signal->setControlSigal(POWER_HEAD_RK, CONTROL_RELEASE);
          head_rk_control_signal->period = SIGNAL_REALSE;
          platform_log("HEAD RK boot signal hold off");
        }
        break;
      case SIGNAL_REALSE:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_ON_PROCESSING_TIME )
        {
          head_rk_control_signal->startTime = 0;
          head_rk_control_signal->period = SIGNAL_DELAY;
          head_rk_control_signal->deviceOnOff = DEVICE_POWER_ON;
          head_rk_control_signal->isDeviceProcessOver = YES;
          platform_log("HEAD RK boot have powered on");
        }
        break;
      default: 
        break;       
      }
    }
    else if( (DEVICE_POWER_ON == head_rk_control_signal->deviceOnOff) && (head_rk_control_signal->startTime != 0) )
    {
      switch( head_rk_control_signal->period )
      {
      case SIGNAL_DELAY:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_OFF_DELAY_TIME )
        {
          head_rk_control_signal->startTime = os_get_time();
          head_rk_control_signal->setControlSigal(POWER_HEAD_RK, CONTROL_HOLD);
          head_rk_control_signal->period = SIGNAL_HOLD;
          platform_log("HEAD RK shutdown signal hold on");
        }
        break;
      case SIGNAL_HOLD:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_OFF_HOLD_TIME )
        {
          head_rk_control_signal->startTime = os_get_time();
          head_rk_control_signal->setControlSigal(POWER_HEAD_RK, CONTROL_RELEASE);
          head_rk_control_signal->period = SIGNAL_REALSE;
          platform_log("HEAD RK shutdown signal hold off");
        }
        break;
      case SIGNAL_REALSE:
        if( os_get_time() - head_rk_control_signal->startTime >= HEAD_RK_POWER_OFF_PROCESSING_TIME )
        {
          head_rk_control_signal->startTime = 0;
          head_rk_control_signal->period = SIGNAL_DELAY;
          head_rk_control_signal->deviceOnOff = DEVICE_POWER_OFF;
          head_rk_control_signal->isDeviceProcessOver = YES;
          platform_log("HEAD RK shutdown have powered off");
        }
        break;
      default:
        break;       
      }
    }
  }
/* pad */
  if( (NO == chest_rk_control_signal->isDeviceProcessOver) && (chest_rk_control_signal->setControlSigal != NULL) )
  {
    if( (DEVICE_POWER_OFF == chest_rk_control_signal->deviceOnOff) && (chest_rk_control_signal->startTime != 0) )
    {
      switch( chest_rk_control_signal->period )
      {
      case SIGNAL_DELAY:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_ON_DELAY_TIME )
        {
          chest_rk_control_signal->startTime = os_get_time();
          chest_rk_control_signal->setControlSigal(POWER_CHEST_RK, CONTROL_HOLD);
          chest_rk_control_signal->period = SIGNAL_HOLD;
          platform_log("CHEST RK boot signal hold on");
        }
        break;
      case SIGNAL_HOLD:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_ON_HOLD_TIME )
        {
          chest_rk_control_signal->startTime = os_get_time();
          chest_rk_control_signal->setControlSigal(POWER_CHEST_RK, CONTROL_RELEASE);
          chest_rk_control_signal->period = SIGNAL_REALSE;
          platform_log("CHEST RK boot signal hold off");
        }
        break;
      case SIGNAL_REALSE:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_ON_PROCESSING_TIME )
        {
          chest_rk_control_signal->startTime = 0;
          chest_rk_control_signal->period = SIGNAL_DELAY;
          chest_rk_control_signal->deviceOnOff = DEVICE_POWER_ON;
          chest_rk_control_signal->isDeviceProcessOver = YES;
          platform_log("CHEST RK boot have powered on");
        }
        break;
      default:
        break;       
      }
    }
    else if( (DEVICE_POWER_ON == chest_rk_control_signal->deviceOnOff) && (chest_rk_control_signal->startTime != 0) )
    {
      switch( chest_rk_control_signal->period )
      {
      case SIGNAL_DELAY:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_OFF_DELAY_TIME )
        {
          chest_rk_control_signal->startTime = os_get_time();
          chest_rk_control_signal->setControlSigal(POWER_CHEST_RK, CONTROL_HOLD);
          chest_rk_control_signal->period = SIGNAL_HOLD;
          platform_log("CHEST RK shutdown signal hold on");
        }
        break;
      case SIGNAL_HOLD:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_OFF_HOLD_TIME )
        {
          chest_rk_control_signal->startTime = os_get_time();
          chest_rk_control_signal->setControlSigal(POWER_CHEST_RK, CONTROL_RELEASE);
          chest_rk_control_signal->period = SIGNAL_REALSE;
          platform_log("CHEST RK shutdown signal hold off");
        }
        break;
      case SIGNAL_REALSE:
        if( os_get_time() - chest_rk_control_signal->startTime >= CHEST_RK_POWER_OFF_PROCESSING_TIME )
        {
          chest_rk_control_signal->startTime = 0;
          chest_rk_control_signal->period = SIGNAL_DELAY;
          chest_rk_control_signal->deviceOnOff = DEVICE_POWER_OFF;
          chest_rk_control_signal->isDeviceProcessOver = YES;
          platform_log("CHEST RK shutdown have powered off");
        }
        break;
      default:
        break;       
      }
    }
  }
}

static void BoardStatus_Tick( void )
{
  // boot 
  if( (NO == boardStatus->isPowerOnFinish) && (YES == boardStatus->isPowerOffFinish) )
  {
    if( os_get_time() - boardStatus->startTime >= POWER_ON_PROCEESING_TIME )
    {
      boardStatus->startTime = 0;
      boardStatus->isPowerOnFinish = YES;
      boardStatus->devicesOnOff = DEVICES_ON;
      platform_log("all devices boot finished!");
      boardStatus->sysStatus &= ~(STATE_RUN_BITS);
      boardStatus->sysStatus |= STATE_POWER_ON;

    }
  }
  //shutdown
  if( (NO == boardStatus->isPowerOffFinish) && (YES == boardStatus->isPowerOnFinish) )
  {
    if( os_get_time() - boardStatus->startTime >= POWER_OFF_PROCEESING_TIME )
    {
      boardStatus->startTime = 0;
      boardStatus->isPowerOffFinish = YES;
      boardStatus->setPowerOnoff(POWER_ALL, POWER_OFF);
      boardStatus->devicesOnOff = DEVICES_OFF;
      platform_log("all devices shutdown finished");
      boardStatus->sysStatus &= ~(STATE_RUN_BITS);
      boardStatus->sysStatus |= STATE_POWER_OFF;
      
      //BSP_Power_OnOff(POWER_12V_EN | POWER_5V_EN, POWER_OFF);
      BSP_Power_OnOff(POWER_ALL, POWER_OFF);
      if( ( REBOOT_YES == boardStatus->rebootFlag )
         && (voltageConvert->v_bat >= VBAT_POWER_ON_LEVEL) )
      {       
        boardStatus->startTime = os_get_time();
        //platform_log("board start to reboot after 5 seconds");
        platform_log("board start to reboot right now");
        flashTable.isNeedAutoBoot = 'Y';
        MICOBootConfiguration( &flashTable );
        NVIC_SystemReset();
      }
      else
      {
        NVIC_SystemReset();
        boardStatus->sysStatus |= SYSTEM_IS_SLEEP;
        halEnterSleepMode();
      }      
    }
  }
  if( (REBOOT_YES == boardStatus->rebootFlag) && (YES == boardStatus->isPowerOffFinish) )
  {
    if( os_get_time() - boardStatus->startTime >= BOARD_REBOOT_WAITING_TIME )
    {
      //PowerOnDevices(POWER_HEAD_RK);
      //PowerOnDevices(POWER_CHEST_RK);
      PowerOn();
      boardStatus->rebootFlag = REBOOT_NO;
    }
  }
}


uint32_t getEachModuleStates( void )
{
  uint32_t states;
  
  states = GetModulePowerState( POWER_ALL );

  if( DEVICE_POWER_ON == head_rk_control_signal->deviceOnOff )
  {
      states |= POWER_HEAD_RK;
  }

  if( DEVICE_POWER_ON == chest_rk_control_signal->deviceOnOff )
  {
      states |= POWER_CHEST_RK;
  }
  return states;
}

void Platform_Tick( void )
{
  Switch_Tick();
  ControlSignal_Tick();
  BoardStatus_Tick();
}

void bsp_Init( void )
{
  board_gpios_init();
  //charger_detect_init();
  //key_switch_init();
}


void key_switch_interrupt_cb( void )
{
  if( boardStatus->sysStatus & SYSTEM_IS_SLEEP )
  {
    boardStatus->sysStatus &= ~(SYSTEM_IS_SLEEP);
    halWakeupFormSLeep();       
  }   
  switch_user->isSwitchOver = NO;
  switch_user->startTime = os_get_time();
  platform_log("switch have switched");
}
/*********************END OF FILE**************/
