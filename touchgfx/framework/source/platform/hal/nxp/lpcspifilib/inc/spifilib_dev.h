/*
 * @brief LPCSPIFILIB FLASH library device specific functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licenser disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __SPIFILIB_DEV_H_
#define __SPIFILIB_DEV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Define for inline functions */
#ifndef INLINE
#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif /* __CC_ARM */
#endif /* !INLINE */

/** @defgroup LPCSPIFILIB_DEV LPCSPIFILIB device driver API functions
 * @ingroup LPCSPIFILIB
 * @{
 */
/**
 * @brief Possible error codes that can be returned from functions
 */
typedef enum {
        SPIFI_ERR_NONE = 0,                                                     /**< No error */
        SPIFI_ERR_BUSY,                                                                 /**< Device is busy */
        SPIFI_ERR_GEN,                                                                  /**< General error */
        SPIFI_ERR_NOTSUPPORTED,                                 /**< Capability not supported */
        SPIFI_ERR_ALIGNERR,                                                     /**< Attempted to do an operation on an unaligned section of the device */
        SPIFI_ERR_LOCKED,                                                               /**< Device was locked and a program/erase operation was attempted */
        SPIFI_ERR_PROGERR,                                                      /**< Error programming device (blocking mode only) */
        SPIFI_ERR_ERASEERR,                                                     /**< Erase error (blocking mode only) */
        SPIFI_ERR_NOTBLANK,                                                     /**< Program operation on block that is not blank */
        SPIFI_ERR_PAGESIZE,                                                     /**< PageProgram write size exceeds page size */
        SPIFI_ERR_VAL,                                                                  /**< Program operation failed validation or readback compare */
        SPIFI_ERR_RANGE,                                                                /**< Range error, bad block number, address out of range, etc. */
        SPIFI_ERR_MEMMODE,                                                      /**< Library calls not allowed while in memory mode. */
        SPIFI_ERR_LASTINDEX
} SPIFI_ERR_T;

/**
 * @brief Possible device capabilities returned from getInfo()
 */
#define SPIFI_CAP_QUAD          (1 << 0)                        /**< Supports QUAD device mode */
#define SPIFI_CAP_FULLLOCK      (1 << 1)                        /**< Full device lock supported */
#define SPIFI_CAP_BLOCKLOCK     (1 << 2)                        /**< Individual block device lock supported */
#define SPIFI_CAP_SUBBLKERASE   (1 << 3)                        /**< Sub-block erase supported */
#define SPIFI_CAP_NOBLOCK       (1 << 16)                       /**< Non-blocking mode supported */

/**
 * @brief Possible device statuses returned from getInfo()
 */
#define SPIFI_STAT_BUSY     (1 << 0)                    /**< Device is busy erasing or programming */
#define SPIFI_STAT_ISWP     (1 << 1)                    /**< Device is write protected (software or hardware) */
#define SPIFI_STAT_FULLLOCK (1 << 2)                    /**< Device is fully locked */
#define SPIFI_STAT_PARTLOCK (1 << 3)                    /**< Device is partially locked (device specific) */
#define SPIFI_STAT_PROGERR  (1 << 4)                    /**< Device status shows a program error (non-blocking mode only) */
#define SPIFI_STAT_ERASEERR (1 << 5)                    /**< Device status shows a erase error (non-blocking mode only) */

/**
 * @brief Possible driver options, may not be supported by all drivers
 */
#define SPIFI_OPT_USE_QUAD      (1 << 0)                        /**< Enables QUAD operations, only supported if the SPIFI_CAP_QUAD capability exists */
#define SPIFI_OPT_NOBLOCK       (1 << 16)                       /**< Will not block on program and erase operations, poll device status manually */

/**
 * @brief Device identification data
 */
typedef struct {
        uint8_t mfgId[3];                                                       /**< JEDEC ID data */
        uint8_t extCount;                                                       /**< Number of extended bytes to check */
        uint8_t extId[8];                                                       /**< extended data */
} SPIFI_DEVICE_ID_T;

/**
 * @brief Common data applicable to all devices
 */
typedef struct {
        uint32_t        spifiCtrlAddr;                      /**< SPIFI controller base address */
        uint32_t        baseAddr;                                       /**< Physical base address for the device */
        uint32_t        numBlocks;                                      /**< Number of blocks on the device */
        uint32_t        blockSize;                                      /**< Size of blocks on the device */
        uint32_t        numSubBlocks;                           /**< Number of sub-blocks on the device */
        uint32_t        subBlockSize;                           /**< Size of sub-blocks on the device */
        uint32_t        pageSize;                                       /**< Size of a page, usually denotes maximum write size in bytes for a single write operation */
        uint32_t        maxReadSize;                            /**< Maximum read size in bytes for a single read operation */
        uint32_t        maxClkRate;                                     /**< Maximum clock rate of device in Hz */
        uint32_t        caps;                                           /**< Device capabilities of values SPIFI_CAP_* */
        uint32_t        opts;                                           /**< Device options of values SPIFI_OPT_* */
        const char      *pDevName;                                      /**< Pointer to generic device name */
        SPIFI_ERR_T     lastErr;                                        /**< Last error for the driver */
        SPIFI_DEVICE_ID_T id;                                           /**< Device id structure (JEDEC ID etc) */
} SPIFI_INFODATA_T;

/* Forward type declaration */
struct SPIFI_HANDLE;

/**
 * @brief Possible info lookup requests
 */
typedef enum {
        SPIFI_INFO_DEVSIZE = 0,                                         /**< Device size in Bytes */
        SPIFI_INFO_ERASE_BLOCKS,                                        /**< Number of erase blocks */
        SPIFI_INFO_ERASE_BLOCKSIZE,                             /**< Size of erase blocks */
        SPIFI_INFO_ERASE_SUBBLOCKS,                                     /**< Number of erase sub-blocks */
        SPIFI_INFO_ERASE_SUBBLOCKSIZE,                          /**< Size of erase sub-blocks */
        SPIFI_INFO_PAGESIZE,                                            /**< Size of a page, page write size limit */
        SPIFI_INFO_MAXREADSIZE,                                         /**< Maximum read size, read size limit in bytes */
        SPIFI_INFO_MAXCLOCK,                                            /**< Maximum device speed in Hz */
        SPIFI_INFO_CAPS,                                                        /**< Device capabilities, OR'ed SPIFI_CAP_* values */
        SPIFI_INFO_STATUS,                                                      /**< Device capabilities, Or'ed SPIFI_STAT_* values */
        SPIFI_INFO_OPTIONS                                                      /**< Device capabilities, Or'ed SPIFI_OPT_* values */
} SPIFI_INFO_ID_T;

/**
 * @brief Possible device specific lock / un-lock commands
 */
typedef enum {
    SPIFI_PCMD_UNLOCK_DEVICE = 0,           /**< unlock device */
    SPIFI_PCMD_LOCK_DEVICE,                 /**< lock device */
    SPIFI_PCMD_UNLOCK_BLOCK,                /**< unlock specified block */
    SPIFI_PCMD_LOCK_BLOCK                   /**< lock specified block */
    
} SPIFI_PCMD_LOCK_UNLOCK_T;

/**
 * @brief Possible device specific sub-block commands
 */
typedef enum {
    SPIFI_PCMD_ADDR_TO_SUB_BLOCK = 0,           /**< Convert address to a sub-block */
    SPIFI_PCMD_SUB_BLOCK_TO_ADDR,               /**< Convert sub-block to address */
    SPIFI_PCMD_BLOCK_TO_SUB_BLOCK               /**< Convert block to sub-block */
    
} SPIFI_PCMD_SUBBLK_T;

/**
 * @brief Device specific function pointers
 */
typedef struct {
        /* Device init and de-initialization */
        SPIFI_ERR_T (*init)(struct SPIFI_HANDLE *);                                                                     /**< (Optional) Initialize SPIFI device */

        SPIFI_ERR_T (*deInit)(struct SPIFI_HANDLE *);                                                           /**< (Required) De-initialize SPIFI device */
    
    SPIFI_ERR_T (*lockCmd)(struct SPIFI_HANDLE *, SPIFI_PCMD_LOCK_UNLOCK_T, uint32_t);  /**< (Required) Lock / unlock handler */

        SPIFI_ERR_T (*eraseAll)(struct SPIFI_HANDLE *);                                                                         /**< (Required) Full device erase */

        SPIFI_ERR_T (*eraseBlock)(struct SPIFI_HANDLE *, uint32_t);                                                     /**< (Required) Erase a block by block number */

        SPIFI_ERR_T (*eraseSubBlock)(struct SPIFI_HANDLE *, uint32_t);                                          /**< (Reauired) Erase a sub-block by block number */

        SPIFI_ERR_T (*pageProgram)(struct SPIFI_HANDLE *, uint32_t, const uint32_t *, uint32_t);        /**< (Required) Program up to a page of data at an address */

        SPIFI_ERR_T (*read)(struct SPIFI_HANDLE *, uint32_t, uint32_t *, uint32_t);             /**< (Required) Read an address range */

        SPIFI_ERR_T (*setOpts)(struct SPIFI_HANDLE *, uint32_t, uint8_t);                                       /**< (Required) Set or Unset driver options */

        SPIFI_ERR_T (*reset)(struct SPIFI_HANDLE *);                                                                            /**< (Required) Reset SPIFI device */

        /* Info query functions */
        uint32_t (*getMemModeCmd)(struct SPIFI_HANDLE *, uint8_t);                                                      /**< (Required) Returns memoryMode cmd */

        uint32_t (*getStatus)(struct SPIFI_HANDLE *);                                                                           /**< (Required) Returns device status */
    
    uint32_t (*subBlockCmd)(struct SPIFI_HANDLE *, SPIFI_PCMD_SUBBLK_T, uint32_t);      /**< (Optional) Performs specified cmd */
    
} SPIFI_DEV_T;

/**
 * @brief Register device data.
 */
typedef struct SPIFI_DEV_PDATA {
    const char *pDevName;                       /**< Device friendly name */
        SPIFI_DEVICE_ID_T id;                                           /**< Device id structure */
        uint16_t blks;                                                                  /**< # of blocks */
        uint32_t blkSize;                                                               /**< size of block */
        uint16_t subBlks;                                                               /**< # of sub-blocks */
        uint16_t subBlkSize;                                            /**< size of sub-block */
        uint16_t pageSize;                                                      /**< page size */
        uint32_t maxReadSize;                                           /**< max read allowed in one operation */
        uint32_t maxClkRate;                                            /**< maximum clock rate */
        uint32_t caps;                                                                  /**< capabilities supported */
} SPIFI_DEV_PDATA_T;

/**
 * @brief Register device data node
 */
typedef struct SPIFI_DEV_DATA {
        const SPIFI_DEV_PDATA_T *pDevData;      /**< Pointer to device geometry / caps */
        struct SPIFI_DEV_DATA *pNext;                   /**< Reserved */

} SPIFI_DEV_DATA_T;

/**
 * @brief LPCSPIFILIB device handle, used with all device and info functions
 */
typedef struct SPIFI_HANDLE {
        const SPIFI_DEV_T       *pDev;                                  /**< Pointer to device specific functions */
        SPIFI_INFODATA_T        *pInfoData;                     /**< Pointer to info data area */
        void                    *pDevData;                      /**< Pointer to device context (used by device functions) */
} SPIFI_HANDLE_T;

/**
 * @brief LPCSPIFILIB device descriptor, used to describe devices to non-device specific functions
 */
typedef struct {
        const char              *pFamName;                                              /**< Pointer to generic family name */
        SPIFI_DEV_DATA_T        *pDevList;                                              /**< Pointer to device specific list */
        uint32_t                prvDataSize;                                    /**< Number of bytes needed for driver private data area allocation */

        void (*pPrvDevGetID)(uint32_t baseAddr, SPIFI_DEVICE_ID_T *pID);/**< Pointer to method that querries deviceID */
        SPIFI_ERR_T (*pPrvDevSetup)(SPIFI_HANDLE_T *pHandle, uint32_t spifiCtrlAddr, uint32_t baseAddr);        /**< Pointer to device specific device initialization */
        SPIFI_ERR_T (*pPrvDevRegister)(void *pFamily, SPIFI_DEV_DATA_T *pDevData);      /**< Pointer to device specific register device function */

} SPIFI_FAM_DESC_T;

/**
 * @brief LPCSPIFILIB family data.
 */
typedef struct SPIFI_DEV_FAMILY {
        const SPIFI_FAM_DESC_T *pDesc;                                                  /**< Pointer to device descriptor */
        struct SPIFI_DEV_FAMILY *pNext;                                         /**< Reserved list pointer */

} SPIFI_DEV_FAMILY_T;

/**
 * @}
 */

/** @defgroup LPCSPIFILIB_REGISTERHELPER LPCSPIFILIB family registration functions
 * @ingroup LPCSPIFILIB
 * @{
 */

/**
 * @brief       Family registration function
 * @return      A pointer to a non-volatile SPIFI_DEV_FAMILY_T initialized for family.
 * @note        This function constructs and returns a non-volitile SPIFI_DEV_FAMILY_T
 * structure that contains family specific information needed to register family.
 * This function MUST NOT be called directly and should only be passed to the
 * registration function spifiRegisterFamily()
 */
SPIFI_DEV_FAMILY_T *SPIFI_REG_FAMILY_Spansion_2Byte_PStatus(void);
#define SPIFI_REG_FAMILY_SpansionS25FLP SPIFI_REG_FAMILY_Spansion_2Byte_PStatus /**< Depricated! Do NOT use for new development */

/**
 * @brief       Family registration function
 * @return      A pointer to a non-volatile SPIFI_DEV_FAMILY_T initialized for family.
 * @note        This function constructs and returns a non-volitile SPIFI_DEV_FAMILY_T
 * structure that contains family specific information needed to register family.
 * This function MUST NOT be called directly and should only be passed to the
 * registration function spifiRegisterFamily()
 */
SPIFI_DEV_FAMILY_T *SPIFI_REG_FAMILY_Spansion_3Byte_Status(void);
#define SPIFI_REG_FAMILY_SpansionS25FL1 SPIFI_REG_FAMILY_Spansion_3Byte_Status /**< Depricated! Do NOT use for new development */

/**
 * @brief       Family registration function
 * @return      A pointer to a non-volatile SPIFI_DEV_FAMILY_T initialized for family.
 * @note        This function constructs and returns a non-volitile SPIFI_DEV_FAMILY_T
 * structure that contains family specific information needed to register family.
 * This function MUST NOT be called directly and should only be passed to the
 * registration function spifiRegisterFamily()
 */
SPIFI_DEV_FAMILY_T *SPIFI_REG_FAMILY_Macronix_2Byte_Status(void);
#define SPIFI_REG_FAMILY_MacronixMX25L SPIFI_REG_FAMILY_Macronix_2Byte_Status /**< Depricated! Do NOT use for new development */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __SPIFILIB_DEV_H_ */
