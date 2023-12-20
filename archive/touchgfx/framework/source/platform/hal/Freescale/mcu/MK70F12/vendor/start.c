/*
 * File:        start.c
 * Purpose:     Kinetis start up routines. 
 *
 * Notes:               
 */

#include "start.h"
#include "common.h"
#include "wdog.h"
#include "rcm.h"
#include "sysinit.h"

extern void * __iar_cstart_call_ctors(void *ptr);
//#pragma location = ".intvec"
#pragma section = ".init_array"

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
void start(void)
{
        /* Disable the watchdog timer */
        wdog_disable();

        /* Copy any vector or data sections that need to be in RAM */
        common_startup();
        
        /* Initialize constructors */
        //void *cpp_init = (void*)0x0000a090;
        void *cpp_init = __section_begin(".init_array"); 
        __iar_cstart_call_ctors(cpp_init);
        
        /* Perform processor initialization */
        sysinit();
        
        printf("\n\n");
        
        /* Determine the last cause(s) of reset */
        outSRS();

        /* Determine specific Kinetis device and revision */
        cpu_identify();
        
        /* Jump to main process */
        main();

        /* No actions to perform after this so wait forever */
        while(1);
}
/********************************************************************/
/*!
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_identify (void)
{

    /* Determine the Kinetis family ID */
    switch((SIM_SDID & 0x70)>>4) 
    {  
        case 0x0:
                printf("\nK10-");
                break;
        case 0x1:
                printf("\nK20-");
                break;
        case 0x2:
                printf("\nK61-");
                break;
        case 0x4:
                printf("\nK60-");
                break;
        case 0x5:
                printf("\nK70-");
                break;
default:
        printf("\nUnrecognized Kinetis family device.\n");  
        break;          
    }

    /* Determine the package size */
    switch((SIM_SDID & SIM_SDID_BOID(0xF))>>SIM_SDID_BOID_SHIFT) 
    {  
        case 0x2:
                printf("32pin       ");
                break;
        case 0x4:
                printf("48pin       ");
                break;
        case 0x5:
                printf("64pin       ");
                break;
        case 0x6:
                printf("80pin       ");
                break;
        case 0x7:
                printf("81pin       ");
                break;
        case 0x8:
                printf("100pin      ");
                break;
        case 0x9:
                printf("104pin      ");
                break;
        case 0xA:
                printf("144pin      ");
                break;
        case 0xC:
                printf("196pin      ");
                break;
        case 0xE:
                printf("256pin      ");
                break;
        default:
                printf("\nUnrecognized Kinetis package code.      ");  
                break;          
    }                

    /* Determine the revision ID */
    switch((SIM_SDID & SIM_SDID_REVID(0xF))>>SIM_SDID_REVID_SHIFT) 
    { 
   
    case 0x0:
                printf("Silicon rev 1.0   \n ");
                break;
    default:
                printf("\nThis version of software doesn't recognize the revision code.");  
                break;  
    }
    
    /* Determine the flash revision */
    flash_identify();  
    
/* The documentation needs to be updated with the correct FCFG1 fields.
 * Once the documentation is updated, then the header file can be updated
 * so that the section of code below will compmile correctly.
 */    
//    /* Determine the P-flash size */
//    switch((SIM_FCFG1 & SIM_FCFG1_PFSIZE(0xF))>>SIM_FCFG1_PFSIZE_SHIFT)
//    {
//      case 0x7:
//              printf("128 kBytes of P-flash   ");
//              break;
//      case 0x9:
//              printf("256 kBytes of P-flash   ");
//              break;
//        case 0xB:
//              printf("512 kBytes of P-flash   ");
//              break;
//      case 0xF:
//              printf("512 kBytes of P-flash   ");
//              break;
//      default:
//              printf("ERR!! Undefined P-flash size\n");  
//              break;                  
//    }
//    
//    /* Determine if the part has P-flash only or P-flash and FlexNVM */
//    if (SIM_FCFG2 & SIM_FCFG2_PFLSH_MASK)  
//      printf("P-flash only\n");
//    else
//      /* if part has FlexNVM determine the FlexNVM size*/
//      switch((SIM_FCFG1 & SIM_FCFG1_NVMSIZE(0xF))>>SIM_FCFG1_NVMSIZE_SHIFT)
//      {
//              case 0x0:
//                      printf("0 kBytes of FlexNVM\n");
//                      break;
//      case 0x7:
//                      printf("128 kBytes of FlexNVM\n");
//              break;
//        case 0x9:
//                      printf("256 kBytes of FlexNVM\n");
//              break;
//      case 0xF:
//                      printf("256 kBytes of FlexNVM\n");
//              break;
//      default:
//              printf("ERR!! Undefined FlexNVM size\n");  
//              break;                  
//      }
      
/* The documentation needs to be updated with the correct SOPT1 fields.
 * Once the documentation is updated, then the header file can be updated
 * so that the section of code below will compmile correctly.
 */    
//    /* Determine the RAM size */
//    switch((SIM_SOPT1 & SIM_SOPT1_RAMSIZE(0xF))>>SIM_SOPT1_RAMSIZE_SHIFT)
//    {
//      case 0x5:
//              printf("32 kBytes of RAM\n\n");
//              break;
//      case 0x7:
//              printf("64 kBytes of RAM\n\n");
//              break;
//      case 0x8:
//              printf("96 kBytes of RAM\n\n");
//              break;
//      case 0x9:
//              printf("128 kBytes of RAM\n\n");
//              break;
//              default:
//                      printf("ERR!! Undefined RAM size\n\n");  
//                      break;                  
//    }

}
/********************************************************************/
/*!
 * \brief   flash Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific flash parameters and flash version ID for 
 * the current device. These parameters are obtained using a special
 * flash command call "read resource." The first four bytes returned
 * are the flash parameter revision, and the second four bytes are
 * the flash version ID.
 */
void flash_identify (void)
{
    /* Get the flash parameter version and flash version ID */

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFE_FCCOB0 = 0x03;
    FTFE_FCCOB1 = 0x00;
    FTFE_FCCOB2 = 0x00;
    FTFE_FCCOB3 = 0x08;
    FTFE_FCCOB4 = 0x01;   

    /* All required FCCOBx registers are written, so launch the command */
    FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));
    
    printf("Flash parameter version %d.%d.%d.%d\n",FTFE_FCCOB4,FTFE_FCCOB5,FTFE_FCCOB6,FTFE_FCCOB7);

    printf("Flash version ID %d.%d.%d.%d\n",FTFE_FCCOB8,FTFE_FCCOB9,FTFE_FCCOBA,FTFE_FCCOBB);  
    
    /* Clear all of the flags in the flash status register */
    FTFE_FSTAT = 0x7F;
}
/********************************************************************/

