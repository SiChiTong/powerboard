#include "led_ctrl.h"

#include "platform.h"
#include "platform_peripheral.h"
#include "platform_logging.h"
#include "app_platform.h"
#include "mico.h"
/////////  test code //////////
void LedTick(void)
{
    
}

#define TEST_PERIOD1     2000/SYSTICK_PERIOD
#define TEST_PERIOD2     5000/SYSTICK_PERIOD
void HardWareTestTick(void)
{
    static uint32_t start_time1 = 0;
    static uint32_t start_time2 = 0;
    if(os_get_time() - start_time1 >= TEST_PERIOD1)
    {
        start_time1 = os_get_time();
        MicoGpioOutputTrigger( GPIO_LED1 );
        MicoGpioOutputTrigger( GPIO_LED2 );
        MicoGpioOutputTrigger( GPIO_LED3 );
        MicoGpioOutputTrigger( GPIO_LED4 ); 

        MicoGpioOutputTrigger( GPIO_24V_RIGHT_HAND_MOTOR_EN );
        MicoGpioOutputTrigger( GPIO_24V_HEAD_MOTOR_EN );
        MicoGpioOutputTrigger( GPIO_24V_HEAD_MOTOR_EN );

        MicoGpioOutputTrigger(GPIO_24V_HD_CAMERA_EN);
        MicoGpioOutputTrigger(GPIO_24V_CAMERA_EN);

          
        MicoGpioOutputTrigger( GPIO_12V_SWITCH_EN );
        MicoGpioOutputTrigger( GPIO_12V_ROUTER_EN );
        MicoGpioOutputTrigger( GPIO_12V_HEAD_RK_EN );
        MicoGpioOutputTrigger( GPIO_12V_CHEST_RK_EN ); 
        MicoGpioOutputTrigger( GPIO_12V_CARD_READER_EN );
        MicoGpioOutputTrigger( GPIO_12V_AUDIO_PA_EN );

        //MicoGpioOutputTrigger(GPIO_12V_CHEST_RK_SIGNAL);
        //MicoGpioOutputTrigger(GPIO_12V_HEAD_RK_SIGNAL);

  
        MicoGpioOutputTrigger( GPIO_5V_LEFT_HAND_MOTOR_EN ); 
        MicoGpioOutputTrigger( GPIO_5V_RIGHT_HAND_MOTOR_EN );
        MicoGpioOutputTrigger( GPIO_5V_HEAD_MOTOR_EN );
        MicoGpioOutputTrigger( GPIO_5V_LEDX2_EN );
        MicoGpioOutputTrigger( GPIO_5V_REPAIR_BOARD_EN ); 
        MicoGpioOutputTrigger( GPIO_5V_TOUCH_EN );
        MicoGpioOutputTrigger( GPIO_5V_KEYBORAD_EN );
        MicoGpioOutputTrigger( GPIO_5V_CARD_READER_EN );
        MicoGpioOutputTrigger( GPIO_5V_CODE_EN ); 
        
        MicoGpioOutputTrigger(GPIO_5V_RESERVE_EN);
        MicoGpioOutputTrigger(GPIO_5V_HEAD_LED_EN);
        MicoGpioOutputTrigger(GPIO_5V_HD_CAMERA_EN);
        MicoGpioOutputTrigger(GPIO_5V_HD_CAMERA_STOP_EN);
        MicoGpioOutputTrigger(GPIO_5V_CAMERA_EN);
        MicoGpioOutputTrigger(GPIO_5V_CAMERA_STOP_EN);
        MicoGpioOutputTrigger(GPIO_5V_HEAD_TOUCH_EN);
    }
    if(os_get_time() - start_time2 >= TEST_PERIOD2)
    {
        start_time2 = os_get_time();
        //MicoGpioOutputTrigger( GPIO_12V_CHEST_RK_SIGNAL );
        //MicoGpioOutputTrigger( GPIO_12V_HEAD_RK_SIGNAL );    
    }
}
///////////////////////////////////////////////////////////////


PWM_LED_Typedef pwm_led_ctrl[] = 
{
  [PWM_LED1]   = { 10000, 50, OFF},
  [PWM_LED2]   = { 10000, 50, OFF},
};

void LedPwmInit(void)
{
    MicoPwmInitialize( PWM_LED1, pwm_led_ctrl[PWM_LED1].frq, pwm_led_ctrl[PWM_LED1].pecent );
    MicoPwmInitialize( PWM_LED2, pwm_led_ctrl[PWM_LED2].frq, pwm_led_ctrl[PWM_LED2].pecent );
}

uint8_t SetLedBrightness(LED_TypeDef led, uint8_t brightness)
{
    if(brightness > 100)
    {
        brightness = 100;
    }
    if(led == LED1)
    {
        MicoPwmSetDuty( PWM_LED1, brightness );
        pwm_led_ctrl[PWM_LED1].pecent = brightness;
        return brightness;
    }
    if(led == LED2)
    {
        MicoPwmSetDuty( PWM_LED2, brightness );
        pwm_led_ctrl[PWM_LED2].pecent = brightness;
        return brightness;
    }
    return 0xff;
}

uint8_t GetLedBrightness(LED_TypeDef led)
{
    if(led == LED1)
    {      
        return pwm_led_ctrl[PWM_LED1].pecent;
    }
    if(led == LED2)
    {
        
         return pwm_led_ctrl[PWM_LED2].pecent;
    }
    return 0xff;
}


uint32_t sys_led_count = 0;
void SysLed(void)
{
  sys_led_count ++;
  if( sys_led_count > 50 )
  {
    sys_led_count = 0;
    MicoGpioOutputTrigger( GPIO_SYS_LED );
  }
}