#OS files included here from MiddleWare - They should be replaced by the ones from Makefile (touchgfx's own)
board_c_files := \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery.c \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery_audio.c \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery_lcd.c \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery_sdram.c \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery_qspi.c \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO/stm32f769i_discovery_sd.c 

board_c_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_adc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_adc_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_can.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_cec.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_cortex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dac.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dcmi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dma.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dma2d.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dma_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_dsi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_gpio.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_hcd.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_i2c.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_jpeg.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_ltdc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_nor.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_pwr_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_rcc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_rcc_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_rtc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_sai.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_sai_ex.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_sd.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_sdram.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_tim.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_hal_qspi.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_ll_fmc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_ll_usb.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver/stm32f7xx_ll_sdmmc.c \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/CMSIS/system_stm32f7xx.c

board_cpp_files := \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/bsp/STM32F7Instrumentation.cpp \
	$(touchgfx_path)/board/Mjolner/MIA0130/source/STM32F7HAL_MIA0130.cpp

board_cpp_files += \
	$(touchgfx_path)/framework/source/platform/hal/ST/mcu/stm32f7xx/STM32F7DMA.cpp \
	$(touchgfx_path)/framework/source/platform/core/arm/cortex-m/CortexMMCUInstrumentation.cpp
