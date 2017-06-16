/* 
*  Author: Adam Huang
*  Date:2016/6/8
*/
#include "voltage_detect.h"
#include <stdlib.h>
#include "stm32f1xx_powerboard.h"
#include "app_platform.h"
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
  CURRENTS_NV = 0,
  CURRENTS_12V_RES,
  CURRENTS_LEDS,
  CURRENTS_5V_RES,
  CURRENTS_24V_ALL,
  CURRENTS_12V_ALL,
  CURRENTS_5V_ALL,
  CURRENTS_VBUS,
  CURRENTS_ICCARD,
  CURRENTS_24V_RES,
  CURRENTS_PRINTER,
  CURRENTS_PC,
  TEMP_24V_TS,
  TEMP_12V_TS,
  TEMP_5V_TS,
  TEMP_AIR_TS,
  VOLTAGE_24V,
  VOLTAGE_12V,
  VOLTAGE_5V,
  VOLTAGE_BAT,
} adc_channel_t ;

const struct convert_adc_data convert_data[] = {
  [CURRENTS_NV] = 
  {
    .adc_type           = MICO_ADC_I_NV,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x01,
 //   .fault_bit_mask_num = RES_5V_CURRENTS_FAULT_BIT_MASK_NUM,
  },
  [CURRENTS_12V_RES] = 
  {
    .adc_type           = MICO_ADC_I_12V_RES,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x02,
  //  .fault_bit_mask_num = RES_12V_CURRENTS_FAULT_BIT_MASK_NUM,
  },
  [CURRENTS_LEDS] = 
  {
    .adc_type           = MICO_ADC_I_LEDS,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x03,
  //  .fault_bit_mask_num = RES_12V_CURRENTS_FAULT_BIT_MASK_NUM,
  },
  [CURRENTS_VBUS] = 
  {
    .adc_type           = MICO_ADC_I_VBUS,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x04,
  //  .fault_bit_mask_num = SYS_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_5V_RES] = 
  {
    .adc_type           = MICO_ADC_I_5V_RES,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x07,
  //  .fault_bit_mask_num = SENSOR_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_ICCARD] = 
  {
    .adc_type           = MICO_ADC_I_ICCARD,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0B,
 //   .fault_bit_mask_num = PA_2_1_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_24V_RES] = 
  {
    .adc_type           = MICO_ADC_I_24V_RES,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0C,
  //  .fault_bit_mask_num = PAD_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_PRINTER] = 
  {
    .adc_type           = MICO_ADC_I_PRINTER,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0D,
 //   .fault_bit_mask_num = PRINTER_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_PC] = 
  {
    .adc_type           = MICO_ADC_I_PC,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x0E,
  //  .fault_bit_mask_num = X86_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_BAT] = 
  {
    .adc_type           = MICO_ADC_V_BAT,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'N',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0,
  //  .fault_bit_mask_num = 0,
  },
  [VOLTAGE_24V] = 
  {
    .adc_type           = MICO_ADC_V_24V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'N',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x18,
  //  .fault_bit_mask_num = _24V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_12V] = 
  {
    .adc_type           = MICO_ADC_V_12V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'N',
    .convert_factor     = 11,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x19,
  //  .fault_bit_mask_num = _12V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [VOLTAGE_5V] = 
  {
    .adc_type           = MICO_ADC_V_5V,
    .convert_type       = CONVERT_TYPE_VOLTAGE,
    .isNeedDelay        = 'N',
    .convert_factor     = 2,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1A,
  //  .fault_bit_mask_num = _5V_VOLTAGE_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_5V_ALL] = 
  {
    .adc_type           = MICO_ADC_I_5V,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1B,
 //   .fault_bit_mask_num = _5V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_12V_ALL] = 
  {
    .adc_type           = MICO_ADC_I_12V,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1C,
  //  .fault_bit_mask_num = _12V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [CURRENTS_24V_ALL] = 
  {
    .adc_type           = MICO_ADC_I_24V,
    .convert_type       = CONVERT_TYPE_CURRENTS,
    .isNeedDelay        = 'N',
    .convert_factor     = 5,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1D,
  //  .fault_bit_mask_num = _24V_CURRENTS_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_AIR_TS] = 
  {
    .adc_type           = MICO_ADC_TS_AIR,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'N',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1E,
  //  .fault_bit_mask_num = AMBIENT_TEMP_FAULT_BIT_BIT_MASK_NUM, 
  },
  [TEMP_5V_TS] = 
  {
    .adc_type           = MICO_ADC_TS_5V,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'N',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x1F,
  //  .fault_bit_mask_num = _5V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_12V_TS] = 
  {
    .adc_type           = MICO_ADC_TS_12V,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'N',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x20,
  //  .fault_bit_mask_num = _12V_REGULATOR_TEMP_FAULT_BIT_MASK_NUM, 
  },
  [TEMP_24V_TS] = 
  {
    .adc_type           = MICO_ADC_TS_24V,
    .convert_type       = CONVERT_TYPE_TEMP,
    .isNeedDelay        = 'N',
    .convert_factor     = 1,
    .threshold_low      = 0,
    .threshold_high     = 0xFFFF,
    .err_duration_time  = 300/SYSTICK_PERIOD,
    .err_channel        = 0x21,
  //  .fault_bit_mask_num = RES_12V_CURRENTS_FAULT_BIT_MASK_NUM, 
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
    tempMaxVoltageData->bat_voltage = 60000;//set temp max voltage is 60v
  }
#endif
  voltageConvertData = (voltageConvertData_t *)malloc( sizeof(voltageConvertData_t) );
  require_action( voltageConvertData, exit, err = kNoMemoryErr );
  memset( voltageConvertData, 0x0, sizeof(voltageConvertData_t) );
  if( voltageConvertData )
  {
  }
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

static void computeVoltage( void )
{  
  voltageConvert->nv_currents = processChannelsData( CURRENTS_NV );
  voltageConvert->_12v_res_currents = processChannelsData( CURRENTS_12V_RES );
  voltageConvert->leds_currents = processChannelsData( CURRENTS_LEDS );
  voltageConvert->_5v_res_currents = processChannelsData( CURRENTS_5V_RES );
  voltageConvert->_24v_currents = processChannelsData( CURRENTS_24V_ALL );
  voltageConvert->_12v_currents = processChannelsData( CURRENTS_12V_ALL );
  voltageConvert->_5v_currents = processChannelsData( CURRENTS_5V_ALL );
  voltageConvert->sys_all_currents = processChannelsData( CURRENTS_VBUS );
  voltageConvert->iccard_currents = processChannelsData( CURRENTS_ICCARD );
  voltageConvert->_24v_res_currents = processChannelsData( CURRENTS_24V_RES );
  voltageConvert->printer_currents = processChannelsData( CURRENTS_PRINTER );
  voltageConvert->pc_currents = processChannelsData( CURRENTS_PC );
  voltageConvert->_24v_voltage = processChannelsData( VOLTAGE_24V );
  voltageConvert->_12v_voltage = processChannelsData( VOLTAGE_12V );
  voltageConvert->_5v_voltage = processChannelsData( VOLTAGE_5V );
  voltageConvert->bat_voltage = processChannelsData( VOLTAGE_BAT );
  voltageConvert->air_temperature = get_ntc_temp_from_voltage( processChannelsData( TEMP_AIR_TS ) );    
  voltageConvert->_5v_regulator_temp = get_ntc_temp_from_voltage( processChannelsData( TEMP_5V_TS ) );
  voltageConvert->_12v_regulator_temp = get_ntc_temp_from_voltage( processChannelsData( TEMP_12V_TS ) );
  voltageConvert->_24v_regulator_temp = get_ntc_temp_from_voltage( processChannelsData( TEMP_24V_TS ) );
}

static uint32_t lowVoltageStartTime = 0;
static uint32_t  batteryPercentageStartTime = 0;
void VolDetect_Tick( void )
{
    computeVoltage();
    if( os_get_time() - batteryPercentageStartTime >= 1000/SYSTICK_PERIOD )
    {
      batteryPercentageStartTime = os_get_time();
      battery_percentage_1s_period();
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
      ////
      uint32_t sum_check = 0;
      uint32_t single_num;
      printf("\r\n");
      printf("battery percertage: %d%%\r\n", boardStatus->vBatLevel);//get_percentage_from_battery_voltage( voltageConvert->bat_voltage ) );
      for( uint8_t i = 1; i <= sizeof(voltageData_t)/2; i++ )
      {
        if( i == 11 )
           printf("\r\n");
        single_num = *((uint16_t *)voltageConvert + i - 1);
        printf( "%d: %d\t", i, single_num );
        sum_check += single_num;
      }     
      printf("\r\n");
      vol_detect_log( "above num checksum: %d", sum_check );
      ////
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
      vol_detect_log("max nv = %d mA", tempMaxVoltageData->nv_currents);
      vol_detect_log("max _12v_res = %d mA", tempMaxVoltageData->_12v_res_currents);
      vol_detect_log("max leds = %d mA", tempMaxVoltageData->leds_currents);
      vol_detect_log("max _5v_res = %d mA", tempMaxVoltageData->_5v_res_currents);
      vol_detect_log("max _24v = %d mA", tempMaxVoltageData->_24v_currents);
      vol_detect_log("max _12v = %d mA", tempMaxVoltageData->_12v_currents);
      vol_detect_log("max _5v = %d mA", tempMaxVoltageData->_5v_currents);
      vol_detect_log("max sys_all = %d mA", tempMaxVoltageData->sys_all_currents);
      vol_detect_log("max printer = %d mA", tempMaxVoltageData->iccard_currents);
      vol_detect_log("max 24v_res  = %d mA", tempMaxVoltageData->_24v_res_currents);
      vol_detect_log("max pc = %d mA", tempMaxVoltageData->pc_currents);
      vol_detect_log("min vbat = %.2f V", tempMaxVoltageData->bat_voltage/100.0);
      vol_detect_log("max _24v = %d mV", tempMaxVoltageData->_24v_voltage);
      vol_detect_log("max _12v = %d mV", tempMaxVoltageData->_12v_voltage);
      vol_detect_log("max _5v = %d mV", tempMaxVoltageData->_5v_voltage);
      vol_detect_log("max air_temperature = %d C", tempMaxVoltageData->air_temperature);
      vol_detect_log("max _5v_regulator_temp = %d C", tempMaxVoltageData->_5v_regulator_temp);
      vol_detect_log("max _12v_regulator_temp = %d C", tempMaxVoltageData->_12v_regulator_temp);
      vol_detect_log("max _24v_regulator_temp = %d C", tempMaxVoltageData->_24v_regulator_temp);
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
    soc->charge_i_ma = 0;//voltageConvert->charge_currents;
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
          vol_detect_log("state_of_charge_mws: %u, soc_total_mws: %u, ischarging: %d", \
            soc->state_of_charge_mws, SOC_TOTAL_MWS, (boardStatus->sysStatus & (uint16_t)STATE_IS_CHARGING) ? 1:0 );
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
