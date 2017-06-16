/**
******************************************************************************
* @file    platform.c 
* @author  William Xu
* @version V1.0.0
* @date    05-May-2014
* @brief   This file provides all MICO Peripherals mapping table and platform
*          specific functions.
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

#include "stdio.h"
#include "string.h"

#include "platform.h"
#include "platform_config.h"
#include "platform_peripheral.h"
#include "platform_internal.h"
#include "platform_logging.h"
#include "mico_platform.h"
//#include "keypad/gpio_button/button.h"
#ifndef BOOTLOADER
#include "app_platform.h"
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

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*                    Structures
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/
extern WEAK void PlatformEasyLinkButtonClickedCallback(void);
extern WEAK void PlatformStandbyButtonClickedCallback(void);
extern WEAK void PlatformEasyLinkButtonLongPressedCallback(void);
extern WEAK void bootloader_start(void);

/******************************************************
*               Variables Definitions
******************************************************/



                                  
const platform_gpio_t platform_gpio_pins[] =
{
#if 1
  //[MICO_GPIO_ADAPTER_IN]        = { GPIOB, 14 },    // allocation a new pin in new board
  
  
  //[MICO_GPIO_PWRKEY]            = { GPIOD,  4 },
  
  [MICO_GPIO_MOTOR_EN]          = { GPIOE,  2 },
  [MICO_GPIO_SENSOR_EN]         = { GPIOE,  3 },
  [MICO_GPIO_LEDS_EN]           = { GPIOE,  4 },
  [MICO_GPIO_AIUI_EN]           = { GPIOE,  5 },
  
  [MICO_GPIO_PAD_EN]            = { GPIOF,  0 },
  
  [MICO_GPIO_2_1_PA_EN]         = { GPIOF,  2 },
  [MICO_GPIO_DYP_EN]            = { GPIOF,  3 },
  [MICO_GPIO_X86_EN]            = { GPIOF,  4 },
  [MICO_GPIO_NV_EN]             = { GPIOF,  5 },
  [MICO_GPIO_DLP_EN]            = { GPIOG,  2 },
  [MICO_GPIO_12V_RES_EN]        = { GPIOG,  3 },
  [MICO_GPIO_PRINTER_EN]        = { GPIOG,  4 },
  [MICO_GPIO_24V_RES_EN]        = { GPIOG,  5 },
  [MICO_GPIO_BAT_NV_EN]         = { GPIOG,  6 },
  [MICO_GPIO_5V_ROUTER_EN]      = { GPIOG,  7 },
  
  
  [MICO_GPIO_24V_EN]            = { GPIOE,  9 },//not used ?
  
  

  [MICO_GPIO_PWR_NV]            = { GPIOF, 13 },
  [MICO_GPIO_PWR_DLP]           = { GPIOF, 14 },
  [MICO_GPIO_PWR_PAD]           = { GPIOF, 15 },
  [MICO_GPIO_PWR_X86]           = { GPIOG,  0 },//not used
  [MICO_GPIO_PWR_RES]           = { GPIOG,  1 },

  
  [MICO_GPIO_LED_PWM]           = { GPIOC, 6  },
  
  //[MICO_GPIO_FAN_CTRL]          = { GPIOD, 11 },

  [MICO_GPIO_RECHARGE_LED]      = { GPIOC,  8 }, 
  [MICO_GPIO_IRLED_PWM]         = { GPIOA,  8 },
  
  
  
  
  
 
  
  
  
  //ADC1
  [GPIO_5V_RIGHT_HAND_MOTOR_C_ADC]  = { GPIOA, 0 },
  [GPIO_24V_HD_CAMERE_C_ADC]        = { GPIOA, 1 },
  [GPIO_24V_HEAD_MOTOR_C_ADC]       = { GPIOA, 2 },
  [GPIO_24V_CAMERE_C_ADC]           = { GPIOA, 3 },
  [GPIO_5V_KEYBOARD_C_ADC]          = { GPIOA, 4 },
  [GPIO_5V_CODE_C_ADC]              = { GPIOA, 5 },
  [GPIO_5V_LEFTHAND_MOTOR_C_ADC]    = { GPIOA, 6 },
  [GPIO_5V_CARD_READER_C_ADC]       = { GPIOA, 7 },
  [GPIO_5V_HEAD_C_ADC]              = { GPIOB, 0 },
  [GPIO_24V_RIGHT_HAND_MOTOR_C_ADC] = { GPIOB, 1 },
  [GPIO_12V_HEAD_RK_C_ADC]          = { GPIOC, 0 },
  [GPIO_12V_CHEST_RK_C_ADC]         = { GPIOC, 1 },
  [GPIO_24V_LEFT_HAND_MOTOR_C_ADC]  = { GPIOC, 2 },
  [GPIO_12V_AUDIO_PA_C_ADC]         = { GPIOC, 3 },
  [GPIO_5V_REPAIR_C_ADC]            = { GPIOC, 4 },
  [GPIO_5V_TOUCH_C_ADC]             = { GPIOC, 5 },

  
  //ADC3
  [GPIO_12V_SWITCH_C_ADC]           = { GPIOF, 6 },
  [GPIO_12V_ROUTER_C_ADC]           = { GPIOF, 7 },
  [GPIO_VBUS_C_ADC]                 = { GPIOF, 8 }, 
  [GPIO_12V_CARD_READER_C_ADC]      = { GPIOF, 9 }, 
  [GPIO_SWITCH_C_V_ADC]             = { GPIOF, 10},
  
  
  
  
  [GPIO_SYS_LED]           = { GPIOC,  9 },
  
  [GPIO_LED4]              = { GPIOD, 10 },// new
  [GPIO_LED3]              = { GPIOD, 11 },// new
  [GPIO_LED2]              = { GPIOD, 12 },// new
  [GPIO_LED1]              = { GPIOD, 13 },// new
  
  
  [GPIO_PWM_LED1]          = { GPIOC, 7 },
  [GPIO_PWM_LED2]          = { GPIOC, 6 },
  
  [GPIO_SWITCH_EN]         = { GPIOD,  7 },// Previous OK
  [GPIO_SWITCH_SEL0]       = { GPIOG,  9 },// Previous OK
  [GPIO_SWITCH_SEL1]       = { GPIOG, 10 },// Previous OK
  [GPIO_SWITCH_SEL2]       = { GPIOG, 11 },// Previous OK
  [GPIO_SWITCH_SEL3]       = { GPIOG, 12 },// Previous OK
  [GPIO_SWITCH_ADC]        = { GPIOF, 10 },// Previous OK
  
  
  
  [GPIO_5V_RESERVE_EN]              = { GPIOE,  5 },// change pin 
  [GPIO_5V_EN]                      = { GPIOE,  7 },// Previous OK
  [GPIO_5V_LEFT_HAND_MOTOR_EN]      = { GPIOE, 3 },// new
  [GPIO_5V_RIGHT_HAND_MOTOR_EN]     = { GPIOE, 6 },// new
  [GPIO_5V_HEAD_MOTOR_EN]           = { GPIOE, 2 },// new
  [GPIO_5V_LEDX2_EN]                = { GPIOG, 7 },// new
  [GPIO_5V_HEAD_LED_EN]             = { GPIOE, 9 },
  [GPIO_5V_REPAIR_BOARD_EN]         = { GPIOE, 4 },// new
  [GPIO_5V_TOUCH_EN]                = { GPIOF, 1 },// new
  [GPIO_5V_KEYBORAD_EN]             = { GPIOF, 3 },// new
  [GPIO_5V_CARD_READER_EN]          = { GPIOF, 5 },// new
  [GPIO_5V_CODE_EN]                 = { GPIOG, 8 },// new
  
  [GPIO_5V_HD_CAMERA_EN]            = { GPIOE, 10},
  [GPIO_5V_HD_CAMERA_STOP_EN]       = { GPIOE, 12},
  [GPIO_5V_CAMERA_EN]               = { GPIOE, 11},
  [GPIO_5V_CAMERA_STOP_EN]          = { GPIOE, 13},
  [GPIO_5V_HEAD_TOUCH_EN]           = { GPIOE, 14},
  
  [GPIO_24V_LEFT_HAND_MOTOR_EN]     = { GPIOG, 4 }, // new
  [GPIO_24V_RIGHT_HAND_MOTOR_EN]    = { GPIOG, 5 }, // new
  [GPIO_24V_HEAD_MOTOR_EN]          = { GPIOG, 6 }, // new
  [GPIO_24V_HD_CAMERA_EN]           = { GPIOF, 11},
  [GPIO_24V_CAMERA_EN]              = { GPIOF, 12},
  
  [GPIO_12V_EN]                     = { GPIOE,  8 },// Previous OK
  
  [GPIO_12V_CARD_READER_EN]         = { GPIOF, 14 },// new
  [GPIO_12V_AUDIO_PA_EN]            = { GPIOF, 4 },// new
  [GPIO_12V_SWITCH_EN]              = { GPIOG, 0 },// new
  [GPIO_12V_ROUTER_EN]              = { GPIOG,  1 },// change pin
  
  [GPIO_12V_HEAD_RK_EN]             = { GPIOF, 2 },// new
  [GPIO_12V_HEAD_RK_SIGNAL]         = { GPIOF, 13},
  
  [GPIO_12V_CHEST_RK_EN]            = { GPIOF, 0 },// new
  [GPIO_12V_CHEST_RK_SIGNAL]        = { GPIOF, 15},    
  
  
  

  
  [MICO_GPIO_I2C_SCL]               = { GPIOB,  6 },//
  [MICO_GPIO_I2C_SDA]               = { GPIOB,  7 },//
  
  [MICO_GPIO_UART3_TX]              = { GPIOB, 10 },//
  [MICO_GPIO_UART3_RX]              = { GPIOB, 11 },//
  
  [MICO_GPIO_UART2_TX]              = { GPIOD,  5 },//
  [MICO_GPIO_UART2_RX]              = { GPIOD,  6 },//
  
  [MICO_GPIO_UART1_TX]              = { GPIOA,  9 },//
  [MICO_GPIO_UART1_RX]              = { GPIOA, 10 },//
  
  [MICO_GPIO_SPI_NSS]               = { GPIOA, 15 },//
  [MICO_GPIO_SPI_SCK]               = { GPIOB,  3 },//
  [MICO_GPIO_SPI_MISO]              = { GPIOB,  4 },//
  [MICO_GPIO_SPI_MOSI]              = { GPIOB,  5 },//
    
  [MICO_GPIO_CAN_RX]                = { GPIOA, 11 },//
  [MICO_GPIO_CAN_TX]                = { GPIOA, 12 },//
  [MICO_GPIO_CAN_STB]               = { GPIOD,  3 },//
  [MICO_GPIO_ID1]                   = { GPIOE,  0 },
  [MICO_GPIO_ID2]                   = { GPIOE,  1 },
  [GPIO_PWR_KEY]                    = { GPIOD, 4 },
#endif
};

/*
* Possible compile time inputs:
* - Set which ADC peripheral to use for each ADC. All on one ADC allows sequential conversion on all inputs. All on separate ADCs allows concurrent conversion.
*/

/* TODO : These need fixing */
ADC_HandleTypeDef adc_handles[2];

const platform_adc_t platform_adc_peripherals[] =
{
    [ADC_C_5V_RIGHT_HAND_MOTOR]         = {ADC1, ADC_CHANNEL_0, &adc_handles[0], 1, &platform_gpio_pins[GPIO_5V_RIGHT_HAND_MOTOR_C_ADC]},
    [ADC_C_24V_HD_CAMERA]               = {ADC1, ADC_CHANNEL_1, &adc_handles[0], 2, &platform_gpio_pins[GPIO_24V_HD_CAMERE_C_ADC]},
    [ADC_C_24V_HEAD_MOTOR]              = {ADC1, ADC_CHANNEL_2, &adc_handles[0], 3, &platform_gpio_pins[GPIO_24V_HEAD_MOTOR_C_ADC]},   
    [ADC_C_24V_CAMERA]                  = {ADC1, ADC_CHANNEL_3, &adc_handles[0], 4, &platform_gpio_pins[GPIO_24V_CAMERE_C_ADC]},
    [ADC_C_5V_KEYBOARD]                 = {ADC1, ADC_CHANNEL_4, &adc_handles[0], 5, &platform_gpio_pins[GPIO_5V_KEYBOARD_C_ADC]},
    [ADC_C_5V_CODE]                     = {ADC1, ADC_CHANNEL_5, &adc_handles[0], 6, &platform_gpio_pins[GPIO_5V_CODE_C_ADC]},
    [ADC_C_5V_LEFT_HAND_MOTOR]          = {ADC1, ADC_CHANNEL_6, &adc_handles[0], 7, &platform_gpio_pins[GPIO_5V_LEFTHAND_MOTOR_C_ADC]}, 
    [ADC_C_5V_CARD_READER]              = {ADC1, ADC_CHANNEL_7, &adc_handles[0], 8, &platform_gpio_pins[GPIO_5V_CARD_READER_C_ADC]},
    [ADC_C_5V_HEAD]                     = {ADC1, ADC_CHANNEL_8, &adc_handles[0], 9, &platform_gpio_pins[GPIO_5V_HEAD_C_ADC]},
    [ADC_C_24V_RIGHT_HAND_MOTOR]        = {ADC1, ADC_CHANNEL_9, &adc_handles[0], 10, &platform_gpio_pins[GPIO_24V_RIGHT_HAND_MOTOR_C_ADC]}, 
    [ADC_C_HEAD_RK]                     = {ADC1, ADC_CHANNEL_10, &adc_handles[0], 11, &platform_gpio_pins[GPIO_12V_HEAD_RK_C_ADC]},
    [ADC_C_12V_CHEST_RK]                = {ADC1, ADC_CHANNEL_11, &adc_handles[0], 12, &platform_gpio_pins[GPIO_12V_CHEST_RK_C_ADC]},
    [ADC_C_24V_LEFT_HAND_MOTOR]         = {ADC1, ADC_CHANNEL_12, &adc_handles[0], 13, &platform_gpio_pins[GPIO_24V_LEFT_HAND_MOTOR_C_ADC]},
    [ADC_C_12V_AUDIO_PA]                = {ADC1, ADC_CHANNEL_13, &adc_handles[0], 14, &platform_gpio_pins[GPIO_12V_AUDIO_PA_C_ADC]},  
    [ADC_C_5V_REPAIR]                   = {ADC1, ADC_CHANNEL_14, &adc_handles[0], 15, &platform_gpio_pins[GPIO_5V_REPAIR_C_ADC]},
    [ADC_C_5V_TOUCH]                    = {ADC1, ADC_CHANNEL_15, &adc_handles[0], 16, &platform_gpio_pins[GPIO_5V_TOUCH_C_ADC]},
    
    [ADC_C_12V_SWITCH]                  = {ADC3, ADC_CHANNEL_4, &adc_handles[1], 1, &platform_gpio_pins[GPIO_12V_SWITCH_C_ADC]},
    [ADC_C_12V_ROUTER]                  = {ADC3, ADC_CHANNEL_5, &adc_handles[1], 2, &platform_gpio_pins[GPIO_12V_ROUTER_C_ADC]},
    [ADC_C_VBUS]                        = {ADC3, ADC_CHANNEL_6, &adc_handles[1], 3, &platform_gpio_pins[GPIO_VBUS_C_ADC]},
    [ADC_C_12V_CARD_READER]             = {ADC3, ADC_CHANNEL_7, &adc_handles[1], 4, &platform_gpio_pins[GPIO_12V_CARD_READER_C_ADC]},
    [ADC_SWITCH]                        = {ADC3, ADC_CHANNEL_8, &adc_handles[1], 5, &platform_gpio_pins[GPIO_SWITCH_C_V_ADC]},
};

TIM_HandleTypeDef tim_handles[1];
/* PWM mappings */
const platform_pwm_t platform_pwm_peripherals[] =
{  
 // [MICO_PWM_IRLED] = {TIM1, &tim_handles[0], TIM_CHANNEL_1, RCC_APB2ENR_TIM1EN, NULL, &platform_gpio_pins[MICO_GPIO_IRLED_PWM]},    /* or TIM10/Ch1                       */
  [PWM_LED1]       = {TIM8, &tim_handles[1], TIM_CHANNEL_2, RCC_APB2ENR_TIM8EN, NULL, &platform_gpio_pins[GPIO_PWM_LED1]},
  [PWM_LED2]       = {TIM8, &tim_handles[2], TIM_CHANNEL_1, RCC_APB2ENR_TIM8EN, NULL, &platform_gpio_pins[GPIO_PWM_LED2]},
//  [MICO_PWM_2]  = {TIM12, 1, RCC_APB1Periph_TIM12, GPIO_AF_TIM12, &platform_gpio_pins[MICO_GPIO_13]}, /* or TIM1/Ch2N                       */
//  [MICO_PWM_3]  = {TIM2, 4, RCC_APB1Periph_TIM2, GPIO_AF_TIM2, &platform_gpio_pins[MICO_GPIO_19]},    
  /* TODO: fill in the other options here ... */
};
#if 0
const platform_spi_t platform_spi_peripherals[] =
{
  [MICO_SPI_1]  =
  {
    .port                  = SPI1,
    .gpio_af               = GPIO_AF_SPI1,
    .peripheral_clock_reg  = RCC_APB2Periph_SPI1,
    .peripheral_clock_func = RCC_APB2PeriphClockCmd,
    .pin_mosi              = &platform_gpio_pins[MICO_GPIO_9],
    .pin_miso              = &platform_gpio_pins[MICO_GPIO_8],
    .pin_clock             = &platform_gpio_pins[MICO_GPIO_7],
    .tx_dma =
    {
      .controller          = DMA2,
      .stream              = DMA2_Stream5,
      .channel             = DMA_Channel_3,
      .irq_vector          = DMA2_Stream5_IRQn,
      .complete_flags      = DMA_HISR_TCIF5,
      .error_flags         = ( DMA_HISR_TEIF5 | DMA_HISR_FEIF5 | DMA_HISR_DMEIF5 ),
    },
    .rx_dma =
    {
      .controller          = DMA2,
      .stream              = DMA2_Stream0,
      .channel             = DMA_Channel_3,
      .irq_vector          = DMA2_Stream0_IRQn,
      .complete_flags      = DMA_LISR_TCIF0,
      .error_flags         = ( DMA_LISR_TEIF0 | DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 ),
    },
  }
};

platform_spi_driver_t platform_spi_drivers[MICO_SPI_MAX];
#endif

const platform_uart_t platform_uart_peripherals[] =
{
  [MICO_UART_3] =
  {
    .port                         = USART3,
    .pin_tx                       = &platform_gpio_pins[MICO_GPIO_UART3_TX],
    .pin_rx                       = &platform_gpio_pins[MICO_GPIO_UART3_RX],
    .pin_cts                      = NULL,
    .pin_rts                      = NULL,
    .tx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel2,
      .irq_vector                 = DMA1_Channel2_IRQn,
      .complete_flags             = DMA_ISR_TCIF2,
      .error_flags                = DMA_ISR_TEIF2,
    },
    .rx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel3,
      .irq_vector                 = DMA1_Channel3_IRQn,
      .complete_flags             = DMA_ISR_TCIF3,
      .error_flags                = DMA_ISR_TEIF3,
    },
  },
  [MICO_UART_1] =
  {
    .port                         = USART1,
    .pin_tx                       = &platform_gpio_pins[MICO_GPIO_UART1_TX],
    .pin_rx                       = &platform_gpio_pins[MICO_GPIO_UART1_RX],
    .pin_cts                      = NULL,
    .pin_rts                      = NULL,
    .tx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel4,
      .irq_vector                 = DMA1_Channel4_IRQn,
      .complete_flags             = DMA_ISR_TCIF4,
      .error_flags                = DMA_ISR_TEIF4,
    },
    .rx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel5,
      .irq_vector                 = DMA1_Channel5_IRQn,
      .complete_flags             = DMA_ISR_TCIF5,
      .error_flags                = DMA_ISR_TEIF5,
    },
  },
  [MICO_UART_2] =
  {
    .port                         = USART2,
    .pin_tx                       = &platform_gpio_pins[MICO_GPIO_UART2_TX],
    .pin_rx                       = &platform_gpio_pins[MICO_GPIO_UART2_RX],
    .pin_cts                      = NULL,
    .pin_rts                      = NULL,
    .tx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel7,
      .irq_vector                 = DMA1_Channel7_IRQn,
      .complete_flags             = DMA_ISR_TCIF7,
      .error_flags                = DMA_ISR_TEIF7,
    },
    .rx_dma_config =
    {
      .controller                 = DMA1,
      .channel                    = DMA1_Channel6,
      .irq_vector                 = DMA1_Channel6_IRQn,
      .complete_flags             = DMA_ISR_TCIF6,
      .error_flags                = DMA_ISR_TEIF6,
    },
  },
};
static UART_HandleTypeDef UartHandle[3];
static DMA_HandleTypeDef  uart_tx_dmaHandle[3];
static DMA_HandleTypeDef  uart_rx_dmaHandle[3];
//platform_uart_driver_t platform_uart_drivers[MICO_UART_MAX];
platform_uart_driver_t platform_uart_drivers[] = 
{
  [MICO_UART_1] = 
  {
    .uart_handle = &UartHandle[0],
    .rx_dma_handle = &uart_rx_dmaHandle[0],
    .tx_dma_handle = &uart_tx_dmaHandle[0],
  },
  [MICO_UART_2] = 
  {
    .uart_handle = &UartHandle[1],
    .rx_dma_handle = &uart_rx_dmaHandle[1],
    .tx_dma_handle = &uart_tx_dmaHandle[1],
  },
  [MICO_UART_3] = 
  {
    .uart_handle = &UartHandle[2],
    .rx_dma_handle = &uart_rx_dmaHandle[2],
    .tx_dma_handle = &uart_tx_dmaHandle[2],
  },
};
#if 0
const platform_i2c_t platform_i2c_peripherals[] =
{
  [MICO_I2C_1] =
  {
    .port                    = I2C1,
    .pin_scl                 = &platform_gpio_pins[MICO_GPIO_1],
    .pin_sda                 = &platform_gpio_pins[MICO_GPIO_2],
    .peripheral_clock_reg    = RCC_APB1Periph_I2C1,
    .tx_dma                  = DMA1,
    .tx_dma_peripheral_clock = RCC_AHB1Periph_DMA1,
    .tx_dma_stream           = DMA1_Stream7,
    .rx_dma_stream           = DMA1_Stream0,
    .tx_dma_stream_id        = 7,
    .rx_dma_stream_id        = 0,
    .tx_dma_channel          = DMA_Channel_1,
    .rx_dma_channel          = DMA_Channel_1,
    .gpio_af                 = GPIO_AF_I2C1
  },
};

platform_i2c_driver_t platform_i2c_drivers[MICO_I2C_MAX];
#endif
//platform_flash_driver_t platform_flash_drivers[MICO_FLASH_MAX];

/* Flash memory devices */
const platform_flash_t platform_flash_peripherals[] =
{
  [MICO_FLASH_EMBEDDED] =
  {
    .flash_type                   = FLASH_TYPE_EMBEDDED,
    .flash_start_addr             = 0x08000000,
    .flash_length                 = 0x80000,
  },
};

platform_flash_driver_t platform_flash_drivers[MICO_FLASH_MAX];

/* Logic partition on flash devices */
const mico_logic_partition_t mico_partitions[] =
{
  [MICO_PARTITION_BOOTLOADER] =
  {
    .partition_owner           = MICO_FLASH_EMBEDDED,
    .partition_description     = "Bootloader",
    .partition_start_addr      = 0x08000000,
    .partition_length          = 0x8000,    //32k bytes
    .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
  },
  [MICO_PARTITION_APPLICATION] =
  {
    .partition_owner           = MICO_FLASH_EMBEDDED,
    .partition_description     = "Application",
    .partition_start_addr      = 0x08010000, 
    .partition_length          = 0x38000,   //224k bytes
    .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
  },
  [MICO_PARTITION_OTA_TEMP] =
  {
    .partition_owner           = MICO_FLASH_EMBEDDED,
    .partition_description     = "OTA Storage",
    .partition_start_addr      = 0x08048000,
    .partition_length          = 0x38000, //224k bytes
    .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
  },
  [MICO_PARTITION_PARAMETER_1] =
  {
    .partition_owner           = MICO_FLASH_EMBEDDED,
    .partition_description     = "PARAMETER1",
    .partition_start_addr      = 0x08008000,
    .partition_length          = 0x400,//0x4000, // 16k bytes
    .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
  },
  [MICO_PARTITION_PARAMETER_2] =
  {
    .partition_owner           = MICO_FLASH_EMBEDDED,
    .partition_description     = "PARAMETER2",
    .partition_start_addr      = 0x0800C000,
    .partition_length          = 0x4000, //16k bytes
    .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
  },
};

#if 1//defined (USE_CAN)
CanTxMsgTypeDef         can_tx_msg;
CanRxMsgTypeDef         can_rx_msg;
CAN_HandleTypeDef       can_handle = {
  .pTxMsg      = &can_tx_msg,
  .pRxMsg      = &can_rx_msg,
};

platform_can_driver_t  platform_can_drivers[] = 
{
  [MICO_CAN1] = 
  {
    .port                       = CAN1,
    .handle                     = &can_handle,
    .pin_tx                     = &platform_gpio_pins[MICO_GPIO_CAN_TX],
    .pin_rx                     = &platform_gpio_pins[MICO_GPIO_CAN_RX],
  },
};
#endif
/******************************************************
*           Interrupt Handler Definitions
******************************************************/
MICO_RTOS_DEFINE_ISR( SysTick_Handler )
{
  sysTickHandler();
}

MICO_RTOS_DEFINE_ISR( USART3_IRQHandler )
{
  platform_uart_irq( &platform_uart_drivers[MICO_UART_3] );
}

MICO_RTOS_DEFINE_ISR( USART1_IRQHandler )
{
  platform_uart_irq( &platform_uart_drivers[MICO_UART_1] );
}

MICO_RTOS_DEFINE_ISR( USART2_IRQHandler )
{
  platform_uart_irq( &platform_uart_drivers[MICO_UART_2] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel2_IRQHandler )
{
  platform_uart_tx_dma_irq( &platform_uart_drivers[MICO_UART_3] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel4_IRQHandler )
{
  platform_uart_tx_dma_irq( &platform_uart_drivers[MICO_UART_1] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel7_IRQHandler )
{
  platform_uart_tx_dma_irq( &platform_uart_drivers[MICO_UART_2] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel3_IRQHandler )
{
  platform_uart_rx_dma_irq( &platform_uart_drivers[MICO_UART_3] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel5_IRQHandler )
{
  platform_uart_rx_dma_irq( &platform_uart_drivers[MICO_UART_1] );
}

MICO_RTOS_DEFINE_ISR( DMA1_Channel6_IRQHandler )
{
  platform_uart_rx_dma_irq( &platform_uart_drivers[MICO_UART_2] );
}

#if 0
static void _switch_irq_handler( void* arg )
{
  (void)(arg);
  MicoGpioDisableIRQ( GPIO_PWR_KEY );
#ifndef BOOTLOADER
  key_switch_interrupt_cb();
#endif
  MicoGpioEnableIRQ( GPIO_PWR_KEY , IRQ_TRIGGER_BOTH_EDGES, _switch_irq_handler, NULL);
}
#endif

MICO_RTOS_DEFINE_ISR( DMA1_Channel1_IRQHandler )
{
//  platform_adc_dma_irq( &platform_adc_peripherals[MICO_ADC_5V_RES1] );
  __asm("NOP");
}

MICO_RTOS_DEFINE_ISR( DMA2_Channel4_5_IRQHandler )
{
   __asm("NOP");
}

MICO_RTOS_DEFINE_ISR( USB_LP_CAN1_RX0_IRQHandler )
{
  platform_can_rx_irq( &platform_can_drivers[MICO_CAN1] );
}

/******************************************************
*               Function Definitions
******************************************************/
#if 1
static void _switch_irq_handler( void* arg )
{
  (void)(arg);
#ifndef BOOTLOADER
  key_switch_interrupt_cb();
#endif
}
#endif

bool watchdog_check_last_reset( void )
{
  if ( RCC->CSR & RCC_CSR_IWDGRSTF )
  {
    /* Clear the flag and return */
    RCC->CSR |= RCC_CSR_RMVF;
    return true;
  }
  
  return false;
}

void platform_init_peripheral_irq_priorities( void )
{
  /* Interrupt priority setup. Called by MiCO/platform/MCU/STM32F2xx/platform_init.c */
  NVIC_SetPriority( RTC_IRQn         ,  1 ); /* RTC Wake-up event   */
  
  NVIC_SetPriority( USART1_IRQn      ,  6 ); /* MICO_UART_1         */
  NVIC_SetPriority( USART2_IRQn      ,  6 ); /* MICO_UART_2         */
  NVIC_SetPriority( USART3_IRQn      ,  6 ); /* MICO_UART_3         */
  NVIC_SetPriority( DMA1_Channel4_IRQn,  7 ); /* MICO_UART_1 TX DMA  */
  NVIC_SetPriority( DMA1_Channel5_IRQn,  7 ); /* MICO_UART_1 RX DMA  */
  NVIC_SetPriority( DMA1_Channel2_IRQn,  7 ); /* MICO_UART_2 TX DMA  */
  NVIC_SetPriority( DMA1_Channel3_IRQn,  7 ); /* MICO_UART_2 RX DMA  */
  NVIC_SetPriority( DMA1_Channel6_IRQn,  7 ); /* MICO_UART_3 TX DMA  */
  NVIC_SetPriority( DMA1_Channel7_IRQn,  7 ); /* MICO_UART_3 RX DMA  */
  NVIC_SetPriority( DMA1_Channel1_IRQn,  10 ); /* ADC DMA  */
  NVIC_SetPriority( EXTI0_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI1_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI2_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI3_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI4_IRQn       , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI9_5_IRQn     , 14 ); /* GPIO                */
  NVIC_SetPriority( EXTI15_10_IRQn   , 14 ); /* GPIO                */
  
  NVIC_SetPriority( USB_LP_CAN1_RX0_IRQn,  7 ); /* MICO_CAN1         */
}

void init_platform( void )
{ 
  platform_pin_config_t pin_config;
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_OUTPUT_PP;
  pin_config.gpio_pull = GPIO_PULLUP;
  
  //  Initialise system led
  MicoGpioInitialize( (mico_gpio_t)GPIO_SYS_LED, &pin_config );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SYS_LED ); 
  
  //  Initialise switch
#if 1
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_IT_RISING_FALLING;
  pin_config.gpio_pull = GPIO_PULLUP;//GPIO_NOPULL;//GPIO_PULLDOWN;
  MicoGpioInitialize( GPIO_PWR_KEY, &pin_config );
  MicoGpioEnableIRQ( GPIO_PWR_KEY , IRQ_TRIGGER_BOTH_EDGES, _switch_irq_handler, NULL);
#endif
}

void init_platform_bootloader( void )
{
  platform_pin_config_t pin_config;
  pin_config.gpio_speed = GPIO_SPEED_MEDIUM;
  pin_config.gpio_mode = GPIO_MODE_OUTPUT_PP;
  pin_config.gpio_pull = GPIO_PULLUP;
  MicoGpioInitialize( (mico_gpio_t)GPIO_SYS_LED, &pin_config );
  MicoGpioOutputLow( (mico_gpio_t)GPIO_SYS_LED );
}

void MicoSysLed(bool onoff)
{
  if (onoff) {
    MicoGpioOutputHigh( (mico_gpio_t)GPIO_SYS_LED );
  } else {
    MicoGpioOutputLow( (mico_gpio_t)GPIO_SYS_LED );
  }
}

bool MicoShouldEnterBootloader(void)
{
  if( 1 )//MicoGpioInputGet((mico_gpio_t)BOOT_SEL)==false && MicoGpioInputGet((mico_gpio_t)MFG_SEL)==true)
    return true;
  else
    return false;
}
