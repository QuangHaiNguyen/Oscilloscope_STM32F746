/*****************************************************************************
 *
 *   Copyright(C) 2011, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * Embedded Artists AB assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. Embedded Artists AB
 * reserves the right to make changes in the software without
 * notification. Embedded Artists AB also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/

#include "lpc_types.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_ssp.h"
#include "lpc43xx_timer.h"

#include "tsc2046_touch.h"
#include "calibrate.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#ifndef ABS
#define ABS(x) (  ((int32_t)(x)) < 0 ? (-(x)) : (x))
#endif

#define CS_ON    (LPC_GPIO_PORT->CLR[7] |= (1UL << 15)) // PE.15 (gpio7[15]) -> low
#define CS_OFF   (LPC_GPIO_PORT->SET[7] |= (1UL << 15)) // PE.15 (gpio7[15]) -> high

#define TP_PENDOWN() ((LPC_GPIO_PORT->PIN[4] & (1<<1)) == 0) //P8.1 (gpio4[1])

#define SSP_PORT  (LPC_SSP0)
#define SSP_CLOCK (1500000)

#define ADS_START         (1 << 7)
#define ADS_A2A1A0_d_y    (1 << 4)  /* differential */
#define ADS_A2A1A0_d_z1   (3 << 4)  /* differential */
#define ADS_A2A1A0_d_z2   (4 << 4)  /* differential */
#define ADS_A2A1A0_d_x    (5 << 4)  /* differential */
#define ADS_A2A1A0_temp0  (0 << 4)  /* non-differential */
#define ADS_A2A1A0_vbatt  (2 << 4)  /* non-differential */
#define ADS_A2A1A0_vaux   (6 << 4)  /* non-differential */
#define ADS_A2A1A0_temp1  (7 << 4)  /* non-differential */
#define ADS_8_BIT         (1 << 3)
#define ADS_12_BIT        (0 << 3)
#define ADS_SER           (1 << 2)  /* non-differential */
#define ADS_DFR           (0 << 2)  /* differential */
#define ADS_PD10_PDOWN    (0 << 0)  /* lowpower mode + penirq */
#define ADS_PD10_ADC_ON   (1 << 0)  /* ADC on */
#define ADS_PD10_REF_ON   (2 << 0)  /* vREF on + penirq */
#define ADS_PD10_ALL_ON   (3 << 0)  /* ADC + vREF on */


#define READ_12BIT_DFR(d, adc, vref) (ADS_START | d \
  | ADS_12_BIT | ADS_DFR | \
  (adc ? ADS_PD10_ADC_ON : 0) | (vref ? ADS_PD10_REF_ON : 0))

#define READ_Y(vref)  (READ_12BIT_DFR(ADS_A2A1A0_d_y,  1, vref))
#define READ_Z1(vref) (READ_12BIT_DFR(ADS_A2A1A0_d_z1, 1, vref))
#define READ_Z2(vref) (READ_12BIT_DFR(ADS_A2A1A0_d_z2, 1, vref))
#define READ_X(vref)  (READ_12BIT_DFR(ADS_A2A1A0_d_x,  1, vref))
#define PWRDOWN       (READ_12BIT_DFR(ADS_A2A1A0_d_y,  0, 0))  /* LAST */ 

/* single-ended samples need to first power up reference voltage;
 * we leave both ADC and VREF powered
 */
#define READ_12BIT_SER(x) (ADS_START | x \
  | ADS_12_BIT | ADS_SER)

#define REF_ON  (READ_12BIT_DFR(ADS_A2A1A0_d_x, 1, 1))
#define REF_OFF (READ_12BIT_DFR(ADS_A2A1A0_d_y, 0, 0))


typedef struct
{
  uint32_t validatePattern1;
  MATRIX storedMatrix;
  uint32_t validatePattern2;
} tStoredCalData;


/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

static tStoredCalData storedCalData;
static uint32_t calibrated = 0;

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void pinConfig(void)
{
  // PE.15 FUNC4 => GPIO7[15] output for CS
  scu_pinmux(0xe, 15, GPIO_NOPULL, FUNC4);
  LPC_GPIO_PORT->DIR[7] |= (1UL << 15);

  // P8.1 FUNC0 => GPIO4[1] input for TP_IRQ
  scu_pinmux(0x8, 1, GPIO_NOPULL, FUNC0);
  LPC_GPIO_PORT->DIR[4] &= ~(1UL << 1);

  // PIN config SSP 0
  scu_pinmux(0xf, 0, SSP_IO, FUNC0); //SSP0_SCK
  scu_pinmux(0xf, 2, SSP_IO, FUNC2); //SSP0_MISO
  scu_pinmux(0xf, 3, SSP_IO, FUNC2); //SSP0_MOSI
  
  // CS is controlled via PE.15 and not SSP0_SSEL.
  // Pin PF.1 should be configured as GPIO and set high.
  scu_pinmux(0xf, 1, GPIO_NOPULL, FUNC4); //GPIO7[16]
  LPC_GPIO_PORT->DIR[7] |= (1UL << 16);
  LPC_GPIO_PORT->SET[7] |= (1UL << 16);
}


static uint16_t spiTransfer(uint8_t cmd)
{
  uint8_t data[3];
  uint8_t sdata[3];
  SSP_DATA_SETUP_Type sspCfg;

  sdata[0] = cmd;
  sdata[1] = 0xff;
  sdata[2] = 0xff;


  sspCfg.tx_data = sdata;
  sspCfg.rx_data = NULL;
  sspCfg.length  = 1; 

 
  CS_ON;
  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

  sspCfg.tx_data = NULL;
  sspCfg.rx_data = data;
  sspCfg.length  = 2; 
  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

  CS_OFF;

  return ((data[0] << 8) | data[1]);  
}

#define DEBOUNCE_MAX 10
#define DEBOUNCE_TOL  3

static int32_t getFilteredValue(int p)
{
  int32_t a[7];
  int32_t tmp = 0;
  int i = 0, j = 0;

  /* 
   * Median and averaging filter
   *
   * 1. Get 7 values
   * 2. Sort these values
   * 3. Take average of the 3 values in the middle
   */
  
  for (i = 0; i < 7; i++) {
    a[i] = spiTransfer(p);
  }

  // bubble sort
  for (i = 0; i < 7; i++) {
    for (j = 0; j < (7-(i+1)); j++) {
      if (a[j] > a[j+1]) {
        // swap
        tmp = a[j];
        a[j] = a[j+1];
        a[j+1] = tmp;
      }
    }    
  }

  // average of 3 values in the middle 
  return ((a[2]+a[3]+a[4])/3);
}


static void readAndFilter(int32_t *x, int32_t* y, int32_t* z)
{
  int32_t ix, iy, iz1, iz2 = 0;
  int32_t lastx, lasty, lastz1, lastz2 = 0;
  int i = 0;

  *x = 0;
  *y = 0;
  *z = 0;

  lasty = getFilteredValue(READ_Y(0));
  lasty >>= 3;
  if (lasty >= 4095) {
    lasty = 0;
  }

  lastx = getFilteredValue(READ_X(0));
  lastx >>= 3;
  if (lastx >= 4095) {
    lastx = 0;
  }

  lastz1 = getFilteredValue(READ_Z1(0));
  lastz1 >>= 3;

  lastz2 = getFilteredValue(READ_Z2(0));
  lastz2 >>= 3;


  if (lastx && lastz1) {
   *z = (lastx * ABS(lastz2 - lastz1)) / lastz1;
  }
  else {
   *z = 0;
  }
      
  if (*z > 10500) {
    *z = 0;
  }

  if (*z == 0) {
    return;
  }

  for (i = 0; i < DEBOUNCE_MAX; i++) {
    iy = getFilteredValue(READ_Y(0));
    iy >>= 3;   

    if (ABS (lasty - iy) <= DEBOUNCE_TOL) {
      break;
    } 

    lasty = iy;
  }

  for (i = 0; i < DEBOUNCE_MAX; i++) {
    ix = getFilteredValue(READ_X(0));
    ix >>= 3;
    if (ix > 4095) {
      ix = 0;
    }   

    if (ABS (lastx - ix) <= DEBOUNCE_TOL) {
      break;
    } 

    lastx = ix;
  }  

  for (i = 0; i < DEBOUNCE_MAX; i++) {
    iz1 = getFilteredValue(READ_Z1(0));
    iz1 >>= 3;   

    if (ABS (lastz1 - iz1) <= DEBOUNCE_TOL) {
      break;
    } 

    lastz1 = iz1;
  } 

  for (i = 0; i < DEBOUNCE_MAX; i++) {
    iz2 = getFilteredValue(READ_Z2(0));
    iz2 >>= 3;   

    if (ABS (lastz2 - iz2) <= DEBOUNCE_TOL) {
      break;
    } 

    lastz2 = iz2;
  }
  
  *x = ix;
  *y = iy;
    
  if (ix && iz1) {
   *z = (ix * ABS(iz2 - iz1)) / iz1;
  }
  else {
   *z = 0;
  }

  if (*z > 10500) {
    *z = 0;
  }
          
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Initialize the touch controller
 *
 *****************************************************************************/
void touch_init (void)
{
  uint8_t data[3];
  SSP_CFG_Type SSP_ConfigStruct;
  SSP_DATA_SETUP_Type sspCfg;

  pinConfig();
  CS_OFF;

        // initialize SSP configuration structure to default
        SSP_ConfigStructInit(&SSP_ConfigStruct);

  // set clock rate
  SSP_ConfigStruct.ClockRate = SSP_CLOCK;
  SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
  SSP_ConfigStruct.CPOL = SSP_CPOL_LO;

        // Initialize SSP peripheral with parameter given in structure above
        SSP_Init(SSP_PORT, &SSP_ConfigStruct);

        // Enable SSP peripheral
        SSP_Cmd(SSP_PORT, ENABLE);


  data[0] = REF_ON;
  data[1] = (READ_12BIT_SER(ADS_A2A1A0_vaux) | ADS_PD10_ALL_ON);
  data[2] = PWRDOWN;
  CS_ON;

  sspCfg.tx_data = data;
  sspCfg.rx_data = NULL;
  sspCfg.length  = 3; 

  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
  CS_OFF;
}

/******************************************************************************
 *
 * Description:
 *    Read coordinates from the touch panel. The values (especially z) will
 *    have the value 0 when there are no touch events.
 *
 * Params:
 *   [out] x, y, z 
 *
 * Returns:
 *   None
 *
 *****************************************************************************/
void touch_xyz(int32_t *x, int32_t* y, int32_t* z)
{
  int32_t ix, iy, iz = 0;
  POINT displayPoint, screenSample;
  SSP_DATA_SETUP_Type sspCfg;
  uint8_t pwrDown = PWRDOWN;

  readAndFilter(&ix, &iy, &iz);

  //spiTransfer(PWRDOWN);

  sspCfg.tx_data = &pwrDown;
  sspCfg.rx_data = NULL;
  sspCfg.length  = 1; 
 
  CS_ON;
  SSP_ReadWrite (SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
  CS_OFF;

  *z = iz;

  if (calibrated) 
  {
      screenSample.x = ix;
      screenSample.y = iy;
      getDisplayPoint( &displayPoint, &screenSample, 
        &(storedCalData.storedMatrix) ) ;
      *x = displayPoint.x;
      *y = displayPoint.y;
  } 
  else 
  {
    *x = ix;
    *y = iy;
  }  
}


/******************************************************************************
 *
 * Description:
 *    Store calibration data for the touch panel
 *
 * Params:
 *   [in] 
 *
 * Returns:
 *   None
 *
 *****************************************************************************/
void touch_calibrate(tTouchPoint ref1, tTouchPoint ref2, tTouchPoint ref3, 
  tTouchPoint scr1, tTouchPoint scr2, tTouchPoint scr3)
{
  POINT disp[3];
  POINT scr[3];
  
  disp[0].x = ref1.x;
  disp[0].y = ref1.y;
  disp[1].x = ref2.x;
  disp[1].y = ref2.y;
  disp[2].x = ref3.x;
  disp[2].y = ref3.y;
  
  scr[0].x = scr1.x;
  scr[0].y = scr1.y;                  
  scr[1].x = scr2.x;
  scr[1].y = scr2.y;
  scr[2].x = scr3.x;
  scr[2].y = scr3.y;  
  
  setCalibrationMatrix(disp, scr,
                       &storedCalData.storedMatrix);

  calibrated = TRUE;
  
}


/******************************************************************************
 *
 * Description:
 *    Marks the touch controller as "not calibrated", allowing a recalibration
 *
 * Params:
 *   None
 *
 * Returns:
 *   None
 *
 *****************************************************************************/
void touch_reinit(void)
{
  calibrated = FALSE;
}

