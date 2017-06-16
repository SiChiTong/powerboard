/**
******************************************************************************
* @file    platform.h
* @author  William Xu
* @version V1.0.0
* @date    05-May-2014
* @brief   This file provides all MICO Peripherals defined for current platform.
******************************************************************************
*
*  The MIT License
*  Copyright (c) 2014 MXCHIP Inc.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy 
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights 
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is furnished
*  to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
*  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************
*/ 



#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
   
/******************************************************
 *                   Enumerations
 ******************************************************/

   
   #if 1
#define asm            __asm
#define delay_300ns()     do {asm("nop");asm("nop");asm("nop");asm("nop");\
                              asm("nop");asm("nop");asm("nop");asm("nop");\
                              asm("nop");asm("nop");asm("nop");asm("nop");\
                              asm("nop");asm("nop");asm("nop");asm("nop");\
                              asm("nop");asm("nop");asm("nop");asm("nop");\
                              asm("nop");asm("nop");} while(1==0)
                                
#define delay_600ns()     do { asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");asm("nop");asm("nop");\
                               asm("nop");asm("nop");} while(1==0)
                                
#define delay_us(n)       do { for(uint32_t i=0;i<n;i++){delay_300ns();delay_600ns();}\
                                } while(0==1)
//#else

#define delay_ms          HAL_Delay
#endif
   
   
/*
POWERBOARD V1.2 platform pin definitions ...
+-------------------------------------------------------------------------+
| Enum ID       |Pin | STM32| Peripheral  |    Board     |   Peripheral   |
|               | #  | Port | Available   |  Connection  |     Alias      |
|---------------+----+------+-------------+--------------+----------------|
|               | 1  | VCC  |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 2  | C 13 |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 3  | C 14 |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 4  | C 15 |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 5  | D  0 |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 6  | D  1 |             |              |                |
+---------------+----+--------------------+--------------+----------------+
|               | 7  | NRST |             |              |                |
+---------------+----+--------------------+--------------+----------------+
| MICO_UART1_TX | 29 | B 10 | USART3_TX   |              | MICO_UART_3_TX |
|               |    |      | GPIO        |              |                |
|               |    |      | I2C2_SCL    |              |                |
|---------------+----+------+-------------+--------------+----------------|
| MICO_UART1_RX | 30 | B 11 | USART3_RX   |              | MICO_UART_3_RX |
|               |    |      | GPIO        |              |                |
|               |    |      | I2C2_SDA    |              |                |
|---------------+----+------+-------------+--------------+----------------|
| MICO_UART2_TX | 42 | A  9 | USART1_TX   |              | MICO_UART_1_TX |
|               |    |      | GPIO        |              |                |
|---------------+----+------+-------------+--------------+----------------|
| MICO_UART2_RX | 43 | B 10 | USART1_RX   |              | MICO_UART_1_RX |
|               |    |      | GPIO        |              |                |
|---------------+----+------+-------------+--------------+----------------|
| MICO_SYS_LED  | 40 | C  9 | GPIO        |              |                |
+---------------+----+--------------------+--------------+----------------+
| MICO_SWITCH   | 37 | C  6 | GPIO        |              |                |
+---------------+----+--------------------+--------------+----------------+
| MICO_GPIO_1   | 8  | C  0 | ADC12_IN10  |              |  ADC_2.1_PA    |
|               |    |      | GPIO        |              |                |
|---------------+----+------+-------------+--------------+----------------|
| MICO_GPIO_2   | 9  | C  1 | ADC12_IN11  |              |     ADC_PAD    |
|               |    |      | GPIO        |              |                |
|               |    |      |             |              |                |
+---------------+----+------+-------------+--------------+----------------+
*
*/
  
#define MICO_UNUSED 0xFF

typedef enum
{
#if 1
    
    
    MICO_GPIO_MOTOR_EN,
    MICO_GPIO_SENSOR_EN,
    MICO_GPIO_LEDS_EN,
    MICO_GPIO_AIUI_EN,
    
    MICO_GPIO_PAD_EN,

    MICO_GPIO_2_1_PA_EN,
    MICO_GPIO_DYP_EN,
    MICO_GPIO_X86_EN,
    MICO_GPIO_NV_EN,
    MICO_GPIO_DLP_EN,
    MICO_GPIO_12V_RES_EN,
    MICO_GPIO_PRINTER_EN,
    MICO_GPIO_24V_RES_EN,
    MICO_GPIO_BAT_NV_EN,
    MICO_GPIO_5V_ROUTER_EN,
    
    
    MICO_GPIO_24V_EN,

    MICO_GPIO_PWR_NV,
    MICO_GPIO_PWR_DLP,
    MICO_GPIO_PWR_PAD,
    MICO_GPIO_PWR_X86,//not used
    MICO_GPIO_PWR_RES,
  
    MICO_GPIO_UART3_TX,
    MICO_GPIO_UART3_RX,
  
    MICO_GPIO_LED_PWM,
    //MICO_GPIO_ADAPTER_IN,
    MICO_GPIO_FAN_CTRL,

    MICO_GPIO_RECHARGE_LED,    
    MICO_GPIO_IRLED_PWM,
    MICO_GPIO_CAN_RX,
    MICO_GPIO_CAN_TX,
    MICO_GPIO_CAN_STB,
  
    MICO_GPIO_SPI_NSS,
    MICO_GPIO_SPI_SCK,
    MICO_GPIO_SPI_MISO,
    MICO_GPIO_SPI_MOSI,
  
    MICO_GPIO_I2C_SCL,
    MICO_GPIO_I2C_SDA,
  
    MICO_GPIO_UART2_TX,
    MICO_GPIO_UART2_RX,
  
    MICO_GPIO_UART1_TX,
    MICO_GPIO_UART1_RX,
  
    MICO_GPIO_ID1,
    MICO_GPIO_ID2,

    
    ///////////////////////////////////////
    //ADC1
    GPIO_5V_RIGHT_HAND_MOTOR_C_ADC,
    GPIO_24V_HD_CAMERE_C_ADC,
    GPIO_24V_HEAD_MOTOR_C_ADC,
    GPIO_24V_CAMERE_C_ADC,
    GPIO_5V_KEYBOARD_C_ADC,
    GPIO_5V_CODE_C_ADC,
    GPIO_5V_LEFTHAND_MOTOR_C_ADC,
    GPIO_5V_CARD_READER_C_ADC,
    GPIO_5V_HEAD_C_ADC,
    GPIO_24V_RIGHT_HAND_MOTOR_C_ADC,
    GPIO_12V_HEAD_RK_C_ADC,
    GPIO_12V_CHEST_RK_C_ADC,
    GPIO_24V_LEFT_HAND_MOTOR_C_ADC,
    GPIO_12V_AUDIO_PA_C_ADC,
    GPIO_5V_REPAIR_C_ADC,
    GPIO_5V_TOUCH_C_ADC,
  
    //ADC3
    GPIO_12V_SWITCH_C_ADC,
    GPIO_12V_ROUTER_C_ADC,
    GPIO_VBUS_C_ADC,
    GPIO_12V_CARD_READER_C_ADC,
    GPIO_SWITCH_C_V_ADC,
      
    
    GPIO_SYS_LED,
    
    GPIO_LED4,// new
    GPIO_LED3,// new
    GPIO_LED2,// new
    GPIO_LED1,// new
    
    GPIO_PWM_LED1,
    GPIO_PWM_LED2,
    
    
    
    GPIO_SWITCH_EN,  // Previous OK
    GPIO_SWITCH_SEL0,// Previous OK
    GPIO_SWITCH_SEL1,// Previous OK
    GPIO_SWITCH_SEL2,// Previous OK
    GPIO_SWITCH_SEL3,// Previous OK
    GPIO_SWITCH_ADC,
    
    
    GPIO_24V_LEFT_HAND_MOTOR_EN,  // new
    GPIO_24V_RIGHT_HAND_MOTOR_EN, // new
    GPIO_24V_HEAD_MOTOR_EN,      // new
    GPIO_24V_HD_CAMERA_EN,
    GPIO_24V_CAMERA_EN,
    
    GPIO_5V_EN,                // Previous OK
    GPIO_5V_RESERVE_EN,
    GPIO_12V_EN,
    GPIO_5V_LEFT_HAND_MOTOR_EN,       // new
    GPIO_5V_RIGHT_HAND_MOTOR_EN,      // new
    GPIO_5V_HEAD_MOTOR_EN,           // new
    GPIO_5V_LEDX2_EN,          // new
    GPIO_5V_HEAD_LED_EN,
    GPIO_5V_REPAIR_BOARD_EN,   // new
    GPIO_5V_TOUCH_EN,          // new
    GPIO_5V_KEYBORAD_EN,       // new
    GPIO_5V_CARD_READER_EN,         // new
    GPIO_5V_CODE_EN,           // new
    
    GPIO_5V_HD_CAMERA_EN,
    GPIO_5V_HD_CAMERA_STOP_EN,
    GPIO_5V_CAMERA_EN,
    GPIO_5V_CAMERA_STOP_EN,
    GPIO_5V_HEAD_TOUCH_EN,
    
    GPIO_12V_SWITCH_EN,// new
    GPIO_12V_ROUTER_EN,// new
    GPIO_12V_CARD_READER_EN,// new
    GPIO_12V_AUDIO_PA_EN,// new
    
    GPIO_12V_HEAD_RK_EN,// new
    GPIO_12V_HEAD_RK_SIGNAL,
    
    GPIO_12V_CHEST_RK_EN,// new
    GPIO_12V_CHEST_RK_SIGNAL,
    
    GPIO_PWR_KEY,
    
  
#endif
    MICO_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
    MICO_GPIO_NONE,
} mico_gpio_t;

typedef enum
{
    MICO_SPI_1,
    MICO_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
    MICO_SPI_NONE,
} mico_spi_t;

typedef enum
{
    MICO_I2C_1,
    MICO_I2C_MAX, /* Denotes the total number of I2C port aliases. Not a valid I2C alias */
    MICO_I2C_NONE,
} mico_i2c_t;

typedef enum
{
    //MICO_PWM_IRLED,
    //MICO_PWM_2,
    //MICO_PWM_3,
    
    
    PWM_LED1 = 0,
    PWM_LED2,
    MICO_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
    MICO_PWM_NONE,
} mico_pwm_t;




#if 1
typedef enum
{
    /* following are adc1 channels */
    ADC_C_5V_RIGHT_HAND_MOTOR,
    ADC_C_24V_HD_CAMERA,
    ADC_C_24V_HEAD_MOTOR,
    ADC_C_24V_CAMERA,
    ADC_C_5V_KEYBOARD,
    ADC_C_5V_CODE,
    ADC_C_5V_LEFT_HAND_MOTOR,
    ADC_C_5V_CARD_READER,
    ADC_C_5V_HEAD,
    ADC_C_24V_RIGHT_HAND_MOTOR,
    ADC_C_HEAD_RK,
    ADC_C_12V_CHEST_RK,
    ADC_C_24V_LEFT_HAND_MOTOR,
    ADC_C_12V_AUDIO_PA,
    ADC_C_5V_REPAIR,
    ADC_C_5V_TOUCH,
    

    /* following are adc3 channels */
    ADC_C_12V_SWITCH,   
    ADC_C_12V_ROUTER,
    ADC_C_VBUS,
    ADC_C_12V_CARD_READER,
    ADC_SWITCH,
    
    
    /* begin of virtual adc */
    ADC_12V_TS,             // 1
    ADC_5V_TS,              // 2
    ADC_AIR_TS,             // 3
    ADC_C_5V_HEAD_TOUCH,    // 4
    ADC_C_12V_ALL,          // 5
    ADC_C_5V_ALL,           // 6
    ADC_C_5V_HEAD_LED,      // 7
    ADC_V_12V,              // 8
    ADC_V_5V,               // 9
    ADC_V_BAT,              // 10
    ADC_C_5V_RESERVE,       // 11
    ADC_C_5V_LED,           // 12
    ADC_C_5V_CAMERA,        // 13
    ADC_5V_HD_CAMERA,       // 14
    /* end of vitual adc */
    ADC_MAX, /* Denotes the total number of ADC port aliases. Not a valid ADC alias */
    MICO_ADC_NONE,//ADC_NONE,
} mico_adc_t;
#endif
typedef enum
{
    MICO_UART_1,
    MICO_UART_2,
    MICO_UART_3,
    MICO_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
    MICO_UART_NONE,
} mico_uart_t;

typedef enum
{
  MICO_FLASH_EMBEDDED,
  MICO_FLASH_MAX,
  MICO_FLASH_NONE,
} mico_flash_t;

typedef enum
{
  MICO_PARTITION_USER_MAX
} mico_user_partition_t;

typedef enum 
{
    MICO_CAN1,
    MICO_CAN_MAX,
    MICO_CAN_NONE,
} mico_can_t;

#ifdef BOOTLOADER
#define STDIO_UART          MICO_UART_3
#define STDIO_UART_BAUDRATE (115200) 
#else
#define STDIO_UART          MICO_UART_2
#define STDIO_UART_BAUDRATE (115200) 
#endif

#define COMM_UART        MICO_UART_3
#define COMM_UART_BAUDRATE (115200) 
#define UART_FOR_APP     MICO_UART_1
#define CLI_UART         MICO_UART_3


#ifdef __cplusplus
} /*extern "C" */
#endif

