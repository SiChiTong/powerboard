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

#define VBAT_POWER_ON_LEVEL          24000//x1mV
#define VBAT_LOW_POWER_LEVEL         25000//x1mV
#define VBAT_POWER_OFF_LEVEL         24000//x1mV
#define VBAT_FULL_POWER_LEVEL        28000//x1mV

#define   I_THRESHOLD_H_NV_MA                     0xffff
#define   I_THRESHOLD_L_NV_MA                     0
#define   I_THRESHOLD_H_12V_RES_MA                0xffff
#define   I_THRESHOLD_L_12V_RES_MA                0
#define   I_THRESHOLD_H_LEDS_MA                   0xffff
#define   I_THRESHOLD_L_LEDS_MA                   0
#define   I_THRESHOLD_H_5V_RES_MA                 0xffff
#define   I_THRESHOLD_L_5V_RES_MA                 0
#define   I_THRESHOLD_H_24V_MA                    0xffff
#define   I_THRESHOLD_L_24V_MA                    0
#define   I_THRESHOLD_H_12V_MA                    0xffff
#define   I_THRESHOLD_L_12V_MA                    0
#define   I_THRESHOLD_H_5V_MA                     0xffff
#define   I_THRESHOLD_L_5V_MA                     0
#define   I_THRESHOLD_H_VBUS_MA                   0xffff
#define   I_THRESHOLD_L_VBUS_MA                   0

#define   V_THRESHOLD_H_12V_MV                    12500
#define   V_THRESHOLD_L_12V_MV                    11500
#define   V_THRESHOLD_H_5V_MV                     5500
#define   V_THRESHOLD_L_5V_MV                     4500

#define   I_THRESHOLD_H_ICCARD_MA                 0xffff
#define   I_THRESHOLD_L_ICCARD_MA                 0
#define   I_THRESHOLD_H_24V_RES_MA                0xffff
#define   I_THRESHOLD_L_24V_RES_MA                0
#define   I_THRESHOLD_H_PRINTER_MA                0xffff
#define   I_THRESHOLD_L_PRINTER_MA                0
#define   I_THRESHOLD_H_PC_MA                     0xffff
#define   I_THRESHOLD_L_PC_MA                     0
#define   V_THRESHOLD_H_BAT_MV                    30000
#define   V_THRESHOLD_L_BAT_MV                    23000


struct convert_adc_data {
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
  const uint8_t         fault_bit_mask_num;
};

typedef struct _VoltageData_t {
  uint16_t              nv_currents;
  uint16_t              _12v_res_currents;
  uint16_t              leds_currents;
  uint16_t              _5v_res_currents;
  uint16_t              _24v_currents;
  uint16_t              _12v_currents;
  uint16_t              _5v_currents;
  uint16_t              sys_all_currents;
  uint16_t              iccard_currents;
  uint16_t              _24v_res_currents;
  uint16_t              printer_currents;
  uint16_t              pc_currents;
  
  uint16_t              bat_voltage;  
  uint16_t              _24v_voltage;
  uint16_t              _12v_voltage;
  uint16_t              _5v_voltage;

  int16_t               air_temperature;
  int16_t               _5v_regulator_temp;
  int16_t               _12v_regulator_temp;
  int16_t               _24v_regulator_temp;
} voltageData_t;

typedef struct _voltageConvert_t {
 uint8_t                faultBitTemp[4];
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
} voltageConvertData_t;

typedef struct _voltageDebug_t {
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
extern voltageConvertData_t *voltageConvertData;
extern voltageDebug_t voltageDebug;

typedef struct _soc_contex {
    uint32_t            consume_soc_mws;
    uint32_t            charge_soc_mws;
    uint16_t            bat_v_mv;
    uint16_t            sys_i_ma;
    uint16_t            charge_i_ma;
    uint16_t            bat_ocv_mv;
    uint8_t             volatge_percentage;
    uint8_t             current_percentage;
    uint32_t            period_time_ms;
    uint32_t            last_integrate_time;
    uint32_t            state_of_charge_mws;
    float               bat_inner_res_ohm;
    uint8_t             isInitialized;
} soc_contex_t;
extern soc_contex_t soc_contex;
#define SOC_TOTAL_MWS          (2582723431U)

OSStatus VolDetect_Init( void );
void VolDetect_Tick( void );
void battery_percentage_1s_period( void );
void init_soc( uint32_t soc_value );
void deinit_soc( void );
#endif

