#include "mico.h"
#include "platform.h"
#include "platform_internal.h"
#include "platform_config.h"
#include "board_init.h"
#include "multi_channel_detection.h"
#include "voltage_detect.h"
#include "app_platform.h"
#include "upgrade_flash.h"
#include "can_protocol.h"

#define os_PowerBoard_log(format, ...)  custom_log("PowerBoard", format, ##__VA_ARGS__)

extern void Main_Menu(void);
#define Application_REVISION "v2.1"

#ifdef SIZE_OPTIMIZE
const t char menu[] =
"\r\n"
"PowerBoard application for %s, %s, HARDWARE_REVISION: %s\r\n"
"0:BOOTUPDATE,"
"1:FWUPDATE,"
"2:DRIVERUPDAT,"
"3:PARAUPDATE,"
"4:FLASHUPDATE,"
"5:MEMORYMAP,"
"6:BOOT,"
"7:REBOOT";
#else
const char menu[] =
"\r\n"
"PowerBoard application for %s, %s, HARDWARE_REVISION: %s\r\n"
"+ command -------------------------+ function ------------+\r\n"
"| 0:BOOTUPDATE    <-r>             | Update bootloader    |\r\n"
"| 1:FWUPDATE      <-r>             | Update application   |\r\n"
"| 2:PARUPDATE     <-id n><-r><-e>  | Update MICO partition|\r\n"
"| 3:FLASHUPDATE   <-dev device>    |                      |\r\n"
"|  <-e><-r><-start addr><-end addr>| Update flash content |\r\n"
"| 4:MEMORYMAP                      | List flash memory map|\r\n"
"| 5:BOOT                           | Excute application   |\r\n"
"| 6:REBOOT                         | Reboot               |\r\n"
"| 7:SHUTDOWN                       | Shutdown right now   |\r\n"
"| 8:GPIO 9:IRLed A:Stop vol print B:Start vol print       |\r\n"
"| C:Log Off D:Log On E:Print charger plugin times ?:help  |\r\n"
"+----------------------------------+----------------------+\r\n"
"|    (C) COPYRIGHT 2016 MUYE Corporation  By Driver Group |\r\n"
" Notes:\r\n"
" -e Erase only  -r Read from flash -dev flash device number\r\n"
"  -start flash start address -end flash start address\r\n"
" Example: Input \"4 -dev 0 -start 0x400 -end 0x800\": Update \r\n"
"          flash device 0 from 0x400 to 0x800\r\n";
#endif
int8_t isNeedAutoBoot( void );

int main( void )
{
  init_clocks();
  init_architecture();
  init_platform();
  
  printf ( menu, MODEL, SW_VERSION, HARDWARE_REVISION );

  os_PowerBoard_log( "System clock = %d Hz",HAL_RCC_GetHCLKFreq() );

  bsp_Init();
  Platform_Init();
  VolDetect_Init();
/*
  SerialLeds_Init();
  Protocol_Init();
  */
#if CAN_TEST
  MicoCanInitialize( MICO_CAN1 );
#endif
  if( !isNeedAutoBoot() )
  {
    PowerOnDevices();
  }
  for(;;)
  {
    Platform_Tick();
    VolDetect_Tick();
    Main_Menu();  
  }
}

static uint32_t    indicatorFreshCount = 0;

// period: 10ms
USED INLINE void user_period_tick( void )
{
  indicatorFreshCount ++;
  if( indicatorFreshCount > 50 )
  {
    indicatorFreshCount = 0;
    MicoGpioOutputTrigger( MICO_GPIO_SYS_LED );
  }
}

int8_t isNeedAutoBoot( void )
{
  OSStatus err = kNoErr;
  int8_t  needAutoBoot = -1;
  uint32_t flash_table_offset = 0x0;
  save_flash_data_t        flashTable;
  err = MicoFlashRead( MICO_PARTITION_PARAMETER_2, &flash_table_offset, (uint8_t *)&flashTable, sizeof(save_flash_data_t) );
  if( err )
  {
    printf("\r\n read flash data err\r\n");
    return -1;
  }
  
  if( 'Y' == flashTable.isComfirmedSoc )
  {
    init_soc( flashTable.last_soc_mws );
    flashTable.last_soc_mws = 0;
    flashTable.isComfirmedSoc = 'N';
  }
  if( 'Y' == flashTable.isNeedAutoBoot )
  {
    flashTable.isNeedAutoBoot = 'N';
    needAutoBoot = 0;
  }
  MICOBootConfiguration( &flashTable );
  
  return needAutoBoot;
}