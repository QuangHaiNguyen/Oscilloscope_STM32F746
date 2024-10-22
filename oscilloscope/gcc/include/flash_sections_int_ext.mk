_extflash_:
ifeq ($(program_files_path),)
	$(error $(error_msg))
endif
	@cd "$(st_stm32cube_programmer_path)" && ./$(stm32cube_programmer_filename) -c port=SWD -d $(application_path)/$(binary_output_path)/target.hex -el $(stm32cube_loader_relative_path) -hardRst
_intflash_:
ifeq ($(program_files_path),)
	$(error $(error_msg))
endif
	@cd "$(st_stm32cube_programmer_path)" && ./$(stm32cube_programmer_filename) -c port=SWD -d $(application_path)/$(binary_output_path)/intflash.hex -hardRst
