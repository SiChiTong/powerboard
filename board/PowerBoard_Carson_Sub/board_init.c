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
   
   /////////////////////////////////////////////
    MicoGpioInitialize( (mico_gpio_t)GPIO_LED1, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_LED2, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_LED3, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_LED4, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_LEFT_HAND_MOTOR_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_RIGHT_HAND_MOTOR_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_HEAD_MOTOR_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_LEDX2_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_REPAIR_BOARD_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_TOUCH_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_KEYBORAD_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_CARD_READER_EN, &pin_config );// new

    MicoGpioInitialize( (mico_gpio_t)GPIO_5V_CODE_EN, &pin_config );// new

MicoGpioInitialize( (mico_gpio_t)GPIO_5V_HD_CAMERA_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_HD_CAMERA_STOP_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_CAMERA_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_CAMERA_STOP_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_HEAD_TOUCH_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_RESERVE_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_5V_HEAD_LED_EN, &pin_config );// new



    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_SWITCH_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_ROUTER_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_HEAD_RK_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_CARD_READER_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_CHEST_RK_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_AUDIO_PA_EN, &pin_config );// new
  
  
MicoGpioInitialize( (mico_gpio_t)GPIO_12V_CARD_READER_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_12V_AUDIO_PA_EN, &pin_config );// new

  
    
    MicoGpioInitialize( (mico_gpio_t)GPIO_24V_RIGHT_HAND_MOTOR_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_24V_LEFT_HAND_MOTOR_EN, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_24V_HEAD_MOTOR_EN, &pin_config );// new
  
MicoGpioInitialize( (mico_gpio_t)GPIO_24V_HD_CAMERA_EN, &pin_config );// new
MicoGpioInitialize( (mico_gpio_t)GPIO_24V_CAMERA_EN, &pin_config );// new
  
  

  
  
    
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_HEAD_RK_SIGNAL, &pin_config );// new
    MicoGpioInitialize( (mico_gpio_t)GPIO_12V_CHEST_RK_SIGNAL, &pin_config );// new
///////////////////////////////////////////////////////////////////
  
//  pin_config.gpio_pull = GPIO_PULLDOWN;
    /*
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
  //MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_FAN_CTRL );   // MICO_GPIO_FAN_CTRL has the same pin with the new board
  */


///////////////////////////////////////////////////////
    
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_LED1 );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_LED2 );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_LED3 );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_LED4 );// new
    
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_EN );// new  
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_EN );// new
    
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_24V_LEFT_HAND_MOTOR_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_24V_RIGHT_HAND_MOTOR_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_24V_HEAD_MOTOR_EN );// new
    
MicoGpioOutputHigh( (mico_gpio_t)GPIO_24V_HD_CAMERA_EN );// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_24V_CAMERA_EN );// new

    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_LEFT_HAND_MOTOR_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_RIGHT_HAND_MOTOR_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_HEAD_MOTOR_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_LEDX2_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_REPAIR_BOARD_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_TOUCH_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_KEYBORAD_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_CARD_READER_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_CODE_EN );// new
    
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_HD_CAMERA_EN );// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_HD_CAMERA_STOP_EN);// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_CAMERA_EN);// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_CAMERA_STOP_EN);// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_HEAD_TOUCH_EN);// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_RESERVE_EN );// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_5V_HEAD_LED_EN);// new


    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_SWITCH_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_ROUTER_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_HEAD_RK_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_CHEST_RK_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_CARD_READER_EN );// new
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_AUDIO_PA_EN );// new
    
MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_CARD_READER_EN);// new
MicoGpioOutputHigh( (mico_gpio_t)GPIO_12V_AUDIO_PA_EN );// new
    
    MicoGpioOutputLow( (mico_gpio_t)GPIO_12V_HEAD_RK_SIGNAL );// new
    MicoGpioOutputLow( (mico_gpio_t)GPIO_12V_CHEST_RK_SIGNAL );// new
///////////////////////////////////////////////////////////////////////////
  
    /*
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_NV );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_DLP );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_PAD );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_X86 );
  MicoGpioOutputLow( (mico_gpio_t)MICO_GPIO_PWR_RES );
  */
}

#if 0
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
#endif


#if 0
static void _charger_irq_handler( void* arg );
static void charger_detect_interrupt_cb( void );

static void _charger_irq_handler( void* arg )
{
  (void)(arg);
  charger_detect_interrupt_cb();
}
void charger_detect_init( void )
{
  platform_pin_config_t pin_config;
    //  Initialise charger
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_IT_RISING_FALLING;
  pin_config.gpio_pull = GPIO_PULLDOWN;
  MicoGpioInitialize( MICO_GPIO_ADAPTER_IN, &pin_config );
  MicoGpioEnableIRQ( MICO_GPIO_ADAPTER_IN , IRQ_TRIGGER_BOTH_EDGES, _charger_irq_handler, NULL);
}
static void charger_detect_interrupt_cb( void )
{
  if( MicoGpioInputGet( MICO_GPIO_ADAPTER_IN ) )
  {
    //setCurLedsMode( LIGHTS_MODE_IS_CHARGING );//called before the following line
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
