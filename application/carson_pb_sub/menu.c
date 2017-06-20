/**
 ******************************************************************************
 * @file    menu.c
 * @author  William Xu
 * @version V2.0.0
 * @date    05-Oct-2014
 * @brief   his file provides the software which contains the main menu routine.
 *          The main menu gives the options of:
 *             - downloading a new binary file,
 *             - uploading internal flash memory,
 *             - executing the binary file already loaded
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

/* Includes ------------------------------------------------------------------*/
#include "mico.h"
#include "ymodem.h"
#include "platform_config.h"
#include "platform_internal.h"
#include "StringUtils.h"
#include "bootloader.h"
#include <ctype.h>                    
#include "voltage_detect.h"
#include "upgrade_flash.h"
#include "app_platform.h"
#include "protocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CMD_STRING_SIZE       128
   
#define CNTLQ      0x11
#define CNTLS      0x13
#define DEL        0x7F
#define BACKSPACE  0x08
#define CR         0x0D
#define LF         0x0A
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern platform_flash_t platform_flash_peripherals[];

uint8_t tab_1024[1024] =
{
  0
};

char FileName[FILE_NAME_LENGTH];
char ERROR_STR [] = "\n\r*** ERROR: %s\n\r";    /* ERROR message string in code   */

extern char menu[];
#ifdef NO_BLOCK_MENU
extern int getline (char *line, int n);          /* input line               */
#else
extern void getline (char *line, int n);          /* input line               */
#endif
extern void startApplication( uint32_t app_addr );

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(mico_flash_t flash, uint32_t flashdestination, int32_t maxRecvSize);
void SerialUpload(mico_flash_t flash, uint32_t flashdestination, char * fileName, int32_t maxRecvSize);

/* Private functions ---------------------------------------------------------*/
/**
* @brief  Analyse a command parameter
* @param  commandBody: command string address
* @param  para: The para we are looking for
* @param  paraBody: A pointer to the buffer to receive the para body.
* @param  paraBodyLength: The length, in bytes, of the buffer pointed to by the paraBody parameter.
* @retval the actual length of the paraBody received, -1 means failed to find this paras 
*/
#ifdef NO_BLOCK_MENU
static void uart_putchar( int c )
{
  MicoUartSend( STDIO_UART, &c, 1 );
}
#endif
int findCommandPara(char *commandBody, char *para, char *paraBody, int paraBodyLength)
{
  int i = 0;
  int k, j;
  int retval = -1;
  char para_in_ram[100];
  strncpy(para_in_ram, para, 100);
  
  for (i = 0; para_in_ram[i] != 0; i++)  {                /* convert to upper characters */
    para_in_ram[i] = toupper(para_in_ram[i]);
  }
  
  i = 0;
  while(commandBody[i] != 0) {
    if(commandBody[i] == '-' ){
      for(j=i+1, k=0; *(para_in_ram+k)!=0x0; j++, k++ ){
        if(commandBody[j] != *(para_in_ram+k)){
          break;
        } 
      }
      
      if(*(para+k)!=0x0 || (commandBody[j]!=' '&& commandBody[j]!=0x0)){   /* para not found!             */
        i++;
        continue;
      }
      
      retval = 0;
      for (k = j+1; commandBody[k] == ' '; k++);      /* skip blanks                 */
      for(j = 0; commandBody[k] != ' ' && commandBody[k] != 0 && commandBody[k] != '-'; j++, k++){   /* para body found!             */
        if(paraBody) paraBody[j] = commandBody[k];
        retval ++;
        if( retval == paraBodyLength) goto exit;
      }
      goto exit;
    }
    i++;
  }
  
exit:
  if(paraBody) paraBody[retval] = 0x0;
  return retval;
}

/**
* @brief  Download a file via serial port
* @param  None
* @retval None
*/
void SerialDownload(mico_flash_t flash, uint32_t flashdestination, int32_t maxRecvSize)
{
  char Number[10] = "          ";
  int32_t Size = 0;
  
  printf("Waiting for the file to be sent ... (press 'a' to abort)\r\n");
  Size = Ymodem_Receive( &tab_1024[0], flash, flashdestination, maxRecvSize );
  if (Size > 0)
  {
    printf("\n\n\r Successfully!\n\r\r\n Name: %s", FileName);
    
    Int2Str((uint8_t *)Number, Size);
    printf("\n\r Size: %s Bytes\r\n", Number);
  }
  else if (Size == -1)
  {
    printf("\n\n\rImage size is higher than memory!\n\r");
  }
  else if (Size == -2)
  {
    printf("\n\n\rVerification failed!\r\n");
  }
  else if (Size == -3)
  {
    printf("\r\n\nAborted.\r\n");
  }
  else
  {
    printf("\n\rReceive failed!\r\n");
  }
}

/**
* @brief  Upload a file via serial port.
* @param  None
* @retval None
*/
void SerialUpload(mico_flash_t flash, uint32_t flashdestination, char * fileName, int32_t maxRecvSize)
{
  uint8_t status = 0;
  uint8_t key;
  
  printf("Select Receive File\n\r");
  MicoUartRecv( STDIO_UART, &key, 1, MICO_NEVER_TIMEOUT );
  
  if (key == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit(flash, flashdestination, (uint8_t *)fileName, maxRecvSize);
    
    if (status != 0)
    {
      printf("\n\rError while Transmitting\n\r");
    }
    else
    {
      printf("\n\rSuccessfully\n\r");
    }
  }
}
#ifdef NO_BLOCK_MENU
static char *pCmdBuf = NULL;
static int cnt = 0;
int getline (char *line, int n)
{
  char c;
  
  pCmdBuf = line + cnt;
  if( cnt >= (CMD_STRING_SIZE-2) )
  {
    goto exit;
  }
  if ( MicoUartRecv( STDIO_UART, &c, 1, 0 ) != kNoErr )
  {
      return -1;
  }
  if (c == CR)      /* read character                 */
  {
    c = LF;
    goto exit;
  }
  if (c == BACKSPACE  ||  c == DEL)  {    /* process backspace              */
    if (cnt != 0)  {
      cnt--;                              /* decrement count                */
      pCmdBuf--;                             /* and line pointer               */
      uart_putchar (BACKSPACE);                /* echo backspace                 */
      uart_putchar (' ');
      uart_putchar (BACKSPACE);
    }
    return -1;
  }
  else if (c != CNTLQ && c != CNTLS)  {   /* ignore Control S/Q             */
    uart_putchar (*pCmdBuf = c);                  /* echo and store character       */
    pCmdBuf++;                               /* increment line pointer         */
    cnt++;                                /* and count                      */
    return -1;
  }
exit:
  *(pCmdBuf) = 0;                          /* mark end of string             */ 
  return 0;
}
#endif
/**
* @brief  Display the Main Menu on HyperTerminal
* @param  None
* @retval None
*/
void Main_Menu(void)
{
  static char cmdbuf [CMD_STRING_SIZE] = {0}; 
  char cmdname[15] = {0};     /* command input buffer        */
  int i, j;                                       /* index for command buffer    */
  char idStr[4], startAddressStr[10], endAddressStr[10], flash_dev_str[4];
  int32_t id, startAddress, endAddress;
  bool inputFlashArea = false;
  mico_logic_partition_t *partition;
  mico_flash_t flash_dev;
  OSStatus err = kNoErr;
  
  while (1)  {                                    /* loop forever                */   
#ifndef NO_BLOCK_MENU
    printf ("\n\rPowerBoard> ");
#endif
#if defined __GNUC__
    fflush(stdout);
#endif
#ifdef NO_BLOCK_MENU
    if( -1 == getline (&cmdbuf[0], sizeof (cmdbuf)) )        /* input command line          */
    {
      return;
    }
#else
    getline (&cmdbuf[0], sizeof (cmdbuf));
#endif
    for (i = 0; cmdbuf[i] == ' '; i++);           /* skip blanks on head         */
    for (; cmdbuf[i] != 0; i++)  {                /* convert to upper characters */
      cmdbuf[i] = toupper(cmdbuf[i]);
    }
    
    for (i = 0; cmdbuf[i] == ' '; i++);        /* skip blanks on head         */
    for(j=0; cmdbuf[i] != ' '&&cmdbuf[i] != 0; i++,j++)  {         /* find command name       */
      cmdname[j] = cmdbuf[i];
    }
    cmdname[j] = '\0';
    
    /***************** Command "0" or "BOOTUPDATE": Update the application  *************************/
    if(strcmp(cmdname, "BOOTUPDATE") == 0 || strcmp(cmdname, "0") == 0) {
      partition = MicoFlashGetInfo( MICO_PARTITION_BOOTLOADER );
      if (findCommandPara(cmdbuf, "r", NULL, 0) != -1){
        printf ("\n\rRead Bootloader...\n\r");
        SerialUpload( partition->partition_owner, partition->partition_start_addr, "BootLoaderImage.bin", partition->partition_length );
        continue;
      }
      printf ("\n\rUpdating Bootloader...\n\r");
      err = MicoFlashDisableSecurity( MICO_PARTITION_BOOTLOADER, 0x0, partition->partition_length );
      require_noerr( err, exit);

      SerialDownload( partition->partition_owner, partition->partition_start_addr, partition->partition_length );
    }
    
    /***************** Command "1" or "FWUPDATE": Update the MICO application  *************************/
    else if(strcmp(cmdname, "FWUPDATE") == 0 || strcmp(cmdname, "1") == 0)	{
      partition = MicoFlashGetInfo( MICO_PARTITION_APPLICATION );
      if (findCommandPara(cmdbuf, "r", NULL, 0) != -1){
        printf ("\n\rRead application...\n\r");
        SerialUpload( partition->partition_owner, partition->partition_start_addr, "ApplicationImage.bin", partition->partition_length );
        continue;
      }
      printf ("\n\rUpdating application...\n\r");
      err = MicoFlashDisableSecurity( MICO_PARTITION_APPLICATION, 0x0, partition->partition_length );
      require_noerr( err, exit);
      SerialDownload( partition->partition_owner, partition->partition_start_addr, partition->partition_length ); 							   	
    }
    
    /***************** Command "2" or "PARAUPDATE": Update the application  *************************/
    else if(strcmp(cmdname, "PARUPDATE") == 0 || strcmp(cmdname, "2") == 0)  {
      if (findCommandPara(cmdbuf, "id", idStr, 0) != -1){
        if(Str2Int((uint8_t *)idStr, &id)==0 && id > 0 && id < MICO_PARTITION_MAX ){ //Found Flash start address
          printf ("\n\rIllegal start address.\n\r");
          continue;
        }
        partition = MicoFlashGetInfo( (mico_partition_t)id );
      }else{
        printf ("\n\rMiCO partition not found.\n\r");
        continue;
      }

      if( findCommandPara(cmdbuf, "e", NULL, 0) != -1 ){
        printf( "\n\rErasing %s...\n\r", partition->partition_description );

        err = MicoFlashDisableSecurity( (mico_partition_t)id, 0x0, partition->partition_length );
        require_noerr( err, exit);
        MicoFlashErase( (mico_partition_t)id, 0x0, partition->partition_length );
        continue;
      }
      if (findCommandPara(cmdbuf, "r", NULL, 0) != -1){
        printf ( "\n\rRead %s...\n\r", partition->partition_description );
        SerialUpload( partition->partition_owner, partition->partition_start_addr, "Image.bin", partition->partition_length );
        continue;
      }
      printf ("\n\rUpdating %s...\n\r", partition->partition_description );
      err = MicoFlashDisableSecurity( (mico_partition_t)id, 0x0, partition->partition_length );
      require_noerr( err, exit);
      SerialDownload( partition->partition_owner, partition->partition_start_addr, partition->partition_length );                        
    }
    
    /***************** Command "3" or "FLASHUPDATE": : Update the Flash  *************************/
    else if(strcmp(cmdname, "FLASHUPDATE") == 0 || strcmp(cmdname, "3") == 0) {
      if (findCommandPara(cmdbuf, "dev", flash_dev_str, 1) == -1  ){
        printf ("\n\rUnkown target type! Exiting...\n\r");
        continue;
      }
      
      if(Str2Int((uint8_t *)flash_dev_str, (int32_t *)&flash_dev)==0){ 
        printf ("\n\rDevice Number Err! Exiting...\n\r");
        continue;
      }
      if( flash_dev >= MICO_FLASH_MAX ){
        printf ("\n\rDevice Err! Exiting...\n\r");
        continue;
      }
      
      inputFlashArea = false;
      
      if (findCommandPara(cmdbuf, "start", startAddressStr, 10) != -1){
        if(Str2Int((uint8_t *)startAddressStr, &startAddress)==0){ //Found Flash start address
          printf ("\n\rIllegal start address.\n\r");
          continue;
        }else{
          if (findCommandPara(cmdbuf, "end", endAddressStr, 10) != -1){ //Found Flash end address
            if(Str2Int((uint8_t *)endAddressStr, &endAddress)==0){
              printf ("\n\rIllegal end address.\n\r");
              continue;
            }else{
              inputFlashArea = true;
            }
          }else{
            printf ("\n\rFlash end address not found.\n\r");
            continue;
          }
        }
      }
      
      if(endAddress<startAddress && inputFlashArea == true) {
        printf ("\n\rIllegal address.\n\r");
        continue;
      }
      
      if(inputFlashArea != true){
        startAddress = platform_flash_peripherals[ flash_dev ].flash_start_addr ;
        endAddress = platform_flash_peripherals[ flash_dev ].flash_start_addr 
          + platform_flash_peripherals[ flash_dev ].flash_length - 1;
      }
      
      if (findCommandPara(cmdbuf, "e", NULL, 0) != -1){
        printf ("\n\rErasing dev%d content From 0x%lx to 0x%lx\n\r", flash_dev, startAddress, endAddress);
        platform_flash_init( &platform_flash_peripherals[ flash_dev ] );
        platform_flash_disable_protect( &platform_flash_peripherals[ flash_dev ], startAddress, endAddress );
        platform_flash_erase( &platform_flash_peripherals[ flash_dev ], startAddress, endAddress );
        continue;
      }
      
      if (findCommandPara(cmdbuf, "r", NULL, 0) != -1){
        printf ("\n\rRead dev%d content From 0x%lx to 0x%lx\n\r", flash_dev, startAddress, endAddress);
        SerialUpload(flash_dev, startAddress, "FlashImage.bin", endAddress-startAddress+1);
        continue;
      }
      
      printf ("\n\rUpdating dev%d content From 0x%lx to 0x%lx\n\r", flash_dev, startAddress, endAddress);
      platform_flash_disable_protect( &platform_flash_peripherals[ flash_dev ], startAddress, endAddress );
      SerialDownload(flash_dev, startAddress, endAddress-startAddress+1);                           
    }
    
    
    /***************** Command: MEMORYMAP *************************/
    else if(strcmp(cmdname, "MEMORYMAP") == 0 || strcmp(cmdname, "4") == 0)  {
      printf("\r");
      for( i = 0; i <= MICO_PARTITION_MAX - 1; i++ ){
        partition = MicoFlashGetInfo( (mico_partition_t)i );
        if (partition->partition_owner == MICO_FLASH_NONE)
            continue;
        printf( "|ID:%d| %11s |  Dev:%d  | 0x%08lx | 0x%08lx |\r\n", i, partition->partition_description, partition->partition_owner,
               partition->partition_start_addr, partition->partition_length);
      }
    }
    /***************** Command: Excute the application *************************/
    else if(strcmp(cmdname, "BOOT") == 0 || strcmp(cmdname, "5") == 0)	{
      printf ("\n\rBooting.......\n\r");
      partition = MicoFlashGetInfo( MICO_PARTITION_APPLICATION );
      //bootloader_start_app( partition->partition_start_addr );
    }
    
    /***************** Command: Reboot *************************/
    else if(strcmp(cmdname, "REBOOT") == 0 || strcmp(cmdname, "6") == 0)  {
      printf ("\n\rReBooting.......\n\r");
      MicoSystemReboot();
      break;                              
    }
    
    else if(strcmp(cmdname, "GPIO") == 0 || strcmp(cmdname, "8") == 0)  {
      static char sel_Str[5];
      if (findCommandPara(cmdbuf, "0", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_SYS_LED );
        printf("\r\n triggered sys led\r\n");
      }
      else if (findCommandPara(cmdbuf, "1", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_EN );
        printf("\r\n triggered 5V \r\n");
      }
      else if (findCommandPara(cmdbuf, "2", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_EN );
        printf("\r\n triggered 12V \r\n");
      }
      else if (findCommandPara(cmdbuf, "3", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_24V_LEFT_HAND_MOTOR_EN );
        printf("\r\n triggered 24v left hand motor \r\n");
      }
      //
      else if (findCommandPara(cmdbuf, "4", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_24V_RIGHT_HAND_MOTOR_EN );
        printf("\r\n triggered 24v right hand motor \r\n");
      }//
      else if (findCommandPara(cmdbuf, "5", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_24V_HEAD_MOTOR_EN );
        printf("\r\n triggered 24v head motor \r\n");
      }//
      else if (findCommandPara(cmdbuf, "6", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_24V_HD_CAMERA_EN );
        printf("\r\n triggered 24v head camera \r\n");
      }//
      else if (findCommandPara(cmdbuf, "7", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_24V_CAMERA_EN );
        printf("\r\n triggered 24v camera \r\n");
      }
      else if (findCommandPara(cmdbuf, "8", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_RESERVE_EN );
        printf("\r\n triggered 5v reserve \r\n");
      }//
      else if (findCommandPara(cmdbuf, "9", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_LEFT_HAND_MOTOR_EN );
        printf("\r\n triggered 5v left hand motor \r\n");
      }//
      else if (findCommandPara(cmdbuf, "A", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_RIGHT_HAND_MOTOR_EN );
        printf("\r\n triggered 5v right hand motor \r\n");
      }
      else if (findCommandPara(cmdbuf, "B", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_HEAD_MOTOR_EN );
        printf("\r\n triggered 5v led X 2 \r\n");
      }//
      else if (findCommandPara(cmdbuf, "C", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_LEDX2_EN );
        printf("\r\n triggered x86 \r\n");
      }//
      else if (findCommandPara(cmdbuf, "D", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_HEAD_LED_EN );
        printf("\r\n triggered 5v head led \r\n");
      }//
      else if (findCommandPara(cmdbuf, "E", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_REPAIR_BOARD_EN );
        printf("\r\n triggered 5v repair board \r\n");
      }
      else if (findCommandPara(cmdbuf, "F", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_TOUCH_EN );
        printf("\r\n triggered 5v touch board \r\n");
      }//
      else if (findCommandPara(cmdbuf, "G", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_KEYBORAD_EN );
        printf("\r\n triggered 5v keyboard \r\n");
      }//
      else if (findCommandPara(cmdbuf, "H", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_CARD_READER_EN );
        printf("\r\n triggered 5v card reader\r\n");
      }//
      else if (findCommandPara(cmdbuf, "I", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_CODE_EN  );
        printf("\r\n triggered 5v code \r\n");
      }//
      else if (findCommandPara(cmdbuf, "J", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_HD_CAMERA_EN );
        printf("\r\n triggered 5v hd camera \r\n");
      }//
      else if (findCommandPara(cmdbuf, "K", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_HD_CAMERA_STOP_EN );
        printf("\r\n triggered 5v hd camera stop \r\n");
      }
      else if (findCommandPara(cmdbuf, "L", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_CAMERA_EN );
        printf("\r\n triggered 5v camera \r\n");
      }
      else if (findCommandPara(cmdbuf, "M", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_CAMERA_STOP_EN );
        printf("\r\n triggered 5v camera stop \r\n");
      }
      else if (findCommandPara(cmdbuf, "N", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_5V_HEAD_TOUCH_EN );
        printf("\r\n triggered 5v head touch board \r\n");
      }//
      else if (findCommandPara(cmdbuf, "O", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_SWITCH_EN );
        printf("\r\n triggered 12v switch \r\n");
      }
      else if (findCommandPara(cmdbuf, "P", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_ROUTER_EN );
        printf("\r\n triggered 12v router \r\n");
      }
      else if (findCommandPara(cmdbuf, "Q", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_CARD_READER_EN );
        printf("\r\n triggered 12v card reader \r\n");
      }
      else if (findCommandPara(cmdbuf, "R", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_AUDIO_PA_EN   );
        printf("\r\n triggered 12v audio pa \r\n");
      }
      else if (findCommandPara(cmdbuf, "S", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_HEAD_RK_EN );
        printf("\r\n triggered 12v head rk \r\n");
      }
      else if (findCommandPara(cmdbuf, "T", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_HEAD_RK_SIGNAL    );
        printf("\r\n triggered 12v head rk signal \r\n");
      }
      else if (findCommandPara(cmdbuf, "U", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_CHEST_RK_EN );
        printf("\r\n triggered 12v chest rk \r\n");
      }
      else if (findCommandPara(cmdbuf, "V", NULL, 0) != -1){
        MicoGpioOutputTrigger( GPIO_12V_CHEST_RK_SIGNAL );
        printf("\r\n triggered 12v chest rk signal \r\n");
      }
      
      else if ( findCommandPara(cmdbuf, "Q", sel_Str, 0) != -1 )
      {      
        int32_t        sel_num;
        Str2Int((uint8_t *)sel_Str, &sel_num );
        if( sel_num/10000 )
        {
          MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_EN );
          printf("\r\n switch en: 0\r\n");
        }
        else
        {
          MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_EN );
          printf("\r\n switch en: 1\r\n");
        }
        if( sel_num%10000/1000 )
        {
          MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL0 );
          printf("\r\n switch sel0: 1\r\n");
        }
        else
        {
          MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL0 );
          printf("\r\n switch sel0: 0\r\n");
        }
        if( sel_num%1000/100 )
        {
          MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL1 );
          printf("\r\n switch sel1: 1\r\n");
        }
        else
        {
          MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL1 );
          printf("\r\n switch sel1: 0\r\n");
        }
        if( sel_num%100/10 )
        {
          MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL2 );
          printf("\r\n switch sel2: 1\r\n");
        }
        else
        {
          MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL2 );
          printf("\r\n switch sel2: 0\r\n");
        }
        if( sel_num%10 )
        {
          MicoGpioOutputHigh( (mico_gpio_t)GPIO_SWITCH_SEL3 );
          printf("\r\n switch sel3: 1\r\n");
        }
        else
        {
          MicoGpioOutputLow( (mico_gpio_t)GPIO_SWITCH_SEL3 );
          printf("\r\n switch sel3: 0\r\n");
        }
      }
      else {
        printf(   "\r\n8 -0: sys led\r\n"
                      "8 -1: 5v\r\n"
                      "8 -2: 12v\r\n"
                      "8 -3: 24v left hand motor\r\n"
                      "8 -4: 24v right hand motor\r\n"
                      "8 -5: 24v head motor\r\n"
                      "8 -6: 24v head camera\r\n"
                      "8 -7: 24v camera\r\n"
                      "8 -8: 5v reserve\r\n"
                      "8 -9: 5v left hand motor\r\n"
                      "8 -A: 5v right hand motor\r\n"
                      "8 -B: 5v head motor \r\n"
                      "8 -C: 5v led X 2\r\n"
                      "8 -D: 5v head led \r\n"
                      "8 -E: 5v repair board\r\n"
                      "8 -F: 5v touch board \r\n"                      
                      "8 -G: 5v keyboard\r\n"
                      "8 -H: 5v card reader\r\n"
                      "8 -I: 5v code\r\n"
                      "8 -J: 5v hd camera\r\n"
                      "8 -K: 5v hd camera stop\r\n"
                      "8 -L: 5v camera\r\n"
                      "8 -M: 5v camera stop\r\n"
                      "8 -N: 5v head touch board\r\n"
                        "8 -O: 12v switch\r\n"
                        "8 -P: 12v router\r\n"
                        "8 -Q: 12v card reader\r\n"
                        "8 -R: 12v audio pa\r\n"
                        "8 -S: 12v head rk\r\n"
                        "8 -T: 12v head rk signal\r\n"
                        "8 -U: 12v chest rk\r\n"
                        "8 -V: 12v chest rk signal\r\n");
      }
      break;
    }
    
    else if(strcmp(cmdname, "CURRENT") == 0 || strcmp(cmdname, "C") == 0)  {
      //static char sel_Str[5];
      if (findCommandPara(cmdbuf, "0", NULL, 0) != -1){
       
        printf("\r\n5v right hand motor currents %d\r\n",voltageConvert->c_5V_right_hand_motor);
      }
      else if (findCommandPara(cmdbuf, "1", NULL, 0) != -1){
        
        printf("\r\n 24v hd camera currents %d\r\n",voltageConvert->c_24V_hd_camera);
      }
      
      else if (findCommandPara(cmdbuf, "2", NULL, 0) != -1){
        
       printf("\r\n24v head motor currents  %d\r\n",voltageConvert->c_24V_head_motor);
      }
      else if (findCommandPara(cmdbuf, "3", NULL, 0) != -1){
        
        printf("\r\n24v camera motor currents  %d\r\n",voltageConvert->c_24V_camera);
      }
      //
      else if (findCommandPara(cmdbuf, "4", NULL, 0) != -1){
        
        printf("\r\n5v keyboard currents  %d\r\n",voltageConvert->c_5V_keyboard);
      }//
      else if (findCommandPara(cmdbuf, "5", NULL, 0) != -1){
        
        printf("\r\n5v code board currents  %d\r\n",voltageConvert->c_5V_code);
      }//
      else if (findCommandPara(cmdbuf, "6", NULL, 0) != -1){
        
        printf("\r\n5v left hand motor currents  %d\r\n",voltageConvert->c_5V_left_hand_motor);
      }//
      else if (findCommandPara(cmdbuf, "7", NULL, 0) != -1){
        
        printf("\r\n5v card reader currents  %d\r\n",voltageConvert->c_5V_card_reader);
      }
      else if (findCommandPara(cmdbuf, "8", NULL, 0) != -1){
        
        printf("\r\n5v head currents  %d\r\n",voltageConvert->c_5V_head);
      }//
      else if (findCommandPara(cmdbuf, "9", NULL, 0) != -1){
        printf("\r\n24v right hand motor currents  %d\r\n",voltageConvert->c_24V_right_hand_motor);
      }//
      else if (findCommandPara(cmdbuf, "A", NULL, 0) != -1){
        printf("\r\n12v head rk currents  %d\r\n",voltageConvert->c_12V_head_rk);
      }
      else if (findCommandPara(cmdbuf, "B", NULL, 0) != -1){
        printf("\r\n12v chest rk currents  %d\r\n",voltageConvert->c_12V_chest_rk);
      }//
      else if (findCommandPara(cmdbuf, "C", NULL, 0) != -1){
        printf("\r\n24v left hand motor currents  %d\r\n",voltageConvert->c_24V_left_hand_motor);
      }//
      else if (findCommandPara(cmdbuf, "D", NULL, 0) != -1){
        printf("\r\n12v audio pa currents %d\r\n",voltageConvert->c_12V_audio_pa);
      }//
      else if (findCommandPara(cmdbuf, "E", NULL, 0) != -1){
        printf("\r\n5v repair board currents  %d\r\n",voltageConvert->c_5V_repair);
      }
      else if (findCommandPara(cmdbuf, "F", NULL, 0) != -1){
        printf("\r\n5v touch board currents  %d\r\n",voltageConvert->c_5V_touch);
      }//
      else if (findCommandPara(cmdbuf, "G", NULL, 0) != -1){
        printf("\r\n12v switch currents  %d\r\n",voltageConvert->c_12V_switch);
      }//
      else if (findCommandPara(cmdbuf, "H", NULL, 0) != -1){
       printf("\r\n12v router currents  %d\r\n",voltageConvert->c_12V_router);
      }//
      else if (findCommandPara(cmdbuf, "I", NULL, 0) != -1){
       printf("\r\nvbus currents  %d\r\n",voltageConvert->c_vbus);
      }//
      else if (findCommandPara(cmdbuf, "J", NULL, 0) != -1){
        printf("\r\n12 card reader currents  %d\r\n",voltageConvert->c_12V_card_reader);
      }//
      else if (findCommandPara(cmdbuf, "K", NULL, 0) != -1){
        printf("\r\n12v temperature  %d\r\n",voltageConvert->temp_12V_ts);
      }
      else if (findCommandPara(cmdbuf, "L", NULL, 0) != -1){
        printf("\r\n5v temperature  %d\r\n",voltageConvert->temp_5V_ts);
      }
      else if (findCommandPara(cmdbuf, "M", NULL, 0) != -1){
        printf("\r\nair temperature  %d\r\n",voltageConvert->temp_air_ts);
      }
      else if (findCommandPara(cmdbuf, "N", NULL, 0) != -1){
        printf("\r\n5v head touch currents  %d\r\n",voltageConvert->c_5V_head_touch);
      }//
      else if (findCommandPara(cmdbuf, "O", NULL, 0) != -1){
        printf("\r\n12v all currents  %d\r\n",voltageConvert->c_12V_all);
      }
      else if (findCommandPara(cmdbuf, "P", NULL, 0) != -1){
        printf("\r\n5v all currents  %d\r\n",voltageConvert->c_5V_all);
      }
      else if (findCommandPara(cmdbuf, "Q", NULL, 0) != -1){
        printf("\r\n5v head led currents  %d\r\n",voltageConvert->c_5V_head_led);
      }
      else if (findCommandPara(cmdbuf, "R", NULL, 0) != -1){
        printf("\r\n12v voltage  %d\r\n",voltageConvert->v_12V);
      }
      else if (findCommandPara(cmdbuf, "S", NULL, 0) != -1){
       printf("\r\n5v voltage  %d\r\n",voltageConvert->v_5V);
      }
      else if (findCommandPara(cmdbuf, "T", NULL, 0) != -1){
        printf("\r\nvbat voltage  %d\r\n",voltageConvert->v_bat);
      }
      else if (findCommandPara(cmdbuf, "U", NULL, 0) != -1){
        printf("\r\n 5v reserve currents %d\r\n",voltageConvert->c_5V_reserve);
      }
      else if (findCommandPara(cmdbuf, "V", NULL, 0) != -1){
        printf("\r\n5v led currents  %d\r\n",voltageConvert->c_5V_led);
      }
      else if (findCommandPara(cmdbuf, "W", NULL, 0) != -1){
        printf("\r\n 5v camera currents %d\r\n",voltageConvert->c_5V_camera);
      }
      else if (findCommandPara(cmdbuf, "X", NULL, 0) != -1){
        printf("\r\n5v hd camera currents  %d\r\n",voltageConvert->c_5V_hd_camera);
      }
      else if(findCommandPara(cmdbuf, "Z", NULL, 0) != -1){
        
        extern uint8_t is_need_print_adc_data;
        is_need_print_adc_data ^= 1;
#if 0
        printf("\r\n5v right hand motor currents %d\r\n",voltageConvert->c_5V_right_hand_motor);
        printf("24v hd camera currents %d\r\n",voltageConvert->c_24V_hd_camera);
        printf("24v head motor currents  %d\r\n",voltageConvert->c_24V_head_motor);
        printf("24v camera motor currents  %d\r\n",voltageConvert->c_24V_camera);
        printf("5v keyboard currents  %d\r\n",voltageConvert->c_5V_keyboard);
        printf("5v code board currents  %d\r\n",voltageConvert->c_5V_code);
        printf("5v left hand motor currents  %d\r\n",voltageConvert->c_5V_left_hand_motor);
        printf("5v card reader currents  %d\r\n",voltageConvert->c_5V_card_reader);
        printf("5v head currents  %d\r\n",voltageConvert->c_5V_head);
        printf("24v right hand motor currents  %d\r\n",voltageConvert->c_24V_right_hand_motor);
        printf("12v head rk currents  %d\r\n",voltageConvert->c_12V_head_rk);
        printf("12v chest rk currents  %d\r\n",voltageConvert->c_12V_chest_rk);
        printf("24v left hand motor currents  %d\r\n",voltageConvert->c_24V_left_hand_motor);
        printf("12v audio pa currents %d\r\n",voltageConvert->c_12V_audio_pa);
        printf("5v repair board currents  %d\r\n",voltageConvert->c_5V_repair);
        printf("5v touch board currents  %d\r\n",voltageConvert->c_5V_touch);
        printf("12v switch currents  %d\r\n",voltageConvert->c_12V_switch);
        printf("12v router currents  %d\r\n",voltageConvert->c_12V_router);
        printf("vbus currents  %d\r\n",voltageConvert->c_vbus);
        printf("12 card reader currents  %d\r\n",voltageConvert->c_12V_card_reader);
        printf("12v temperature  %d\r\n",voltageConvert->temp_12V_ts);
        printf("5v temperature  %d\r\n",voltageConvert->temp_5V_ts);
        printf("air temperature  %d\r\n",voltageConvert->temp_air_ts);
        printf("5v head touch currents  %d\r\n",voltageConvert->c_5V_head_touch);
        printf("12v all currents  %d\r\n",voltageConvert->c_12V_all);
        printf("5v all currents  %d\r\n",voltageConvert->c_5V_all);
        printf("5v head led currents  %d\r\n",voltageConvert->c_5V_head_led);
        printf("12v voltage  %d\r\n",voltageConvert->v_12V);
        printf("5v voltage  %d\r\n",voltageConvert->v_5V);
        printf("vbat voltage  %d\r\n",voltageConvert->v_bat);
        printf("5v reserve currents %d\r\n",voltageConvert->c_5V_reserve);
        printf("5v led currents  %d\r\n",voltageConvert->c_5V_led);
        printf("5v camera currents %d\r\n",voltageConvert->c_5V_camera);
        printf("5v hd camera currents  %d\r\n",voltageConvert->c_5V_hd_camera);
#endif
      }
      else {
        printf(   
#if 0
               "\r\n8 -0: 5v right hand motor currents\r\n"
                      "8 -1: 24v hd camera currents\r\n"
                      "8 -2: 24v head motor currents \r\n"
                      "8 -3: 24v camera motor currents\r\n"
                      "8 -4: 5v keyboard currents\r\n"
                      "8 -5: 5v code board currents\r\n"
                      "8 -6: 5v left hand motor currents\r\n"
                      "8 -7: 5v card reader currents\r\n"
                      "8 -8: 5v head currents\r\n"
                      "8 -9: 24v right hand motor currents\r\n"
                      "8 -A: 12v head rk currents\r\n"
                      "8 -B: 12v chest rk currents \r\n"
                      "8 -C: 24v left hand motor currents\r\n"
                      "8 -D: 12v audio pa currents \r\n"
                      "8 -E: 5v repair board currents\r\n"
                      "8 -F: 5v touch board currents \r\n"                      
                      "8 -G: 12v switch currents\r\n"
                      "8 -H: 12v router currents\r\n"
                      "8 -I: vbus currents\r\n"
                      "8 -J: 12 card reader currents\r\n"
                      "8 -K: 12v temperature\r\n"
                      "8 -L: 5v temperature\r\n"
                      "8 -M: air temperature\r\n"
                      "8 -N: 5v head touch currents \r\n"
                        "8 -O: 12v all currents\r\n"
                        "8 -P: 5v all currents\r\n"
                        "8 -Q: 5v head led currents\r\n"
                        "8 -R: 12v voltage\r\n"
                        "8 -S: 5v voltage\r\n"
                        "8 -T: vbat voltage\r\n"
                        "8 -U: 5v reserve currents\r\n"
                        "8 -V: 5v led currents\r\n"
                        "8 -W: 5v camera currents\r\n"
                        "8 -X: 5v hd camera currents\r\n"
#else
                    "\r\n8 -1: 5v right hand motor currents\r\n"
                        "8 -2: 24v hd camera currents\r\n"
                        "8 -3: 24v head motor currents \r\n"
                        "8 -4: 24v camera motor currents\r\n"
                        "8 -5: 5v keyboard currents\r\n"
                        "8 -6: 5v code board currents\r\n"
                        "8 -7: 5v left hand motor currents\r\n"
                        "8 -8: 5v card reader currents\r\n"
                        "8 -9: 5v head currents\r\n"
                        "8 -10: 24v right hand motor currents\r\n"
                        "8 -11: 12v head rk currents\r\n"
                        "8 -12: 12v chest rk currents \r\n"
                        "8 -13: 24v left hand motor currents\r\n"
                        "8 -14: 12v audio pa currents \r\n"
                        "8 -15: 5v repair board currents\r\n"
                        "8 -16: 5v touch board currents \r\n"                      
                        "8 -17: 12v switch currents\r\n"
                        "8 -18: 12v router currents\r\n"
                        "8 -19: vbus currents\r\n"
                        "8 -20: 12 card reader currents\r\n"
                        "8 -21: 12v temperature\r\n"
                        "8 -22: 5v temperature\r\n"
                        "8 -23: air temperature\r\n"
                        "8 -24: 5v head touch currents \r\n"
                        "8 -25: 12v all currents\r\n"
                        "8 -26: 5v all currents\r\n"
                        "8 -27: 5v head led currents\r\n"
                        "8 -28: 12v voltage\r\n"
                        "8 -29: 5v voltage\r\n"
                        "8 -30: vbat voltage\r\n"
                        "8 -31: 5v reserve currents\r\n"
                        "8 -32: 5v led currents\r\n"
                        "8 -33: 5v camera currents\r\n"
                        "8 -34: 5v hd camera currents\r\n"
#endif
                          );
      }
      break;
    }
    
    else if(strcmp(cmdname, "RK") == 0 || strcmp(cmdname, "D") == 0)  {
      //static char sel_Str[5];
      if (findCommandPara(cmdbuf, "0", NULL, 0) != -1){
        PowerOnDevices(POWER_HEAD_RK);
        //
      }
      else if (findCommandPara(cmdbuf, "1", NULL, 1) != -1){
        PowerOnDevices(POWER_CHEST_RK);
         //
      }
        else if (findCommandPara(cmdbuf, "2", NULL, 0) != -1){
            PowerOffDevices(POWER_HEAD_RK);
        //
        }
        else if (findCommandPara(cmdbuf, "3", NULL, 1) != -1){
            PowerOffDevices(POWER_CHEST_RK);
        //
        }
      else
      {
        printf("\r\nD -0: power on HEAD RK\r\n"
                   "D -1: power on CHEST RK\r\n"
                   "D -2: power off HEAD RK\r\n"
                   "D -3: power off CHEST RK\r\n");
      }
      break;
    }
    else if(strcmp(cmdname, "ADC") == 0 || strcmp(cmdname, "CC") == 0)  {
      
        extern uint8_t is_need_print_adc_data;
        is_need_print_adc_data ^= 1;
        break;
    }
    
#if 0
    else if( strcmp(cmdname, "IRLED") == 0 || strcmp(cmdname, "9") == 0 )
    {
      static char dutyStr[2], frequencyStr[6];
      int32_t   duty, freguency;
      if (findCommandPara(cmdbuf, "0", NULL, 0) != -1){
        startTps611xx();
        printf("\r\n start tps611xx work \r\n");
      }
      else if (findCommandPara(cmdbuf, "1", dutyStr, 0) != -1){
        Str2Int((uint8_t *)dutyStr, &duty );
#if (defined TPS_CONTROL_MODE) && (TPS_CONTROL_MODE == EASY_SCALE)
        brightness_dimming_by_duty( (uint8_t)duty );      
        printf("\r\n dimming duty:%d\r\n",duty );
#elif (defined TPS_CONTROL_MODE) && (TPS_CONTROL_MODE == PWM)
        if (findCommandPara(cmdbuf, "f", frequencyStr, 0) != -1)
        {
          Str2Int((uint8_t *)frequencyStr, &freguency );
          brightness_dimming( (uint32_t)freguency, (uint8_t)duty );
          //brightness_dimming_by_duty( (uint8_t)duty );
          printf("\r\n dimming frequency and duty:%d %d\r\n",freguency, duty );
        }
#endif
      }
      else
      {
        stopTps611xx( );
        printf("\r\n stop tps611xx work\r\n" );
      }
      break;
    }
#endif
#if 0
    else if( strcmp(cmdname, "SHOW") == 0 || strcmp(cmdname, "A") == 0 )
    {
      voltageDebug.printType = PRINT_NO;
      printf("\r\nadc sequence num: (mV)\r\n");
      printf("1: _5V_reserve1_currents\r\n");
      printf("2: _12V_reserve2_currents\r\n");
      printf("3: bat_voltage\r\n");
      printf("4: sys_all_currents\r\n");
      printf("5: dh_12V_currents\r\n");
      printf("6: dh_5V_currents\r\n");
      printf("7: sensor_currents\r\n");
      printf("8: dlp_currents\r\n");
      printf("9: motor_5v_currents\r\n");
      printf("10: _24V_reserve1_currents\r\n");
      printf("11: _2_1_pa_currents\r\n");
      printf("12: pad_currents\r\n");
      printf("13: printer_currents\r\n");
      printf("14: x86_currents\r\n");
      printf("15: motor_currents\r\n");
      printf("16: _5V_led_currents\r\n");

      printf("17: bat_nv_currents\r\n");
      printf("18: _12V_nv_currents\r\n");
      printf("19: _12V_router_currents\r\n");
      printf("20: dyp_currents\r\n");
      printf("21: ir_led_currents\r\n");
      printf("22: charger_currents\r\n");
      printf("23: charge_currents\r\n");
      printf("24: _24V_voltage\r\n");
      printf("25: _12V_voltage\r\n");
      printf("26: _5V_voltage\r\n");
      printf("27: _5V_currents\r\n");
      printf("28: _12V_currents\r\n");
      printf("29: _24V_currents\r\n");
      printf("30: ambient_temperature\r\n");
      printf("31: _5V_regulator_temp\r\n");
      printf("32: _12V_regulator_temp\r\n");
      printf("33: _24V_regulator_temp\r\n");
      printf("34: aiui_currents\r\n");
      printf("35: _5V_router_currents\r\n");
      break;
    }
#endif
#if 0
    else if(strcmp(cmdname, "VOL") == 0 || strcmp(cmdname, "B") == 0)	{
      voltageDebug.printType = PRINT_PEROID;
      voltageDebug.peroid = 500;
      break;
    }
#endif
    else if(strcmp(cmdname, "COMLOGOFF") == 0 || strcmp(cmdname, "C") == 0)  {
      break;
    }
    else if(strcmp(cmdname, "COMLOGON") == 0 || strcmp(cmdname, "D") == 0)  {
      break;
    }
#ifdef MIKE_TEST
    else if(strcmp(cmdname, "CHARGER") == 0 || strcmp(cmdname, "E") == 0)  {
      printf("\r\n have detected charger plugin: %d times\r\n", boardStatus->charger_times);
      break;
    }     
#endif
    else if(strcmp(cmdname, "HELP") == 0 || strcmp(cmdname, "?") == 0)	{
        printf ( menu, MODEL, SW_VERSION, HARDWARE_REVISION );  /* display command menu        */
      break;
    }
    else if(strcmp(cmdname, "") == 0 )	{                     
      break;
    }
    else{
      printf (ERROR_STR, "UNKNOWN COMMAND");
      break;
    }
    
#ifdef NO_BLOCK_MENU
  }
exit:
    memset( cmdbuf, 0x0, cnt );
    cnt = 0;
    printf("\n\r[%d] PowerBoard> ",HAL_GetTick());
    return;
#else
exit:
    continue;
  }
#endif

}
