/* 
*  Author: Adam Huang
*  Date:2016/6/8
*/
#ifndef __VOLTAGE_DETECT_H
#define __VOLTAGE_DETECT_H

#include <stdint.h>
#include "Common.h"
#include "Debug.h"
#include "Mico.h"
#define  VOLTAGE_DEBUG

#define VBAT_POWER_ON_LEVEL          2400//x10mV
#define VBAT_LOW_POWER_LEVEL         2500//x10mV
#define VBAT_POWER_OFF_LEVEL         2400//x10mV
#define VBAT_FULL_POWER_LEVEL        2800//x10mV



#define                         SEND_RATE_SINGLE        ((uint8_t)0x00)
#define                         SEND_RATE_1HZ           ((uint8_t)0x01)
#define                         SEND_RATE_2HZ           ((uint8_t)0x02)
#define                         SEND_RATE_5HZ           ((uint8_t)0x03)
#define                         SEND_RATE_10HZ          ((uint8_t)0x04)
#define                         SEND_RATE_50HZ          ((uint8_t)0x05)
#define                         SEND_RATE_100HZ         ((uint8_t)0x06)
#define                         SEND_RATE_0_5HZ         ((uint8_t)0x07)
#define                         SEND_RATE_0_2HZ         ((uint8_t)0x08)
#define                         SEND_RATE_0_1HZ         ((uint8_t)0x09)


#define         IS_SEND_RATE_DATA(x) (((x) == SEND_RATE_SINGLE) ||\
                                      ((x) == SEND_RATE_1HZ) ||\
                                      ((x) == SEND_RATE_2HZ) ||\
                                      ((x) == SEND_RATE_5HZ) || \
                                      ((x) == SEND_RATE_10HZ) ||\
                                      ((x) == SEND_RATE_50HZ) ||\
                                      ((x) == SEND_RATE_100HZ) ||\
                                      ((x) == SEND_RATE_0_5HZ) ||\
                                      ((x) == SEND_RATE_0_2HZ) ||\
                                      ((x) == SEND_RATE_0_1HZ) )


#if 0
#define   CURRENT_THRESHOLD_H_PRINTER_MA          2500
#define   CURRENT_THRESHOLD_L_PRINTER_MA          0
#define   CURRENT_THRESHOLD_H_DLP_MA              2500
#define   CURRENT_THRESHOLD_L_DLP_MA              0
#define   CURRENT_THRESHOLD_H_2_1_PA_MA           3000
#define   CURRENT_THRESHOLD_L_2_1_PA_MA           0
#define   CURRENT_THRESHOLD_H_PAD_MA              2500
#define   CURRENT_THRESHOLD_L_PAD_MA              0
#define   CURRENT_THRESHOLD_H_5V_RESERVE1_MA      1500
#define   CURRENT_THRESHOLD_L_5V_RESERVE1_MA      0
#define   CURRENT_THRESHOLD_H_X86_MA              3000
#define   CURRENT_THRESHOLD_L_X86_MA              0
#define   CURRENT_THRESHOLD_H_DH_5V_MA            200
#define   CURRENT_THRESHOLD_L_DH_5V_MA            0
#define   CURRENT_THRESHOLD_H_SENSOR_MA           200
#define   CURRENT_THRESHOLD_L_SENSOR_MA           0
#define   CURRENT_THRESHOLD_H_5V_LED_MA           3500
#define   CURRENT_THRESHOLD_L_5V_LED_MA           0
#define   CURRENT_THRESHOLD_H_MOTOR_5V_MA         100
#define   CURRENT_THRESHOLD_L_MOTOR_5V_MA         0
#define   CURRENT_THRESHOLD_H_MOTOR_MA            2000
#define   CURRENT_THRESHOLD_L_MOTOR_MA            0
#define   CURRENT_THRESHOLD_H_DH_12V_MA           2500
#define   CURRENT_THRESHOLD_L_DH_12V_MA           0
#define   CURRENT_THRESHOLD_H_VBAT_MA             30000
#define   CURRENT_THRESHOLD_L_VBAT_MA             0
#define   CURRENT_THRESHOLD_H_12V_RESEVE2_MA      10000
#define   CURRENT_THRESHOLD_L_12V_RESEVE2_MA      0
#define   CURRENT_THRESHOLD_H_24V_RESEVE_MA       10000
#define   CURRENT_THRESHOLD_L_24V_RESEVE_MA       0
#endif

#define CURRENT_THRESHOLD_MAX_24V_HD_CAMERA_MA          3000
#define CURRENT_THRESHOLD_MAX_24V_HEAD_MOTOR_MA         3000
#define CURRENT_THRESHOLD_MAX_24V_CAMERA_MA             3000
#define CURRENT_THRESHOLD_MAX_5V_KEYBOARD_MA            3000
#define CURRENT_THRESHOLD_MAX_5V_CODE_MA                3000
#define CURRENT_THRESHOLD_MAX_5V_LEFT_HAND_MOTOR_MA     3000
#define CURRENT_THRESHOLD_MAX_5V_RIGHT_HAND_MOTOR_MA    3000
#define CURRENT_THRESHOLD_MAX_5V_CARD_READER_MA         3000
#define CURRENT_THRESHOLD_MAX_5V_HEAD_MA                3000
#define CURRENT_THRESHOLD_MAX_24V_RITHT_HAND_MOTOR      3000
#define CURRENT_THRESHOLD_MAX_12V_ROUTER_MA             3000
#define CURRENT_THRESHOLD_MAX_12V_HEAD_RK_MA           3000
#define CURRENT_THRESHOLD_MAX_12V_CHEST_RK_MA           3000
#define CURRENT_THRESHOLD_MAX_24V_LEFT_HAND_MOTOR       3000
#define CURRENT_THRESHOLD_MAX_12V_AUDIO_PA_MA           3000
#define CURRENT_THRESHOLD_MAX_5V_REPAIR_MA              3000
#define CURRENT_THRESHOLD_MAX_5V_TOUCH_MA               3000
#define CURRENT_THRESHOLD_MAX_12V_SWITCH_MA             3000
#define CURRENT_THRESHOLD_MAX_BAT_MA                    15000
#define CURRENT_THRESHOLD_MAX_12V_CARD_READER_MA        3000
#define CURRENT_THRESHOLD_MAX_12V_SWITCH_MA             3000
#define TEMP_THRESHOLD_MAX_12V_MA                       300
#define TEMP_THRESHOLD_MAX_5V_MA                        300
#define TEMP_THRESHOLD_MAX_AIR_MA                       300
#define CURRENT_THRESHOLD_MAX_5V_HEAD_TOUCH_MA          3000
#define CURRENT_THRESHOLD_MAX_12V_MA                    9000
#define CURRENT_THRESHOLD_MAX_5V_MA                     9000
#define CURRENT_THRESHOLD_MAX_5V_HEAD_LED_MA            3000
#define VOLTAGE_THRESHOLD_MAX_5V_MV                     7000
#define VOLTAGE_THRESHOLD_MAX_12V_MV                    14000
#define VOLTAGE_THRESHOLD_MAX_BAT_MV                    25000
#define CURRENT_THRESHOLD_MAX_5V_RESERVE_MA             3000
#define CURRENT_THRESHOLD_MAX_5V_LED_MA                 3000
#define CURRENT_THRESHOLD_MAX_5V_HD_CAMERA_MA           3000
#define CURRENT_THRESHOLD_MAX_5V_CAMERA_MA              3000

#define CURRENT_THRESHOLD_MIN_24V_HD_CAMERA_MA          0
#define CURRENT_THRESHOLD_MIN_24V_HEAD_MOTOR_MA         0
#define CURRENT_THRESHOLD_MIN_24V_CAMERA_MA             0
#define CURRENT_THRESHOLD_MIN_5V_KEYBOARD_MA            0
#define CURRENT_THRESHOLD_MIN_5V_CODE_MA                0
#define CURRENT_THRESHOLD_MIN_5V_LEFT_HAND_MOTOR_MA     0
#define CURRENT_THRESHOLD_MIN_5V_RIGHT_HAND_MOTOR_MA    0
#define CURRENT_THRESHOLD_MIN_5V_CARD_READER_MA         0
#define CURRENT_THRESHOLD_MIN_5V_HEAD_MA                0
#define CURRENT_THRESHOLD_MIN_24V_RITHT_HAND_MOTOR      0
#define CURRENT_THRESHOLD_MIN_12V_ROUTER_MA             0
#define CURRENT_THRESHOLD_MIN_12V_HEAD_RK_MA            0
#define CURRENT_THRESHOLD_MIN_12V_CHEST_RK_MA           0
#define CURRENT_THRESHOLD_MIN_24V_LEFT_HAND_MOTOR       0
#define CURRENT_THRESHOLD_MIN_12V_AUDIO_PA_MA           0
#define CURRENT_THRESHOLD_MIN_5V_REPAIR_MA              0
#define CURRENT_THRESHOLD_MIN_5V_TOUCH_MA               0
#define CURRENT_THRESHOLD_MIN_12V_SWITCH_MA             0
#define CURRENT_THRESHOLD_MIN_BAT_MA                    0
#define CURRENT_THRESHOLD_MIN_12V_CARD_READER_MA        0
#define CURRENT_THRESHOLD_MIN_12V_SWITCH_MA             0
#define TEMP_THRESHOLD_MIN_12V_MA                       0
#define TEMP_THRESHOLD_MIN_5V_MA                        0
#define TEMP_THRESHOLD_MIN_AIR_MA                       0
#define CURRENT_THRESHOLD_MIN_5V_HEAD_TOUCH_MA          0
#define CURRENT_THRESHOLD_MIN_12V_MA                    0
#define CURRENT_THRESHOLD_MIN_5V_MA                     0
#define CURRENT_THRESHOLD_MIN_5V_HEAD_LED_MA            0
#define VOLTAGE_THRESHOLD_MIN_5V_MV                     0
#define VOLTAGE_THRESHOLD_MIN_12V_MV                    0
#define VOLTAGE_THRESHOLD_MIN_BAT_MV                    0
#define CURRENT_THRESHOLD_MIN_5V_RESERVE_MA             0
#define CURRENT_THRESHOLD_MIN_5V_LED_MA                 0
#define CURRENT_THRESHOLD_MIN_5V_HD_CAMERA_MA           0
#define CURRENT_THRESHOLD_MIN_5V_CAMERA_MA              0
typedef enum
{
    FAULT_TEMP_AIR_BIT_NUM                           = 0,
    FAULT_TEMP_12V_BIT_NUM,
    FAULT_TEMP_5V_BIT_NUM,
    FAULT_VOLTAGE_BAT_BIT_NUM,
    FAULT_VOLTAGE_12V_BIT_NUM,
    FAULT_VOLTAGE_5V_BIT_NUM,
    FAULT_CURRENTS_VBUS_BIT_NUM,
    FAULT_CURRENTS_12V_BIT_NUM,

    FAULT_CURRENTS_5V_BIT_NUM                        = 8,
    FAULT_CURRENTS_24V_LEFT_HAND_MOTOR_BIT_NUM,
    FAULT_CURRENTS_24V_RIGHT_HAND_MOTOR_BIT_NUM,
    FAULT_CURRENTS_24V_HEAD_MOTOR_BIT_NUM,
    FAULT_CURRENTS_24V_HD_CAMERA_BIT_NUM,
    FAULT_CURRENTS_24V_CAMERA_BIT_NUM,
    FAULT_CURRENTS_12V_SWITCH_BIT_NUM,
    FAULT_CURRENTS_12V_AUDIO_PA_BIT_NUM,

    FAULT_CURRENTS_12V_HEAD_RK_BIT_NUM               = 16,
    FAULT_CURRENTS_12V_CHEST_RK_BIT_NUM,
    FAULT_CURRENTS_12V_ROUTER_BIT_NUM,
    FAULT_CURRENTS_12V_CARD_READER_BIT_NUM,
    FAULT_CURRENTS_5V_HEAD_BIT_NUM,
    FAULT_CURRENTS_5V_CARD_READER_BIT_NUM,
    FAULT_CURRENTS_5V_CODE_BIT_NUM,
    FAULT_CURRENTS_5V_KEYBOARD_BIT_NUM,
    
    FAULT_CURRENTS_5V_LEFT_HAND_MOTOR_BIT_NUM        = 24,
    FAULT_CURRENTS_5V_RIGHT_HAND_MOTOR_BIT_NUM,
    FAULT_CURRENTS_5V_REPAIR_BIT_NUM,
    FAULT_CURRENTS_5V_HEAD_TOUCH_BIT_NUM,
    FAULT_CURRENTS_5V_HEAD_LED_BIT_NUM,
    FAULT_CURRENTS_5V_HD_LED_BIT_NUM,
    FAULT_CURRENTS_5V_CAMERA_BIT_NUM,
    FAULT_CURRENTS_5V_HD_CAMERA_BIT_NUM,

    FAULT_CURRENTS_5V_RESERVE_BIT_NUM                = 32,
    FAULT_CURRENTS_5V_TOUCH_BIT_NUM,
}fault_bit_t;

struct convert_adc_data 
{
  const mico_adc_t      adc_type;
  const uint8_t         convert_type;
#define                         CONVERT_TYPE_VOLTAGE            0x01
#define                         CONVERT_TYPE_CURRENTS           0x02
#define                         CONVERT_TYPE_TEMP               0x03
  const uint8_t         isNeedDelay;  
  const float           convert_factor;  
  const uint16_t        threshold_low;
  const uint16_t        threshold_high;
  uint16_t              err_start_time;
  const uint16_t        err_duration_time;
  uint16_t              err_value;
  const uint8_t         err_channel;
  const fault_bit_t         fault_bit_mask_num;
};

typedef struct _VoltageData_t 
{
  uint16_t              c_5V_right_hand_motor;
  uint16_t              c_24V_hd_camera;
  uint16_t              c_24V_head_motor;
  uint16_t              c_24V_camera;
  uint16_t              c_5V_keyboard;
  uint16_t              c_5V_code;
  uint16_t              c_5V_left_hand_motor;
  uint16_t              c_5V_card_reader;
  uint16_t              c_5V_head;
  uint16_t              c_24V_right_hand_motor;
  uint16_t              c_12V_head_rk;
  uint16_t              c_12V_chest_rk;
  uint16_t              c_24V_left_hand_motor;
  uint16_t              c_12V_audio_pa;
  uint16_t              c_5V_repair;
  uint16_t              c_5V_touch;

  //uint16_t              c_switch;
  uint16_t              c_12V_router;
  uint16_t              c_vbus;
  uint16_t              c_12V_card_reader;
  uint16_t              c_12V_switch;
  
  uint16_t              temp_12V_ts;
  uint16_t              temp_5V_ts;
  uint16_t              temp_air_ts;
  uint16_t              c_5V_head_touch;
  uint16_t              c_12V_all;
  uint16_t              c_5V_all;
  uint16_t              c_5V_head_led;  //Í·²¿µÆ°å
  uint16_t              v_12V;
  int16_t               v_5V;
  int16_t               v_bat;
  int16_t               c_5V_reserve;
  int16_t               c_5V_led;   //¸ßÅÄ LED
  int16_t               c_5V_camera;
  int16_t               c_5V_hd_camera;
} voltageData_t;

typedef struct 
{
#define CURRENT_NUM_TOTAL   34
 uint8_t                faultBitTemp[CURRENT_NUM_TOTAL/8 + 1];
 

#if 0
#define     FAULT_TEMP_AIR_BIT_NUM          0
#define     FAULT_TEMP_12V_BIT_NUM          1
#define     FAULT_TEMP_5V_BIT_NUM           2
#define     FAULT_VOLTAGE_BAT_BIT_NUM       3
#define     FAULT_VOLTAGE_12V_BIT_NUM       4 
#define     FAULT_VOLTAGE_5V_BIT_NUM        5
#define     FAULT_CURRENTS_VBUS_BIT_NUM     6
#define     FAULT_CURRENTS_12V_BIT_NUM      7

#define     FAULT_CURRENTS_5V_BIT_NUM       8
#define     FAULT_CURRENTS_24V_LEFT_HAND_MOTOR_BIT_NUM      9
#define     FAULT_CURRENTS_24V_RIGHT_HAND_MOTOR_BIT_NUM     10
#define     FAULT_CURRENTS_24V_HEAD_MOTOR_BIT_NUM           11
#define     FAULT_CURRENTS_24V_HD_CAMERA_BIT_NUM            12
#define     FAULT_CURRENTS_24V_CAMERA_BIT_NUM               13
#define     FAULT_CURRENTS_12V_SWITCH_BIT_NUM               14
#define     FAULT_CURRENTS_12V_AUDIO_PA_BIT_NUM             15
 
#define     FAULT_CURRENTS_12V_HEAD_RK_BIT_NUM              16
#define     FAULT_CURRENTS_12V_CHEST_RK_BIT_NUM             17
#define     FAULT_CURRENTS_12V_ROUTER_BIT_NUM               18
#define     FAULT_CURRENTS_12V_CARD_READER_BIT_NUM          19
#define     FAULT_CURRENTS_5V_HEAD_BIT_NUM                  20
#define     FAULT_CURRENTS_5V_CARD_READER_BIT_NUM           21
#define     FAULT_CURRENTS_5V_CODE_BIT_NUM                  22
#define     FAULT_CURRENTS_5V_KEYBOARD_BIT_NUM              23
#define     FAULT_CURRENTS_5V_LEFT_HAND_MOTOR_BIT_NUM       24
#define     FAULT_CURRENTS_5V_RIGHT_HAND_MOTOR_BIT_NUM      25
#define     FAULT_CURRENTS_5V_REPAIR_BIT_NUM                26
#define     FAULT_CURRENTS_5V_HEAD_TOUCH_BIT_NUM            27
#define     FAULT_CURRENTS_5V_HEAD_LED_BIT_NUM              28
#define     FAULT_CURRENTS_5V_HD_LED_BIT_NUM                29
#define     FAULT_CURRENTS_5V_CAMERA_BIT_NUM                30
#define     FAULT_CURRENTS_5V_HD_CAMERA_BIT_NUM             31
 
#define     FAULT_CURRENTS_5V_RESERVE_BIT_NUM               32
#define     FAULT_CURRENTS_5V_TOUCH_BIT_NUM                 33
#endif 
 
 
 
#if 0 
#define         AMBIENT_TEMP_FAULT_BIT_BIT_MASK_NUM     (0)
#define         AMBIENT_TEMP_FAULT_BIT                  ((uint32_t)0x00000001)
#define         SYS_CURRENTS_FAULT_BIT_MASK_NUM         (1)
#define         SYS_CURRENTS_FAULT_BIT                  ((uint32_t)0x00000002)
#define         BAT_MOTOR_CURRENTS_FAULT_BIT_MASK_NUM   (2)
#define         BAT_MOTOR_CURRENTS_FAULT_BIT            ((uint32_t)0x00000004)
#define         MOTOR_CURRENTS_FAULT_BIT_MASK_NUM       (3)
#define         MOTOR_CURRENTS_FAULT_BIT                ((uint32_t)0x00000008)
#define         DH_5V_CURRENTS_FAULT_BIT_MASK_NUM       (4)
#define         DH_5V_CURRENTS_FAULT_BIT                ((uint32_t)0x00000010)
#define         SENSOR_CURRENTS_FAULT_BIT_MASK_NUM      (5)
#define         SENSOR_CURRENTS_FAULT_BIT               ((uint32_t)0x00000020)
#define         DLP_CURRENTS_FAULT_BIT_MASK_NUM         (6)
#define         DLP_CURRENTS_FAULT_BIT                  ((uint32_t)0x00000040)
#define         LEDS_CURRENTS_FAULT_BIT_MASK_NUM        (7)
#define         LEDS_CURRENTS_FAULT_BIT                 ((uint32_t)0x00000080)
#define         PAD_CURRENTS_FAULT_BIT_MASK_NUM         (8)
#define         PAD_CURRENTS_FAULT_BIT                  ((uint32_t)0x00000100)
#define         X86_CURRENTS_FAULT_BIT_MASK_NUM         (9)
#define         X86_CURRENTS_FAULT_BIT                  ((uint32_t)0x00000200)
#define         PA_2_1_CURRENTS_FAULT_BIT_MASK_NUM      (10)
#define         PA_2_1_CURRENTS_FAULT_BIT               ((uint32_t)0x00000400)
#define         DH_12V_CURRENTS_FAULT_BIT_MASK_NUM      (11)
#define         DH_12V_CURRENTS_FAULT_BIT               ((uint32_t)0x00000800)
#define         PRINTER_CURRENTS_FAULT_BIT_MASK_NUM     (12)
#define         PRINTER_CURRENTS_FAULT_BIT              ((uint32_t)0x00001000)
#define         RES_5V_CURRENTS_FAULT_BIT_MASK_NUM      (13)
#define         RES_5V_CURRENTS_FAULT_BIT               ((uint32_t)0x00002000)
#define         RES_12V_CURRENTS_FAULT_BIT_MASK_NUM     (14)
#define         RES_12V_CURRENTS_FAULT_BIT              ((uint32_t)0x00004000)
#define         RES_24V_CURRENTS_FAULT_BIT_MASK_NUM     (15)
#define         RES_24V_CURRENTS_FAULT_BIT              ((uint32_t)0x00008000)
#define         BAT_NV_CURRENTS_FAULT_BIT_MASK_NUM      (16)
#define         BAT_NV_CURRENTS_FAULT_BIT               ((uint32_t)0x00010000)
#define         _12V_NV_CURRENTS_FAULT_BIT_MASK_NUM     (17)
#define         _12V_NV_CURRENTS_FAULT_BIT              ((uint32_t)0x00020000)
#define         ROUTER_CURRENTS_FAULT_BIT_MASK_NUM      (18)
#define         ROUTER_CURRENTS_FAULT_BIT               ((uint32_t)0x00040000)
#define         DYP_CURRENTS_FAULT_BIT_MASK_NUM         (19)
#define         DYP_CURRENTS_FAULT_BIT                  ((uint32_t)0x00080000)
#define         IRLED_CURRENTS_FAULT_BIT_MASK_NUM       (20)
#define         IRLED_CURRENTS_FAULT_BIT                ((uint32_t)0x00100000)
#define         CHARGE_CURRENTS_FAULT_BIT_MASK_NUM      (21)
#define         CHARGE_CURRENTS_FAULT_BIT               ((uint32_t)0x00200000)
#define         BATIN_CURRENTS_FAULT_BIT_MASK_NUM       (22)
#define         BATIN_CURRENTS_FAULT_BIT                ((uint32_t)0x00400000)
#define         _24V_VOLTAGE_FAULT_BIT_MASK_NUM         (23)
#define         _24V_VOLTAGE_FAULT_BIT                  ((uint32_t)0x00800000)
#define         _12V_VOLTAGE_FAULT_BIT_MASK_NUM         (24)
#define         _12V_VOLTAGE_FAULT_BIT                  ((uint32_t)0x01000000)
#define         _5V_VOLTAGE_FAULT_BIT_MASK_NUM          (25)
#define         _5V_VOLTAGE_FAULT_BIT                   ((uint32_t)0x02000000)
#define         _5V_CURRENTS_FAULT_BIT_MASK_NUM         (26)
#define         _5V_CURRENTS_FAULT_BIT                  ((uint32_t)0x04000000)
#define         _12V_CURRENTS_FAULT_BIT_MASK_NUM        (27)
#define         _12V_CURRENTS_FAULT_BIT                 ((uint32_t)0x08000000)
#define         _24V_CURRENTS_FAULT_BIT_MASK_NUM        (28)
#define         _24V_CURRENTS_FAULT_BIT                 ((uint32_t)0x10000000)
#define         _5V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM   (29)
#define         _5V_REGULATOR_TEMP_FAULT_BIT            ((uint32_t)0x20000000)
#define         _12V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM  (30)
#define         _12V_REGULATOR_TEMP_FAULT_BIT           ((uint32_t)0x40000000)
#define         _24V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM  (31)
#define         _24V_REGULATOR_TEMP_FAULT_BIT           ((uint32_t)0x80000000)
#endif
}convert_fault_t;


typedef struct _voltageDebug_t 
{
  uint8_t               printType;
#define                         PRINT_NO        0x00
#define                         PRINT_ONCE      0x01
#define                         PRINT_PEROID    0x02
#ifdef  VOLTAGE_DEBUG
  uint8_t               printMaxType;
#define                         RESET_MAX_BUF   0x03
#endif
  uint16_t              peroid;
  uint32_t              startTime; 
  uint8_t               uploadRate;
  uint8_t               isNeedUpload;
  uint32_t              uploadFlagTime;
} voltageDebug_t;

extern voltageData_t *voltageConvert;
extern convert_fault_t *voltageConvertData;
extern voltageDebug_t voltageDebug;

OSStatus VolDetect_Init( void );
void VolDetect_Tick( void );
void battery_percentage_1s_period( void );

#endif

