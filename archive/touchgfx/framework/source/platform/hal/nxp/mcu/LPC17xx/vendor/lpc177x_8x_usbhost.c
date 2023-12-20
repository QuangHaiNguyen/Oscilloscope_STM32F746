/**********************************************************************
* $Id$          lpc177x_8x_usbhost.c                    2011-09-05
*//**
* @file         lpc177x_8x_usbhost.c
* @brief                Host Controller functions.
* @version      1.0
* @date         05. September. 2011
* @author       NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
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
/** @addtogroup USBHostLite
 * @{
 */

/*
**************************************************************************************************************
*                                            INCLUDE HEADER FILES
**************************************************************************************************************
*/
#include  "lpc177x_8x_usbhost.h"
#include  "lpc177x_8x_pinsel.h"

/*
**************************************************************************************************************
*                                            DEFINITIONS
**************************************************************************************************************
*/

#define         DEVICE_DESCRIPTOR_SIZE          18
#define         DEVICE_CONFIGURATION_SIZE       9
#define         DEVICE_INTERFACE_SIZE           9

/*
**************************************************************************************************************
*                                              GLOBAL VARIABLES
**************************************************************************************************************
*/
static int32_t       gUSBConnected = 0;
static USB_DEVICE_TYPE gUSBDeviceType = USB_DEVICE_UNKNOWN;
volatile  uint32_t   HOST_RhscIntr = 0;         /* Root Hub Status Change interrupt                       */
volatile  uint32_t   HOST_WdhIntr  = 0;         /* Semaphore to wait until the TD is submitted            */
volatile  uint8_t   HOST_TDControlStatus = 0;
volatile  HCED        *EDCtrl;                    /* Control endpoint descriptor structure                  */
volatile  HCED        *EDBulkIn;                  /* BulkIn endpoint descriptor  structure                  */
volatile  HCED        *EDBulkOut;                 /* BulkOut endpoint descriptor structure                  */
volatile  HCTD        *TDHead;                    /* Head transfer descriptor structure                     */
volatile  HCTD        *TDTail;                    /* Tail transfer descriptor structure                     */
volatile  HCCA        *Hcca;                      /* Host Controller Communications Area structure          */ 
          uint16_t  *TDBufNonVol;               /* Identical to TDBuffer just to reduce compiler warnings */
volatile  uint8_t  *TDBuffer;                  /* Current Buffer Pointer of transfer descriptor          */
volatile  uint8_t  *FATBuffer;                 /* Buffer used by FAT file system                         */
volatile  uint8_t  *UserBuffer;                /* Buffer used by application                             */

volatile uint32_t  UnrecoverableIntCount = 0;
volatile uint8_t*               USB_DeviceDescriptor;
volatile uint8_t*               USB_ConfigDescriptor;
volatile uint8_t*               USB_InterfaceDescriptor;

int32_t     Host_ParseConfiguration (void);
void        Host_TDInit   (volatile  HCTD *td);
void        Host_EDInit   (volatile  HCED *ed);
void        Host_HCCAInit (volatile  HCCA  *hcca);
void        Host_FillSetup(          uint8_t   bm_request_type,
                                     uint8_t   b_request,
                                     uint16_t   w_value,
                                     uint16_t   w_index,
                                     uint16_t   w_length);
void        Host_WDHWait  (void);



/*********************************************************************//**
 * @brief                       delay in milli seconds
 * @param[in]           delay    The delay required (in milliseconds)
 * @return              None.
 **********************************************************************/
void  Host_DelayMS (uint32_t  delay)
{
    volatile  uint32_t  i;


    for (i = 0; i < delay; i++) {
        Host_DelayUS(1000);
    }
}

/*********************************************************************//**
 * @brief                       delay in micro seconds
 * @param[in]           delay    The delay required (in microseconds)
 * @return              None.
 **********************************************************************/
void  Host_DelayUS (uint32_t  delay)
{
    volatile  uint32_t  i;


    for (i = 0; i < (4 * delay); i++) {    /* This logic was tested. It gives app. 1 micro sec delay        */
        ;
    }
}

/*********************************************************************//**
 * @brief                       Init OTG host controller.
 * @param[in]           None.
 * @return              None.
 **********************************************************************/
void Host_CtrlInit(void)
{
        uint32_t HostBaseAddr;
        
        /* Set up host base address and TD and ED descriptors */
        HostBaseAddr = HOST_BASE_ADDR;

    Hcca       = (volatile  HCCA       *)(HostBaseAddr+0x000);
    TDHead     = (volatile  HCTD       *)(HostBaseAddr+0x100);
    TDTail     = (volatile  HCTD       *)(HostBaseAddr+0x110);
    EDCtrl     = (volatile  HCED       *)(HostBaseAddr+0x120); 
    EDBulkIn   = (volatile  HCED       *)(HostBaseAddr+0x130);
    EDBulkOut  = (volatile  HCED       *)(HostBaseAddr+0x140);
    TDBuffer   = (volatile  uint8_t *)(HostBaseAddr+0x150);
    FATBuffer  = (volatile  uint8_t *)(HostBaseAddr+0x1D0);
        USB_DeviceDescriptor = (volatile  uint8_t *)(HostBaseAddr+0x1000);
        USB_ConfigDescriptor = (volatile  uint8_t *) (USB_DeviceDescriptor + DEVICE_DESCRIPTOR_SIZE);
        UserBuffer = (volatile  uint8_t *)(USB_ConfigDescriptor+DEVICE_CONFIGURATION_SIZE);     
        

    /* Initialize all the TDs, EDs and HCCA to 0  */
    Host_EDInit(EDCtrl);
    Host_EDInit(EDBulkIn);
    Host_EDInit(EDBulkOut);
    Host_TDInit(TDHead);
    Host_TDInit(TDTail);
    Host_HCCAInit(Hcca);

    Host_DelayMS(50);                /* Wait 50 ms before apply reset              */
        
        LPC_USB->Control       = 0;    /* HARDWARE RESET  (HCFS = 00b)       */
    LPC_USB->ControlHeadED = 0;    /* Initialize Control list head to Zero       */
    LPC_USB->BulkHeadED    = 0;    /* Initialize Bulk list head to Zero          */
    
    /* SOFTWARE RESET                             */
    LPC_USB->CommandStatus = OR_CMD_STATUS_HCR;
    LPC_USB->FmInterval    = DEFAULT_FMINTERVAL;  /* Write Fm Interval and Largest Data Packet Counter */

                                                    /* Put HC in operational state                */
    LPC_USB->Control  = (LPC_USB->Control & (~OR_CONTROL_HCFS)) | OR_CONTROL_HC_OPER;
    LPC_USB->RhStatus = OR_RH_STATUS_LPSC;        /* Set Global Power                           */
    
    LPC_USB->HCCA = (uint32_t)Hcca;
    LPC_USB->InterruptStatus |= LPC_USB->InterruptStatus;                   /* Clear Interrrupt Status                    */

    LPC_USB->InterruptEnable  = OR_INTR_ENABLE_MIE |
                         OR_INTR_ENABLE_WDH |
                         OR_INTR_ENABLE_RHSC |
                         OR_INTR_ENABLE_UE;
}

/*********************************************************************//**
 * @brief                       Init host controller.
 * @param[in]           None.
 * @return              None.
 **********************************************************************/
void  Host_Init (void)
{

    LPC_SC->PCONP   |= 0x80000000;              /* Enable USB Interface */
        
    LPC_USB->OTGClkCtrl   = 0x00000019;  /* Enable USB host clock, OTG clock & AHB master clock         */
    while ((LPC_USB->OTGClkSt & 0x00000019) != 0x19);
        
        LPC_USB->StCtrl = 0x1;

#if _CURR_USING_BRD == _EA_PA_BOARD

        // Port U1
        PINSEL_ConfigPin(0,29,1);       /* USB_D+1      */
        PINSEL_ConfigPin(0,30,1);       /* USB_D-1      */
        
        PINSEL_ConfigPin(2,9,1);                /* USB_CONNECT1 */
        PINSEL_ConfigPin(1,18,1);               /* USB_UP_LED1  */

#if 0
        PINSEL_ConfigPin(1,19,1);           /* USB_TX_E1*/ 
        PINSEL_ConfigPin(1,22,1);           /* USB_RX_E1*/ 
    PINSEL_ConfigPin(1,20,1);           /* USB_TX_DP1 */
    PINSEL_ConfigPin(1,21,1);           /* USB_TX_DM1 */
    PINSEL_ConfigPin(1,23,1);           /* USB_RX_DP1 */
    PINSEL_ConfigPin(1,24,1);           /* USB_RX_DM1 */
#else
        PINSEL_ConfigPin(1,19,2);               /* USB_PPWR1    */
        PINSEL_ConfigPin(1,27,1);               /* USB_OVRCR1   */
#endif

        // Port U2
        PINSEL_ConfigPin(0,31,1);               /* USB_D+2      */
        PINSEL_ConfigPin(0,14,3);               /* USB_CONNECT2 */
        PINSEL_ConfigPin(0,13,1);               /* USB_UP_LED2  */

#if 1 /* Use USB Power Switch */
        PINSEL_ConfigPin(0,12,1);               /* USB_PPWR2    */
        PINSEL_ConfigPin(1,31,1);               /* USB_OVRCR2   */
#else 
        PINSEL_ConfigPin(1,30,2);               /* USB_VBUS     */
#endif

#else     /* _CURR_USING_BRD == _IAR_OLIMEX_BOARD */
        // Port U2
        PINSEL_ConfigPin(0,31,1);               /* USB_D+2      */
        PINSEL_ConfigPin(0,14,3);               /* USB_CONNECT2 */
        PINSEL_ConfigPin(0,13,1);               /* USB_UP_LED2  */

        PINSEL_ConfigPin(0,12,1);               /* USB_PPWR2    */
        PINSEL_ConfigPin(1,31,1);               /* USB_OVRCR2   */

#endif /* _CURR_USING_BRD == _EA_PA_BOARD */

    Host_CtrlInit();


    /* Enable the USB Interrupt */
        NVIC_EnableIRQ(USB_IRQn);               /* enable USB interrupt */
        NVIC_SetPriority (USB_IRQn, 0);                 /* highest priority */


}

/*********************************************************************//**
 * @brief                       services the interrupt caused by host controller.
 * @param[in]           None.
 * @return              None.
 **********************************************************************/
void  USB_IRQHandler (void)
{
    uint32_t   int_status;
    uint32_t   ie_status;

    int_status    = LPC_USB->InterruptStatus;                          /* Read Interrupt Status                */
    ie_status     = LPC_USB->InterruptEnable;                          /* Read Interrupt enable status         */
 
    if (!(int_status & ie_status)) {
        return;
    } else {

        int_status = int_status & ie_status;
        if (int_status & OR_INTR_STATUS_RHSC) {                 /* Root hub status change interrupt     */
            if (LPC_USB->RhPortStatus1 & OR_RH_PORT_CSC) {
                                if (LPC_USB->RhStatus & OR_RH_STATUS_DRWE) {
                                        /*
                                         * When DRWE is on, Connect Status Change
                                         * means a remote wakeup event.
                                        */
                                        HOST_RhscIntr |= 0x01;// JUST SOMETHING FOR A BREAKPOINT
                                }
                                else {
                                        /*
                                         * When DRWE is off, Connect Status Change
                                         * is NOT a remote wakeup event
                                        */
                                        if (LPC_USB->RhPortStatus1 & OR_RH_PORT_CCS) {
                                                        HOST_TDControlStatus = 0;
                                                        HOST_WdhIntr = 0;
                                                        HOST_RhscIntr |= 0x01;
                                                        gUSBConnected = 1;
                                        } else {
                                                        //LPC_USB->InterruptEnable = 0; // why do we get multiple disc. rupts???
                                                        HOST_RhscIntr &= ~0x01;
                                                        gUSBConnected = 0;
                                        }
                }
                LPC_USB->RhPortStatus1 = OR_RH_PORT_CSC;
            }
                        if (LPC_USB->RhPortStatus2 & OR_RH_PORT_CSC) {
                                if (LPC_USB->RhStatus & OR_RH_STATUS_DRWE) {
                                        /*
                                        * When DRWE is on, Connect Status Change
                                        * means a remote wakeup event.
                                        */
                                        HOST_RhscIntr |= 0x02;// JUST SOMETHING FOR A BREAKPOINT
                                }
                                else {
                                        /*
                                        * When DRWE is off, Connect Status Change
                                        * is NOT a remote wakeup event
                                        */
                                        if (LPC_USB->RhPortStatus2 & OR_RH_PORT_CCS) {
                                                        HOST_TDControlStatus = 0;
                                                        HOST_WdhIntr = 0;
                                                        HOST_RhscIntr |= 0x02;
                                                        gUSBConnected = 1;
                                        } else {
                                                        //LPC_USB->InterruptEnable = 0; // why do we get multiple disc. rupts???
                                                        HOST_RhscIntr &= ~0x02;
                                                        gUSBConnected = 0;
                                        }
                                }
                                LPC_USB->RhPortStatus2 = OR_RH_PORT_CSC;
                        }
                        if (LPC_USB->RhPortStatus1 & OR_RH_PORT_PRSC) {
                                LPC_USB->RhPortStatus1 = OR_RH_PORT_PRSC;
                        }
                        if (LPC_USB->RhPortStatus2 & OR_RH_PORT_PRSC) {
                                LPC_USB->RhPortStatus2 = OR_RH_PORT_PRSC;
                        }
                }
        if (int_status & OR_INTR_STATUS_WDH) {                  /* Writeback Done Head interrupt        */
            HOST_WdhIntr = 1;
                        HOST_TDControlStatus = (TDHead->Control >> 28) & 0xf;
        }
                if (int_status & OR_INTR_STATUS_UE) {                   /* Unrecoverable Error interrupt        */
                        UnrecoverableIntCount++;
        }    

        LPC_USB->InterruptStatus = int_status;                         /* Clear interrupt status register      */
    }           
    return;
}

/*********************************************************************//**
 * @brief                       enumerate the device connected.
 * @param[in]           None.
 * @return              None.
 **********************************************************************/
int32_t  Host_EnumDev (void)
{
    int32_t  rc;

    while (!gUSBConnected);
    Host_DelayMS(100);                             /* USB 2.0 spec says atleast 50ms delay beore port reset */
        
        if ( HOST_RhscIntr & 0x01 )
        {
          LPC_USB->RhPortStatus1 = OR_RH_PORT_PRS; // Initiate port reset
          while (LPC_USB->RhPortStatus1 & OR_RH_PORT_PRS)
                ; // Wait for port reset to complete...
          LPC_USB->RhPortStatus1 = OR_RH_PORT_PRSC; // ...and clear port reset signal
        }
        if ( HOST_RhscIntr & 0x02 )
        {
          LPC_USB->RhPortStatus2 = OR_RH_PORT_PRS; // Initiate port reset
          while (LPC_USB->RhPortStatus2 & OR_RH_PORT_PRS)
                ; // Wait for port reset to complete...
          LPC_USB->RhPortStatus2 = OR_RH_PORT_PRSC; // ...and clear port reset signal
        }
    Host_DelayMS(200);                                                 /* Wait for 100 MS after port reset  */

    EDCtrl->Control = DEVICE_DESCRIPTOR_SIZE << 16;                    /* Put max pkt size = 8              */
                                                                       /* Read device desc */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_DEVICE, 0, USB_DeviceDescriptor, DEVICE_DESCRIPTOR_SIZE);
    if (rc != USB_HOST_FUNC_OK) {
        return (rc);
    }
    EDCtrl->Control = USB_DeviceDescriptor[7] << 16;                               /* Get max pkt size of endpoint 0    */
    rc = HOST_SET_ADDRESS(1);                                          /* Set the device address to 1       */
    if (rc != USB_HOST_FUNC_OK) {
        return (rc);
    }
    Host_DelayMS(2);
    EDCtrl->Control = (EDCtrl->Control) | 1;                          /* Modify control pipe with address 1 */
                                                                      /* Get the configuration descriptor   */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_CONFIGURATION, 0, USB_ConfigDescriptor, DEVICE_CONFIGURATION_SIZE);
    if (rc != USB_HOST_FUNC_OK) {
        return (rc);
    }
                                                                       /* Get the first configuration data  */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_CONFIGURATION, 0, USB_ConfigDescriptor, 
                                                        ReadLE16U(&USB_ConfigDescriptor[2]));
    if (rc != USB_HOST_FUNC_OK) {
        return (rc);
    }
    rc = Host_ParseConfiguration();                                      /* Parse the configuration           */
    if (rc != USB_HOST_FUNC_OK) {
        return (rc);
    }
    rc = USBH_SET_CONFIGURATION(1);                                    /* Select device configuration 1     */
    if (rc != USB_HOST_FUNC_OK) {
    }
    Host_DelayMS(100);                                               /* Some devices may require this delay */
    return (rc);
}

/*********************************************************************//**
 * @brief                       parse the configuration.
 * @param[in]           None.
 * @return              OK                            if Success.
 *                         ERR_BAD_CONFIGURATION    Failed in case of bad configuration.
 *                              ERR_NO_MS_INTERFACE       Failed in case of no MS interface.
 **********************************************************************/
int32_t  Host_ParseConfiguration (void)
{
    volatile  uint8_t  *desc_ptr;
              uint8_t   ms_int_found;


    desc_ptr     = USB_ConfigDescriptor;
    ms_int_found = 0;

    if (desc_ptr[1] != USB_DESCRIPTOR_TYPE_CONFIGURATION) {    
        return (ERR_BAD_CONFIGURATION);
    }
    desc_ptr += desc_ptr[0];

    while (desc_ptr != USB_ConfigDescriptor + ReadLE16U(&USB_ConfigDescriptor[2])) {

        switch (desc_ptr[1]) {

            case USB_DESCRIPTOR_TYPE_INTERFACE:                       /* If it is an interface descriptor   */
                 if (desc_ptr[5] == MASS_STORAGE_CLASS &&             /* check if the class is mass storage */
                     desc_ptr[6] == MASS_STORAGE_SUBCLASS_SCSI &&     /* check if the subclass is SCSI      */
                     desc_ptr[7] == MASS_STORAGE_PROTOCOL_BO) {       /* check if the protocol is Bulk only */
                     ms_int_found = 1;
                                         gUSBDeviceType = MASS_STORAGE_DEVICE;
                     desc_ptr    += desc_ptr[0];                      /* Move to next descriptor start      */
                 }
                 break;

            case USB_DESCRIPTOR_TYPE_ENDPOINT:                        /* If it is an endpoint descriptor    */
                 if ((desc_ptr[3] & 0x03) == 0x02) {                  /* If it is Bulk endpoint             */
                     if (desc_ptr[2] & 0x80) {                        /* If it is In endpoint               */
                         EDBulkIn->Control =  1                             |      /* USB address           */
                                              ((desc_ptr[2] & 0x7F) << 7)   |      /* Endpoint address      */
                                              (2 << 11)                     |      /* direction             */
                                              (ReadLE16U(&desc_ptr[4]) << 16);     /* MaxPkt Size           */
                         desc_ptr += desc_ptr[0];                     /* Move to next descriptor start      */
                     } else {                                         /* If it is Out endpoint              */
                         EDBulkOut->Control = 1                             |      /* USB address           */
                                              ((desc_ptr[2] & 0x7F) << 7)   |      /* Endpoint address      */
                                              (1 << 11)                     |      /* direction             */
                                              (ReadLE16U(&desc_ptr[4]) << 16);     /* MaxPkt Size           */
                         desc_ptr += desc_ptr[0];                     /* Move to next descriptor start      */
                     }
                 } else {                                             /* If it is not bulk end point        */
                     desc_ptr += desc_ptr[0];                         /* Move to next descriptor start      */
                 }
                 break;

            default:                                 /* If the descriptor is neither interface nor endpoint */
                 desc_ptr += desc_ptr[0];                             /* Move to next descriptor start      */
                 break;
        }
    }
    if (ms_int_found) {
        return (USB_HOST_FUNC_OK);
    } else {
        return (ERR_NO_MS_INTERFACE);
    }
}
/*********************************************************************//**
 * @brief                       Get the type of the USB which is being connected.
 * @param[in]           None.
 * @return              USB_DEVICE_TYPE value
 **********************************************************************/
USB_DEVICE_TYPE Host_GetDeviceType(void)
{
        return gUSBDeviceType;
}

/*********************************************************************//**
 * @brief                       processes the transfer descriptor.
 * @param[in]           ed            Endpoint descriptor that contains this transfer descriptor.
 * @param[in]            token         SETUP, IN, OUT
 * @param[in]            buffer        Current Buffer Pointer of the transfer descriptor
 * @param[in]            buffer_len    Length of the buffer
 * @return              USB_HOST_FUNC_OK       if TD submission is successful.
 *                          ERROR    if TD submission fails
 **********************************************************************/
int32_t  Host_ProcessTD (volatile  HCED       *ed,
                            volatile  uint32_t  token,
                            volatile  uint8_t *buffer,
                                      uint32_t  buffer_len)
{
    volatile  uint32_t   td_toggle;


    if (ed == EDCtrl) {
        if (token == TD_SETUP) {
            td_toggle = TD_TOGGLE_0;
        } else {
            td_toggle = TD_TOGGLE_1;
        }
    } else {
        td_toggle = 0;
    }
    TDHead->Control = (TD_ROUNDING    |
                      token           |
                      TD_DELAY_INT(0) |                           
                      td_toggle       |
                      TD_CC);
    TDTail->Control = 0;
    TDHead->CurrBufPtr   = (uint32_t) buffer;
    TDTail->CurrBufPtr   = 0;
    TDHead->Next         = (uint32_t) TDTail;
    TDTail->Next         = 0;
    TDHead->BufEnd       = (uint32_t)(buffer + (buffer_len - 1));
    TDTail->BufEnd       = 0;

    ed->HeadTd  = (uint32_t)TDHead | ((ed->HeadTd) & 0x00000002);
    ed->TailTd  = (uint32_t)TDTail;
    ed->Next    = 0;

    if (ed == EDCtrl) {
        LPC_USB->ControlHeadED  = (uint32_t)ed;
        LPC_USB->CommandStatus |= OR_CMD_STATUS_CLF;
        LPC_USB->Control       |= OR_CONTROL_CLE;
    } else {
        LPC_USB->BulkHeadED     = (uint32_t)ed;
        LPC_USB->CommandStatus |= OR_CMD_STATUS_BLF;
        LPC_USB->Control       |= OR_CONTROL_BLE;
    }    

    Host_WDHWait();

//    if (!(TDHead->Control & 0xF0000000)) {
        if (!HOST_TDControlStatus) {
        return (USB_HOST_FUNC_OK);
    } else {      
        return (ERR_TD_FAIL);
    }
}
   
/*********************************************************************//**
 * @brief                       receive the control information.
 * @param[in]           bm_request_type.
 * @param[in]            b_request
 * @param[in]            w_value
 * @param[in]           w_index
 * @param[in]           w_length
 * @param[in]           buffer
 * @return              USB_HOST_FUNC_OK       if Success
 *                          ERROR    if Failed
 **********************************************************************/
int32_t  Host_CtrlRecv (         uint8_t   bm_request_type,
                                    uint8_t   b_request,
                                    uint16_t   w_value,
                                    uint16_t   w_index,
                                    uint16_t   w_length,
                          volatile  uint8_t  *buffer)
{
    int32_t  rc;


    Host_FillSetup(bm_request_type, b_request, w_value, w_index, w_length);
    rc = Host_ProcessTD(EDCtrl, TD_SETUP, TDBuffer, 8);
    if (rc == USB_HOST_FUNC_OK) {
        if (w_length) {
            rc = Host_ProcessTD(EDCtrl, TD_IN, buffer, w_length);
        }
        if (rc == USB_HOST_FUNC_OK) {
            rc = Host_ProcessTD(EDCtrl, TD_OUT, NULL, 0);
        }
    }
    return (rc);
}

/*********************************************************************//**
 * @brief                       send the control information.
 * @param[in]           bm_request_type.
 * @param[in]            b_request
 * @param[in]            w_value
 * @param[in]           w_index
 * @param[in]           w_length
 * @param[in]           buffer
 * @return              USB_HOST_FUNC_OK       if Success
 *                          ERROR    if Failed
 **********************************************************************/
int32_t  Host_CtrlSend (          uint8_t   bm_request_type,
                                     uint8_t   b_request,
                                     uint16_t   w_value,
                                     uint16_t   w_index,
                                     uint16_t   w_length,
                           volatile  uint8_t  *buffer)
{
    int32_t  rc;

    Host_FillSetup(bm_request_type, b_request, w_value, w_index, w_length);

    rc = Host_ProcessTD(EDCtrl, TD_SETUP, TDBuffer, 8);
    if (rc == USB_HOST_FUNC_OK) {
        if (w_length) {
            rc = Host_ProcessTD(EDCtrl, TD_OUT, buffer, w_length);
        }
        if (rc == USB_HOST_FUNC_OK) {
            rc = Host_ProcessTD(EDCtrl, TD_IN, NULL, 0);
        }
    }
    return (rc);
}

/*********************************************************************//**
 * @brief                       fill the setup packet.
 * @param[in]           None.
 * @return              USB_HOST_FUNC_OK       if Success
 *                          ERROR    if Failed
 **********************************************************************/
void  Host_FillSetup (uint8_t   bm_request_type,
                      uint8_t   b_request,
                      uint16_t   w_value,
                      uint16_t   w_index,
                      uint16_t   w_length)
{
        int i;
        for (i=0;i<w_length;i++)
                TDBuffer[i] = 0;
        
    TDBuffer[0] = bm_request_type;
    TDBuffer[1] = b_request;
    WriteLE16U(&TDBuffer[2], w_value);
    WriteLE16U(&TDBuffer[4], w_index);
    WriteLE16U(&TDBuffer[6], w_length);
}

/*********************************************************************//**
 * @brief                       initializes transfer descriptor.
 * @param[in]           td       Pointer to TD structure.
 * @return              None.
 **********************************************************************/
void  Host_TDInit (volatile  HCTD *td)
{

    td->Control    = 0;
    td->CurrBufPtr = 0;
    td->Next       = 0;
    td->BufEnd     = 0;
}


/*********************************************************************//**
 * @brief                       initializes endpoint descriptor.
 * @param[in]           td       Pointer to ED structure.
 * @return              None.
 **********************************************************************/
void  Host_EDInit (volatile  HCED *ed)
{

    ed->Control = 0;
    ed->TailTd  = 0;
    ed->HeadTd  = 0;
    ed->Next    = 0;
}

/*********************************************************************//**
 * @brief                       initializes host controller communications area.
 * @param[in]           hcca       Pointer to HCCA.
 * @return              None.
 **********************************************************************/
void  Host_HCCAInit (volatile  HCCA  *hcca)
{
    uint32_t  i;

    for (i = 0; i < 32; i++) {

        hcca->IntTable[i] = 0;
        hcca->FrameNumber = 0;
        hcca->DoneHead    = 0;
    }

}


/*********************************************************************//**
 * @brief                       infinite loop which breaks when ever a WDH interrupt rises.
 * @param[in]           None.
 * @return              None.
 **********************************************************************/
void  Host_WDHWait (void)
{
  while (!HOST_WdhIntr) {
    ;
  }
  HOST_WdhIntr = 0;
}


/*********************************************************************//**
 * @brief                       read an unsigned integer from a charecter buffer in the platform
 *              containing little endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @return              val     Unsigned integer.
 **********************************************************************/
uint32_t  ReadLE32U (volatile  uint8_t  *pmem)
{
    uint32_t   val;

    ((uint8_t *)&val)[0] = pmem[0];
    ((uint8_t *)&val)[1] = pmem[1];
    ((uint8_t *)&val)[2] = pmem[2];
    ((uint8_t *)&val)[3] = pmem[3];
    return (val);
}


/*********************************************************************//**
 * @brief                       write an unsigned integer into a charecter buffer in the platform 
 *              containing little endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @param[in]           val     Integer value to be placed in the charecter buffer
 * @return              None.
 **********************************************************************/
 void  WriteLE32U (volatile  uint8_t  *pmem,
                            uint32_t   val)
{
    pmem[0] = ((uint8_t *)&val)[0];
    pmem[1] = ((uint8_t *)&val)[1];
    pmem[2] = ((uint8_t *)&val)[2];
    pmem[3] = ((uint8_t *)&val)[3];
}


/*********************************************************************//**
 * @brief                       read an unsigned short integer from a charecter buffer in the platform
*              containing little endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @return              val     Unsigned short integer.
 **********************************************************************/
uint16_t  ReadLE16U (volatile  uint8_t  *pmem)
{
    uint16_t   val;

    ((uint8_t *)&val)[0] = pmem[0];
    ((uint8_t *)&val)[1] = pmem[1];
    return (val);
}

/*********************************************************************//**
 * @brief                       write an unsigned short integer into a charecter buffer in the
 *              platform containing little endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @param[in]          val     Value to be placed in the charecter buffer
 * @return              None.
 **********************************************************************/
void  WriteLE16U (volatile  uint8_t  *pmem,
                            uint16_t   val)
{
    pmem[0] = ((uint8_t *)&val)[0];
    pmem[1] = ((uint8_t *)&val)[1];
}

/*
**************************************************************************************************************
*                                         READ BE 32U
*
* Description: This function is used to read an unsigned integer from a charecter buffer in the platform
*              containing big endian processor
*
* Arguments  : pmem    Pointer to the charecter buffer
*
* Returns    : val     Unsigned integer
*
**************************************************************************************************************
*/

/*********************************************************************//**
 * @brief                       read an unsigned integer from a charecter buffer in the platform
 *              containing big endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @return              val     Unsigned integer.
 **********************************************************************/
uint32_t  ReadBE32U (volatile  uint8_t  *pmem)
{
    uint32_t   val;

    ((uint8_t *)&val)[0] = pmem[3];
    ((uint8_t *)&val)[1] = pmem[2];
    ((uint8_t *)&val)[2] = pmem[1];
    ((uint8_t *)&val)[3] = pmem[0];
    return (val);
}

/*********************************************************************//**
 * @brief                       write an unsigned integer into a charecter buffer in the platform
 *              containing big endian processor
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @param[in]          val     Value to be placed in the charecter buffer       
 * @return              None.
 **********************************************************************/
void  WriteBE32U (volatile  uint8_t  *pmem,
                            uint32_t   val)
{
    pmem[0] = ((uint8_t *)&val)[3];
    pmem[1] = ((uint8_t *)&val)[2];
    pmem[2] = ((uint8_t *)&val)[1];
    pmem[3] = ((uint8_t *)&val)[0];
}

/*********************************************************************//**
 * @brief                       read an unsigned short integer from a charecter buffer in the platform
*              containing big endian processor
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @return              val     Unsigned short integer.
 **********************************************************************/
 uint16_t  ReadBE16U (volatile  uint8_t  *pmem)
{
    uint16_t  val;

    ((uint8_t *)&val)[0] = pmem[1];
    ((uint8_t *)&val)[1] = pmem[0];
    return (val);
}

/*********************************************************************//**
 * @brief                       write an unsigned short integer into the charecter buffer in the
 *              platform containing big endian processor.
 * @param[in]           pmem    Pointer to the charecter buffer.
 * @param[in]            val     Value to be placed in the charecter buffer
 * @return              None.
 **********************************************************************/
 void  WriteBE16U (volatile  uint8_t  *pmem,
                            uint16_t   val)
{
    pmem[0] = ((uint8_t *)&val)[1];
    pmem[1] = ((uint8_t *)&val)[0];
}

/**
* @}
*/

