/* 
*  Author: Adam Huang
*  Date:2016/12/13
*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include <stdint.h>
#include "RingBufferUtils.h"
#include "voltage_detect.h"

#define PROTOCOL_DEBUG

#if 1
#define UART_PROTOCOL
//#else
#define CAN_PROTOCOL
#endif



#define HW_VERSION                      "Carson_Sub_PM_V0.0"
#define SW_VERSION                      "CarsonC001M08A001"//"CarsonC001M08Axxx"//"CARSON08001"
#define PROTOCOL_VERSION                "20170505P0001"


#ifdef CAN_PROTOCOL
#pragma pack(1)
typedef struct _SysStatusVbat_t {
  uint8_t               cmd_num;
#define             SYS_VBAT_READ_CMD          0x00
#define             SYS_VCHARGE_READ_CMD       0x01
  uint16_t              sysStatus;
  uint16_t              voltage;
} SysStatusVbat_t;
#pragma pack()
#endif



/***************** begin of upgrade defines ********************/

#define UPGRADE_FRAME_TYPE_PREPARE      0x00
#define UPGRADE_FRAME_TYPE_GOING        0x01
#define UPGRADE_FRAME_TYPE_FINISH       0x02

#define MCU_PREPARE_OK                  0x00
#define MCU_STORAGE_NOT_ENOUGH          0x01
#define MCU_RETRY_LATER                 0x02

#define PACKGE_PROCESS_OK               0x00
#define PACKGE_PROCESS_FAIL             0x01

#define FIRMWARE_CHECKSUM_RIGHT         0x00
#define FIRMWARE_CHECKSUM_ERR           0x01


/***************** end of upgrade defines *********************/


void protocol_period( void );
OSStatus Protocol_Init( void );

uint32_t sendRateToTime(uint8_t sendRate);
//OSStatus Protocol_Init( serial_t *serial );

#endif
