/**
 ******************************************************************************
 * @file    multi_channel_detection.c
 * @author  Adam Huang
 * @version V1.0.0
 * @date    29-Nov-2016
 * @brief   
 ******************************************************************************
*/

#include "multi_channel_detection.h"

#define   ADC1_DMA_CHANNELS_NUM  (16)
#define   FILTER_TIMES          (5)
#define   ADC1_DMA_BURRER_SIZE   (ADC1_DMA_CHANNELS_NUM*FILTER_TIMES)

#define   ADC3_DMA_CHANNELS_NUM  (4)
#define   FILTER_TIMES          (5)
#define   ADC3_DMA_BURRER_SIZE   (ADC3_DMA_CHANNELS_NUM*FILTER_TIMES)

extern const platform_adc_t             platform_adc_peripherals[];
__IO uint16_t adc1_dma_buffer[ADC1_DMA_BURRER_SIZE];
__IO uint16_t adc3_dma_buffer[ADC3_DMA_BURRER_SIZE];


#define ADC1_USED_CHANNELELS    (16)
#define ADC3_USED_CHANNELELS    (4)
#define SAMPLE_CYCLE            (235)

OSStatus multi_channel_adc_dma_init( void )
{
  OSStatus err = kNoErr;
  
  err = MicoAdcStreamInitializeEarly( MICO_ADC_I_NV, ADC1_USED_CHANNELELS );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamAddChannel( MICO_ADC_I_NV, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_12V_RES, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_LEDS, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_5V_RES, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_24V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_12V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_5V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_VBUS, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_V_12V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_V_5V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_ICCARD, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_24V_RES, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_PRINTER, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_I_PC, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_V_BAT, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_V_24V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamInitializeLate( MICO_ADC_V_24V, (void *)adc1_dma_buffer, ADC1_DMA_BURRER_SIZE );
  require_noerr_quiet( err, exit );
/*----------------------------------------------------------------------------*/  
  err = MicoAdcStreamInitializeEarly( MICO_ADC_TS_AIR, ADC3_USED_CHANNELELS );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamAddChannel( MICO_ADC_TS_AIR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_TS_24V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_TS_12V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( MICO_ADC_TS_5V, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamInitializeLate( MICO_ADC_TS_5V, (void *)adc3_dma_buffer, ADC3_DMA_BURRER_SIZE );
  require_noerr_quiet( err, exit );
exit:
  return err;
}

uint16_t  read_channel_values_mV( mico_adc_t adc )
{
  uint32_t      temp_total = 0;
  
  if( adc >= MICO_ADC_MAX )
    return 0;
  if ( /*adc >= MICO_ADC_I_NV && */adc <= MICO_ADC_V_24V )
  {
    for( uint8_t i = 0; i < FILTER_TIMES; i++ )
    {
      temp_total += (uint16_t)adc1_dma_buffer[platform_adc_peripherals[adc].rank - 1 + i * ADC1_DMA_CHANNELS_NUM ];
    }
    temp_total /= FILTER_TIMES;
    return (uint16_t)(temp_total/4096.0*3.3*1000);
  }
  else if ( adc <=  MICO_ADC_TS_5V )
  {    
    for( uint8_t i = 0; i < FILTER_TIMES; i++ )
    {
      temp_total += (uint16_t)adc3_dma_buffer[ADC3_USED_CHANNELELS - 1 + i * ADC3_DMA_CHANNELS_NUM ];
    }
    temp_total /= FILTER_TIMES;
    return (uint16_t)(temp_total/4096.0*3.3*1000);
  }
  else
  {
    printf("read channel values err\r\n");
    return 0;
  }
}
#if 0
#define FIXIT_FACTOR    (5)
#define FIXIT_FACTOR_VOL    (1)
#define FIXIT_FACTOR_CUR     (50)
void read_multi_convert_values( void )
{  
  printf("\r\n");
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_5V_RES1 )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_12V_RES2 )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_BAT_NV )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_12V_NV )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_ROUTER_12V )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_DYP )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_SENSOR )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_DLP )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_MOTOR )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_24V_RES1 )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_2_1_PA )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_PAD )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_PRINTER )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_X86 )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_IRLED )* FIXIT_FACTOR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_LEDS )* FIXIT_FACTOR));
#if 1
  printf("follows are adc3 channels\r\n");
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_CHARGE )* FIXIT_FACTOR_CUR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_BATIN )* FIXIT_FACTOR_CUR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_VBUS )* FIXIT_FACTOR_CUR));
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_BAT_MOTOR )* FIXIT_FACTOR_CUR));
  select_multi_channel( MICO_ADC_24V_TS );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_24V_TS )* FIXIT_FACTOR_VOL));
  select_multi_channel( MICO_ADC_12V_TS );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_12V_TS )* FIXIT_FACTOR_VOL));
  select_multi_channel( MICO_ADC_5V_TS );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_5V_TS )* FIXIT_FACTOR_VOL));
  select_multi_channel( MICO_ADC_AIR_TS );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_AIR_TS )* FIXIT_FACTOR_VOL));
  select_multi_channel( MICO_ADC_24V_ALL );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_24V_ALL )* FIXIT_FACTOR));
  select_multi_channel( MICO_ADC_12V_ALL );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_12V_ALL )* FIXIT_FACTOR));
  select_multi_channel( MICO_ADC_5V_ALL );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_5V_ALL )* FIXIT_FACTOR));
  select_multi_channel( MICO_ADC_VDET_24V );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_VDET_24V )* 11));
  select_multi_channel( MICO_ADC_VDET_12V );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_VDET_12V )* 11));
  select_multi_channel( MICO_ADC_VDET_5V );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_VDET_5V )* 2));
  select_multi_channel( MICO_ADC_VDET_BAT );
  HAL_Delay(1);
  printf("%d\t",(uint16_t)(read_channel_values_mV( MICO_ADC_VDET_BAT )* 11));
#endif
}
#endif