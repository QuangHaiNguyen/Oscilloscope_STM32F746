#include "LPC1788.h"
#include "vendor/FDI_Memory.hpp"

#define nop()     __NOP()
#define nops5()    nop();nop();nop();nop();nop()
#define nops10()   nops5();nops5()
#define nops50()   nops10();nops10();nops10();nops10();nops10()

static void UEZBSPDelay1US(void)
{
    nops50();
    nops50();
    nop();
    nop();
    nop();
    nop();
}

static void UEZBSPDelay1MS(void)
{
    uint32_t i;

    // Approximate delays here
    for (i = 0; i < 1000; i++)
        UEZBSPDelay1US();
}

static void UEZBSPDelayMS(unsigned int aMilliseconds)
{
    while (aMilliseconds--) {
        UEZBSPDelay1MS();
    }
}


volatile int audio_delay_1 = 500;
volatile int audio_delay_2 = 300;
#include "lpc177x_8x_gpio.h"

void Test_Audio1()
{
  GPIO_SetValue(2, (1<<1));
  for(int i = 0; i < audio_delay_1; i++)
  {
    UEZBSPDelay1US();
  }
  GPIO_ClearValue(2, (1<<1));
  for(int i = 0; i < audio_delay_2; i++)
  {
    UEZBSPDelay1US();
  }
}

volatile int test_loopcnt = 3;
void FDI_PlayTone()
{
  for(int i = 0; i < test_loopcnt; i++)
    Test_Audio1();
}


static void LPC1788_ExternalAddressBus_ConfigureIOPins(uint8_t aBitSize)
{
    __IO uint32_t *p_io = &LPC_IOCON->P4_0;

    if (aBitSize > 24)
        aBitSize = 24;

    // Configure address pins
    while (aBitSize--) {
        *p_io = (*p_io & ~7) | 1;
        p_io++;
    }
}

static void LPC1788_ExternalDataBus_ConfigureIOPins(uint8_t aBitSize)
{
    __IO uint32_t *p_io = &LPC_IOCON->P3_0;

    if (aBitSize > 32)
        aBitSize = 32;

    // Configure address pins
    while (aBitSize--) {
        *p_io = (*p_io & ~7) | 1;
        p_io++;
    }
}

/*****************************************************************************
 ** Function name:       ILPC1788_SDRAM_QuickTest
 **
 ** Descriptions:        sdram test
 **
 ** parameters:          None
 **
 ** Returned value:      1 if test passed, otherwise 0
 **
 *****************************************************************************/
static uint32_t ILPC1788_SDRAM_QuickTest(uint32_t aBaseAddress, uint32_t aSize)
{
    volatile uint32_t *wr_ptr;
    volatile uint16_t *short_wr_ptr;
    uint32_t data;
    uint32_t i, j;

    wr_ptr = (uint32_t *)aBaseAddress;
    short_wr_ptr = (uint16_t *)wr_ptr;
    /* Clear content before 16 bit access test */
    //  for (i = 0; i < G_SDRAMSize/4; i++)
    //  {
    //  *wr_ptr++ = 0;
    //  }

    /* 16 bit write */
    for (i = 0; i < aSize / 0x40000; i++) {
        for (j = 0; j < 0x100; j++) {
            *short_wr_ptr++ = (i + j);
            *short_wr_ptr++ = (i + j) + 1;
        }
    }

    /* Verifying */
    wr_ptr = (uint32_t *)aBaseAddress;
    for (i = 0; i < aSize / 0x40000; i++) {
        for (j = 0; j < 0x100; j++) {
            data = *wr_ptr;
            if (data != (((((i + j) + 1) & 0xFFFF) << 16) | ((i + j) & 0xFFFF))) {
                return 0x0;
            }
            wr_ptr++;
        }
    }
    return 0x1;
}

/*****************************************************************************
 ** Function name:       ILPC1788_SDRAM_FindCmdDelay
 **
 ** Descriptions:        find CMDDLY
 **
 ** parameters:          None
 **
 ** Returned value:      1 if test passed, otherwise 0
 **
 *****************************************************************************/
static uint32_t ILPC1788_SDRAM_FindCmdDelay(uint32_t aBaseAddress, uint32_t aSize)
{
    uint32_t cmddly, cmddlystart, cmddlyend, dwtemp;
    uint32_t ppass = 0x0, pass = 0x0;

    cmddly = 0x0;
    cmddlystart = cmddlyend = 0xFF;

    while (cmddly < 32) {
        dwtemp = LPC_SC->EMCDLYCTL & ~0x1F;
        LPC_SC->EMCDLYCTL = dwtemp | cmddly;

        if (ILPC1788_SDRAM_QuickTest(aBaseAddress, aSize) == 0x1) {
            /* Test passed */
            if (cmddlystart == 0xFF) {
                cmddlystart = cmddly;
            }
            ppass = 0x1;
        } else {
            /* Test failed */
            if (ppass == 1) {
                cmddlyend = cmddly;
                pass = 0x1;
                ppass = 0x0;
            }
        }

        /* Try next value */
        cmddly++;
    }

    /* If the test passed, the we can use the average of the min */
    /* and max values to get an optimal DQSIN delay */
    if (pass == 0x1) {
        cmddly = (cmddlystart + cmddlyend) / 2;
    } else if (ppass == 0x1) {
        cmddly = (cmddlystart + 0x1F) / 2;
    } else {
        /* A working value couldn't be found, just pick something */
        /* safe so the system doesn't become unstable */
        cmddly = 0x10;
    }

    dwtemp = LPC_SC->EMCDLYCTL & ~0x1F;
    LPC_SC->EMCDLYCTL = dwtemp | cmddly;

    return (pass | ppass);
}

/*****************************************************************************
 ** Function name:       ILPC1788_SDRAM_FindFBClockDelay
 **
 ** Descriptions:        find FBCLKDLY
 **
 ** parameters:          None
 **
 ** Returned value:      1 if test passed, otherwise 0
 **
 *****************************************************************************/
static uint32_t ILPC1788_SDRAM_FindFBClockDelay(
        uint32_t aBaseAddress,
        uint32_t aSize)
{
    uint32_t fbclkdly, fbclkdlystart, fbclkdlyend, dwtemp;
    uint32_t ppass = 0x0, pass = 0x0;

    fbclkdly = 0x0;
    fbclkdlystart = fbclkdlyend = 0xFF;

    while (fbclkdly < 32) {
        dwtemp = LPC_SC->EMCDLYCTL & ~0x1F00;
        LPC_SC->EMCDLYCTL = dwtemp | (fbclkdly << 8);

        if (ILPC1788_SDRAM_QuickTest(aBaseAddress, aSize) == 0x1) {
            /* Test passed */
            if (fbclkdlystart == 0xFF) {
                fbclkdlystart = fbclkdly;
            }
            ppass = 0x1;
        } else {
            /* Test failed */
            if (ppass == 1) {
                fbclkdlyend = fbclkdly;
                pass = 0x1;
                ppass = 0x0;
            }
        }

        /* Try next value */
        fbclkdly++;
    }

    /* If the test passed, the we can use the average of the min and max values to get an optimal DQSIN delay */
    if (pass == 0x1) {
        fbclkdly = (fbclkdlystart + fbclkdlyend) / 2;
    } else if (ppass == 0x1) {
        fbclkdly = (fbclkdlystart + 0x1F) / 2;
    } else {
        /* A working value couldn't be found, just pick something safe so the system doesn't become unstable */
        fbclkdly = 0x10;
    }

    dwtemp = LPC_SC->EMCDLYCTL & ~0x1F00;
    LPC_SC->EMCDLYCTL = dwtemp | (fbclkdly << 8);

    return (pass | ppass);
}

/*****************************************************************************
 ** Function name:       ILPC1788_SDRAM_Calibration
 **
 ** Descriptions:        Calibration
 **
 ** parameters:          None
 **
 ** Returned value:      current ring osc count
 **
 *****************************************************************************/
static uint32_t ILPC1788_SDRAM_Calibration(void)
{
    uint32_t dwtemp, i;
    uint32_t cnt = 0;

    for (i = 0; i < 10; i++) {
        dwtemp = LPC_SC->EMCCAL & ~0x4000;
        LPC_SC->EMCCAL = dwtemp | 0x4000;

        dwtemp = LPC_SC->EMCCAL;
        while ((dwtemp & 0x8000) == 0x0000) {
            dwtemp = LPC_SC->EMCCAL;
        }
        cnt += (dwtemp & 0xFF);
    }
    return (cnt / 10);
}

/*****************************************************************************
 ** Function name:       ILPC1788_SDRAM_AdjustTiming
 **
 ** Descriptions:        Adjust timing
 **
 ** parameters:          None
 **
 ** Returned value:      None
 **
 *****************************************************************************/
static void ILPC1788_SDRAM_AdjustTiming(volatile uint32_t ringosccount[2])
{
    uint32_t dwtemp, cmddly, fbclkdly;

    /* Current value */
    ringosccount[1] = ILPC1788_SDRAM_Calibration();

    dwtemp = LPC_SC->EMCDLYCTL;
    cmddly = ((dwtemp & 0x1F) * ringosccount[0] / ringosccount[1]) & 0x1F;
    fbclkdly = ((dwtemp & 0x1F00) * ringosccount[0] / ringosccount[1]) & 0x1F00;
    LPC_SC->EMCDLYCTL = (dwtemp & ~0x1F1F) | fbclkdly | cmddly;
}


void FDI_SDRAM_Init()
{
    __IO uint32_t *dc;
    uint32_t i;
    volatile uint32_t dmy = 0;
    volatile uint32_t ringosccount[2] = { 0, 0 };

    // EMC Reset Disable = Enabled, Both EMC resets are asserted
    // when any type of reset occurs.
    LPC_SC->SCS &= ~(1 << 1);

    // Turn on EMC PCLK (if not already on)
    LPC_SC->PCONP |= (1 << 11);

    // Setup slow/default delays
    LPC_SC->EMCDLYCTL = 0x00001010;

    // Reset the EMC and put configuration
    // back to power up reset (little-endian mode, 1:1 clock)
    LPC_EMC->Control = 0x00000001;
    LPC_EMC->Config = 0x00000000;

    // Setup 32-bit data bus
    LPC1788_ExternalDataBus_ConfigureIOPins(32);

    // Setup A0 .. A14 for SDRAM
    LPC1788_ExternalAddressBus_ConfigureIOPins(15);

    // Configure control pins (we really only care for bus lane 0)
    LPC_IOCON->P4_24 = (LPC_IOCON->P4_24 & ~7) | 1;
    LPC_IOCON->P4_25 = (LPC_IOCON->P4_25 & ~7) | 1;

    int dynconfig = 0x00005300;
    /* CSN[0] @ P4.30 */
    LPC_IOCON->P4_30 = (LPC_IOCON->P4_30 & ~7) | 1;

    /* DYCSN[0] @ P2.20 */
    LPC_IOCON->P2_20 = (LPC_IOCON->P2_20 & ~7) | 1;

    /* CKE[0] @ P2.24 */
    LPC_IOCON->P2_24 = (LPC_IOCON->P2_24 & ~7) | 1;

    dc = &LPC_EMC->DynamicConfig0;
    LPC_EMC->DynamicConfig0 = dynconfig;

    /* CASN @ P2.16 */
    LPC_IOCON->P2_16 = (LPC_IOCON->P2_16 & ~7) | 1;

    /* RASN @ P2.17 */
    LPC_IOCON->P2_17 = (LPC_IOCON->P2_17 & ~7) | 1;

    /* CLK[0] @ P2.18 */
    LPC_IOCON->P2_18 = (LPC_IOCON->P2_18 & ~7) | 1;

    // Setup for 32-bit bus
    /* DQM[0] @ P2.28 */
    LPC_IOCON->P2_28 = (LPC_IOCON->P2_28 & ~7) | 1;
    /* DQM[1] @ P2.29 */
    LPC_IOCON->P2_29 = (LPC_IOCON->P2_29 & ~7) | 1;
    /* DQM[2] @ P2.30 */
    LPC_IOCON->P2_30 = (LPC_IOCON->P2_30 & ~7) | 1;
    /* DQM[3] @ P2.31 */
    LPC_IOCON->P2_31 = (LPC_IOCON->P2_31 & ~7) | 1;

    // RAS=2, CAS=2
    LPC_EMC->DynamicRasCas0 = 0x00000202;
    LPC_EMC->DynamicReadConfig = 0x00000001; /* Command delayed strategy, using EMCCLKDELAY */

    // -6 timings

    LPC_EMC->DynamicRP = 2 - 1;
    LPC_EMC->DynamicRAS = 3 - 1;
    LPC_EMC->DynamicSREX = 5 - 1;
    LPC_EMC->DynamicAPR = 2 - 1;
    LPC_EMC->DynamicDAL = 4;
    LPC_EMC->DynamicWR = 2 - 1;
    LPC_EMC->DynamicRC = 4 - 1;
    LPC_EMC->DynamicRFC = 4 - 1;
    LPC_EMC->DynamicXSR = 5 - 1;
    LPC_EMC->DynamicRRD = 1 - 1;
    LPC_EMC->DynamicMRD = 2 - 1;

    //Send command: NOP
    LPC_EMC->DynamicControl = 0x00000183;

    //wait 200mS
    UEZBSPDelayMS(200);

    //Send command: PRECHARGE-ALL, shortest possible refresh period
    LPC_EMC->DynamicControl = 0x00000100;
    LPC_EMC->DynamicRefresh = 0x00000001;

    //wait at least 128 ABH clock cycles
    for (i = 0; i < 0x100; i++)
        __NOP();

    //Set correct refresh period
    LPC_EMC->DynamicRefresh
            = (unsigned int)((((60000000 / 1000)
                    * (64)) / 8192)
                    / 16) + 1;

    //wait at least 128 ABH clock cycles
    for (i = 0; i < 0x40; i++)
        __NOP();

    //Send command: MODE
    LPC_EMC->DynamicControl = 0x00000080;

    //Set mode register in SDRAM
    dmy = *((volatile unsigned int*)(0xA0000000 | (0x22 << 10)));

    //wait 128 ABH clock cycles
    for (i = 0; i < 0x40; i++)
        __NOP();

    //Send command: NORMAL
    LPC_EMC->DynamicControl = 0x00000000;

    //Enable buffer
    *dc |= 0x00080000;

    //initial system delay
    UEZBSPDelayMS(1);

    ringosccount[0] = ILPC1788_SDRAM_Calibration();

    ILPC1788_SDRAM_FindCmdDelay(0xA0000000, 0x800000);
    ILPC1788_SDRAM_FindFBClockDelay(0xA0000000, 0x800000);
    ILPC1788_SDRAM_AdjustTiming(ringosccount);
}

typedef struct {
    __IO uint32_t StaticConfig;              /*!< Offset: 0x200 (R/W)  Static Memory Configuration Register */
    __IO uint32_t StaticWaitWen;             /*!< Offset: 0x204 (R/W)  Static Memory Write Enable Delay Register */
    __IO uint32_t StaticWaitOen;             /*!< Offset: 0x208 (R/W)  Static Memory Output Enable Delay Register */
    __IO uint32_t StaticWaitRd;              /*!< Offset: 0x20C (R/W)  Static Memory Read Delay Register */
    __IO uint32_t StaticWaitPage;            /*!< Offset: 0x210 (R/W)  Static Memory Page Mode Read Delay Register */
    __IO uint32_t StaticWaitWr;              /*!< Offset: 0x214 (R/W)  Static Memory Write Delay Register */
    __IO uint32_t StaticWaitTurn;            /*!< Offset: 0x218 (R/W)  Static Memory Turn Round Delay Register */
} T_LPC1788_EMC_Static_Regs;

void FDI_NOR_Init()
{
    T_LPC1788_EMC_Static_Regs *emc;
    uint8_t numAddrBits;
    uint32_t remaining;

    emc = (T_LPC1788_EMC_Static_Regs *)&LPC_EMC->StaticConfig0;
    /* CSN[0] @ P4.30 */
    LPC_IOCON->P4_30 = (LPC_IOCON->P4_30 & ~7) | 1;

    // Configure control pins (we really only care for bus lane 0)
    /* OEN @ P4.24 */
    LPC_IOCON->P4_24 = (LPC_IOCON->P4_24 & ~7) | 1;
    /* WEN @ P4.25 */
    LPC_IOCON->P4_25 = (LPC_IOCON->P4_25 & ~7) | 1;

    LPC1788_ExternalAddressBus_ConfigureIOPins(25);

    // Configurate static memory
    emc->StaticConfig =
            (1) | // 1=16-bit
            (0 << 3) | // 0=page mode disabled (TBD: for now)
            (0 << 6) | // 0=Acive LOW chip select (CS0n -> En)
            (1 << 7) | // 0=For Reads BLSn[3:0] HIGH, For writes BLSn[3:0] LOW (not used)
            (0 << 8) | // 0=Extended Wait disabled (POR reset value)
            (0 << 19) | // 0=Buffer disabled (POR reset value, TBD)
            (0 << 20); // 0=Write not protected

    // Delay from chip select or address change, whichever is later to
    // the output enable
    // Use tAVQV = tACC = Address valid to Output Valid
    emc->StaticWaitOen = 0;

    // Delay from the chip select to the read access
    // Use tELQV = tEn = Address valid to Next Address Valid
    emc->StaticWaitRd = 5;

    // Page Mode Read Delay register
    // Delay for asynchronous page mode sequential access
    // use tRC = address valid to address valid
    emc->StaticWaitPage = 0;

    // Delay from Chip select to write enable time
    // Use tELWL = tCS = Chip Enable Low to Write Enable Low
    emc->StaticWaitWen = 0;

    // Write Delay Register
    // Delay from the Chip Select to the Write Access
    // use tWLRH = tWP = Write Enable Low to Write Enable High
    emc->StaticWaitWr = 4;

    // Turn Round Delay register
    // Number of bus turnaround cycles, cycles between read and write access
    // use tWLRH = tWP = Write Enable Low to Write Enable High
    emc->StaticWaitTurn = 1;

    // Make sure address are byte wise on the address bus
    LPC_SC->SCS |= (1<<0);
}
