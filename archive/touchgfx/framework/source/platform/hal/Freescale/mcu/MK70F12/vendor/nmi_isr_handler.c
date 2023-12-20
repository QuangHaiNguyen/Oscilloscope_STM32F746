/* File:         nmi_isr_handler.c
 * Purpose:     Provides routines for handling nmi isr.
 *
 * Notes:       
 *              
 */
void nmi_isr(void);

#include "common.h"
#include "nmi_isr_handler.h"

#ifdef CMSIS
void NMI_Handler(void)
#else
void nmi_isr(void)
#endif
{
  int i;
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  for(i= 0;i<0x1ffff;i++){
     if ((GPIOA_PDIR & 0x00000010) == 0x10)   // if pin returns high stop
       break;  // debounce
  }
  return;  
}

void enable_NMI_button(void)
{
    /* Configure the PTA4 pin for its NMI function */
  
  PORTA_PCR4 =  PORT_PCR_PS_MASK |
                PORT_PCR_PE_MASK | 
                PORT_PCR_PFE_MASK |
                PORT_PCR_MUX(7); /* NMI Falling edge */    
   // NMI is alt7 function for this pin
    
}
