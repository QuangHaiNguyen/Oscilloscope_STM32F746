# Oscilloscope on STM32F746 Discovery kit

## Introduction
Oscilloscope application running on STM32F746 discovery kit/
Capabilities:
* 2 channel 
* Sampling rate 1 mega-sample/s
* Maximuminput voltage 3.3V
* Rising/Falling edge 
* Adjust voltage level, time scale
* Graphical stack: TouchGFX, supporting touch 
* Marker for signal mesurement 
## Requirements 
### Hardware
* STM32F746 Discovery Kit
### Software
These tools are used for developing the application 
* Microsoft Visual Studio (Graphical interface development)
* Keil (Embedded firmware development)
This tool is used for flasing the application 
* STM32 ST-Link Utility
## Flashing the application 
* Start STM32 ST-Link utility, connect the board to PC, press **Connect to the target** button
* On the top menu bar, choose *External Loader -> Add External Loader -> Choose N25Q128A_STM32F746G-DISCO -> Click Validate*
* Click **Program verify**, click ***Start**
