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

#define   ADC3_DMA_CHANNELS_NUM  (5)
#define   FILTER_TIMES          (5)
#define   ADC3_DMA_BURRER_SIZE   (ADC3_DMA_CHANNELS_NUM*FILTER_TIMES)

extern const platform_adc_t             platform_adc_peripherals[];
__IO uint16_t adc1_dma_buffer[ADC1_DMA_BURRER_SIZE];
__IO uint16_t adc3_dma_buffer[ADC3_DMA_BURRER_SIZE];

void analog_multiplexer_init( void )
{
  platform_pin_config_t pin_config;
  
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_OUTPUT_PP;
  pin_config.gpio_pull = GPIO_PULLUP;
  
  MicoGpioInitialize( (mico_gpio_t)GPIO_SWITCH_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)GPIO_SWITCH_SEL0, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)GPIO_SWITCH_SEL1, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)GPIO_SWITCH_SEL2, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)GPIO_SWITCH_SEL3, &pin_config );

  MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_EN );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
}


OSStatus select_multi_channel( mico_adc_t adc )
{
  OSStatus err = kNoErr;
  if ( adc >= ADC_MAX || adc <  ADC_SWITCH )
    return kUnsupportedErr;
  switch( adc )
  {
    /*
  case MICO_ADC_24V_TS:
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
    */
  case ADC_12V_TS:             // 1
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_5V_TS:              // 2
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_AIR_TS:             // 3
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_HEAD_TOUCH:    // 4
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_12V_ALL:          // 5
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_ALL:           // 6
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_HEAD_LED:      // 7
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break; 
  case ADC_V_12V:              // 8
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_V_5V:               // 9
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_V_BAT:              // 10
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_RESERVE:       // 11
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_LED:           // 12
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_C_5V_CAMERA:        // 13
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  case ADC_5V_HD_CAMERA:       // 14
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
    break;
  default:
    break;
  } 
  return err;
}

#define ADC1_USED_CHANNELELS    (16)
#define ADC3_USED_CHANNELELS    (5)
#define SAMPLE_CYCLE            (235)

OSStatus multi_channel_adc_dma_init( void )
{
  OSStatus err = kNoErr;
  
  err = MicoAdcStreamInitializeEarly( ADC_C_5V_RIGHT_HAND_MOTOR, ADC1_USED_CHANNELELS );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamAddChannel( ADC_C_5V_RIGHT_HAND_MOTOR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( ADC_C_24V_HD_CAMERA, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );    
  err = MicoAdcStreamAddChannel( ADC_C_24V_HEAD_MOTOR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_24V_CAMERA, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_KEYBOARD, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_CODE, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_LEFT_HAND_MOTOR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_CARD_READER, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_HEAD, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_24V_RIGHT_HAND_MOTOR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );  
  err = MicoAdcStreamAddChannel( ADC_C_HEAD_RK, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );  
  err = MicoAdcStreamAddChannel( ADC_C_12V_CHEST_RK, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );  
  err = MicoAdcStreamAddChannel( ADC_C_24V_LEFT_HAND_MOTOR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );    
  err = MicoAdcStreamAddChannel( ADC_C_12V_AUDIO_PA, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );   
  err = MicoAdcStreamAddChannel( ADC_C_5V_REPAIR, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( ADC_C_5V_TOUCH, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  
    
  select_multi_channel( ADC_12V_TS );
  err = MicoAdcStreamInitializeLate( ADC_C_5V_TOUCH, (void *)adc1_dma_buffer, ADC1_DMA_BURRER_SIZE );
  require_noerr_quiet( err, exit );
/*----------------------------------------------------------------------------*/  
  err = MicoAdcStreamInitializeEarly( ADC_C_12V_SWITCH, ADC3_USED_CHANNELELS );
  require_noerr_quiet( err, exit );  
  err = MicoAdcStreamAddChannel( ADC_C_12V_SWITCH, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( ADC_C_12V_ROUTER, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );  
  err = MicoAdcStreamAddChannel( ADC_C_VBUS, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( ADC_C_12V_CARD_READER, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  err = MicoAdcStreamAddChannel( ADC_SWITCH, SAMPLE_CYCLE );
  require_noerr_quiet( err, exit );
  
  err = MicoAdcStreamInitializeLate( ADC_SWITCH, (void *)adc3_dma_buffer, ADC3_DMA_BURRER_SIZE );
  require_noerr_quiet( err, exit );
exit:
  return err;
}

uint16_t  read_channel_values_mV( mico_adc_t adc )
{
  uint32_t      temp_total = 0;
  
  if( adc >= ADC_MAX )
    return 0;
  if ( /*adc >= MICO_ADC_5V_RES1 && */adc <= ADC_C_5V_TOUCH )
  {
    for( uint8_t i = 0; i < FILTER_TIMES; i++ )
    {
      temp_total += (uint16_t)adc1_dma_buffer[platform_adc_peripherals[adc].rank - 1 + i * ADC1_DMA_CHANNELS_NUM ];
    }
    temp_total /= FILTER_TIMES;
    return (uint16_t)(temp_total/4096.0*3.3*1000);
  }
  else if ( adc > ADC_SWITCH && adc <=  ADC_5V_HD_CAMERA )
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
    for( uint8_t i = 0; i < FILTER_TIMES; i++ )
    {
      temp_total += (uint16_t)adc3_dma_buffer[platform_adc_peripherals[adc].rank - 1 + i * ADC3_DMA_CHANNELS_NUM ];
    }
    temp_total /= FILTER_TIMES;
    return (uint16_t)(temp_total/4096.0*3.3*1000);
  }
}

#define FIXIT_FACTOR    (5)
#define FIXIT_FACTOR_VOL    (1)
#define FIXIT_FACTOR_CUR     (50)
#if 0
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