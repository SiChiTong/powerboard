/* 
*  Author: Adam Huang
*  Date:2016/12/13
*/
#include "protocol.h"
#include "app_platform.h"
#include <stdlib.h>
#include "serial_leds.h"
#include "voltage_detect.h"
#include "stringUtils.h"
//#include "nbosDriverFlash.h"
#include "upgrade_flash.h"
#include "tps611xx_bl.h"

#define protocol_log(M, ...) custom_log("Protocol", M, ##__VA_ARGS__)
#define protocol_log_trace() custom_log_trace("Protocol")


static  uint8_t    rxBuf[UART_RX_BUFFER_LENGTH];
static  uint8_t    txBuf[UART_TX_BUFFER_LENGTH];

recBuf_t                rxInfoInRam;

serial_t                serialDataInRam = {
    .rx_buf = {
        .pBuffer = rxBuf,
        .offset = rxBuf + 3, //header,length,ctype
        .bufferSize = UART_RX_BUFFER_LENGTH,
    },
    .tx_buf = {
        .pBuffer = txBuf,
        .offset = txBuf + 2, //header,length
        .bufferSize = UART_TX_BUFFER_LENGTH,
    },
};

uart_serial_t           uartSerialInRam;

serial_t * const serial = &serialDataInRam;
//UART_HandleTypeDef      ComUartHandle;

static OSStatus uart_frame_send( serial_t *serial, const uint8_t *pData, uint32_t size );
static OSStatus recSerialLedsFrameProcess( serial_t *serial );
static OSStatus ackSerialLedsFrameProcess( serial_t *serial );
static OSStatus recReadSysStatusVbatFrameProcess( serial_t *serial );
static OSStatus ackReadSysStatusVbatFrameProcess( serial_t *serial, uint8_t cmd_num );
static OSStatus recReadModuleStatusFrameProcess( serial_t *serial );
static OSStatus ackReadModuleStatusFrameProcess( serial_t *serial, uint8_t cmd_num );
static OSStatus recReadFaultStatusFrameProcess( serial_t *serial );
static OSStatus ackReadFaultStatusFrameProcess( serial_t *serial, uint8_t cmd_num );
static OSStatus recModuleControlFrameProcess( serial_t *serial );
static OSStatus ackModuleControlFrameProcess( serial_t *serial, uint8_t ack );
static OSStatus ackNotSupportFrameProcess( serial_t *serial, uint8_t ctype );
static OSStatus recTestCurrentCmdFrame( serial_t *serial );
static OSStatus ackReadErrChannelFrameProcess( serial_t *serial );
static OSStatus recIRLedControlFrameProcess( serial_t *serial );
static OSStatus ackIRLedControlFrameProcess( serial_t *serial );

OSStatus Protocol_Init( void )//( serial_t *serial )
{
  OSStatus err = kNoErr;
  
  //serial = &serialDataInRam;
  //require_action( serial , exit, err = kGeneralErr );  
  if( serial->isSerialInitialized )
  {    
//    if( STATE_POWER_ON != (boardStatus->sysStatus & STATE_RUN_BITS) )
    {
      goto exit;
    }
    MicoUartFinalize( COMM_UART );
    serial->isSerialInitialized = 0;
//    protocol_log("protocol had initialized, deinit and init again");
  }
  
  serial->rx_info = &rxInfoInRam;
  require_action( serial->rx_info , exit, err = kGeneralErr );
  serial->uart_serial = &uartSerialInRam;
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  serial->rx_info->pData = 0;
  serial->rx_info->rxBuffer = rxBuf;
  serial->rx_info->rxCount = 0;

 // serial->uart_serial->uartHandle = &ComUartHandle;
 // require_action( serial->uart_serial->uartHandle, exit, err = kGeneralErr );

//  serial->uart_serial->UARTx = USART_COM;
  
  err = uart_ser_open( serial->uart_serial, 115200 );  
  require_action( err == kNoErr, exit,  protocol_log("open uart err") ); 
  
  require_action( serial->rx_buf.pBuffer , exit, err = kGeneralErr );  
  memset( (uint8_t *)serial->rx_buf.pBuffer, 0x00, serial->rx_buf.bufferSize );
#ifdef COMM_DMA_USE_INT
  startDmaRecive( serial->uart_serial->uartHandle, (uint8_t *)serial->rx_buf.pBuffer );
#else
  startDmaRecive( COMM_UART, (uint8_t *)serial->rx_buf.pBuffer );
#endif
  serial->isStartDmaReceive = 1;  
  serial->isSerialInitialized = 1;
  protocol_log( "protocol initialize success" );
  
exit:
  return err;
}

static OSStatus recSerialLedsFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t       ligthMode;
  uint16_t      lightEffect;
  recSerialLedsFrame_t *recSerialLedsFrame;
    
  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  recSerialLedsFrame = (recSerialLedsFrame_t *)serial->rx_buf.offset;
  ligthMode = recSerialLedsFrame->lightMode;
  lightEffect = (recSerialLedsFrame->lightEffectH << 8) | recSerialLedsFrame->lightEffectL;
  
  if( STATE_IS_POWER_OFF != (boardStatus->sysStatus & STATE_RUN_BITS) )
  {
    setSerialLedsEffect( (lightsMode_t)ligthMode, lightEffect );
  }

  err = ackSerialLedsFrameProcess( serial );

exit:
  return err;
}

static OSStatus ackSerialLedsFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof(ackSerialLedsFrame_t);
  ackSerialLedsFrame_t *ackSerialLedsFrame;

  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );  

  ackSerialLedsFrame = (ackSerialLedsFrame_t *)serial->tx_buf.offset;
  require_action( ackSerialLedsFrame , exit, err = kGeneralErr );

  ackSerialLedsFrame->ctype = FRAME_TYPE_LEDS_CONTROL;
  ackSerialLedsFrame->curLightMode = serial_leds->modeType;
  ackSerialLedsFrame->curLightEffectH = (serial_leds->effectType & 0xff00) >> 8;
  ackSerialLedsFrame->curLightEffectL = (serial_leds->effectType & 0x00ff);

  err = uart_frame_send( serial, (uint8_t *)ackSerialLedsFrame, length );

exit:
  return err;
}

static OSStatus 
uart_frame_send( serial_t *serial, const uint8_t *pData, uint32_t size )
{
  OSStatus err = kNoErr;
  uint8_t  checkSum;
  ram_buff_t *tx_buff = NULL;
  
  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );  
  
  tx_buff = &serial->tx_buf;
  require_action( tx_buff->pBuffer , exit, err = kGeneralErr );  
  require_action( size <= tx_buff->bufferSize - 4, exit, err = kParamErr );
  
  *(uint8_t *)tx_buff->pBuffer = FRAME_HEADER;
  *(uint8_t *)(tx_buff->pBuffer + 1) = size + 4;//header,length,footer,checksum
  
  checkSum = 0;
  tx_buff->pData = (uint8_t *)tx_buff->pBuffer;
  if( pData != tx_buff->offset )
  {
    memmove( (void *)tx_buff->offset, (void *)pData, size );
  }
  for( uint8_t i = 0; i < size + 2; i++ )
  {
    checkSum += *tx_buff->pData ++;
  }
  *tx_buff->pData ++ = checkSum;  
  *tx_buff->pData ++ = FRAME_FOOTER;
  
  err = uart_ser_write( serial->uart_serial, \
    (uint8_t *)tx_buff->pBuffer, tx_buff->pData - tx_buff->pBuffer );
  
  memset( (uint8_t *)tx_buff->pBuffer, 0x00, tx_buff->bufferSize );
  
exit:
  return err;
}

static OSStatus recReadSysStatusVbatFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t  cmd_num;
  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  cmd_num = *(uint8_t *)serial->rx_buf.offset;

  err = ackReadSysStatusVbatFrameProcess( serial, cmd_num );

exit:
  return err;
}

static OSStatus ackReadSysStatusVbatFrameProcess( serial_t *serial, uint8_t cmd_num )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof(ackSysStatusVbatFrame_t);
  ackSysStatusVbatFrame_t *ackSysStatusVbatFrame;

  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );
  

  ackSysStatusVbatFrame = (ackSysStatusVbatFrame_t *)serial->tx_buf.offset;
  require_action( ackSysStatusVbatFrame , exit, err = kGeneralErr );

  ackSysStatusVbatFrame->ctype = FRAME_TYPE_S_SYS_V_BAT;
  ackSysStatusVbatFrame->cmd_num = cmd_num;
  switch( ackSysStatusVbatFrame->cmd_num )
  {
    case SYS_VBAT_READ_CMD:
    ackSysStatusVbatFrame->sysStatus = boardStatus->sysStatus;
    ackSysStatusVbatFrame->voltage = voltageConvert->bat_voltage/10;
    break;
    case SYS_VCHARGE_READ_CMD:
    ackSysStatusVbatFrame->sysStatus = boardStatus->sysStatus;
    ackSysStatusVbatFrame->voltage = boardStatus->vChargerLevel;
    break;
    case SYS_PSOC_READ_CMD:
    ackSysStatusVbatFrame->sysStatus = boardStatus->sysStatus;
    ackSysStatusVbatFrame->voltage = boardStatus->vBatLevel & 0x00FF;
    break;
    default:
    ackSysStatusVbatFrame->sysStatus = boardStatus->sysStatus;
    ackSysStatusVbatFrame->voltage = 0xFFFF;
    break;
  }
  err = uart_frame_send( serial, (uint8_t *)ackSysStatusVbatFrame, length );

exit:
  return err;
}

static OSStatus recReadModuleStatusFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t  cmd_num;
  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  cmd_num = *(uint8_t *)serial->rx_buf.offset;

  err = ackReadModuleStatusFrameProcess( serial, cmd_num );

exit:
  return err;
}

static OSStatus ackReadModuleStatusFrameProcess( serial_t *serial, uint8_t cmd_num )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof( ackModuleStatusFrame_t );
  ackModuleStatusFrame_t *ackModuleStatusFrame;
  uint32_t moduleState;

  require_action( serial , exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );
  

  ackModuleStatusFrame = (ackModuleStatusFrame_t *)serial->tx_buf.offset;
  require_action( ackModuleStatusFrame , exit, err = kGeneralErr );

  ackModuleStatusFrame->ctype = FRAME_TYPE_MODULE_STATE;
  ackModuleStatusFrame->cmd_num = cmd_num;

  switch( ackModuleStatusFrame->cmd_num )
  {
    case MODULE_STATUS_CMD_NUM:
    moduleState = getEachModuleStates(); 
    memcpy( ackModuleStatusFrame->moduleStatus, (uint8_t *)&moduleState, sizeof(uint32_t) );
    break;
    default:
    memset( ackModuleStatusFrame->moduleStatus, 0xFF, 4 );
    break;
  }
  err = uart_frame_send( serial, (uint8_t *)ackModuleStatusFrame, length );

exit:
  return err;
}

static OSStatus recReadFaultStatusFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t  cmd_num;
  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  cmd_num = *(uint8_t *)serial->rx_buf.offset;

  err = ackReadFaultStatusFrameProcess( serial, cmd_num );

exit:
  return err;
}

static OSStatus ackReadFaultStatusFrameProcess( serial_t *serial, uint8_t cmd_num )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof( ackFaultStatusFrame_t );
  ackFaultStatusFrame_t *ackFaultStatusFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );
  

  ackFaultStatusFrame = (ackFaultStatusFrame_t *)serial->tx_buf.offset;
  require_action( ackFaultStatusFrame , exit, err = kGeneralErr );

  ackFaultStatusFrame->ctype = FRAME_TYPE_FAULT_BIT;
  ackFaultStatusFrame->cmd_num = cmd_num;

  switch( ackFaultStatusFrame->cmd_num )
  {
    case FAULT_STATUS_CMD_NUM:
    memcpy( ackFaultStatusFrame->faultBit, voltageConvertData->faultBitTemp, 4);
    break;
    default:
    memset( ackFaultStatusFrame->faultBit, 0xFF, 4 );
    break;
  }
  err = uart_frame_send( serial, (uint8_t *)ackFaultStatusFrame, length );

exit:
  return err;
}

static void setModulePowerOnOff( uint8_t module, uint8_t onoff )
{
  switch( module )
  {
  case MOTOR_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_MOTOR, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_MOTOR, POWER_OFF );
    }
    break;
  case SENSOR_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_SENSOR, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_SENSOR, POWER_OFF );
    }
    break;
  case LEDS_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_LEDS, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_LEDS, POWER_OFF );
    }
    break;
  case _5VRESERVE_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_RES, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_RES, POWER_OFF );
    }
    break;
  case _12V_ROUTER_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_RES, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_RES, POWER_OFF );
    }
    break;
  case _2_1_PA_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_2_1_PA, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_2_1_PA, POWER_OFF );
    }
    break;
  case DYP_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_DYP, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_DYP, POWER_OFF );
    }
    break;
  case _12V_RESERVE_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_RES, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_RES, POWER_OFF );
    }
    break;
  case PRINTER_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_PRINTER, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_PRINTER, POWER_OFF );
    }
    break;
  case _24V_RESERVE_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_RES, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_RES, POWER_OFF );
    }
    break;
  case BAT_NV_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_BAT_NV, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_BAT_NV, POWER_OFF );
    }
    break;
  case _5V_ALL_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_EN, POWER_OFF );
    }
    break;
  case _12V_ALL_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_12V_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_12V_EN, POWER_OFF );
    }
    break;
  case _24V_ALL_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_24V_EN, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_24V_EN, POWER_OFF );
    }
    break;
  case AIUI_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_AIUI, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_AIUI, POWER_OFF );
    }
    break;
  case _5V_ROUTER_MODULE:
    if( onoff == 0x01 ) {
      BSP_Power_OnOff( POWER_5V_ROUTOR, POWER_ON );
    } 
    else {
      BSP_Power_OnOff( POWER_5V_ROUTOR, POWER_OFF );
    }
    break;
  }
}

static OSStatus recModuleControlFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  recModuleControlFrame_t *recModuleControlFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  recModuleControlFrame = (recModuleControlFrame_t *)serial->rx_buf.offset;
  require_action( recModuleControlFrame , exit, err = kGeneralErr );

  switch( recModuleControlFrame->module )
  {
    case SYSTEM_MODULE:
    if( recModuleControlFrame->control == 0x00 )//shutdown system
    {
        PowerOffDevices();
    }
    else if( recModuleControlFrame->control == 0x01 )// reboot system
    {
        PowerOffDevices();
        boardStatus->rebootFlag = REBOOT_YES;
    }
    break;
    case MOTOR_MODULE:
    case SENSOR_MODULE:
    case LEDS_MODULE:
    case _5VRESERVE_MODULE:
    case _12V_ROUTER_MODULE:
    case _2_1_PA_MODULE:
    case DYP_MODULE:
    case _12V_RESERVE_MODULE:
    case PRINTER_MODULE:
    case _24V_RESERVE_MODULE:
    case BAT_NV_MODULE:
    case _5V_ALL_MODULE:
    case _12V_ALL_MODULE:
    case _24V_ALL_MODULE:
    case AIUI_MODULE:
    case _5V_ROUTER_MODULE:
      setModulePowerOnOff( recModuleControlFrame->module, recModuleControlFrame->control );
      break;
    case PAD_MODULE:
    case X86_MODULE:
    case NV_MODULE:
    case DLP_MODULE:
      setModulePowerSignalOnOff( recModuleControlFrame->module, recModuleControlFrame->control );
      break;
    default:
      err = ackModuleControlFrameProcess( serial, HW_NO_SUPPORT );
      goto exit;
      break;
  }

  err = ackModuleControlFrameProcess( serial, ACK_SUCCESS );

exit:
  return err;
}

static OSStatus ackModuleControlFrameProcess( serial_t *serial, uint8_t ack )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof( ackGeneralFrame_t );
  ackGeneralFrame_t *ackGeneralFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );
  

  ackGeneralFrame = (ackGeneralFrame_t *)serial->tx_buf.offset;
  require_action( ackGeneralFrame , exit, err = kGeneralErr );

  ackGeneralFrame->ctype = FRAME_TYPE_MODULE_CONTROL;
  ackGeneralFrame->ack = ack;

  err = uart_frame_send( serial, (uint8_t *)ackGeneralFrame, length );

exit:
  return err;
}

static OSStatus recIRLedControlFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  recIRLedControlFrame_t *recIRLedControlFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );

  recIRLedControlFrame = (recIRLedControlFrame_t *)serial->rx_buf.offset;
  require_action( recIRLedControlFrame , exit, err = kGeneralErr );

  if( recIRLedControlFrame->type_wr == 0x00 )//read
  {
  }
  else if( recIRLedControlFrame->type_wr == 0x01 )//write
  {
    brightness_dimming( 50000, recIRLedControlFrame->brightness );
    boardStatus->irled_duty = recIRLedControlFrame->brightness;
  }
  err = ackIRLedControlFrameProcess( serial );
  
exit:
  return err;
}

static OSStatus ackIRLedControlFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof( ackGeneralFrame_t );
  ackGeneralFrame_t *ackGeneralFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial , exit, err = kGeneralErr );
  

  ackGeneralFrame = (ackGeneralFrame_t *)serial->tx_buf.offset;
  require_action( ackGeneralFrame , exit, err = kGeneralErr );

  ackGeneralFrame->ctype = FRAME_TYPE_IRLED_CONTROL;
  ackGeneralFrame->ack = (uint8_t) boardStatus->irled_duty;

  err = uart_frame_send( serial, (uint8_t *)ackGeneralFrame, length );

exit:
  return err;
}

static OSStatus ackNotSupportFrameProcess( serial_t *serial, uint8_t ctype )
{
  OSStatus err = kNoErr;
  uint8_t  length = sizeof( ackGeneralFrame_t );
  ackGeneralFrame_t *ackGeneralFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );
  

  ackGeneralFrame = (ackGeneralFrame_t *)serial->tx_buf.offset;
  require_action( ackGeneralFrame, exit, err = kGeneralErr );

  ackGeneralFrame->ctype = ctype;
  ackGeneralFrame->ack = HW_NO_SUPPORT;

  err = uart_frame_send( serial, (uint8_t *)ackGeneralFrame, length );

exit:
  return err;  
}

static OSStatus ackFwVersionInfoFrameProcess( serial_t *serial );
static OSStatus ackProtocolVersionInfoFrameProcess( serial_t *serial );

static OSStatus recVersionInfoFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t readCommand;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  readCommand = *serial->rx_buf.offset;
  switch( readCommand )
  {
    case READ_FW_VERSION_COMMAND:
    err = ackFwVersionInfoFrameProcess( serial );
    break;
    case READ_PROTOCOL_VERSION:
    err = ackProtocolVersionInfoFrameProcess( serial );
    break;
    default:
    break;
  }
  
exit:
  return err; 
}

static OSStatus ackFwVersionInfoFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t length = sizeof(ackVersionInfoFrame_t);
  uint8_t swVersion[12] = SW_VERSION;
  uint8_t hwVersion[3] = HW_VERSION;
  ackVersionInfoFrame_t   *ackVersionInfoFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  ackVersionInfoFrame = (ackVersionInfoFrame_t *)serial->tx_buf.offset;
  require_action( ackVersionInfoFrame, exit, err = kGeneralErr );

  ackVersionInfoFrame->ctype = FRAME_TYPE_VERSION_INFO;
  ackVersionInfoFrame->ver_type = VERSION_TYPE_FARMWARE;
  memcpy( ackVersionInfoFrame->hw, (void const*)hwVersion, 2 );
  memcpy( ackVersionInfoFrame->sw, (void const*)swVersion, 11 );

  err = uart_frame_send( serial, (uint8_t *)ackVersionInfoFrame, length );

exit:
  return err;  
}

static OSStatus ackProtocolVersionInfoFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t length = sizeof( ackVersionInfoFrame_t );
  int8_t swVersion[13] = PROTOCOL_VERSION;
  ackVersionInfoFrame_t   *ackVersionInfoFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  ackVersionInfoFrame = (ackVersionInfoFrame_t *)serial->tx_buf.offset;
  require_action( ackVersionInfoFrame, exit, err = kGeneralErr );
  
  ackVersionInfoFrame->ctype = FRAME_TYPE_VERSION_INFO;
  ackVersionInfoFrame->ver_type = VERSION_TYPE_PROTOCOL;
  memcpy( &ackVersionInfoFrame->hw, (void const*)swVersion, 13 );

  err = uart_frame_send( serial, (uint8_t *)ackVersionInfoFrame, length );

exit:
  return err;  
}

static OSStatus recFirmwareUpgradeProcess( serial_t *serial );
static OSStatus generalAckUpgradeProcess( serial_t *serial, uint8_t upgradeType, uint8_t ackState );
static OSStatus recPrepareUpgradeFrameProcess( serial_t *serial );
static OSStatus recFirmwarePackageFrameProcess( serial_t *serial );
static OSStatus recUpgradeFinishCheckFrameProcess( serial_t *serial );

static OSStatus recFirmwareUpgradeProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t upgradeType;
  
  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  upgradeType = *(uint8_t *)serial->rx_buf.offset;

  switch( upgradeType )
  {
  case UPGRADE_FRAME_TYPE_PREPARE:
    boardStatus->isUpgrading = 1;
    err = recPrepareUpgradeFrameProcess( serial );
    break;
  case UPGRADE_FRAME_TYPE_GOING:
    err = recFirmwarePackageFrameProcess( serial );
    break;
  case UPGRADE_FRAME_TYPE_FINISH:
    err = recUpgradeFinishCheckFrameProcess( serial );
    boardStatus->isUpgrading = 0;
    break;
  default:
    err = generalAckUpgradeProcess( serial, 0xFF, 0xFF );//no support
    break;
  }

exit:
  return err;  
}

static OSStatus recPrepareUpgradeFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint32_t firmwareSize;
  recPrepareUpgradeFrame_t *recPrepareUpgradeFrame;
  mico_logic_partition_t *ota_partition_info;
  
  ota_partition_info = MicoFlashGetInfo( MICO_PARTITION_OTA_TEMP );
  require_action( ota_partition_info->partition_owner != MICO_FLASH_NONE, exit, err = kUnsupportedErr );
  
  recPrepareUpgradeFrame = (recPrepareUpgradeFrame_t *)serial->rx_buf.offset;
  require_action( recPrepareUpgradeFrame , exit, err = kGeneralErr );

  firmwareSize = ReadBig32(recPrepareUpgradeFrame->firmwareSize);
  if( firmwareSize > ota_partition_info->partition_length )
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_PREPARE, MCU_STORAGE_NOT_ENOUGH );
    protocol_log( "not enough storage" );
    goto exit;
  }
  protocol_log( "firmwareSize is:%d", firmwareSize );
  if( !upgradePrepareFlash( recPrepareUpgradeFrame->md5, firmwareSize ) )
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_PREPARE, MCU_PREPARE_OK );
    protocol_log( "mcu prepare ok" );
  }
  else
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_PREPARE, MCU_RETRY_LATER );
    protocol_log( "mcu retry later" );
  }

exit:
  return err;  
}

static OSStatus generalAckUpgradeProcess( serial_t *serial, uint8_t upgradeType, uint8_t ackState )
{
  OSStatus err = kNoErr;
  uint8_t length = sizeof( upgradeGeneralAckFrame_t );
  upgradeGeneralAckFrame_t *upgradeGeneralAckFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  upgradeGeneralAckFrame = (upgradeGeneralAckFrame_t *)serial->tx_buf.offset;
  require_action( upgradeGeneralAckFrame, exit, err = kGeneralErr );

  upgradeGeneralAckFrame->ctype = FRAME_TYPE_FW_UPGRADE;
  upgradeGeneralAckFrame->upgradeFrameType = upgradeType;
  upgradeGeneralAckFrame->ackState = ackState;

  err = uart_frame_send( serial, (uint8_t *)upgradeGeneralAckFrame, length );

exit:
  return err;  
}

static OSStatus recFirmwarePackageFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  recFirmwarePackageFrame_t  recFirmwarePackageFrame;
  uint32_t packageDataLength;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  packageDataLength = *(uint8_t *)(serial->rx_buf.offset - 2) - 6;
  require_action( packageDataLength, exit, err = kGeneralErr );

  recFirmwarePackageFrame.upgradeFrameType = *(uint8_t *)(serial->rx_buf.offset);
  recFirmwarePackageFrame.pPackageData = (uint8_t *)(serial->rx_buf.offset + 1);

  if( !upgradeWriteFlashData( (uint32_t *)recFirmwarePackageFrame.pPackageData, packageDataLength ) )
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_GOING, PACKGE_PROCESS_OK );
  }
  else
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_GOING, PACKGE_PROCESS_FAIL );
  }
exit:
  return err;
}

static OSStatus recUpgradeFinishCheckFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  recUpgradeFinishCheckFrame_t *recUpgradeFinishCheckFrame;
  
  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  recUpgradeFinishCheckFrame = (recUpgradeFinishCheckFrame_t *)serial->rx_buf.offset;
  require_action( recUpgradeFinishCheckFrame, exit, err = kGeneralErr );

  if( recUpgradeFinishCheckFrame->finishFlag == 0x00 )
  {
    if( !upgradeCheckFlash() )
    {
      err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_FINISH, PACKGE_PROCESS_OK );
      protocol_log("MD5 success,sent right ack");
    }
    else
    {
      err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_FINISH, FIRMWARE_CHECKSUM_ERR );
      protocol_log("MD5 err,sent err ack");
    }
  }
  else
  {
    err = generalAckUpgradeProcess( serial, UPGRADE_FRAME_TYPE_FINISH, 0xFF );
  }

exit:
  return err;
}

static OSStatus recTestCurrentCmdFrame( serial_t *serial )
{
  OSStatus err = kNoErr;
  recTestCurrentCmdFrame_t *recTestCurrentCmdFrame;
  
  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );
  
  recTestCurrentCmdFrame = (recTestCurrentCmdFrame_t *)serial->rx_buf.offset;
  require_action( recTestCurrentCmdFrame, exit, err = kGeneralErr );

  switch( recTestCurrentCmdFrame->sendRate )
  {
  case SEND_RATE_SINGLE:
    uploadCurrentInformation( serial, voltageConvert );
    break;
  case SEND_RATE_1HZ:
  case SEND_RATE_2HZ:
  case SEND_RATE_5HZ:
  case SEND_RATE_10HZ:
  case SEND_RATE_50HZ:
  case SEND_RATE_100HZ:
  case SEND_RATE_0_5HZ:
  case SEND_RATE_0_2HZ:
  case SEND_RATE_0_1HZ:
    voltageDebug.uploadRate = recTestCurrentCmdFrame->sendRate;
    break;
  default:
//    ackTestCurrentCmdFrame( 0x01 );
    goto exit;
  }

//  ackTestCurrentCmdFrame( 0x00 );

  voltageDebug.uploadFlagTime = os_get_time();
  switch( recTestCurrentCmdFrame->command )
  {
  case 0x00:
    voltageDebug.isNeedUpload = NO;
    break;
  case 0x01:
    voltageDebug.isNeedUpload = YES;
    break;
  default:
    break;
  }

exit:
  return err;
}

OSStatus uploadCurrentInformation( serial_t *serial, voltageData_t *voltageInfo )
{
  OSStatus err = kNoErr; 
  uploadCurrentInfoFrame_t *uploadCurrentInfoFrame;
  uint8_t length = sizeof(uploadCurrentInfoFrame_t);
  
  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );
  require_action( voltageInfo, exit, err = kGeneralErr );

  
  uploadCurrentInfoFrame = (uploadCurrentInfoFrame_t *)serial->tx_buf.offset;
  require_action( uploadCurrentInfoFrame, exit, err = kGeneralErr );

  uploadCurrentInfoFrame->ctype = FRAME_TYPE_TEST_CURRENT;
  uploadCurrentInfoFrame->cmdType = 0x00;

  memcpy(&uploadCurrentInfoFrame->currentInfo, voltageInfo, sizeof(uploadCurrentInfoFrame_t));  
  uploadCurrentInfoFrame->sendRate = voltageDebug.uploadRate;
  memcpy( uploadCurrentInfoFrame->faultBit, voltageConvertData->faultBitTemp, 4 );

  err = uart_frame_send( serial, (uint8_t *)uploadCurrentInfoFrame, length );

exit:
  return err;
}

uint32_t sendRateToTime(uint8_t sendRate)
{
  if(!IS_SEND_RATE_DATA(sendRate))
  {
    return 0;
  }
  switch( sendRate )
  {
  case SEND_RATE_1HZ:
    return 1000/SYSTICK_PERIOD;
  case SEND_RATE_2HZ:
    return 500/SYSTICK_PERIOD;
  case SEND_RATE_5HZ:
    return 200/SYSTICK_PERIOD;
  case SEND_RATE_10HZ:
    return 100/SYSTICK_PERIOD;
  case SEND_RATE_50HZ:
    return 20/SYSTICK_PERIOD;
  case SEND_RATE_100HZ:
    return 10/SYSTICK_PERIOD;
  case SEND_RATE_0_5HZ:
    return 2000/SYSTICK_PERIOD;
  case SEND_RATE_0_2HZ:
    return 5000/SYSTICK_PERIOD;
  case SEND_RATE_0_1HZ:
    return 10000/SYSTICK_PERIOD;
  default:
    return 0;
  }
}

static OSStatus recReadErrChannelFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t readLatestErrCmd;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  readLatestErrCmd = *(uint8_t *)serial->rx_buf.offset;
  
  if( readLatestErrCmd == 0x00 )
  {
     err = ackReadErrChannelFrameProcess( serial );
  }
  else
  {
    err = ackNotSupportFrameProcess( serial, *(uint8_t *)(serial->rx_buf.offset - 1) );
  }

exit:
  return err;
}

static OSStatus ackReadErrChannelFrameProcess( serial_t *serial )
{
  OSStatus err = kNoErr;
  uint8_t length = sizeof( errInfoFrame_t );
  errInfoFrame_t *errInfoFrame;

  require_action( serial, exit, err = kGeneralErr );
  require_action( serial->uart_serial, exit, err = kGeneralErr );

  errInfoFrame = (errInfoFrame_t *)(uint8_t *)serial->tx_buf.offset;
  require_action( errInfoFrame, exit, err = kGeneralErr );
  errInfoFrame->ctype = FRAME_TYPE_READ_ERR_CURRENT;
  errInfoFrame->errChannel = boardStatus->errChannel;
  errInfoFrame->errData = boardStatus->errValue;
  err = uart_frame_send( serial, (uint8_t *)errInfoFrame, length );

exit:
  return err;
}

static int is_receive_right_frame( void  );
static int is_receive_right_frame( void  )
{ 
  if( serial->rx_info->rxCount > 0 && *serial->rx_buf.pBuffer == FRAME_HEADER )
  {
    if( *(serial->rx_buf.pBuffer + 1) != 0)
    {
      if( *(serial->rx_buf.pBuffer + *(serial->rx_buf.pBuffer + 1) - 1 ) == FRAME_FOOTER )
      {
        return 0;
      }
    }
  }
  return -1;
}

void protocol_period( void )
{
  uint8_t checksum;
  uint8_t detectType;
  
  if( !serial->isSerialInitialized )
  {
    Protocol_Init();
  }
#ifdef COMM_DMA_USE_INT
  serial->rx_info->rxCount = receviedDmaDataLength( serial->uart_serial->uartHandle );
#else
  serial->rx_info->rxCount = receviedDmaDataLength( COMM_UART );
#endif
  
  if( !is_receive_right_frame() )//serial->rx_info->rxCount > 0 && serial->rx_buf.buffer)//&& rx_buf.rxBuffer[rxBuf[1]-1] == FRAME_FOOTER )//( rx_buf.rxCount > 0 && rx_buf.rxBuffer[rx_buf.rxCount-1] == 0xA5)
  {        
#ifdef PROTOCOL_DEBUG
    if( OPEN_SHOW_LOG == serial->rx_info->showLogFlag )
    {
      char *debugString;
      debugString = DataToHexStringWithSpaces(\
        (const uint8_t *)serial->rx_info->rxBuffer, serial->rx_info->rxCount );
      protocol_log( "rxBuf: %s", debugString );
      free( debugString );
    }
#endif
    serial->rx_info->pData = 0;
    if( serial->rx_info->startTime == 0 )
    {
      if( STATE_POWER_ON == (boardStatus->sysStatus & STATE_RUN_BITS) )
      {
        setCurLedsMode( LIGHTS_MODE_IDLE );
      }
      protocol_log( "start communicating" );
    }
    serial->rx_info->startTime = os_get_time();
  }
  else
  {
    if( ( serial->rx_info->startTime != 0 ) && \
      ((os_get_time() - serial->rx_info->startTime) >= \
        COMMUNICATION_TIMEOUT_TIME_MS/SYSTICK_PERIOD) )
    {
      serial->rx_info->startTime = 0;
      protocol_log( "communicate timeout" );
      if( STATE_POWER_ON == (boardStatus->sysStatus & STATE_RUN_BITS) )
      {
        setCurLedsMode( LIGHTS_MODE_COM_FAULT );
      }      
#ifdef COMM_DMA_USE_INT
      stopDmaRecive( serial->uart_serial->uartHandle );
#else
      stopDmaRecive( COMM_UART );
#endif
      serial->isStartDmaReceive = 0;
//      serial->rx_info->startTime = os_get_time();
      Protocol_Init();
    }
    goto exit;
  }
#ifdef COMM_DMA_USE_INT
  stopDmaRecive( serial->uart_serial->uartHandle );
#else
  stopDmaRecive( COMM_UART );
#endif
  serial->isStartDmaReceive = 0;
  serial->rx_buf.receiveStartTime = 0;
  
  checksum = 0;
  
  serial->rx_buf.pData = (uint8_t *)serial->rx_buf.pBuffer;
  
  checksum += *serial->rx_buf.pData;
  
  serial->rx_info->bufferHeader.detectLength = *++serial->rx_buf.pData; 
  if( *(serial->rx_buf.pData + serial->rx_info->bufferHeader.detectLength - 2) != FRAME_FOOTER )
  {
    protocol_log("not known cmd");
    goto exit;
  }
  checksum += *serial->rx_buf.pData;
  
  serial->rx_info->bufferHeader.detectType = *++serial->rx_buf.pData;
  checksum += *serial->rx_buf.pData;
    
  for( uint8_t i = 0; i < (serial->rx_info->bufferHeader.detectLength - 5); i++ )
  {
    checksum += *++serial->rx_buf.pData;
  }
  require_action_quiet( checksum == *++serial->rx_buf.pData, exit, \
    protocol_log("check sum not match") );

  detectType = serial->rx_info->bufferHeader.detectType;
  
  switch( detectType )
  {
  case FRAME_TYPE_LEDS_CONTROL:
        boardStatus->isUpgrading = 0;
        recSerialLedsFrameProcess( serial );
    break;
  case FRAME_TYPE_S_SYS_V_BAT:
        recReadSysStatusVbatFrameProcess( serial );
    break;
  case FRAME_TYPE_MODULE_STATE:
        recReadModuleStatusFrameProcess( serial );
    break;
  case FRAME_TYPE_FAULT_BIT:
        recReadFaultStatusFrameProcess( serial );
    break;
  case FRAME_TYPE_MODULE_CONTROL:
        recModuleControlFrameProcess( serial );
    break;
  case FRAME_TYPE_VERSION_INFO:
        recVersionInfoFrameProcess( serial );
    break;
  case FRAME_TYPE_FW_UPGRADE:
        recFirmwareUpgradeProcess( serial );
    break;  
  case FRAME_TYPE_TEST_CURRENT:
        recTestCurrentCmdFrame( serial );
    break;
  case FRAME_TYPE_READ_ERR_CURRENT:
        recReadErrChannelFrameProcess( serial );
    break;
  case FRAME_TYPE_IRLED_CONTROL:
        recIRLedControlFrameProcess( serial );
    break;
  default:
        ackNotSupportFrameProcess( serial, detectType );
    break;
  }
#if 1
  if( serial->rx_info->rxCount )
  {
    memset( (uint8_t *)serial->rx_buf.pBuffer, 0x00, serial->rx_buf.bufferSize );  
  }
#ifdef COMM_DMA_USE_INT
  startDmaRecive( serial->uart_serial->uartHandle, (uint8_t *)serial->rx_buf.pBuffer );
#else
  startDmaRecive( COMM_UART, (uint8_t *)serial->rx_buf.pBuffer );
#endif 
  serial->isStartDmaReceive = 1;
#endif
exit:
  if( serial->rx_info->rxCount == 0xFF )
  {
    memset( (uint8_t *)serial->rx_buf.pBuffer, 0x00, serial->rx_buf.bufferSize );
#ifdef COMM_DMA_USE_INT
    startDmaRecive( serial->uart_serial->uartHandle, (uint8_t *)serial->rx_buf.pBuffer );
    stopDmaRecive( serial->uart_serial->uartHandle );
#else
    startDmaRecive( COMM_UART, (uint8_t *)serial->rx_buf.pBuffer );
    stopDmaRecive( COMM_UART );
#endif
    serial->isStartDmaReceive = 0;     
  }
  if( !serial->isStartDmaReceive )
  {
    serial->rx_info->rxCount = 0;
    memset( (uint8_t *)serial->rx_buf.pBuffer, 0x00, serial->rx_buf.bufferSize );   
#ifdef COMM_DMA_USE_INT
    startDmaRecive( serial->uart_serial->uartHandle, (uint8_t *)serial->rx_buf.pBuffer );
#else
    startDmaRecive( COMM_UART, (uint8_t *)serial->rx_buf.pBuffer );
#endif
    serial->isStartDmaReceive = 1;
  }
  return;
}