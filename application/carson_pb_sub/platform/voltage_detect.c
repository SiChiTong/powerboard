/* 
*  Author: Adam Huang
*  Date:2016/6/8
*/
#include "voltage_detect.h"
#include <stdlib.h>
#include "stm32f1xx_powerboard.h"
#include "app_platform.h"
#include "protocol.h"
#include "can_protocol.h"
#include "multi_channel_detection.h"
#include "temp_reference.h"

#define vol_detect_log(M, ...) custom_log("VolDetect", M, ##__VA_ARGS__)
#define vol_detect_log_trace() custom_log_trace("VolDetect")

#define BAT_RES                0.1 //unit: ¦¸

static voltageData_t ramVoltageConvert;
voltageData_t *voltageConvert = NULL;//&ramVoltageConvert;

#ifdef VOLTAGE_DEBUG
static voltageData_t ramTempMaxVoltageData;
static voltageData_t *tempMaxVoltageData = NULL;
#endif

convert_fault_t *voltageConvertData;
voltageDebug_t voltageDebug;

static void computeVoltage( void );


#if 1
typedef enum
{
    CURRENTS_5V_RIGHT_HAND,
    CURRENTS_24V_HEAD_CAMERA,
    CURRENTS_24V_HEAD,
    CURRENTS_24V_CAMERA,
    CURRENTS_5V_KEYBOARD,
    CURRENTS_5V_CODE,
    CURRENTS_5V_LEFT_HAND,
    CURRENTS_5V_ID_CARD,
    CURRENTS_5V_HEAD,
    CURRENTS_24V_RIGHT_HAND_MOTOR,
    CURRENTS_HEAD_RK,
    CURRENTS_CHEST_RK,
    CURRENTS_24V_LEFT_HAND_MOTOR,
    CURRENTS_AUDIO_PA,
    CURRENTS_5V_REPAIR,
    CURRENTS_5V_TOUCH,
    
    CURRENTS_12V_SWITCH,
    CURRENTS_12V_ROUTER,
    CURRENTS_VBUS,
    CURRENTS_CARD_READER,
    C_V_SWTICH,
    
    TEMP_12_TS,
    TEMP_5V_TS,
    TEMP_AIR_TS,
    CURRENTS_5V_HEAD_TOUCH,
    CURRENTS_12V_ALL,
    CURRENTS_5V_ALL,
    CURRENTS_5V_HEAD_LED,
    VOLTAGE_12V,
    VOLTAGE_5V,
    VOLTAGE_BAT,
    CURRENTS_5V_RESERVE,
    CURRENTS_5V_LED,
    CURRENTS_5V_CAMERA,
    CURRENTS_5V_HD_CAMERA,
}adc_channel_t;

#endif

#define ADC_FACTOR_1    50
#define ADC_FACTOR_2    5
const struct convert_adc_data convert_data[] = {
  [CURRENTS_5V_RIGHT_HAND] = 
  {
    .adc_type           = ADC_C_5V_RIGHT_HAND_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//6.05,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_RIGHT_HAND_MOTOR_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_RIGHT_HAND_MOTOR_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x01,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_RIGHT_HAND_MOTOR_BIT_NUM,
  },
  [CURRENTS_24V_HEAD_CAMERA] = 
  {
    .adc_type           = ADC_C_24V_HD_CAMERA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//5.95,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_24V_HD_CAMERA_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_24V_HD_CAMERA_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x02,
    .fault_bit_mask_num = FAULT_CURRENTS_24V_HD_CAMERA_BIT_NUM,
  },
  [CURRENTS_24V_HEAD] = 
  {
    .adc_type           = ADC_C_24V_HEAD_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//50,
    .threshold_low      = CURRENT_THRESHOLD_MIN_24V_HEAD_MOTOR_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_24V_HEAD_MOTOR_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x04,
    .fault_bit_mask_num = FAULT_CURRENTS_24V_HEAD_MOTOR_BIT_NUM, 
  },
  //DH12V
  //DH5V
  [CURRENTS_24V_CAMERA] = 
  {
    .adc_type           = ADC_C_24V_CAMERA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//5.55,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_24V_CAMERA_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_24V_CAMERA_MA,//0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x07,
    .fault_bit_mask_num = FAULT_CURRENTS_24V_CAMERA_BIT_NUM, 
  },
  [CURRENTS_5V_KEYBOARD] = 
  {
    .adc_type           = ADC_C_5V_KEYBOARD,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//6.2,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_KEYBOARD_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_KEYBOARD_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x08,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_KEYBOARD_BIT_NUM, 
  },
  [CURRENTS_5V_CODE] = 
  {
    .adc_type           = ADC_C_5V_CODE,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5,//5.8,//
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_CODE_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_CODE_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x09,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_CODE_BIT_NUM, 
  },
  [CURRENTS_5V_LEFT_HAND] = 
  {
    .adc_type           = ADC_C_5V_LEFT_HAND_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//4.985,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_LEFT_HAND_MOTOR_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_LEFT_HAND_MOTOR_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0A,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_LEFT_HAND_MOTOR_BIT_NUM, 
  }, 
  [CURRENTS_5V_ID_CARD] = 
  {
    .adc_type           = ADC_C_5V_CARD_READER,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5.59,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_CARD_READER_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_CARD_READER_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0B,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_CARD_READER_BIT_NUM, 
  },
  [CURRENTS_5V_HEAD] = 
  {
    .adc_type           = ADC_C_5V_HEAD,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//6.3,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_HEAD_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_HEAD_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0C,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_HEAD_BIT_NUM, 
  },
  [CURRENTS_24V_RIGHT_HAND_MOTOR] = 
  {
    .adc_type           = ADC_C_24V_RIGHT_HAND_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//5.56,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_24V_RITHT_HAND_MOTOR,
    .threshold_high     = CURRENT_THRESHOLD_MAX_24V_RITHT_HAND_MOTOR,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0D,
    .fault_bit_mask_num = FAULT_CURRENTS_24V_RIGHT_HAND_MOTOR_BIT_NUM, 
  },
  [CURRENTS_HEAD_RK] = 
  {
    .adc_type           = ADC_C_HEAD_RK,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5.1,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_HEAD_RK_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_HEAD_RK_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0E,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_HEAD_RK_BIT_NUM, 
  },
  [CURRENTS_CHEST_RK] = 
  {
    .adc_type           = ADC_C_12V_CHEST_RK,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//50,//41.17,//
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_CHEST_RK_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_CHEST_RK_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0F,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_CHEST_RK_BIT_NUM, 
  },
  [CURRENTS_24V_LEFT_HAND_MOTOR] = 
  {
    .adc_type           = ADC_C_24V_LEFT_HAND_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//5.45,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_24V_LEFT_HAND_MOTOR,
    .threshold_high     = CURRENT_THRESHOLD_MAX_24V_LEFT_HAND_MOTOR,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x10,
    .fault_bit_mask_num = FAULT_CURRENTS_24V_LEFT_HAND_MOTOR_BIT_NUM, 
  },
  [CURRENTS_AUDIO_PA] = 
  {
    .adc_type           = ADC_C_12V_AUDIO_PA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5,//5.65,//
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_AUDIO_PA_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_AUDIO_PA_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x11,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_AUDIO_PA_BIT_NUM,    
  },
  [CURRENTS_5V_REPAIR] = 
  {
    .adc_type           = ADC_C_5V_REPAIR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//6.2,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_REPAIR_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_REPAIR_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x12,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_REPAIR_BIT_NUM,    
  },
  [CURRENTS_5V_TOUCH] = 
  {
    .adc_type           = ADC_C_5V_TOUCH,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//7.55,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_TOUCH_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_TOUCH_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x13,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_TOUCH_BIT_NUM, 
  },
  [CURRENTS_12V_SWITCH] = 
  {
    .adc_type           = ADC_C_12V_SWITCH,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5,//7.25,//
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_SWITCH_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_SWITCH_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x14,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_SWITCH_BIT_NUM, 
  },  
  [CURRENTS_12V_ROUTER] = 
  {
    .adc_type           = ADC_C_12V_ROUTER,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_ROUTER_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_ROUTER_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x15,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_ROUTER_BIT_NUM, 
  }, 
  [CURRENTS_VBUS] = 
  {
    .adc_type           = ADC_C_VBUS,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_1,//50,
    .threshold_low      = CURRENT_THRESHOLD_MIN_BAT_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_BAT_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x16,
    .fault_bit_mask_num = FAULT_CURRENTS_VBUS_BIT_NUM, 
  },
  [CURRENTS_CARD_READER] = 
  {
    .adc_type           = ADC_C_12V_CARD_READER,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = ADC_FACTOR_2,//50,
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_CARD_READER_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_CARD_READER_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x17,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_CARD_READER_BIT_NUM, 
  },
  
  
  
#if 0  
  //switch adc channel
  [C_V_SWTICH] = 
  {
    .adc_type           = ADC_SWITCH,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x18,
    .fault_bit_mask_num = _24V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
#endif 
  
 
  
  //switch adc
  [TEMP_12_TS] = 
  {
    .adc_type           = ADC_12V_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = TEMP_THRESHOLD_MIN_12V_MA,
    .threshold_high     = TEMP_THRESHOLD_MAX_12V_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x19,
    .fault_bit_mask_num = FAULT_TEMP_12V_BIT_NUM, 
  },
  [TEMP_5V_TS] = 
  {
    .adc_type           = ADC_5V_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = TEMP_THRESHOLD_MIN_5V_MA,
    .threshold_high     = TEMP_THRESHOLD_MAX_5V_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1A,
    .fault_bit_mask_num = FAULT_TEMP_5V_BIT_NUM, 
  },
  [TEMP_AIR_TS] = 
  {
    .adc_type           = ADC_AIR_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = TEMP_THRESHOLD_MIN_AIR_MA,
    .threshold_high     = TEMP_THRESHOLD_MAX_AIR_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1B,
    .fault_bit_mask_num = FAULT_TEMP_AIR_BIT_NUM, 
  },
  [CURRENTS_5V_HEAD_TOUCH] = 
  {
    .adc_type           = ADC_C_5V_HEAD_TOUCH,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_HEAD_TOUCH_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_HEAD_TOUCH_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1C,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_HEAD_TOUCH_BIT_NUM, 
  },
  [CURRENTS_12V_ALL] = 
  {
    .adc_type           = ADC_C_12V_ALL,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_12V_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_12V_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1D,
    .fault_bit_mask_num = FAULT_CURRENTS_12V_BIT_NUM, 
  },
  [CURRENTS_5V_ALL] = 
  {
    .adc_type           = ADC_C_5V_ALL,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//1,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1E,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_BIT_NUM, 
  },
  [CURRENTS_5V_HEAD_LED] = 
  {
    .adc_type           = ADC_C_5V_HEAD_LED,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//1,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_HEAD_LED_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_HEAD_LED_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1F,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_HEAD_LED_BIT_NUM, 
  },
  [VOLTAGE_12V] = 
  {
    .adc_type           = ADC_V_12V,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = VOLTAGE_THRESHOLD_MIN_12V_MV,
    .threshold_high     = VOLTAGE_THRESHOLD_MAX_12V_MV,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x20,
    .fault_bit_mask_num = FAULT_VOLTAGE_12V_BIT_NUM, 
  },
  [VOLTAGE_5V] = 
  {
    .adc_type           = ADC_V_5V,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 2,
    .threshold_low      = VOLTAGE_THRESHOLD_MIN_5V_MV,
    .threshold_high     = VOLTAGE_THRESHOLD_MAX_5V_MV,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x21,
    .fault_bit_mask_num = FAULT_VOLTAGE_5V_BIT_NUM, 
  },  
  [VOLTAGE_BAT] = 
  {
    .adc_type           = ADC_V_BAT,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = VOLTAGE_THRESHOLD_MIN_BAT_MV,
    .threshold_high     = VOLTAGE_THRESHOLD_MAX_BAT_MV,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = FAULT_VOLTAGE_BAT_BIT_NUM,
  },
  [CURRENTS_5V_RESERVE] = 
  {
    .adc_type           = ADC_C_5V_RESERVE,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5.4,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_RESERVE_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_RESERVE_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_RESERVE_BIT_NUM,
  },
  [CURRENTS_5V_LED] = 
  {
    .adc_type           = ADC_C_5V_LED,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5.55,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_LED_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_LED_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_HD_LED_BIT_NUM,
  },
  [CURRENTS_5V_CAMERA] = 
  {
    .adc_type           = ADC_C_5V_CAMERA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5.55,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_CAMERA_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_CAMERA_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_CAMERA_BIT_NUM,
  },
  [CURRENTS_5V_HD_CAMERA] = 
  {
    .adc_type           = ADC_5V_HD_CAMERA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = ADC_FACTOR_2,//5.55,//5,
    .threshold_low      = CURRENT_THRESHOLD_MIN_5V_HD_CAMERA_MA,
    .threshold_high     = CURRENT_THRESHOLD_MAX_5V_HD_CAMERA_MA,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = FAULT_CURRENTS_5V_HD_CAMERA_BIT_NUM,
  },
};

OSStatus VolDetect_Init( void )
{
  OSStatus err = kNoErr;  

  voltageConvert = &ramVoltageConvert;
  memset( voltageConvert, 0x0, sizeof(voltageData_t) );

#ifdef VOLTAGE_DEBUG
  tempMaxVoltageData = &ramTempMaxVoltageData;
  memset( tempMaxVoltageData, 0x0, sizeof(voltageData_t) );
  if( tempMaxVoltageData )
  {
    tempMaxVoltageData->v_bat = 10000;//set temp max voltage is 100v
  }
#endif
  voltageConvertData = (convert_fault_t *)malloc( sizeof(convert_fault_t) );
  require_action( voltageConvertData, exit, err = kNoMemoryErr );
  memset( voltageConvertData, 0x0, sizeof(convert_fault_t) );
  if( voltageConvertData )
  {
  }
  analog_multiplexer_init();
  err = multi_channel_adc_dma_init();
  require_noerr_quiet( err, exit );
exit:
  if( kNoErr != err )
  {
    vol_detect_log("voltage detection init error!");
  }
  else
    vol_detect_log("voltage detection init success!");
  return err;
}

static void SetFaultBit(fault_bit_t bit_num)
{
    voltageConvertData->faultBitTemp[bit_num/8] |= 1 << (bit_num % 8);
}
static uint16_t processChannelsData( adc_channel_t type )
{
  uint16_t      readData;
  
  struct convert_adc_data *pConvertAdcData = (struct convert_adc_data *)&convert_data[type];
  
  if( pConvertAdcData->isNeedDelay == 'Y' )
  {
    select_multi_channel( pConvertAdcData->adc_type );
    //HAL_Delay(1);//10ms
    delay_us(8000);
  }
  
  readData = (uint16_t)(read_channel_values_mV( pConvertAdcData->adc_type ) * pConvertAdcData->convert_factor );
  
  if( readData > pConvertAdcData->threshold_high || readData < pConvertAdcData->threshold_low )
  {
     if( pConvertAdcData->err_start_time == 0 )
     {
       pConvertAdcData->err_start_time = (uint16_t)os_get_time();
     }
     else
     {
       if( (uint16_t)os_get_time() - pConvertAdcData->err_start_time > \
         pConvertAdcData->err_duration_time )
       {       
         //*(uint32_t *)voltageConvertData->faultBitTemp |= (uint32_t)(0x1) << pConvertAdcData->fault_bit_mask_num;
         SetFaultBit(pConvertAdcData->fault_bit_mask_num);
         pConvertAdcData->err_value = readData;
         boardStatus->errChannel = pConvertAdcData->err_channel;
         boardStatus->errValue = readData;
       }
     }
  }
  else
  {
    if( pConvertAdcData->err_start_time != 0 )
    {
       pConvertAdcData->err_start_time = 0;
    }
  }
  return readData;
}

static uint8_t  sample_index = TEMP_12_TS;
static void computeVoltage( void )
{  
  voltageConvert->v_bat = processChannelsData( VOLTAGE_BAT );
  //boardStatus->vBatLevel = get_percentage_from_battery_voltage( voltageConvert->bat_voltage );
  
#if 1
  voltageConvert->c_5V_right_hand_motor = processChannelsData( CURRENTS_5V_RIGHT_HAND );  
  voltageConvert->c_24V_hd_camera = processChannelsData( CURRENTS_24V_HEAD_CAMERA );
  voltageConvert->c_24V_head_motor = processChannelsData( CURRENTS_24V_HEAD ); 
  voltageConvert->c_24V_camera = processChannelsData( CURRENTS_24V_CAMERA ); 
  voltageConvert->c_5V_keyboard = processChannelsData( CURRENTS_5V_KEYBOARD ); 
  voltageConvert->c_5V_code = processChannelsData( CURRENTS_5V_CODE );
  voltageConvert->c_5V_left_hand_motor = processChannelsData( CURRENTS_5V_LEFT_HAND ); 
  voltageConvert->c_5V_card_reader = processChannelsData( CURRENTS_5V_ID_CARD );
  voltageConvert->c_5V_head = processChannelsData( CURRENTS_5V_HEAD ); 
  voltageConvert->c_24V_right_hand_motor = processChannelsData( CURRENTS_24V_RIGHT_HAND_MOTOR ); 
  voltageConvert->c_12V_head_rk = processChannelsData( CURRENTS_HEAD_RK );
  voltageConvert->c_12V_chest_rk = processChannelsData( CURRENTS_CHEST_RK ); 
  voltageConvert->c_24V_left_hand_motor = processChannelsData( CURRENTS_24V_LEFT_HAND_MOTOR ); 
  voltageConvert->c_12V_audio_pa = processChannelsData( CURRENTS_AUDIO_PA ); 
  voltageConvert->c_5V_repair = processChannelsData( CURRENTS_5V_REPAIR );   
  voltageConvert->c_5V_touch = processChannelsData( CURRENTS_5V_TOUCH );
  voltageConvert->c_12V_switch = processChannelsData( CURRENTS_12V_SWITCH   );
  voltageConvert->c_12V_router = processChannelsData( CURRENTS_12V_ROUTER );
  voltageConvert->c_vbus = processChannelsData( CURRENTS_VBUS );
  voltageConvert->c_12V_card_reader = processChannelsData( CURRENTS_CARD_READER );
#endif
  
  switch( sample_index )
  {
  case TEMP_12_TS:
    voltageConvert->temp_12V_ts = get_ntc_temp_from_voltage(processChannelsData( TEMP_12_TS ));
    //voltageConvert->temp_12V_ts = processChannelsData( TEMP_12_TS );//test
    sample_index = TEMP_5V_TS;
  break;
  case TEMP_5V_TS:
    voltageConvert->temp_5V_ts = get_ntc_temp_from_voltage(processChannelsData( TEMP_5V_TS ));
    //voltageConvert->temp_5V_ts = processChannelsData( TEMP_5V_TS );//test
    sample_index = TEMP_AIR_TS;
  break;
  case TEMP_AIR_TS:
    voltageConvert->temp_air_ts = get_ntc_temp_from_voltage(processChannelsData( TEMP_AIR_TS ));
    //voltageConvert->temp_air_ts = processChannelsData( TEMP_AIR_TS );//test
    sample_index = CURRENTS_5V_HEAD_TOUCH;
  break;
  case CURRENTS_5V_HEAD_TOUCH:
    voltageConvert->c_5V_head_touch = processChannelsData( CURRENTS_5V_HEAD_TOUCH );
    sample_index = CURRENTS_5V_HEAD_LED;
  break;
  case CURRENTS_5V_HEAD_LED:
    voltageConvert->c_5V_head_led = processChannelsData( CURRENTS_5V_HEAD_LED );
    sample_index = VOLTAGE_12V;
  break;
  case VOLTAGE_12V:
    voltageConvert->v_12V = processChannelsData( VOLTAGE_12V );
    sample_index = VOLTAGE_5V;
  break;
  case VOLTAGE_5V:
    voltageConvert->v_5V = \
                    processChannelsData( VOLTAGE_5V );
    sample_index = VOLTAGE_BAT;
  break;
  case VOLTAGE_BAT:
    voltageConvert->v_bat = \
                processChannelsData( VOLTAGE_BAT );
    sample_index = CURRENTS_5V_RESERVE;
  break;
  case CURRENTS_5V_RESERVE:
    voltageConvert->c_5V_reserve = \
                processChannelsData( CURRENTS_5V_RESERVE );
    sample_index = CURRENTS_5V_LED;
  break;
  case CURRENTS_5V_LED:
    voltageConvert->c_5V_led = \
                processChannelsData( CURRENTS_5V_LED );
    sample_index = CURRENTS_5V_CAMERA;
  break;
  case CURRENTS_5V_CAMERA:
    voltageConvert->c_5V_camera = processChannelsData( CURRENTS_5V_CAMERA );
    sample_index = CURRENTS_5V_HD_CAMERA;
  break;
  case CURRENTS_5V_HD_CAMERA:
    voltageConvert->c_5V_hd_camera = processChannelsData( CURRENTS_5V_HD_CAMERA );
    sample_index = TEMP_12_TS;
  break;
  }
}

uint8_t is_need_print_adc_data = 0;
void PrintAdcData(void)
{
printf("\r\n5v right hand motor currents        %d\r\n",voltageConvert->c_5V_right_hand_motor);
    printf("24v hd camera currents              %d\r\n",voltageConvert->c_24V_hd_camera);
    printf("24v head motor currents             %d\r\n",voltageConvert->c_24V_head_motor);
    printf("24v camera motor currents           %d\r\n",voltageConvert->c_24V_camera);
    printf("5v keyboard currents                %d\r\n",voltageConvert->c_5V_keyboard);
    printf("5v code board currents              %d\r\n",voltageConvert->c_5V_code);
    printf("5v left hand motor currents         %d\r\n",voltageConvert->c_5V_left_hand_motor);
    printf("5v card reader currents             %d\r\n",voltageConvert->c_5V_card_reader);
    printf("5v head currents                    %d\r\n",voltageConvert->c_5V_head);
    printf("24v right hand motor currents       %d\r\n",voltageConvert->c_24V_right_hand_motor);
    printf("12v head rk currents                %d\r\n",voltageConvert->c_12V_head_rk);
    printf("12v chest rk currents               %d\r\n",voltageConvert->c_12V_chest_rk);
    printf("24v left hand motor currents        %d\r\n",voltageConvert->c_24V_left_hand_motor);
    printf("12v audio pa currents               %d\r\n",voltageConvert->c_12V_audio_pa);
    printf("5v repair board currents            %d\r\n",voltageConvert->c_5V_repair);
    printf("5v touch board currents             %d\r\n",voltageConvert->c_5V_touch);
    printf("12v switch currents                 %d\r\n",voltageConvert->c_12V_switch);
    printf("12v router currents                 %d\r\n",voltageConvert->c_12V_router);
    printf("vbus currents                       %d\r\n",voltageConvert->c_vbus);
    printf("12 card reader currents             %d\r\n",voltageConvert->c_12V_card_reader);
    printf("12v temperature                     %d\r\n",voltageConvert->temp_12V_ts);
    printf("5v temperature                      %d\r\n",voltageConvert->temp_5V_ts);
    printf("air temperature                     %d\r\n",voltageConvert->temp_air_ts);
    printf("5v head touch currents              %d\r\n",voltageConvert->c_5V_head_touch);
    printf("12v all currents                    %d\r\n",voltageConvert->c_12V_all);
    printf("5v all currents                     %d\r\n",voltageConvert->c_5V_all);
    printf("5v head led currents                %d\r\n",voltageConvert->c_5V_head_led);
    printf("12v voltage                         %d\r\n",voltageConvert->v_12V);
    printf("5v voltage                          %d\r\n",voltageConvert->v_5V);
    printf("vbat voltage                        %d\r\n",voltageConvert->v_bat);
    printf("5v reserve currents                 %d\r\n",voltageConvert->c_5V_reserve);
    printf("5v led currents                     %d\r\n",voltageConvert->c_5V_led);
    printf("5v camera currents                  %d\r\n",voltageConvert->c_5V_camera);
    printf("5v hd camera currents               %d\r\n",voltageConvert->c_5V_hd_camera);
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

//static uint32_t lowVoltageStartTime = 0;
static uint32_t  batteryPercentageStartTime = 0;
void VolDetect_Tick( void )
{
    computeVoltage();
    if( os_get_time() - batteryPercentageStartTime >= 1000/SYSTICK_PERIOD )
    {
      batteryPercentageStartTime = os_get_time();
      battery_percentage_1s_period();
#if 1
      if(is_need_print_adc_data == 1)
      {
        PrintAdcData();
      }
      //UploadAdcData();//////
      
#endif
    }
    
    if( ( YES == voltageDebug.isNeedUpload ) && IS_SEND_RATE_DATA( voltageDebug.uploadRate ) )
    {     
      if( voltageDebug.uploadRate == SEND_RATE_SINGLE )
      {
//        uploadCurrentInformation( serial, voltageConvert );
        UploadAdcData();
        voltageDebug.isNeedUpload = NO;
        boardStatus->sysStatus &= ~(uint16_t)STATE_IS_AUTO_UPLOAD;
      }
      else
      {
        if( (os_get_time() - voltageDebug.uploadFlagTime) >= sendRateToTime(voltageDebug.uploadRate) )
        {
          boardStatus->sysStatus |= (uint16_t)STATE_IS_AUTO_UPLOAD;
          voltageDebug.uploadFlagTime = os_get_time();
          //uploadCurrentInformation( serial, voltageConvert );
          //CanUpLoadCurrentInfo();
          UploadAdcData();
        }
      }
    }
    if( PRINT_ONCE == voltageDebug.printType || PRINT_PEROID == voltageDebug.printType )
    {
      if( voltageDebug.printType == PRINT_ONCE )
      {
          voltageDebug.printType = PRINT_NO;
      }
      if( PRINT_PEROID == voltageDebug.printType )
      {
        if( os_get_time() - voltageDebug.startTime < voltageDebug.peroid/SYSTICK_PERIOD )
        {
          return;
        }
      }
#if 0     

        
#else      
      printf("\r\n");
      printf("battery percertage: %d%\r\n", boardStatus->vBatLevel);//get_percentage_from_battery_voltage( voltageConvert->bat_voltage ) );
      for( uint8_t i = 1; i <= sizeof(voltageData_t)/2; i++ )
      {
        if( i == 17 )
           printf("\r\n");
        printf( "%d: %d\t", i, *((uint16_t *)voltageConvert + i - 1) );
      }     
      printf("\r\n");

#endif
      if( PRINT_PEROID == voltageDebug.printType )
      {
        voltageDebug.startTime = os_get_time();
        //vol_detect_log("print peroid = %d ms",voltageDebug.peroid );
      }
    }
#ifdef  VOLTAGE_DEBUG
    if( PRINT_ONCE == voltageDebug.printMaxType || RESET_MAX_BUF == voltageDebug.printMaxType )
    {
      if( voltageDebug.printMaxType == PRINT_ONCE )
      {
          voltageDebug.printMaxType = PRINT_NO;
      }
      if( RESET_MAX_BUF == voltageDebug.printMaxType )
      {
        memset(tempMaxVoltageData, 0x0, sizeof(voltageData_t));
        if( tempMaxVoltageData )
        {
          tempMaxVoltageData->v_bat = 10000;//set temp max voltage is 100v
        }
        voltageDebug.printMaxType = PRINT_NO;
      }
      vol_detect_log("min vbat = %.2f V", tempMaxVoltageData->v_bat/100.0);
    }
#endif //#ifdef  VOLTAGE_DEBUG
#if 0  
    if( SWITCH_ON == switch_user->switchOnOff )
    {
      if( voltageConvert->v_bat < VBAT_LOW_POWER_LEVEL )
      {
        boardStatus->sysStatus |= STATE_IS_LOW_POWER;
      }
      else
      {
        boardStatus->sysStatus &= ~STATE_IS_LOW_POWER;
      }
         
      if( voltageConvert->v_bat < VBAT_POWER_OFF_LEVEL )
      {
        if( lowVoltageStartTime == 0)
        {
          lowVoltageStartTime = os_get_time();
        }
        if( os_get_time() - lowVoltageStartTime >= 5000/SYSTICK_PERIOD )
        {
          PowerOffDevices(POWER_HEAD_RK);
          PowerOffDevices(POWER_CHEST_RK);
          lowVoltageStartTime = 0;
        }
      }
      else
      {
        if( lowVoltageStartTime != 0 )
        {
          lowVoltageStartTime = 0;
        }
      }
    }
#endif
}

//1s compute once
#define BUFFER_SIZE   20
static uint16_t percentage[BUFFER_SIZE];
static uint32_t percentage_sum;
void battery_percentage_1s_period( void )
{
    uint16_t batVolPlusInsideRes;
    static uint16_t minBatLevel = 0;
    static uint8_t flag = 0;
    for( uint32_t i = 0; i < BUFFER_SIZE - 1; i++ )
    {
      percentage[i] = percentage[i+1];
    }
    batVolPlusInsideRes = (uint16_t)(voltageConvert->v_bat + \
      voltageConvert->c_vbus * BAT_RES);//V(bat)=V(output)+I(bat)*R(batRes)
    percentage[BUFFER_SIZE-1] = get_percentage_from_battery_voltage( batVolPlusInsideRes );
    percentage_sum = 0;
    uint8_t percentage_sum_count = 0;
    for( uint32_t i = 0; i < BUFFER_SIZE; i++ )
    {
      if( percentage[i] != 0 )
      {
        percentage_sum += percentage[i];
        percentage_sum_count ++;
      }
    }
    minBatLevel = (uint16_t)( percentage_sum/(percentage_sum_count * 1.0) );
    if(flag == 0)
    {
        flag = 1;
        boardStatus->vBatLevel = minBatLevel;
    }
    //if
    if(minBatLevel < boardStatus->vBatLevel)
    {
        boardStatus->vBatLevel = minBatLevel;
    }
    //boardStatus->vBatLevel = (uint16_t)( percentage_sum/(percentage_sum_count * 1.0) );
}
/*********************END OF FILE**************/
