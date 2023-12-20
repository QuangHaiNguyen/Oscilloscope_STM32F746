#define UEZ_LIBRARY                 1
#define UEZ_PROCESSOR               NXP_LPC1788
#define COMPILER_TYPE               IAR

#define UEZ_DEFAULT_LCD_CONFIG              LCD_CONFIG_SHARP_LQ043T1DG01

#define FATFS_MAX_MASS_STORAGE_DEVICES      2

#define UEZ_LCD_COLOR_DEPTH         UEZLCD_COLOR_DEPTH_16_BIT

#define UEZ_LCD_DISPLAY_HEIGHT      UEZPlatform_LCDGetHeight()
#define UEZ_LCD_DISPLAY_WIDTH       UEZPlatform_LCDGetWidth()
#define FRAME(x)                    UEZPlatform_LCDGetFrame(x)
#define LCD_FRAME_BUFFER            UEZPlatform_LCDGetFrameBuffer()
#define FRAME_SIZE                  UEZPlatform_LCDGetFrameSize()
#define GUI_COLOR_CONVERSION        UEZPlatform_GUIColorConversion()
#define LCD_DISPLAY_BASE_ADDRESS    UEZPlatform_GetBaseAddress()


#define SERIAL_PORTS_DEFAULT_BAUD   UEZPlatform_SerialGetDefaultBaud()

#define PROCESSOR_OSCILLATOR_FREQUENCY  120000000

#define INTERRUPT_BASED_EMAC        1

