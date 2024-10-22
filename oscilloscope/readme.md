# STM32F746G_DISCO TBS.

The default IDE is set to STM32CubeIDE, to change IDE open the STM32F746G_DISCO.ioc with CubeMX and select from the supported IDEs (EWARM from version 8.50.9, MDK-ARM, and STM32CubeIDE). Supports flashing of the STM32F746G_DISCO board directly from TouchGFX Designer using GCC and STM32CubeProgrammer.Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage. 

This TBS is configured for 272 x 480 pixels 16bpp screen resolution.  

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ  - Pin PB4(D3), RENDER_TIME - Pin PC6(D1), FRAME_RATE  - Pin PC7(D0), MCU_ACTIVE  - Pin PG6(D2)
 