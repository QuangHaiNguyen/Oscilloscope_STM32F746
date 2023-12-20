/*-------------------------------------------------------------------------*
 * File:  uEZPlatform_Settings.h
 *-------------------------------------------------------------------------*
 * Description:
 *     
 *-------------------------------------------------------------------------*/
#ifndef UEZPLATFORM_SETTINGS_H_
#define UEZPLATFORM_SETTINGS_H_

/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2012 Future Designs, Inc.
 *--------------------------------------------------------------------------
 * This file is part of the uEZ(R) distribution.  See the included
 * uEZLicense.txt or visit http://www.teamfdi.com/uez for details.
 *
 *    *===============================================================*
 *    |  Future Designs, Inc. can port uEZ(tm) to your own hardware!  |
 *    |             We can get you up and running fast!               |
 *    |      See http://www.teamfdi.com/uez for more details.         |
 *    *===============================================================*
 *
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include <Config.h>
#include <UEZNetwork.h>
#include <Types/GPIO.h>
#include <UEZPlatformAPI.h>

/*-------------------------------------------------------------------------*
 * Platform Settings:
 *-------------------------------------------------------------------------*/
#define UEZ_PROCESSOR                       NXP_LPC1788

#ifndef UEZ_NUM_HANDLES
#define UEZ_NUM_HANDLES           150 // more than the average number of handles
#endif

#ifndef PROCESSOR_OSCILLATOR_FREQUENCY
#define PROCESSOR_OSCILLATOR_FREQUENCY      120000000   // clocks per second (when using LCD, must be slower)
#endif

#ifndef SDRAM_CLOCK_FREQUENCY
    #define SDRAM_CLOCK_FREQUENCY               (PROCESSOR_OSCILLATOR_FREQUENCY/2)
#endif

#ifndef EMC_CLOCK_FREQUENCY
    #define EMC_CLOCK_FREQUENCY                 (PROCESSOR_OSCILLATOR_FREQUENCY/2)
#endif

#ifndef UEZBSP_SDRAM
    #define UEZBSP_SDRAM                        1
    #define UEZBSP_SDRAM_SIZE                   (8*1024*1024)
    #define UEZBSP_SDRAM_BASE_ADDR              0xA0000000
#endif

#ifndef UEZ_DEFAULT_AUDIO_LEVEL
    #define UEZ_DEFAULT_AUDIO_LEVEL  48
#endif

#ifndef UEZ_CONSOLE_READ_BUFFER_SIZE
    #define UEZ_CONSOLE_READ_BUFFER_SIZE        128 // bytes
#endif
#ifndef UEZ_CONSOLE_WRITE_BUFFER_SIZE
    #define UEZ_CONSOLE_WRITE_BUFFER_SIZE       128 // bytes
#endif

#ifndef LCD_DISPLAY_BASE_ADDRESS
    #define LCD_DISPLAY_BASE_ADDRESS            0xA0000000
#endif

#ifndef FATFS_MAX_MASS_STORAGE_DEVICES
    #define FATFS_MAX_MASS_STORAGE_DEVICES      2
#endif

#define UEZBSP_NOR_FLASH                    UEZ_ENABLE_FLASH
#define UEZBSP_NOR_FLASH_BASE_ADDRESS       0x80000000

#include "lwipopts.h"

#if (COMPILER_TYPE==IAR)
    #define NOP() __no_operation()
#endif

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
TBool UEZGUIIsExpansionBoardConnected(void);
TBool UEZGUIIsExpansionBoard70Connected(void);
TBool UEZGUIIsLoopbackBoardConnected(void);

void UEZPlatform_SSP0_Require(void);
void UEZPlatform_SSP1_Require(void);
void UEZPlatform_Accel0_Require(void);
void UEZPlatform_ADC0_Require(void);
void UEZPlatform_ADC0_0_Require(void);
void UEZPlatform_ADC0_1_Require(void);
void UEZPlatform_ADC0_2_Require(void);
void UEZPlatform_ADC0_3_Require(void);
void UEZPlatform_ADC0_4_Require(void);
void UEZPlatform_ADC0_5_Require(void);
void UEZPlatform_ADC0_6_Require(void);
void UEZPlatform_ADC0_7_Require(void);
void UEZPlatform_AudioAmp_Require(void);
void UEZPlatform_Backlight_Require(void);
void UEZPlatform_Console_Expansion_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_FullDuplex_UART_Require(
        const char *aHALSerialName,
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_FullDuplex_UART0_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_FullDuplex_UART1_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_FullDuplex_UART2_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_FullDuplex_UART3_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_Console_HalfDuplex_UART_Require(
        const char *aHALSerialName,
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize,
        T_uezGPIOPortPin aDriveEnablePortPin,
        TBool aDriveEnablePolarity,
        TUInt32 aDriveEnableReleaseTime);
void UEZPlatform_Console_ISPHeader_Require(
        TUInt32 aWriteBufferSize,
        TUInt32 aReadBufferSize);
void UEZPlatform_CRC0_Require(void);
void UEZPlatform_DAC0_Require(void);
void UEZPlatform_EEPROM_I2C_Require(void);
void UEZPlatform_EEPROM_LPC1788_Require(void);
void UEZPlatform_EEPROM0_Require(void);
void UEZPlatform_EMAC_Require(void);
void UEZPlatform_ERTC_Require(void);
void UEZPlatform_FileSystem_Require(void);
void UEZPlatform_I2C0_Require(void);
void UEZPlatform_I2C1_Require(void);
void UEZPlatform_I2C2_Require(void);
void UEZPlatform_IAP_Require(void);
void UEZPlatform_IRTC_Require(void);
void UEZPlatform_Flash0_Require(void);
void UEZPlatform_GPDMA0_Require(void);
void UEZPlatform_GPDMA1_Require(void);
void UEZPlatform_GPDMA2_Require(void);
void UEZPlatform_GPDMA3_Require(void);
void UEZPlatform_GPDMA4_Require(void);
void UEZPlatform_GPDMA5_Require(void);
void UEZPlatform_GPDMA6_Require(void);
void UEZPlatform_GPDMA7_Require(void);
void UEZPlatform_LCD_Require(void);
void UEZPlatform_MS0_Require(void);
void UEZPlatform_MS1_Require(void);
void UEZPlatform_PWM0_Require(void);
void UEZPlatform_PWM1_Require(void);
void UEZPlatform_RTC_Require(void);
void UEZPlatform_SDCard_Drive_Require(TUInt8 aDriveNum);
void UEZPlatform_Speaker_Require(void);
void UEZPlatform_Temp0_Require(void);
void UEZPlatform_Timer0_Require(void);
void UEZPlatform_Timer1_Require(void);
void UEZPlatform_Timer2_Require(void);
void UEZPlatform_Timer3_Require(void);
void UEZPlatform_Touchscreen_Require(void);
void UEZPlatform_USBDevice_Require(void);
void UEZPlatform_USBFlash_Drive_Require(TUInt8 aDriveNum);
void UEZPlatform_USBHost_Require(void);
void UEZPlatform_Watchdog_Require(void);
void UEZPlatform_WiredNetwork0_Require(void);

void UEZPlatform_Require(void);
void UEZPlatform_Standard_Require(void);
void UEZPlatform_Full_Require(void);
void UEZPlatform_Minimal_Require(void);

// Utility function to connect to the Wired Network
T_uezError UEZPlatform_WiredNetwork0_Connect(
        T_uezDevice *aNetwork,
        T_uezNetworkStatus *aStatus);

// Information available about the connected wired network
extern T_uezDevice G_network;
extern T_uezNetworkStatus G_networkStatus;

#endif // UEZPLATFORM_SETTINGS_H_
/*-------------------------------------------------------------------------*
 * End of File:  uEZPlatform_Settings.h
 *-------------------------------------------------------------------------*/
