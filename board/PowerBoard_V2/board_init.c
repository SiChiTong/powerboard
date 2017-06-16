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
#include "serial_leds.h"

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
  
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_IRLED_PWM, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_5V_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_MOTOR_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_SENSOR_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_LEDS_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_AIUI_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_5V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PAD_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_ROUTER_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_2_1_PA_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_DYP_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_X86_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_NV_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_DLP_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_12V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PRINTER_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_24V_RES_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_BAT_NV_EN, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_5V_ROUTER_EN, &pin_config );
  
//  pin_config.gpio_pull = GPIO_PULLDOWN;
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_NV, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_DLP, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_PAD, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_X86, &pin_config );
  MicoGpioInitialize( (mico_gpio_t)MICO_GPIO_PWR_RES, &pin_config );
  
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_IRLED_PWM );
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_5V_EN );  
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_12V_EN );
  MicoGpioOutputHigh( (mico_gpio_t)MICO_GPIO_24V_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_MOTOR_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_SENSOR_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_LEDS_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_AIUI_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_5V_RES_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PAD_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_12V_ROUTER_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_2_1_PA_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_DYP_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_X86_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_NV_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_DLP_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_12V_RES_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PRINTER_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_24V_RES_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_BAT_NV_EN );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_5V_ROUTER_EN );
  
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_NV );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_DLP );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_PAD );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_X86 );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_RES );
  
}

void board_adc_dma_init( void *buffer, uint32_t length )
{
  MicoAdcStreamInitializeEarly( MICO_ADC_5V_RES1,16 );
  MicoAdcStreamAddChannel( MICO_ADC_5V_RES1, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_12V_RES2, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_BAT_NV, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_12V_NV, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_ROUTER_12V, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_DYP, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_SENSOR, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_DLP, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_MOTOR, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_24V_RES1, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_2_1_PA, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_PAD, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_PRINTER, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_X86, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_IRLED, 235 );
  MicoAdcStreamAddChannel( MICO_ADC_LEDS, 235 );
  MicoAdcStreamInitializeLate( MICO_ADC_LEDS, buffer, length );
}
#if CHARGE_DETECT_USE_INT
#ifdef HW_V2_0
static void _charger_irq_handler( void* arg );
static void charger_detect_interrupt_cb( void );
#endif

#ifdef HW_V2_1
static void _charger_irq_handler( void* arg );
static void charger_detect_interrupt_cb( void );
static void _recharger_irq_handler( void* arg );
static void recharger_detect_interrupt_cb( void );
#endif

static void _charger_irq_handler( void* arg )
{
  (void)(arg);
  MicoGpioDisableIRQ( MICO_GPIO_CHARGE_IN );
  charger_detect_interrupt_cb();
  MicoGpioEnableIRQ( MICO_GPIO_CHARGE_IN , IRQ_TRIGGER_BOTH_EDGES, _charger_irq_handler, NULL);
}

#ifdef HW_V2_1
static void _recharger_irq_handler( void* arg )
{
  (void)(arg);
  MicoGpioDisableIRQ( MICO_GPIO_RECHARGE_IN );
  recharger_detect_interrupt_cb();
  MicoGpioEnableIRQ( MICO_GPIO_RECHARGE_IN , IRQ_TRIGGER_BOTH_EDGES, _recharger_irq_handler, NULL);
}
#endif

void charger_detect_init( void )
{
  platform_pin_config_t pin_config;
    //  Initialise charger
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_IT_RISING_FALLING;
  pin_config.gpio_pull = GPIO_PULLDOWN;

#ifdef HW_V2_0
  MicoGpioInitialize( MICO_GPIO_ADAPTER_IN, &pin_config );
  MicoGpioEnableIRQ( MICO_GPIO_ADAPTER_IN , IRQ_TRIGGER_BOTH_EDGES, _charger_irq_handler, NULL);
#endif
  
#ifdef HW_V2_1
  MicoGpioInitialize( MICO_GPIO_CHARGE_IN, &pin_config );
  MicoGpioInitialize( MICO_GPIO_RECHARGE_IN, &pin_config );
  HAL_Delay(1);//need have a delay to ensure correct io register config
  MicoGpioEnableIRQ( MICO_GPIO_CHARGE_IN , IRQ_TRIGGER_BOTH_EDGES, _charger_irq_handler, NULL);
  MicoGpioEnableIRQ( MICO_GPIO_RECHARGE_IN , IRQ_TRIGGER_BOTH_EDGES, _recharger_irq_handler, NULL);
#endif
}

#ifdef HW_V2_0
static void charger_detect_interrupt_cb( void )
{
  if( MicoGpioInputGet( MICO_GPIO_ADAPTER_IN ) )
  {
    setCurLedsMode( LIGHTS_MODE_IS_CHARGING );//called before the following line
    boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGER_IN;
    board_log("charger pin is high");
#ifdef MIKE_TEST
    boardStatus->charger_times += 1;
#endif
  }
  else
  {
    boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGER_IN);
    board_log("charger pin is low");
  } 
}
#endif

#ifdef HW_V2_1

static bool  previous_charge_io_state = 0;
static bool  previous_recharge_io_state = 0;
static uint32_t previous_charge_time = 0;
static uint32_t previous_recharge_time = 0;

static void charger_detect_interrupt_cb( void )
{
  if( os_get_time() - previous_charge_time < 50/SYSTICK_PERIOD || \
      previous_charge_io_state == MicoGpioInputGet( MICO_GPIO_CHARGE_IN ) )
  {
    goto exit;
  }
  if( previous_charge_io_state = MicoGpioInputGet( MICO_GPIO_CHARGE_IN ) )
  {    
    boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGER_IN;
    board_log("charger pin is high");
#ifdef MIKE_TEST
    boardStatus->charger_times += 1;
#endif
  }
  else
  {
    boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGER_IN);
    board_log("charger pin is low");
  }
  //previous_charge_io_state = MicoGpioInputGet( MICO_GPIO_CHARGE_IN );
exit:
  previous_charge_time = os_get_time();
}

static void recharger_detect_interrupt_cb( void )
{
  if( os_get_time() - previous_recharge_time < 50/SYSTICK_PERIOD || \
      previous_recharge_io_state == MicoGpioInputGet( MICO_GPIO_RECHARGE_IN ) )
  {
    goto exit;
  }
  if( previous_recharge_io_state = MicoGpioInputGet( MICO_GPIO_RECHARGE_IN ) )
  {   
    boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGER_IN;
    board_log("recharger pin is high");
#ifdef MIKE_TEST
    boardStatus->charger_times += 1;
#endif
  }
  else
  {
    boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGER_IN);
    board_log("recharger pin is low");
  } 
  //previous_recharge_io_state = MicoGpioInputGet( MICO_GPIO_RECHARGE_IN );
exit:
  previous_recharge_time = os_get_time();
}
#endif

#else
void charger_detect_init( void )
{
  platform_pin_config_t pin_config;
    //  Initialise charger
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_INPUT;
  pin_config.gpio_pull = GPIO_PULLDOWN;

#ifdef HW_V2_0
  MicoGpioInitialize( MICO_GPIO_ADAPTER_IN, &pin_config );
  MicoGpioEnableIRQ( MICO_GPIO_ADAPTER_IN , IRQ_TRIGGER_BOTH_EDGES, _charger_irq_handler, NULL);
#endif
  
#ifdef HW_V2_1
  MicoGpioInitialize( MICO_GPIO_CHARGE_IN, &pin_config );
  MicoGpioInitialize( MICO_GPIO_RECHARGE_IN, &pin_config );
#endif
}
static bool  previous_charge_io_state = 0;
static bool  previous_recharge_io_state = 0;

void charger_detect_period( void )
{
  bool current_charge_io_state = MicoGpioInputGet( MICO_GPIO_CHARGE_IN );
  bool current_recharge_io_state = MicoGpioInputGet( MICO_GPIO_RECHARGE_IN );
  
  if( previous_charge_io_state != current_charge_io_state )
  {
    previous_charge_io_state = current_charge_io_state;
    if( current_charge_io_state )
    {
      boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGER_IN;
      board_log("charger pin is high");
    }
    else
    {
      if( !current_recharge_io_state )
      {
        boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGER_IN);
      }
      board_log("charger pin is low");
    }
  }
  if( previous_recharge_io_state != current_recharge_io_state )
  {
    previous_recharge_io_state = current_recharge_io_state;
    if( current_recharge_io_state )
    {
      boardStatus->sysStatus |= (uint16_t)STATE_IS_CHARGER_IN;
      board_log("recharger pin is high");
    }
    else
    {
      if( !current_charge_io_state )
      {
        boardStatus->sysStatus &= ~((uint16_t)STATE_IS_CHARGER_IN);
      }
      board_log("recharger pin is low");
    }
  }
}
#endif //#if CHARGE_DETECT_USE_INT
