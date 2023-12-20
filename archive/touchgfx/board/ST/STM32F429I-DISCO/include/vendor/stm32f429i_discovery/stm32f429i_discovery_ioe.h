/**
  ******************************************************************************
  * @file    stm32f429i_discovery_ioe.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    28-October-2013
  * @brief   This file contains all the functions prototypes for the
  *          stm32f429i_discovery_ioe.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F429I_DISCOVERY_IOE_H
#define __STM32F429I_DISCOVERY_IOE_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery.h"
   
/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */ 

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
    
/** @defgroup STM32F429I_DISCOVERY_IOE   
  * @{
  */    


/** @defgroup STM32F429I_DISCOVERY_IOE_Exported_Types
  * @{
  */ 

/** 
  * @brief  Touch Panel Information structure  
  */ 
typedef struct
{
  uint16_t TouchDetected;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
}TP_STATE; 

/** 
  * @brief  IO_Expander Error codes  
  */ 
typedef enum
{
  IOE_OK = 0,
  IOE_FAILURE, 
  IOE_TIMEOUT,
  PARAM_ERROR,
  IOE_NOT_OPERATIONAL, 
}IOE_Status_TypDef;

/** 
  * @brief  IO bit values  
  */ 
typedef enum
{
  BitReset = 0,
  BitSet = 1
}IOE_BitValue_TypeDef;

/** 
  * @brief  IOE DMA Direction  
  */ 
typedef enum
{
  IOE_DMA_TX = 0,
  IOE_DMA_RX = 1
}IOE_DMADirection_TypeDef;

/**
  * @}
  */ 


/** @defgroup STM32F429I_DISCOVERY_IOE_Exported_Constants
  * @{
  */ 

/**
 * @brief Uncomment the line below to enable verfying each written byte in write
 *        operation. The I2C_WriteDeviceRegister() function will then compare the
 *        written and read data and return error status if a mismatch occurs.
 */
/* #define VERIFY_WRITTENDATA */

/**
 * @brief Uncomment the line below if you want to use user timeout callback.
 *        Function prototypes is declared in this file but function body may be
 *        implemented into user application.  
 */
/* #define USE_TIMEOUT_USER_CALLBACK */

/**
 * @brief Uncomment the line below if you want to use user defined Delay function
 *        (for precise timing), otherwise default _delay_ function defined within
 *         this driver is used (less precise timing).  
 */
/* #define USE_Delay */

#ifdef USE_Delay
#include "main.h"
 
  #define _delay_     Delay  /* !< User can provide more timing precise _delay_ function
                                   (with 10ms time base), using SysTick for example */
#else
  #define _delay_     delay      /* !< Default _delay_ function with less precise timing */
#endif    

/*------------------------------------------------------------------------------
    Hardware Configuration 
------------------------------------------------------------------------------*/
/** 
  * @brief  I2C port definitions  
  */ 
#define IOE_I2C                    I2C3
#define IOE_I2C_CLK                RCC_APB1Periph_I2C3
#define IOE_I2C_SCL_PIN            GPIO_Pin_8
#define IOE_I2C_SCL_GPIO_PORT      GPIOA
#define IOE_I2C_SCL_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define IOE_I2C_SCL_SOURCE         GPIO_PinSource8
#define IOE_I2C_SCL_AF             GPIO_AF_I2C3
#define IOE_I2C_SDA_PIN            GPIO_Pin_9
#define IOE_I2C_SDA_GPIO_PORT      GPIOC
#define IOE_I2C_SDA_GPIO_CLK       RCC_AHB1Periph_GPIOC
#define IOE_I2C_SDA_SOURCE         GPIO_PinSource9
#define IOE_I2C_SDA_AF             GPIO_AF_I2C3
#define IOE_I2C_DR                 ((uint32_t)0x40005C10)

/** 
  * @brief  IOE DMA definitions  
  */
#define IOE_DMA_CLK                RCC_AHB1Periph_DMA1
#define IOE_DMA_CHANNEL            DMA_Channel_1

#ifndef IOE_DMA_TX_STREAM
 #define IOE_DMA_TX_STREAM         DMA1_Stream6
#endif /* IOE_DMA_TX_STREAM */

#ifndef IOE_DMA_TX_TCFLAG 
 #define IOE_DMA_TX_TCFLAG         DMA_FLAG_TCIF6
#endif /* IOE_DMA_TX_TCFLAG */

#ifndef IOE_DMA_RX_STREAM
 #define IOE_DMA_RX_STREAM         DMA1_Stream0
#endif /* IOE_DMA_RX_STREAM */

#ifndef IOE_DMA_RX_TCFLAG 
 #define IOE_DMA_RX_TCFLAG         DMA_FLAG_TCIF0
#endif /* IOE_DMA_RX_TCFLAG */

/** 
  * @brief  IO Expander Interrupt line on EXTI  
  */ 
#define IOE_IT_PIN                 GPIO_Pin_2
#define IOE_IT_GPIO_PORT           GPIOI
#define IOE_IT_GPIO_CLK            RCC_AHB1Periph_GPIOI
#define IOE_IT_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOI
#define IOE_IT_EXTI_PIN_SOURCE     EXTI_PinSource2
#define IOE_IT_EXTI_LINE           EXTI_Line2
#define IOE_IT_EXTI_IRQn           EXTI2_IRQn   

/**
  * @brief Eval Board IO Exapander Pins definition 
  */ 
#define IO1_IN_ALL_PINS            (uint32_t)(MEMS_INT1_PIN | MEMS_INT2_PIN)
#define IO2_IN_ALL_PINS            (uint32_t)0x00
#define IO1_OUT_ALL_PINS           (uint32_t)(VBAT_DIV_PIN)
#define IO2_OUT_ALL_PINS           (uint32_t)(AUDIO_RESET_PIN | MII_INT_PIN)

/** 
  * @brief  The 7 bits IO Expanders addresses and chip IDs  
  */ 
#define IOE_ADDR                   0x82    
#define STMPE811_ID                0x0811


/*------------------------------------------------------------------------------
    Functional and Interrupt Management
------------------------------------------------------------------------------*/
/** 
  * @brief  IO Expander Functionalities definitions  
  */ 
#define IOE_ADC_FCT                0x01
#define IOE_TP_FCT                 0x02
#define IOE_IO_FCT                 0x04

/** 
  * @brief  Interrupt source configuration definitons  
  */ 
#define IOE_ITSRC_TP               0x01

/** 
  * @brief  Glaobal Interrupts definitions  
  */ 
#define IOE_GIT_GPIO               0x80
#define IOE_GIT_ADC                0x40
#define IOE_GIT_FE                 0x10
#define IOE_GIT_FF                 0x08
#define IOE_GIT_FOV                0x04
#define IOE_GIT_FTH                0x02
#define IOE_GIT_TOUCH              0x01


/*------------------------------------------------------------------------------
    STMPE811 device register definition
------------------------------------------------------------------------------*/
/** 
  * @brief  Identification registers  
  */ 
#define IOE_REG_CHP_ID             0x00
#define IOE_REG_ID_VER             0x02

/** 
  * @brief  General Control Registers  
  */ 
#define IOE_REG_SYS_CTRL1          0x03
#define IOE_REG_SYS_CTRL2          0x04
#define IOE_REG_SPI_CFG            0x08 

/** 
  * @brief  Interrupt Control register  
  */ 
#define IOE_REG_INT_CTRL           0x09
#define IOE_REG_INT_EN             0x0A
#define IOE_REG_INT_STA            0x0B
#define IOE_REG_GPIO_INT_EN        0x0C
#define IOE_REG_GPIO_INT_STA       0x0D

/** 
  * @brief  GPIO Registers  
  */ 
#define IOE_REG_GPIO_SET_PIN       0x10
#define IOE_REG_GPIO_CLR_PIN       0x11
#define IOE_REG_GPIO_MP_STA        0x12
#define IOE_REG_GPIO_DIR           0x13
#define IOE_REG_GPIO_ED            0x14
#define IOE_REG_GPIO_RE            0x15
#define IOE_REG_GPIO_FE            0x16
#define IOE_REG_GPIO_AF            0x17

/** 
  * @brief  ADC Registers  
  */ 
#define IOE_REG_ADC_INT_EN         0x0E
#define IOE_REG_ADC_INT_STA        0x0F
#define IOE_REG_ADC_CTRL1          0x20
#define IOE_REG_ADC_CTRL2          0x21
#define IOE_REG_ADC_CAPT           0x22
#define IOE_REG_ADC_DATA_CH0       0x30 
#define IOE_REG_ADC_DATA_CH1       0x32 
#define IOE_REG_ADC_DATA_CH2       0x34 
#define IOE_REG_ADC_DATA_CH3       0x36 
#define IOE_REG_ADC_DATA_CH4       0x38 
#define IOE_REG_ADC_DATA_CH5       0x3A 
#define IOE_REG_ADC_DATA_CH6       0x3B 
#define IOE_REG_ADC_DATA_CH7       0x3C 

/** 
  * @brief  TouchPanel Registers  
  */ 
#define IOE_REG_TP_CTRL            0x40
#define IOE_REG_TP_CFG             0x41
#define IOE_REG_WDM_TR_X           0x42 
#define IOE_REG_WDM_TR_Y           0x44
#define IOE_REG_WDM_BL_X           0x46
#define IOE_REG_WDM_BL_Y           0x48
#define IOE_REG_FIFO_TH            0x4A
#define IOE_REG_FIFO_STA           0x4B
#define IOE_REG_FIFO_SIZE          0x4C
#define IOE_REG_TP_DATA_X          0x4D 
#define IOE_REG_TP_DATA_Y          0x4F
#define IOE_REG_TP_DATA_Z          0x51
#define IOE_REG_TP_DATA_XYZ        0x52 
#define IOE_REG_TP_FRACT_XYZ       0x56
#define IOE_REG_TP_DATA            0x57
#define IOE_REG_TP_I_DRIVE         0x58
#define IOE_REG_TP_SHIELD          0x59



/*------------------------------------------------------------------------------
    Functions parameters defines
------------------------------------------------------------------------------*/
/**
  * @brief Touch Panel Pins definition 
  */ 
#define TOUCH_YD                   IO_Pin_1 
#define TOUCH_XD                   IO_Pin_2 
#define TOUCH_YU                   IO_Pin_3 
#define TOUCH_XU                   IO_Pin_4 
#define TOUCH_IO_ALL               (uint32_t)(IO_Pin_1 | IO_Pin_2 | IO_Pin_3 | IO_Pin_4)

/** 
  * @brief  IO Pins  
  */ 
#define IO_Pin_0                   0x01
#define IO_Pin_1                   0x02
#define IO_Pin_2                   0x04
#define IO_Pin_3                   0x08
#define IO_Pin_4                   0x10
#define IO_Pin_5                   0x20
#define IO_Pin_6                   0x40
#define IO_Pin_7                   0x80
#define IO_Pin_ALL                 0xFF

/** 
  * @brief  IO Pin directions  
  */ 
#define Direction_IN               0x00
#define Direction_OUT              0x01

/** 
  * @brief  Interrupt Line output parameters  
  */ 
#define Polarity_Low               0x00
#define Polarity_High              0x04
#define Type_Level                 0x00
#define Type_Edge                  0x02

/** 
  * @brief IO Interrupts  
  */ 
#define IO_IT_0                    0x01
#define IO_IT_1                    0x02
#define IO_IT_2                    0x04
#define IO_IT_3                    0x08
#define IO_IT_4                    0x10
#define IO_IT_5                    0x20
#define IO_IT_6                    0x40
#define IO_IT_7                    0x80
#define ALL_IT                     0xFF
#define IOE_TP_IT                  (uint8_t)(IO_IT_0 | IO_IT_1 | IO_IT_2)
#define IOE_INMEMS_IT              (uint8_t)(IO_IT_2 | IO_IT_3)

/** 
  * @brief  Edge detection value  
  */ 
#define EDGE_FALLING               0x01
#define EDGE_RISING                0x02

/** 
  * @brief  Global interrupt Enable bit  
  */ 
#define IOE_GIT_EN                 0x01

/**
  * @}
  */  

/** @defgroup STM32F429I_DISCOVERY_IOE_Exported_Functions
  * @{
  */ 

/** 
  * @brief  Configuration and initialization functions  
  */
uint8_t IOE_Config(void);
uint8_t IOE_TPITConfig(void);

/** 
  * @brief Interrupts Mangement functions
  */
FlagStatus  IOE_GetGITStatus(uint8_t Global_IT);
uint8_t     IOE_ClearGITPending(uint8_t IO_IT);

/** 
  * @brief IO-Expander Control functions
  */

uint8_t  IOE_IsOperational(void);
uint8_t  IOE_Reset(void);
uint16_t IOE_ReadID(void);
uint8_t  IOE_FnctCmd(uint8_t Fct, FunctionalState NewState);
uint8_t  IOE_GITCmd(FunctionalState NewState);
uint8_t  IOE_GITConfig(uint8_t Global_IT, FunctionalState NewState);
uint8_t  IOE_IOAFConfig(uint8_t IO_Pin, FunctionalState NewState);

/** 
  * @brief Touch Panel controller functions
  */
TP_STATE* IOE_TP_GetState(void);
uint8_t   IOE_TP_Config(void);

/** 
  * @brief Low Layer functions
  */
uint8_t  I2C_WriteDeviceRegister(uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t  I2C_ReadDeviceRegister(uint8_t RegisterAddr);
uint16_t I2C_ReadDataBuffer(uint32_t RegisterAddr);
uint8_t  I2C_DMA_WriteDeviceRegister(uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t  I2C_DMA_ReadDeviceRegister(uint8_t RegisterAddr);
uint16_t I2C_DMA_ReadDataBuffer(uint32_t RegisterAddr);


/** 
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with IO Expander. Only prototype
  *         of this function is declared in IO Expander driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and IO Expander.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.          
  */

uint8_t  IOE_TimeoutUserCallback(void);

#ifdef __cplusplus
}
#endif
#endif /* __STM32F429I_DISCOVERY_IOE_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */       
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
