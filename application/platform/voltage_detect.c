/* 
*  Author: Adam Huang
*  Date:2016/6/8
*/
#include "voltage_detect.h"
#include <stdlib.h>
#include "stm32f1xx_powerboard.h"
#include "app_platform.h"
#include "serial_leds.h"
#include "protocol.h"
#include "multi_channel_detection.h"
#include "temp_reference.h"
#include "board_init.h"

#define vol_detect_log(M, ...) custom_log("VolDetect", M, ##__VA_ARGS__)
#define vol_detect_log_trace() custom_log_trace("VolDetect")

#define BAT_INNER_RES                0.1 //unit: ¦¸

static voltageData_t ramVoltageConvert;
voltageData_t *voltageConvert = NULL;//&ramVoltageConvert;

#ifdef VOLTAGE_DEBUG
static voltageData_t ramTempMaxVoltageData;
static voltageData_t *tempMaxVoltageData = NULL;
#endif

voltageConvertData_t *voltageConvertData;
voltageDebug_t voltageDebug;

static void computeVoltage( void );

typedef enum {
  CURRENTS_5V_RES = 0,
  CURRENTS_12V_RES,
  CURRENTS_BAT_NV,
  CURRENTS_12V_NV,
  CURRENTS_ROUTER_12V,
  CURRENTS_DYP,
  CURRENTS_SENSOR,
  CURRENTS_DLP,
  CURRENTS_MOTOR,
  CURRENTS_24V_RES,
  CURRENTS_2_1_PA,
  CURRENTS_PAD,
  CURRENTS_PRINTER,
  CURRENTS_X86,
  CURRENTS_IRLED,
  CURRENTS_LEDS,
  CURRENTS_CHARGE,
  CURRENTS_BATIN,
  CURRENTS_VBUS,
  CURRENTS_BAT_MOTOR,
  TEMP_24V_TS,
  TEMP_12V_TS,
  TEMP_5V_TS,
  TEMP_AIR_TS,
  CURRENTS_24V_ALL,
  CURRENTS_12V_ALL,
  CURRENTS_5V_ALL,
  VOLTAGE_24V,
  VOLTAGE_12V,
  VOLTAGE_5V,
  VOLTAGE_BAT,
  CURRENTS_AIUI,
  CURRENTS_ROUTER_5V,
} adc_channel_t ;

const struct convert_adc_data convert_data[] = {
  [CURRENTS_5V_RES] = 
  {
    .adc_type           = MICO_ADC_5V_RES1,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 6.05,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x01,
    .fault_bit_mask_num = RES_5V_CURRENTS_FAULT_BIT_MASK_NUM,
  },
  [CURRENTS_12V_RES] = 
  {
    .adc_type           = MICO_ADC_12V_RES2,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.95,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x02,
    .fault_bit_mask_num = RES_12V_CURRENTS_FAULT_BIT_MASK_NUM,
  },
  [CURRENTS_VBUS] = 
  {
    .adc_type           = MICO_ADC_VBUS,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 50,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x04,
    .fault_bit_mask_num = SYS_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  //DH12V
  //DH5V
  [CURRENTS_SENSOR] = 
  {
    .adc_type           = MICO_ADC_SENSOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.55,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x07,
    .fault_bit_mask_num = SENSOR_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_DLP] = 
  {
    .adc_type           = MICO_ADC_DLP,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 6.2,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x08,
    .fault_bit_mask_num = DLP_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_MOTOR] = 
  {
    .adc_type           = MICO_ADC_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,//5.8,//
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x09,
    .fault_bit_mask_num = MOTOR_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_24V_RES] = 
  {
    .adc_type           = MICO_ADC_24V_RES1,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 4.985,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0A,
    .fault_bit_mask_num = RES_24V_CURRENTS_FAULT_BIT_MASK_NUM, 
  }, 
  [CURRENTS_2_1_PA] = 
  {
    .adc_type           = MICO_ADC_2_1_PA,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.59,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0B,
    .fault_bit_mask_num = PA_2_1_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_PAD] = 
  {
    .adc_type           = MICO_ADC_PAD,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 6.3,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0C,
    .fault_bit_mask_num = PAD_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_PRINTER] = 
  {
    .adc_type           = MICO_ADC_PRINTER,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.56,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0D,
    .fault_bit_mask_num = PRINTER_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_X86] = 
  {
    .adc_type           = MICO_ADC_X86,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.1,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0E,
    .fault_bit_mask_num = X86_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_BAT_MOTOR] = 
  {
    .adc_type           = MICO_ADC_BAT_MOTOR,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 50,//41.17,//
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0F,
    .fault_bit_mask_num = BAT_MOTOR_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_LEDS] = 
  {
    .adc_type           = MICO_ADC_LEDS,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5.45,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x10,
    .fault_bit_mask_num = LEDS_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_BAT_NV] = 
  {
    .adc_type           = MICO_ADC_BAT_NV,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,//5.65,//
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x11,
    .fault_bit_mask_num = BAT_NV_CURRENTS_FAULT_BIT_MASK_NUM,    
  },
  [CURRENTS_12V_NV] = 
  {
    .adc_type           = MICO_ADC_12V_NV,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 6.2,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x12,
    .fault_bit_mask_num = _12V_NV_CURRENTS_FAULT_BIT_MASK_NUM,    
  },
  [CURRENTS_ROUTER_12V] = 
  {
    .adc_type           = MICO_ADC_ROUTER_12V,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 7.55,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x13,
    .fault_bit_mask_num = ROUTER_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_DYP] = 
  {
    .adc_type           = MICO_ADC_DYP,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,//7.25,//
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x14,
    .fault_bit_mask_num = DYP_CURRENTS_FAULT_BIT_MASK_NUM, 
  },  
  [CURRENTS_IRLED] = 
  {
    .adc_type           = MICO_ADC_IRLED,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x15,
    .fault_bit_mask_num = IRLED_CURRENTS_FAULT_BIT_MASK_NUM, 
  }, 
  [CURRENTS_CHARGE] = 
  {
    .adc_type           = MICO_ADC_CHARGE,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 50,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x16,
    .fault_bit_mask_num = CHARGE_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_BATIN] = 
  {
    .adc_type           = MICO_ADC_BATIN,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 50,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x17,
    .fault_bit_mask_num = BATIN_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_24V] = 
  {
    .adc_type           = MICO_ADC_VDET_24V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x18,
    .fault_bit_mask_num = _24V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_12V] = 
  {
    .adc_type           = MICO_ADC_VDET_12V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x19,
    .fault_bit_mask_num = _12V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_5V] = 
  {
    .adc_type           = MICO_ADC_VDET_5V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 2,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1A,
    .fault_bit_mask_num = _5V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_5V_ALL] = 
  {
    .adc_type           = MICO_ADC_5V_ALL,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1B,
    .fault_bit_mask_num = _5V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_12V_ALL] = 
  {
    .adc_type           = MICO_ADC_12V_ALL,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1C,
    .fault_bit_mask_num = _12V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_24V_ALL] = 
  {
    .adc_type           = MICO_ADC_24V_ALL,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1D,
    .fault_bit_mask_num = _24V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_AIR_TS] = 
  {
    .adc_type           = MICO_ADC_AIR_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1E,
    .fault_bit_mask_num = AMBIENT_TEMP_FAULT_BIT_BIT_MASK_NUM, 
  },
  [TEMP_5V_TS] = 
  {
    .adc_type           = MICO_ADC_5V_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1F,
    .fault_bit_mask_num = _5V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_12V_TS] = 
  {
    .adc_type           = MICO_ADC_12V_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x20,
    .fault_bit_mask_num = _12V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_24V_TS] = 
  {
    .adc_type           = MICO_ADC_24V_TS,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'Y',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x21,
    .fault_bit_mask_num = RES_12V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },  
  [VOLTAGE_BAT] = 
  {
    .adc_type           = MICO_ADC_VDET_BAT,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'Y',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = 0,
  },
  [CURRENTS_AIUI] = 
  {
    .adc_type           = MICO_ADC_AIUI,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = 5.4,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = 0,
  },
  [CURRENTS_ROUTER_5V] = 
  {
    .adc_type           = MICO_ADC_ROUTER_5V,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'Y',
    .convert_factor     = 5.55,//5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
    .fault_bit_mask_num = 0,
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
    tempMaxVoltageData->bat_voltage = 10000;//set temp max voltage is 100v
  }
#endif
  voltageConvertData = (voltageConvertData_t *)malloc( sizeof(voltageConvertData_t) );
  require_action( voltageConvertData, exit, err = kNoMemoryErr );
  memset( voltageConvertData, 0x0, sizeof(voltageConvertData_t) );
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

static uint16_t processChannelsData( adc_channel_t type )
{
  uint16_t      readData;
  
  struct convert_adc_data *pConvertAdcData = (struct convert_adc_data *)&convert_data[type];
  
  if( pConvertAdcData->isNeedDelay == 'Y' )
  {
    select_multi_channel( pConvertAdcData->adc_type );
    HAL_Delay(1);//10ms
  }
  
  readData = (uint16_t)(read_channel_values_mV( pConvertAdcData->adc_type ) * pConvertAdcData->convert_factor );
  
  if( readData > pConvertAdcData->threshold_high || readData < pConvertAdcData->threshold_low )
  {
     if( !pConvertAdcData->err_start_time )
     {
       pConvertAdcData->err_start_time = (uint16_t)os_get_time();
     }
     else
     {
       if( (uint16_t)os_get_time() - pConvertAdcData->err_start_time > \
         pConvertAdcData->err_duration_time )
       {
         *(uint32_t *)voltageConvertData->faultBitTemp |= \
           (uint32_t)(0x1) << pConvertAdcData->fault_bit_mask_num;
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

static uint8_t  sample_index = VOLTAGE_24V;
static void computeVoltage( void )
{  
  voltageConvert->bat_voltage = processChannelsData( VOLTAGE_BAT );
  //boardStatus->vBatLevel = get_percentage_from_battery_voltage( voltageConvert->bat_voltage );
  
  voltageConvert->_5V_reserve1_currents = processChannelsData( CURRENTS_5V_RES );
  voltageConvert->_12V_reserve2_currents = processChannelsData( CURRENTS_12V_RES );
  voltageConvert->sys_all_currents = processChannelsData( CURRENTS_VBUS );
  voltageConvert->sensor_currents = processChannelsData( CURRENTS_SENSOR );
  voltageConvert->dlp_currents = processChannelsData( CURRENTS_DLP );
  voltageConvert->motor_5v_currents = processChannelsData( CURRENTS_MOTOR );
  voltageConvert->_24V_reserve1_currents = processChannelsData( CURRENTS_24V_RES );
  voltageConvert->_2_1_pa_currents = processChannelsData( CURRENTS_2_1_PA );
  voltageConvert->pad_currents = processChannelsData( CURRENTS_PAD );
  voltageConvert->printer_currents = processChannelsData( CURRENTS_PRINTER );
  voltageConvert->x86_currents = processChannelsData( CURRENTS_X86 );
  voltageConvert->motor_currents = processChannelsData( CURRENTS_BAT_MOTOR );
  voltageConvert->_5V_led_currents = processChannelsData( CURRENTS_LEDS );
  voltageConvert->bat_nv_currents = processChannelsData( CURRENTS_BAT_NV );
  voltageConvert->_12V_nv_currents = processChannelsData( CURRENTS_12V_NV );    
  voltageConvert->_12V_router_currents = processChannelsData( CURRENTS_ROUTER_12V );
  voltageConvert->dyp_currents = processChannelsData( CURRENTS_DYP );
  voltageConvert->ir_led_currents = processChannelsData( CURRENTS_IRLED );
  voltageConvert->charger_currents = processChannelsData( CURRENTS_CHARGE );
  voltageConvert->charge_currents = processChannelsData( CURRENTS_BATIN );

  switch( sample_index )
  {
  case VOLTAGE_24V:
    voltageConvert->_24V_voltage = processChannelsData( VOLTAGE_24V );
    sample_index = VOLTAGE_12V;
  break;
  case VOLTAGE_12V:
    voltageConvert->_12V_voltage = processChannelsData( VOLTAGE_12V );
    sample_index = VOLTAGE_5V;
  break;
  case VOLTAGE_5V:
    voltageConvert->_5V_voltage = processChannelsData( VOLTAGE_5V );
    sample_index = CURRENTS_5V_ALL;
  break;
  case CURRENTS_5V_ALL:
    voltageConvert->_5V_currents = processChannelsData( CURRENTS_5V_ALL );
    sample_index = CURRENTS_12V_ALL;
  break;
  case CURRENTS_12V_ALL:
    voltageConvert->_12V_currents = processChannelsData( CURRENTS_12V_ALL );
    sample_index = CURRENTS_24V_ALL;
  break;
  case CURRENTS_24V_ALL:
    voltageConvert->_24V_currents = processChannelsData( CURRENTS_24V_ALL );
    sample_index = TEMP_AIR_TS;
  break;
  case TEMP_AIR_TS:
    voltageConvert->ambient_temperature = \
                get_ntc_temp_from_voltage( processChannelsData( TEMP_AIR_TS ) );
    sample_index = TEMP_5V_TS;
  break;
  case TEMP_5V_TS:
    voltageConvert->_5V_regulator_temp = \
                get_ntc_temp_from_voltage( processChannelsData( TEMP_5V_TS ) );
    sample_index = TEMP_12V_TS;
  break;
  case TEMP_12V_TS:
    voltageConvert->_12V_regulator_temp = \
                get_ntc_temp_from_voltage( processChannelsData( TEMP_12V_TS ) );
    sample_index = TEMP_24V_TS;
  break;
  case TEMP_24V_TS:
    voltageConvert->_24V_regulator_temp = \
                get_ntc_temp_from_voltage( processChannelsData( TEMP_24V_TS ) );
    sample_index = CURRENTS_AIUI;
  break;
  case CURRENTS_AIUI:
    voltageConvert->aiui_currents = processChannelsData( CURRENTS_AIUI );
    sample_index = CURRENTS_ROUTER_5V;
  break;
  case CURRENTS_ROUTER_5V:
    voltageConvert->_5V_router_currents = processChannelsData( CURRENTS_ROUTER_5V );
    sample_index = VOLTAGE_24V;
  break;
  }
}

#if !CHARGE_DETECT_USE_INT
static uint32_t noChargeStartTime = 0;
static void isChargeInPeriod( void )
{
    if( voltageConvert->charge_currents > 0 )
    {
        boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGING;
        noChargeStartTime = os_get_time();
    }
    else
    {
        if( os_get_time() - noChargeStartTime >= 60000/SYSTICK_PERIOD && soc_contex.volatge_percentage > 90 )
        {
          boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGING);
          soc_contex.state_of_charge_mws = SOC_TOTAL_MWS;
        }
    }
}
#endif

static uint32_t lowVoltageStartTime = 0;
static uint32_t  batteryPercentageStartTime = 0;
void VolDetect_Tick( void )
{
    computeVoltage();
    if( os_get_time() - batteryPercentageStartTime >= 1000/SYSTICK_PERIOD )
    {
      batteryPercentageStartTime = os_get_time();
      battery_percentage_1s_period();
#if !CHARGE_DETECT_USE_INT
      charger_detect_period();
      if( boardStatus->sysStatus & (uint16_t)STATE_IS_CHARGER_IN )
      {
        isChargeInPeriod();
      }
      else
      {
        boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGING);
      }
#endif
    }
    
    if( ( YES == voltageDebug.isNeedUpload ) && IS_SEND_RATE_DATA( voltageDebug.uploadRate ) )
    {     
      if( voltageDebug.uploadRate == SEND_RATE_SINGLE )
      {
//        uploadCurrentInformation( serial, voltageConvert );
        voltageDebug.isNeedUpload = NO;
        boardStatus->sysStatus &= ~(uint16_t)STATE_IS_AUTO_UPLOAD;
      }
      else
      {
        if( (os_get_time() - voltageDebug.uploadFlagTime) >= sendRateToTime(voltageDebug.uploadRate) )
        {
          boardStatus->sysStatus |= (uint16_t)STATE_IS_AUTO_UPLOAD;
          voltageDebug.uploadFlagTime = os_get_time();
          uploadCurrentInformation( serial, voltageConvert );
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
      printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
             voltageConvert->bat_voltage,\
             voltageConvert->sys_all_currents,\
             voltageConvert->dh_12V_currents,\
             voltageConvert->dh_5V_currents,\
             voltageConvert->sensor_currents,\
             voltageConvert->dlp_currents,\
             voltageConvert->motor_5v_currents,\
             voltageConvert->_2_1_pa_currents,\
             voltageConvert->pad_currents,\
             voltageConvert->printer_currents,\
             voltageConvert->x86_currents,\
             voltageConvert->motor_currents,\
             voltageConvert->_5V_led_currents,\
             voltageConvert->_5V_reserve1_currents,\
             voltageConvert->_12V_reserve2_currents,\
             voltageConvert->_24V_reserve1_currents  );
#else      
      uint32_t sum_check = 0;
      uint32_t single_num;
      printf("\r\n");
      printf("battery percertage: %d%%\r\n", boardStatus->vBatLevel);//get_percentage_from_battery_voltage( voltageConvert->bat_voltage ) );
      for( uint8_t i = 1; i <= sizeof(voltageData_t)/2; i++ )
      {
        if( i == 17 )
           printf("\r\n");
        single_num = *((uint16_t *)voltageConvert + i - 1);
        printf( "%d: %d\t", i, single_num );
        sum_check += single_num;
      }     
      printf("\r\n");
      vol_detect_log( "above num checksum: %d", sum_check );

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
          tempMaxVoltageData->bat_voltage = 10000;//set temp max voltage is 100v
        }
        voltageDebug.printMaxType = PRINT_NO;
      }
      vol_detect_log("min vbat = %.2f V", tempMaxVoltageData->bat_voltage/100.0);
      vol_detect_log("max sys_all = %d mA", tempMaxVoltageData->sys_all_currents);
      vol_detect_log("max dh_12V = %d mA", tempMaxVoltageData->dh_12V_currents);
      vol_detect_log("max dh_5v = %d mA", tempMaxVoltageData->dh_5V_currents);
      vol_detect_log("max sensor = %d mA", tempMaxVoltageData->sensor_currents);
      vol_detect_log("max dlp = %d mA", tempMaxVoltageData->dlp_currents);
      vol_detect_log("max motor_5v = %d mA", tempMaxVoltageData->motor_5v_currents);
      vol_detect_log("max _2_1_pa = %d mA", tempMaxVoltageData->_2_1_pa_currents);
      vol_detect_log("max pad = %d mA", tempMaxVoltageData->pad_currents);
      vol_detect_log("max printer = %d mA", tempMaxVoltageData->printer_currents);
      vol_detect_log("max x86  = %d mA", tempMaxVoltageData->x86_currents);
      vol_detect_log("max motor = %d mA", tempMaxVoltageData->motor_currents);
      vol_detect_log("max _5V_led = %d mA", tempMaxVoltageData->_5V_led_currents);
      vol_detect_log("max _5V_reserve1 = %d mA", tempMaxVoltageData->_5V_reserve1_currents);
      vol_detect_log("max _12V_reserve2 = %d mA", tempMaxVoltageData->_12V_reserve2_currents);
      vol_detect_log("max _24V_reserve1 = %d mA", tempMaxVoltageData->_24V_reserve1_currents);
    }
#endif //#ifdef  VOLTAGE_DEBUG
    if( SWITCH_ON == switch_user->switchOnOff )
    {
      if( voltageConvert->bat_voltage < VBAT_LOW_POWER_LEVEL )
      {
        boardStatus->sysStatus |= STATE_IS_LOW_POWER;
      }
      else
      {
        boardStatus->sysStatus &= ~STATE_IS_LOW_POWER;
      }
         
      if( voltageConvert->bat_voltage < VBAT_POWER_OFF_LEVEL )
      {
        if( lowVoltageStartTime == 0)
        {
          lowVoltageStartTime = os_get_time();
        }
        if( os_get_time() - lowVoltageStartTime >= 5000/SYSTICK_PERIOD )
        {
          PowerOffDevices();
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
}

//1s compute once
#define BUFFER_SIZE   20
static uint8_t percentage_ref_v[BUFFER_SIZE];
static uint32_t percentage_ref_v_sum;

soc_contex_t soc_contex;

void battery_percentage_1s_period( void )
{
    soc_contex_t *soc = &soc_contex;
    
    if( soc->isInitialized )
    {
      soc->period_time_ms = (os_get_time() - soc->last_integrate_time)*SYSTICK_PERIOD;
      soc->last_integrate_time = os_get_time();

      if( soc->period_time_ms > 2000 )
      {
          return;
      }
    }

    soc->bat_v_mv = voltageConvert->bat_voltage;
    soc->sys_i_ma = voltageConvert->sys_all_currents;
    soc->charge_i_ma = voltageConvert->charge_currents;
    soc->bat_inner_res_ohm = BAT_INNER_RES;

    for( uint32_t i = 0; i < BUFFER_SIZE - 1; i++ )
    {
      percentage_ref_v[i] = percentage_ref_v[i+1];
    }
    
    if( boardStatus->sysStatus & (uint16_t)STATE_IS_CHARGER_IN )
    {
      soc->bat_ocv_mv = (uint16_t)(soc->bat_v_mv - soc->charge_i_ma * soc->bat_inner_res_ohm);
      if( soc->isInitialized )
      {
        soc->charge_soc_mws = ((soc->charge_i_ma * soc->bat_ocv_mv)/1000 * soc->period_time_ms)/1000;
        if( soc->state_of_charge_mws + soc->charge_soc_mws < SOC_TOTAL_MWS )
        {
          soc->state_of_charge_mws += soc->charge_soc_mws;
        }
      }
    }
    else
    {
      soc->bat_ocv_mv = (uint16_t)(soc->bat_v_mv + soc->sys_i_ma * soc->bat_inner_res_ohm);
      if( soc->isInitialized )
      {
        soc->consume_soc_mws = ((soc->sys_i_ma * soc->bat_ocv_mv)/1000 * soc->period_time_ms)/1000;
        if( soc->state_of_charge_mws - soc->consume_soc_mws > 0 )
        {
          soc->state_of_charge_mws -= soc->consume_soc_mws; 
        }
        else
        {
          soc->state_of_charge_mws = 0;
        }
      }
    }
    if( soc->isInitialized )
    {
      if( soc->state_of_charge_mws <= SOC_TOTAL_MWS )
      {
        soc->current_percentage = (uint8_t)((soc->state_of_charge_mws/(SOC_TOTAL_MWS*1.0))*100);
        if( boardStatus->onoff_battery_print )
        {
          vol_detect_log("state_of_charge_mws: %u, soc_total_mws: %u, recharge_s: %d, charge_s: %d, ischarging: %d", \
            soc->state_of_charge_mws, SOC_TOTAL_MWS, MicoGpioInputGet( MICO_GPIO_RECHARGE_IN ), \
              MicoGpioInputGet( MICO_GPIO_CHARGE_IN ), (boardStatus->sysStatus & (uint16_t)STATE_IS_CHARGING) ? 1:0 );
        }
      }
    }
    percentage_ref_v[BUFFER_SIZE-1] = get_percentage_from_battery_voltage( soc->bat_ocv_mv );
    percentage_ref_v_sum = 0;
    uint32_t percentage_sum_count = 0;
    for( uint32_t i = 0; i < BUFFER_SIZE; i++ )
    {
      if( percentage_ref_v[i] != 0 )
      {   
        percentage_ref_v_sum += percentage_ref_v[i];
        percentage_sum_count ++;
      }
    }
    
    soc->volatge_percentage = (uint16_t)( percentage_ref_v_sum/( percentage_sum_count * 1.0 ) );
    if( soc->isInitialized )
    {
      if( boardStatus->onoff_battery_print )
      {
        vol_detect_log("volatge_percentage: %d%%, current_percentage: %d%%", soc->volatge_percentage, soc->current_percentage);
      }
#if 0
      if( abs(soc->volatge_percentage -  soc->current_percentage) > 15 )
      {
          if( soc->volatge_percentage > soc->current_percentage )
          {
            if( soc->state_of_charge_mws + SOC_TOTAL_MWS/100 <= SOC_TOTAL_MWS )
            {
              soc->state_of_charge_mws += SOC_TOTAL_MWS/100;
            }
          }
          else
          {
            if( soc->state_of_charge_mws - SOC_TOTAL_MWS/100 > 0 )
            {
              soc->state_of_charge_mws -= SOC_TOTAL_MWS/100;
            }
          }
      }
      else
      {
        if( soc->volatge_percentage > 99 )
        {
          soc->state_of_charge_mws = SOC_TOTAL_MWS;
        }
        else if( soc->volatge_percentage > 95 )
        {
          if( soc->volatge_percentage > soc->current_percentage )
          {
            if( soc->state_of_charge_mws + SOC_TOTAL_MWS/100 <= SOC_TOTAL_MWS )
            {
              soc->state_of_charge_mws += SOC_TOTAL_MWS/100;
            }
          }
        }
        if( soc->volatge_percentage <= 1 )
        {
          soc->state_of_charge_mws = 0;
        }
        else if( soc->volatge_percentage <= 10 )
        {
          if( soc->volatge_percentage < soc->current_percentage )
          {
            if( soc->state_of_charge_mws - SOC_TOTAL_MWS/100 > 0 )
            {
              soc->state_of_charge_mws -= SOC_TOTAL_MWS/100;
            }
          }
        }
      }
#endif
    }

    if( soc->isInitialized )
    {
      if( ( soc->current_percentage > 0 && soc->current_percentage <= 100 ) )
      {
          boardStatus->vBatLevel = soc->current_percentage;
      }
    }
    else
    {
      if( soc->volatge_percentage > 0 && soc->volatge_percentage <= 100 )
      {
          boardStatus->vBatLevel = soc->volatge_percentage;
      }
    }
}

void init_soc( uint32_t soc_value )
{
    soc_contex.isInitialized = 1;
    soc_contex.last_integrate_time = os_get_time();
    if( soc_value > SOC_TOTAL_MWS || soc_value == 0 )
    {
      soc_contex.state_of_charge_mws = (uint32_t)(SOC_TOTAL_MWS/100 * soc_contex.volatge_percentage);
    }
    else
    {
      soc_contex.state_of_charge_mws = soc_value;
    }
    vol_detect_log("init state_of_charge_mws is: %u", soc_contex.state_of_charge_mws);
}

void deinit_soc( void )
{
  soc_contex.isInitialized = 0;
}
/*********************END OF FILE**************/
