/**
 ******************************************************************************
 * @file    board_init.c
 * @author  Adam Huang
 * @version V1.0.0
 * @date    26-Nov-2016
 * @brief   
 ******************************************************************************
*/
#include "board_init.h"
#include "platform.h"
#include "mico_platform.h"
#include "app_platform.h"

#define board_log(M, ...) custom_log("Board", M, ##__VA_ARGS__)
#define board_log_trace() custom_log_trace("Board")

extern const platform_gpio_t            platform_gpio_pins[];
extern const platform_adc_t             platform_adc_peripherals[];

void board_gpios_init( void )
{
  platform_pin_config_t pin_config;
  
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_OUTPUT_PP;
  pin_config.gpio_pull = GPIO_PULLUP;
  
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_5V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_ICCARD_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_PRINTER_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_PC_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_BAT_NV_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_5V_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_EN, &pin_config );

  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_RESERVE2, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_SUB_PWR_KEY, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_SUB_PWR_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_RUN_LED, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_SHUTDOWN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_RESERVE1, &pin_config );
//  pin_config.gpio_pull = GPIO_PULLDOWN;
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_NV, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_PC, &pin_config );

  
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_5V_EN );  
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_12V_EN );
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_24V_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_5V_RES_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_24V_ICCARD_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_24V_RES_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_24V_PRINTER_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_12V_PC_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_BAT_NV_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_12V_RES_EN );

  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_RESERVE2 );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_SUB_PWR_KEY );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_SUB_PWR_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_RUN_LED );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_SHUTDOWN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_RESERVE1 );
  
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_NV );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_PC );
}

void board_adc_dma_init( void *buffer, uint32_t length )
{
  MicoAdcStreamInitializeEarly( MICO_ADC_I_NV,16 );
  MicoAdcStreamAddChannel( MICO_ADC_I_NV, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_12V_RES, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_LEDS, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_5V_RES, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_24V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_12V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_5V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_VBUS, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_V_12V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_V_5V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_ICCARD, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_24V_RES, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_PRINTER, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_I_PC, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_V_BAT, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_V_24V, 235 );
  MicoAdcStreamInitializeLate( MICO_ADC_V_24V, buffer, length );
}
