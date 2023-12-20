board_include_paths := \
	$(touchgfx_path)/framework/include/platform/core/arm/cortex-m/CMSIS \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f7xx \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f7xx/STM32F7xx_HAL_Driver \
	$(touchgfx_path)/framework/include/platform/hal/ST/mcu/stm32f7xx/CMSIS \
	$(touchgfx_path)/board/Mjolner/MIA0130/include \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/include \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/include/vendor \
	$(touchgfx_path)/board/ST/STM32F769I-DISCO/source/vendor/STM32769I_DISCO 

c_compiler_options += -DST -DSTM32F769xx -DSTM32F769I_DISCO -DUSE_FREERTOS_8 -DUSE_OS_SYSTICK -DUSE_IOEXPANDER
cpp_compiler_options += -DST -DSTM32F769xx -DSTM32F769I_DISCO -DUSE_FREERTOS_8 -DUSE_OS_SYSTICK -DUSE_IOEXPANDER
