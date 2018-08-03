/***********************************************************************//**
 * @file                lpc18xx_clkpwr.c
 * @brief               Contains all functions support for Clock and Power Control
 *                              firmware library on LPC18xx
 * @version             1.0
 * @date                14. Dec. 2010
 * @author              NXP MCU SW Application Team
 **************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup CLKPWR
 * @{
 */

/* Includes ------------------------------------------------------------------- */

#include "lpc_types.h"
#include "lpc18xx_scu.h"
#include "lpc18xx_clkpwr.h"

/*TODO List:
 * SET PLL0
 * UPDATE Clock from PLL0
 * SetDIV uncheck value
 * GetBaseStatus BASE_SAFE
 * */

/* Local definition */
#define CLKPWR_ADDRESS32(x,y) (*(uint32_t*)((uint32_t)x+y))

/* Local Variable */
const int16_t CLKPWR_Entity_ControlReg_Offset[CLKPWR_ENTITY_NUM] = {
                -1,             //CLKPWR_CLKSRC_32KHZ_OSC,
                -1,             //CLKPWR_CLKSRC_IRC,
                -1,             //CLKPWR_CLKSRC_ENET_RX_CLK,
                -1,             //CLKPWR_CLKSRC_ENET_TX_CLK,
                -1,             //CLKPWR_CLKSRC_GP_CLKIN,
                -1,             //CLKPWR_CLKSRC_TCK,
                0x18,   //CLKPWR_CLKSRC_XTAL_OSC,
                0x20,   //CLKPWR_CLKSRC_PLL0,
                0x30,   //CLKPWR_CLKSRC_PLL1,
                -1,             //CLKPWR_CLKSRC_RESERVE,
                -1,             //CLKPWR_CLKSRC_RESERVE,
                0x34,   //CLKPWR_CLKSRC_IDIVA,,
                0x38,   //CLKPWR_CLKSRC_IDIVB,
                0x3C,   //CLKPWR_CLKSRC_IDIVC,
                0x40,   //CLKPWR_CLKSRC_IDIVD,
                0x44,   //CLKPWR_CLKSRC_IDIVE,

                0x48,   //CLKPWR_BASE_SAFE,
                0x4C,   //CLKPWR_BASE_USB0,
                -1,             //CLKPWR_BASE_RESERVE,
                0x54,   //CLKPWR_BASE_USB1,
                0x58,   //CLKPWR_BASE_M3,
                0x5C,   //CLKPWR_BASE_SPIFI,
                -1,             //CLKPWR_BASE_RESERVE,
                0x64,   //CLKPWR_BASE_PHY_RX,
                0x68,   //CLKPWR_BASE_PHY_TX,
                0x6C,   //CLKPWR_BASE_APB1,
                0x70,   //CLKPWR_BASE_APB3,
                0x74,   //CLKPWR_BASE_LCD,
                -1,             //CLKPWR_BASE_RESERVE
                0x7C,   //CLKPWR_BASE_SDIO,
                0x80,   //CLKPWR_BASE_SSP0,
                0x84,   //CLKPWR_BASE_SSP1,
                0x88,   //CLKPWR_BASE_UART0,
                0x8C,   //CLKPWR_BASE_UART1,
                0x90,   //CLKPWR_BASE_UART2,
                0x94,   //CLKPWR_BASE_UART3,
                0x98    //CLKPWR_BASE_CLKOUT
};

const uint8_t CLKPWR_ConnectAlloc_Tbl[CLKPWR_CLKSRC_NUM][CLKPWR_ENTITY_NUM] = {
//       3 I E E G T X P P x x D D D D D S U x U M S x P P A A L x S S S U U U U C
//       2 R R T P C T L L     I I I I I A S   S 3 P   H H P P C   D S S R R R R O
//         C X X I K A 0 1     A B C D E F B   B   F   RxTx1 3 D   I 0 1 0 1 2 3
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_32KHZ_OSC = 0,*/
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_IRC,*/
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_ENET_RX_CLK,*/
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_ENET_TX_CLK,*/
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_GP_CLKIN,*/
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},/*CLKPWR_CLKSRC_TCK,*/
                {0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_XTAL_OSC,*/
                {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},/*CLKPWR_CLKSRC_PLL0,*/
                {0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_PLL1,*/
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_IDIVA = CLKPWR_CLKSRC_PLL1 + 3,*/
                {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_IDIVB,*/
                {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_IDIVC,*/
                {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1},/*CLKPWR_CLKSRC_IDIVD,*/
                {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1}/*CLKPWR_CLKSRC_IDIVE,*/
};

const CLKPWR_PERIPHERAL_S CLKPWR_PERIPHERAL_Info[CLKPWR_PERIPHERAL_NUM] = {
        /*      Register Clock                  |       Peripheral Clock
                 |      BASE    |               BRANCH  |       BASE            |       BRANCH          */
                {CLKPWR_BASE_APB3,      0x1118, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_ADC0,
                {CLKPWR_BASE_APB3,      0x1120, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_ADC1,
                {CLKPWR_BASE_M3,        0x1460, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_AES,
                ////    CLKPWR_PERIPHERAL_ALARMTIMER_CGU_RGU_RTC_WIC,
                {CLKPWR_BASE_APB1,      0x1200, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_APB1_BUS,
                {CLKPWR_BASE_APB3,      0x1100, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_APB3_BUS,
                {CLKPWR_BASE_APB3,      0x1128, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_CAN,
                {CLKPWR_BASE_M3,        0x1538, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_CREG,
                {CLKPWR_BASE_APB3,      0x1110, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_DAC,
                {CLKPWR_BASE_M3,        0x1440, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_DMA,
                {CLKPWR_BASE_M3,        0x1430, CLKPWR_BASE_M3,         0x1478, 0},//CLKPWR_PERIPHERAL_EMC,
                {CLKPWR_BASE_M3,        0x1420, CLKPWR_BASE_PHY_RX,     0x0000, CLKPWR_PERIPHERAL_ETHERNET_TX},//CLKPWR_PERIPHERAL_ETHERNET,
                {CLKPWR_ENTITY_NONE,0x0000, CLKPWR_BASE_PHY_TX, 0x0000, 0},//CLKPWR_PERIPHERAL_ETHERNET_TX
                {CLKPWR_BASE_M3,        0x1410, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_GPIO,
                {CLKPWR_BASE_APB1,      0x1210, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_I2C0,
                {CLKPWR_BASE_APB3,      0x1108, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_I2C1,
                {CLKPWR_BASE_APB1,      0x1218, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_I2S,
                {CLKPWR_BASE_M3,        0x1418, CLKPWR_BASE_LCD,        0x0000, 0},//CLKPWR_PERIPHERAL_LCD,
                {CLKPWR_BASE_M3,        0x1448, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_M3CORE,
                {CLKPWR_BASE_M3,        0x1400, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_M3_BUS,
                {CLKPWR_BASE_APB1,      0x1208, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_MOTOCON,
                {CLKPWR_BASE_M3,        0x1630, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_QEI,
                {CLKPWR_BASE_M3,        0x1600, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_RITIMER,
                {CLKPWR_BASE_M3,        0x1468, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_SCT,
                {CLKPWR_BASE_M3,        0x1530, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_SCU,
                {CLKPWR_BASE_M3,        0x1438, CLKPWR_BASE_SDIO,       0x2800, 0},//CLKPWR_PERIPHERAL_SDIO,
                {CLKPWR_BASE_M3,        0x1408, CLKPWR_BASE_SPIFI,      0x1300, 0},//CLKPWR_PERIPHERAL_SPIFI,
                {CLKPWR_BASE_M3,        0x1518, CLKPWR_BASE_SSP0,       0x2700, 0},//CLKPWR_PERIPHERAL_SSP0,
                {CLKPWR_BASE_M3,        0x1628, CLKPWR_BASE_SSP1,       0x2600, 0},//CLKPWR_PERIPHERAL_SSP1,
                {CLKPWR_BASE_M3,        0x1520, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_TIMER0,
                {CLKPWR_BASE_M3,        0x1528, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_TIMER1,
                {CLKPWR_BASE_M3,        0x1618, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_TIMER2,
                {CLKPWR_BASE_M3,        0x1620, CLKPWR_ENTITY_NONE,     0x0000, 0},//CLKPWR_PERIPHERAL_TIMER3,
                {CLKPWR_BASE_M3,        0x1508, CLKPWR_BASE_UART0,      0x2500, 0},//CLKPWR_PERIPHERAL_UART0,
                {CLKPWR_BASE_M3,        0x1510, CLKPWR_BASE_UART1,      0x2400, 0},//CLKPWR_PERIPHERAL_UART1,
                {CLKPWR_BASE_M3,        0x1608, CLKPWR_BASE_UART2,      0x2300, 0},//CLKPWR_PERIPHERAL_UART2,
                {CLKPWR_BASE_M3,        0x1610, CLKPWR_BASE_UART3,      0x2200, 0},//CLKPWR_PERIPHERAL_UART3,
                {CLKPWR_BASE_M3,        0x1428, CLKPWR_BASE_USB0,       0x1800, 0},//CLKPWR_PERIPHERAL_USB0,
                {CLKPWR_BASE_M3,        0x1470, CLKPWR_BASE_USB1,       0x1900, 0},//CLKPWR_PERIPHERAL_USB1,
                {CLKPWR_BASE_M3,        0x1500, CLKPWR_BASE_SAFE,       0x0000, 0},//CLKPWR_PERIPHERAL_WWDT,
};

uint32_t CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_NUM] = {0,12000000,0,0,0,0, 0, 480000000,0,0,0,0,0,0,0,0};

#define CLKPWR_CGU_ADDR ((uint32_t)LPC_CGU)
#define CLKPWR_REG_BASE_CTRL(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_Entity_ControlReg_Offset[CLKPWR_PERIPHERAL_Info[x].RegBaseEntity]))
#define CLKPWR_REG_BRANCH_CTRL(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_PERIPHERAL_Info[x].RegBranchOffset))
#define CLKPWR_REG_BRANCH_STATUS(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_PERIPHERAL_Info[x].RegBranchOffset+4))

#define CLKPWR_PER_BASE_CTRL(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_Entity_ControlReg_Offset[CLKPWR_PERIPHERAL_Info[x].PerBaseEntity]))
#define CLKPWR_PER_BRANCH_CTRL(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_PERIPHERAL_Info[x].PerBranchOffset))
#define CLKPWR_PER_BRANCH_STATUS(x) (*(uint32_t*)(CLKPWR_CGU_ADDR+CLKPWR_PERIPHERAL_Info[x].PerBranchOffset+4))

/**
 * Init Default Clock for LPC1800 Eval Board
 */
uint32_t        CLKPWR_Init(void){
        CLKPWR_SetXTALOSC(12000000);
        CLKPWR_EnableEntity(CLKPWR_CLKSRC_XTAL_OSC, ENABLE);
        CLKPWR_EntityConnect(CLKPWR_CLKSRC_XTAL_OSC, CLKPWR_CLKSRC_PLL1);
        // Disable PLL1 CPU hang???
        //CLKPWR_EnableEntity(CLKPWR_CLKSRC_PLL1, DISABLE);
        CLKPWR_SetPLL1(6);
        CLKPWR_EnableEntity(CLKPWR_CLKSRC_PLL1, ENABLE);
        CLKPWR_EntityConnect(CLKPWR_CLKSRC_PLL1, CLKPWR_BASE_M3);
        CLKPWR_UpdateClock();
        return 0;
}

uint32_t CLKPWR_ConfigPWR (CLKPWR_PERIPHERAL_T PPType,  FunctionalState en){
        if(PPType >= CLKPWR_PERIPHERAL_WWDT && PPType <= CLKPWR_PERIPHERAL_ADC0)
                return CLKPWR_ERROR_INVALID_PARAM;
        if(en == DISABLE){/* Going to disable clock */
                /*Get Reg branch status */
                if(CLKPWR_PERIPHERAL_Info[PPType].RegBranchOffset!= 0 &&
                                CLKPWR_REG_BRANCH_STATUS(PPType) & 1){
                        CLKPWR_REG_BRANCH_CTRL(PPType) &= ~1; /* Disable branch clock */
                        while(CLKPWR_REG_BRANCH_STATUS(PPType) & 1);
                }
                /* GetBase Status*/
                if((CLKPWR_PERIPHERAL_Info[PPType].RegBaseEntity!=CLKPWR_ENTITY_NONE) &&
                        CLKPWR_GetBaseStatus((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].RegBaseEntity) == 0){
                        /* Disable Base */
                        CLKPWR_EnableEntity((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].RegBaseEntity,0);
                }

                /* Same for Peripheral */
                if((CLKPWR_PERIPHERAL_Info[PPType].PerBranchOffset!= 0) && (CLKPWR_PER_BRANCH_STATUS(PPType) & 1)){
                        CLKPWR_PER_BRANCH_CTRL(PPType) &= ~1; /* Disable branch clock */
                        while(CLKPWR_PER_BRANCH_STATUS(PPType) & 1);
                }
                /* GetBase Status*/
                if((CLKPWR_PERIPHERAL_Info[PPType].PerBaseEntity!=CLKPWR_ENTITY_NONE) &&
                        CLKPWR_GetBaseStatus((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].PerBaseEntity) == 0){
                        /* Disable Base */
                        CLKPWR_EnableEntity((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].PerBaseEntity,0);
                }
        }else{
                /* enable */
                /* GetBase Status*/
                if((CLKPWR_PERIPHERAL_Info[PPType].RegBaseEntity!=CLKPWR_ENTITY_NONE) && CLKPWR_REG_BASE_CTRL(PPType) & 1){
                        /* Enable Base */
                        CLKPWR_EnableEntity((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].RegBaseEntity, 1);
                }
                /*Get Reg branch status */
                if((CLKPWR_PERIPHERAL_Info[PPType].RegBranchOffset!= 0) && !(CLKPWR_REG_BRANCH_STATUS(PPType) & 1)){
                        CLKPWR_REG_BRANCH_CTRL(PPType) |= 1; /* Enable branch clock */
                        while(!(CLKPWR_REG_BRANCH_STATUS(PPType) & 1));
                }

                /* Same for Peripheral */
                /* GetBase Status*/
                if((CLKPWR_PERIPHERAL_Info[PPType].PerBaseEntity != CLKPWR_ENTITY_NONE) &&
                                (CLKPWR_PER_BASE_CTRL(PPType) & 1)){
                        /* Enable Base */
                        CLKPWR_EnableEntity((CLKPWR_ENTITY_T)CLKPWR_PERIPHERAL_Info[PPType].PerBaseEntity, 1);
                }
                /*Get Reg branch status */
                if((CLKPWR_PERIPHERAL_Info[PPType].PerBranchOffset!= 0) && !(CLKPWR_PER_BRANCH_STATUS(PPType) & 1)){
                        CLKPWR_PER_BRANCH_CTRL(PPType) |= 1; /* Enable branch clock */
                        while(!(CLKPWR_PER_BRANCH_STATUS(PPType) & 1));
                }

        }

        if(CLKPWR_PERIPHERAL_Info[PPType].next){
                return CLKPWR_ConfigPWR((CLKPWR_PERIPHERAL_T)CLKPWR_PERIPHERAL_Info[PPType].next, en);
        }
        return CLKPWR_ERROR_SUCCESS;
}

uint32_t CLKPWR_GetPCLKFrequency (CLKPWR_PERIPHERAL_T Clock){
        uint32_t ClkSrc;
        if(Clock >= CLKPWR_PERIPHERAL_WWDT && Clock <= CLKPWR_PERIPHERAL_ADC0)
                return CLKPWR_ERROR_INVALID_PARAM;

        if(CLKPWR_PERIPHERAL_Info[Clock].PerBaseEntity != CLKPWR_ENTITY_NONE){
                /* Get Base Clock Source */
                ClkSrc = (CLKPWR_PER_BASE_CTRL(Clock) & CLKPWR_CTRL_SRC_MASK) >> 24;
                /* GetBase Status*/
                if(CLKPWR_PER_BASE_CTRL(Clock) & 1)
                        return 0;
                /* check Branch if it is enabled */
                if((CLKPWR_PERIPHERAL_Info[Clock].PerBranchOffset!= 0) && !(CLKPWR_PER_BRANCH_STATUS(Clock) & 1)) return 0;
        }else{
                if(CLKPWR_REG_BASE_CTRL(Clock) & 1)     return 0;
                ClkSrc = (CLKPWR_REG_BASE_CTRL(Clock) & CLKPWR_CTRL_SRC_MASK) >> 24;
                /* check Branch if it is enabled */
                if((CLKPWR_PERIPHERAL_Info[Clock].RegBranchOffset!= 0) && !(CLKPWR_REG_BRANCH_STATUS(Clock) & 1)) return 0;
        }
        return CLKPWR_ClockSourceFrequency[ClkSrc];
}

void CLKPWR_UpdateClock(void){
        uint32_t ClkSrc;
        uint32_t div;
        uint32_t divisor;
        int32_t RegOffset;
        /* 32OSC */
        if(ISBITSET(LPC_CREG->CREG0,1) && ISBITCLR(LPC_CREG->CREG0,3))
                CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_32KHZ_OSC] = 32768;
        else
                CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_32KHZ_OSC] = 0;
        /*PLL0*/
        /* PLL1 */
        if(ISBITCLR(LPC_CGU->PLL1_CTRL,1) /* Enabled */
                        && (LPC_CGU->PLL1_STAT&1)){ /* Locked? */
                ClkSrc = (LPC_CGU->PLL1_CTRL & CLKPWR_CTRL_SRC_MASK)>>24;
                CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_PLL1] = CLKPWR_ClockSourceFrequency[ClkSrc] *
                                                                                                                        (((LPC_CGU->PLL1_CTRL>>16)&0xFF)+1);
        }else
                CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_PLL1] = 0;

        /* DIV */
        for(div = CLKPWR_CLKSRC_IDIVA; div <= CLKPWR_CLKSRC_IDIVE; div++){
                RegOffset = CLKPWR_Entity_ControlReg_Offset[div];
                if(ISBITCLR(CLKPWR_ADDRESS32(LPC_CGU,RegOffset),1)){
                        ClkSrc = (CLKPWR_ADDRESS32(LPC_CGU,RegOffset) & CLKPWR_CTRL_SRC_MASK) >> 24;
                        divisor = (CLKPWR_ADDRESS32(LPC_CGU,RegOffset)>>2) & 0xFF;
                        divisor ++;
                        CLKPWR_ClockSourceFrequency[div] = CLKPWR_ClockSourceFrequency[ClkSrc] / divisor;
                }else
                        CLKPWR_ClockSourceFrequency[div] = 0;
        }
}

uint32_t        CLKPWR_SetXTALOSC(uint32_t ClockFrequency){
        if(ClockFrequency < 15000000){
                LPC_CGU->XTAL_OSC_CTRL &= ~(1<<2);
        }else if(ClockFrequency < 25000000){
                LPC_CGU->XTAL_OSC_CTRL |= (1<<2);
        }else
                return CLKPWR_ERROR_FREQ_OUTOF_RANGE;

        CLKPWR_ClockSourceFrequency[CLKPWR_CLKSRC_XTAL_OSC] = ClockFrequency;
        return CLKPWR_ERROR_SUCCESS;
}

/* divisor number must >=1*/
uint32_t        CLKPWR_SetDIV(CLKPWR_ENTITY_T SelectDivider, uint32_t divisor){
        int32_t RegOffset;
        uint32_t tempReg;
        if(SelectDivider>=CLKPWR_CLKSRC_IDIVA && SelectDivider<=CLKPWR_CLKSRC_IDIVE){
                RegOffset = CLKPWR_Entity_ControlReg_Offset[SelectDivider];
                if(RegOffset == -1) return CLKPWR_ERROR_INVALID_ENTITY;
                tempReg = CLKPWR_ADDRESS32(LPC_CGU,RegOffset);
                tempReg &= ~(0xFF<<2);
                tempReg |= ((divisor-1)&0xFF)<<2;
                CLKPWR_ADDRESS32(LPC_CGU,RegOffset) = tempReg;
                return CLKPWR_ERROR_SUCCESS;
        }
        return CLKPWR_ERROR_INVALID_ENTITY;
}

uint32_t CLKPWR_EnableEntity(CLKPWR_ENTITY_T ClockEntity, uint32_t en){
        int32_t RegOffset;
        int32_t i;
        if(ClockEntity == CLKPWR_CLKSRC_32KHZ_OSC){
                if(en){
                        LPC_CREG->CREG0 &= ~((1<<3)|(1<<2));
                        LPC_CREG->CREG0 |= (1<<1)|(1<<0);
                }else{
                        LPC_CREG->CREG0 &= ~((1<<1)|(1<<0));
                        LPC_CREG->CREG0 |= (1<<3);
                }
                for(i = 0;i<1000000;i++);

        }else if(ClockEntity == CLKPWR_CLKSRC_ENET_RX_CLK){
                scu_pinmux(0xC ,0 , MD_PLN, FUNC3);

        }else if(ClockEntity == CLKPWR_CLKSRC_ENET_TX_CLK){
                scu_pinmux(0x1 ,19 , MD_PLN, FUNC0);

        }else if(ClockEntity == CLKPWR_CLKSRC_GP_CLKIN){

        }else if(ClockEntity == CLKPWR_CLKSRC_TCK){

        }else if(ClockEntity == CLKPWR_CLKSRC_XTAL_OSC){
                if(!en)
                        LPC_CGU->XTAL_OSC_CTRL |= CLKPWR_CTRL_EN_MASK;
                else
                        LPC_CGU->XTAL_OSC_CTRL &= ~CLKPWR_CTRL_EN_MASK;
                /*Delay for stable clock*/
                for(i = 0;i<1000000;i++);

        }else{
                RegOffset = CLKPWR_Entity_ControlReg_Offset[ClockEntity];
                if(RegOffset == -1) return CLKPWR_ERROR_INVALID_ENTITY;
                if(!en){
                        CLKPWR_ADDRESS32(CLKPWR_CGU_ADDR,RegOffset) |= CLKPWR_CTRL_EN_MASK;
                }else{
                        CLKPWR_ADDRESS32(CLKPWR_CGU_ADDR,RegOffset) &= ~CLKPWR_CTRL_EN_MASK;
                        /*if PLL is selected check if it is locked */
                        if(ClockEntity == CLKPWR_CLKSRC_PLL0){
                                while((LPC_CGU->PLL0USB_STAT&1) == 0x0);
                        }
                        if(ClockEntity == CLKPWR_CLKSRC_PLL1){
                                while((LPC_CGU->PLL1_STAT&1) == 0x0);
                                /*post check lock status */
                                if(!(LPC_CGU->PLL1_STAT&1))
                                        while(1);
                        }
                }
        }
        return CLKPWR_ERROR_SUCCESS;
}

uint32_t CLKPWR_EntityConnect(CLKPWR_ENTITY_T ClockSource, CLKPWR_ENTITY_T ClockEntity){
        int32_t RegOffset;
        uint32_t tempReg;

        if(ClockSource > CLKPWR_CLKSRC_IDIVE)
                return CLKPWR_ERROR_INVALID_CLOCK_SOURCE;

        if(ClockEntity >= CLKPWR_CLKSRC_PLL0 && ClockEntity <= CLKPWR_BASE_CLKOUT){
                if(CLKPWR_ConnectAlloc_Tbl[ClockSource][ClockEntity]){
                        RegOffset = CLKPWR_Entity_ControlReg_Offset[ClockSource];
                        if(RegOffset != -1){
                                if(ClockEntity<=CLKPWR_CLKSRC_IDIVE &&
                                        ClockEntity>=CLKPWR_CLKSRC_PLL0)
                                {
                                        //RegOffset = (CLKPWR_ADDRESS32(LPC_CGU,RegOffset)>>24)&0xF;
                                        if((CLKPWR_ADDRESS32(LPC_CGU,RegOffset)>>24)&0xF == ClockEntity)
                                                return CLKPWR_ERROR_CONNECT_TOGETHER;
                                }
                        }
                        RegOffset = CLKPWR_Entity_ControlReg_Offset[ClockEntity];
                        if(RegOffset == -1) return CLKPWR_ERROR_INVALID_ENTITY;
                        tempReg = CLKPWR_ADDRESS32(LPC_CGU,RegOffset);
                        tempReg &= ~CLKPWR_CTRL_SRC_MASK;
                        tempReg |= ClockSource<<24 | CLKPWR_CTRL_AUTOBLOCK_MASK;
                        CLKPWR_ADDRESS32(LPC_CGU,RegOffset) = tempReg;
                        return CLKPWR_ERROR_SUCCESS;
                }else
                        return CLKPWR_ERROR_INVALID_CLOCK_SOURCE;
        }else
                return CLKPWR_ERROR_INVALID_ENTITY;
}

/* Currently USB PLL Get Clock from XTAL */
uint32_t CLKPWR_SetPLL0(void){
        // Setup PLL550 to generate 480MHz from 12 MHz crystal
        //LPC_CGU->PL550M_0_C |= PD_ENABLE;     // Power down PLL
                                                //      P                       N
        LPC_CGU->PLL0USB_NP_DIV = (98<<0) | (514<<12);
                                                //      SELP    SELI    SELR    MDEC
        LPC_CGU->PLL0USB_MDIV = (0xB<<17)|(0x10<<22)|(0<<28)|(0x7FFA<<0);
        LPC_CGU->PLL0USB_CTRL =(CLKPWR_CLKSRC_XTAL_OSC<<24) | (0x3<<2) | (1<<4);
        return CLKPWR_ERROR_SUCCESS;
}

/*To set PLL first disable it
 * connect it to source clock
 * set PLL mult
 * enable it */
uint32_t        CLKPWR_SetPLL1(uint32_t mult){
        uint32_t msel=0, nsel=0, psel=0, pval=1;
        uint32_t freq;
        uint32_t ClkSrc = (LPC_CGU->PLL1_CTRL & CLKPWR_CTRL_SRC_MASK)>>24;
        freq = CLKPWR_ClockSourceFrequency[ClkSrc];
        freq *= mult;
        msel = mult-1;

        LPC_CGU->PLL1_CTRL &= ~(CLKPWR_PLL1_FBSEL_MASK |
                                                                        CLKPWR_PLL1_BYPASS_MASK |
                                                                        CLKPWR_PLL1_DIRECT_MASK |
                                                                        (0x03<<8) | (0xFF<<16) | (0x03<<12));

        if(freq<156000000){
                //psel is encoded such that 0=1, 1=2, 2=4, 3=8
                while(2*(pval)*freq < 156000000) {
                        psel++;
                        pval*=2;
                }
//              if(2*(pval)*freq > 320000000) {
//                      //THIS IS OUT OF RANGE!!!
//                      //HOW DO WE ASSERT IN SAMPLE CODE?
//                      //__breakpoint(0);
//                      return CLKPWR_ERROR_INVALID_PARAM;
//              }
                LPC_CGU->PLL1_CTRL |= (msel<<16) | (nsel<<12) | (psel<<8) | CLKPWR_PLL1_FBSEL_MASK;
        }else if(freq<320000000){
                LPC_CGU->PLL1_CTRL |= (msel<<16) | (nsel<<12) | (psel<<8) |CLKPWR_PLL1_DIRECT_MASK | CLKPWR_PLL1_FBSEL_MASK;
        }else
                return CLKPWR_ERROR_INVALID_PARAM;

        return CLKPWR_ERROR_SUCCESS;
}

uint32_t        CLKPWR_GetBaseStatus(CLKPWR_ENTITY_T Base){
        switch(Base){
        /*CCU1*/
        case CLKPWR_BASE_APB3:
                return LPC_CCU1->BASE_STAT & 1;

        case CLKPWR_BASE_APB1:
                return (LPC_CCU1->BASE_STAT>>1) & 1;

        case CLKPWR_BASE_SPIFI:
                return (LPC_CCU1->BASE_STAT>>2) & 1;

        case CLKPWR_BASE_M3:
                return (LPC_CCU1->BASE_STAT>>3) & 1;

        case CLKPWR_BASE_USB0:
                return (LPC_CCU1->BASE_STAT>>7) & 1;

        case CLKPWR_BASE_USB1:
                return (LPC_CCU1->BASE_STAT>>8) & 1;

        /*CCU2*/
        case CLKPWR_BASE_UART3:
                return (LPC_CCU2->BASE_STAT>>1) & 1;

        case CLKPWR_BASE_UART2:
                return (LPC_CCU2->BASE_STAT>>2) & 1;

        case CLKPWR_BASE_UART1:
                return (LPC_CCU2->BASE_STAT>>3) & 1;

        case CLKPWR_BASE_UART0:
                return (LPC_CCU2->BASE_STAT>>4) & 1;

        case CLKPWR_BASE_SSP1:
                return (LPC_CCU2->BASE_STAT>>5) & 1;

        case CLKPWR_BASE_SSP0:
                return (LPC_CCU2->BASE_STAT>>6) & 1;

        case CLKPWR_BASE_SDIO:
                return (LPC_CCU2->BASE_STAT>>7) & 1;

        /*BASE SAFE is used by WWDT and RGU*/
        case CLKPWR_BASE_SAFE:
                break;
        }
        return 0;
}

/*
 * Compare to IRC clock
 * FClock = F_IRC* m / d
 * */
int CLKPWR_FrequencyMonitor(CLKPWR_ENTITY_T Clock, uint32_t *m, uint32_t *d){
        uint32_t n,c,temp;
        int i;

        /* Maximum allow RCOUNT number */
        c= 511;
        /* Check Source Clock Freq is larger or smaller */
        LPC_CGU->FREQ_MON = (Clock<<24) | 1<<23 | c;
        while(LPC_CGU->FREQ_MON & (1 <<23));
        for(i=0;i<10000;i++);
        temp = (LPC_CGU->FREQ_MON >>9) & 0x3FFF;

        if(temp == 0) /* too low F < 12000000/511*/
                return -1;
        if(temp > 511){ /* larger */

                c = 511 - (LPC_CGU->FREQ_MON&0x1FF);
        }else{
                do{
                        c--;
                        LPC_CGU->FREQ_MON = (Clock<<24) | 1<<23 | c;
                        while(LPC_CGU->FREQ_MON & (1 <<23));
                        for(i=0;i<10000;i++);
                        n = (LPC_CGU->FREQ_MON >>9) & 0x3FFF;
                }while(n==temp);
                c++;
        }
        *m = temp;
        *d = c;
        return 0;
}

/*
        Compare Clock to CompareToClock
        FClock = m*FCompareToClock/d
*/
uint32_t CLKPWR_RealFrequencyCompare(CLKPWR_ENTITY_T Clock, CLKPWR_ENTITY_T CompareToClock, uint32_t *m, uint32_t *d){
        uint32_t m1,m2,d1,d2;
        /* Check Parameter */
        if((Clock>CLKPWR_CLKSRC_IDIVE) || (CompareToClock>CLKPWR_CLKSRC_IDIVE))
                return CLKPWR_ERROR_INVALID_PARAM;
        /* Check for Clock Enable - Not yet implement
         * The Comparator will hang if Clock has not been set*/
        CLKPWR_FrequencyMonitor(Clock, &m1, &d1);
        CLKPWR_FrequencyMonitor(CompareToClock, &m2, &d2);
        *m= m1*d2;
        *d= d1*m2;
        return 0;

}
/*********************************************************************//**
 * @brief               Enter Sleep mode with co-operated instruction by the Cortex-M3.
 * @param[in]   None
 * @return              None
 **********************************************************************/
void CLKPWR_Sleep(void)
{
        //LPC_PMC->SLEEP0_MODE = 0x00;
        /* Sleep Mode*/
        __WFI();
}


/*********************************************************************//**
 * @brief               Enter Deep Sleep mode with co-operated instruction by the Cortex-M3.
 * @param[in]   None
 * @return              None
 **********************************************************************/
void CLKPWR_DeepSleep(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
        SCB->SCR = 0x4;
        LPC_PMC->PD0_SLEEP0_MODE = CLKPWR_SLEEP_MODE_DEEP_SLEEP;
        /* Deep Sleep Mode*/
        __WFI();
}


/*********************************************************************//**
 * @brief               Enter Power Down mode with co-operated instruction by the Cortex-M3.
 * @param[in]   None
 * @return              None
 **********************************************************************/
void CLKPWR_PowerDown(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
        SCB->SCR = 0x4;
        LPC_PMC->PD0_SLEEP0_MODE = CLKPWR_SLEEP_MODE_POWER_DOWN;
        /* Power Down Mode*/
        __WFI();
}


/*********************************************************************//**
 * @brief               Enter Deep Power Down mode with co-operated instruction by the Cortex-M3.
 * @param[in]   None
 * @return              None
 **********************************************************************/
void CLKPWR_DeepPowerDown(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
        SCB->SCR = 0x4;
        LPC_PMC->PD0_SLEEP0_MODE = CLKPWR_SLEEP_MODE_DEEP_POWER_DOWN;
        /* Deep Power Down Mode*/
        __WFI();
}

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
