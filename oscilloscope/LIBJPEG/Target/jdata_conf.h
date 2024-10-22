/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : jdata_conf.h
  * Description        : This file provides header to "jdata_conf.h" module.
  *                      It implements also file based read/write functions.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
**/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/*Stdio is chosen for File storage*/
#include <stdio.h>

/*FreeRtos Api*/
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*This defines the memory allocation methods.*/
#define JMALLOC   pvPortMalloc
#define JFREE     vPortFree

/*This defines the File data manager type.*/
#define JFILE            FILE

#define JFREAD(file,buf,sizeofbuf)  \
((size_t) fread((void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))

#define JFWRITE(file,buf,sizeofbuf)  \
((size_t) fwrite((const void *) (buf), (size_t) 1, (size_t) (sizeofbuf), (file)))

