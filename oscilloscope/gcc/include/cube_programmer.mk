stm32cube_programmer_path := STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin
stm32cube_programmer_filename := STM32_Programmer_CLI.exe
stm32cube_programmer_filepath := $(stm32cube_programmer_path)/$(stm32cube_programmer_filename)
stm32cube_loader_relative_path := ExternalLoader/$(stldr)
stm32cube_loader := $(stm32cube_programmer_path)/$(stm32cube_loader_relative_path)

program_files_win32 = /e/Programs
program_files_win64 = /e/Programs
program_files_CLT = /c/ST/STM32CubeCLT/STM32CubeCLT/

# Check which tools are installed
ifneq ("$(shell find "$(program_files_win64)/$(stm32cube_programmer_filepath)" 2> nul )","")
program_files_path := $(program_files_win64)
else ifneq ("$(shell find "$(program_files_win32)/$(stm32cube_programmer_filepath)" 2> nul )","")
program_files_path := $(program_files_win32)
else ifneq ("$(shell find "$(program_files_CLT)/$(stm32cube_programmer_filepath)" 2> nul )","")
program_files_path := $(program_files_CLT)
endif

st_stm32cube_programmer_path := $(program_files_path)/$(stm32cube_programmer_path)
st_stm32cube_programmer_filepath := $(program_files_path)/$(stm32cube_programmer_filepath)
st_stm32cube_external_loader := $(program_files_path)/$(stm32cube_loader)

# define variable n as some line breaks for the error message
define error_msg

 

****************************************************************************************************************************
*                                                                                                                          *
* STM32CubeProgrammer is not found. Make sure the STM32CubeProgrammer is installed in its default directory.               *
* You can download it at https://www.st.com/en/development-tools/stm32cubeprog.html                                        *
*                                                                                                                          *
****************************************************************************************************************************
 
 

endef