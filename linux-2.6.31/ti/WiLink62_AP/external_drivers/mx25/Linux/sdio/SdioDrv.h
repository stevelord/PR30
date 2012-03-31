/*
 * SdioDrv.h
 *
 */

#ifndef __MX25_SDIODRV_API_H
#define __MX25_SDIODRV_API_H

#include <asm/types.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>



unsigned char tiwlan_direct_readb(struct sdio_func *func, int func_no ,unsigned int addr,
        int *err_ret);
void tiwlan_direct_writeb(struct sdio_func *func, int func_no , unsigned char b, unsigned int addr,
        int *err_ret);

/* Pprotocol defined constants */  

#define CCCR_SDIO_REVISION                  0x00
#define CCCR_SD_SPECIFICATION_REVISION      0x01
#define CCCR_IO_ENABLE                      0x02
#define CCCR_IO_READY                       0x03
#define CCCR_INT_ENABLE                     0x04
#define CCCR_INT_PENDING                    0x05
#define CCCR_IO_ABORT                       0x06
#define CCCR_BUS_INTERFACE_CONTOROL         0x07
#define CCCR_CARD_CAPABILITY                0x08
#define CCCR_COMMON_CIS_POINTER             0x09        //0x09-0x0B
#define CCCR_FNO_BLOCK_SIZE                     0x10    //0x10-0x11
#define FN0_CCCR_REG_32                     0x64

         
/********************************************************************/
/*	SDIO driver functions prototypes                                */
/********************************************************************/
/*
int sdioDrv_ConnectBus     (void *       fCbFunc,
                            void *       hCbArg,
                            unsigned int uBlkSizeShift,
                            unsigned int uSdioThreadPriority,
                            unsigned char **pTxDmaSrcAddr);
*/
int sdioDrv_ConnectBus     (void *       fCbFunc,
                            void *       hCbArg,
                            unsigned int uBlkSizeShift,
                            unsigned int uSdioThreadPriority);

int sdioDrv_DisconnectBus  (void);

int sdioDrv_ExecuteCmd     (unsigned int uCmd, 
                            unsigned int uArg, 
                            unsigned int uRespType, 
                            void *       pResponse, 
                            unsigned int uLen);
                           
int sdioDrv_ReadAsync      (unsigned int uFunc, 
                            unsigned int uHwAddr, 
                            void *       pData, 
                            unsigned int uLen, 
                            unsigned int bBlkMode,
                            unsigned int bIncAddr,
                            unsigned int bMore);

int sdioDrv_WriteAsync     (unsigned int uFunc, 
                            unsigned int uHwAddr, 
                            void *       pData, 
                            unsigned int uLen, 
                            unsigned int bBlkMode,
                            unsigned int bIncAddr,
                            unsigned int bMore);

int sdioDrv_ReadSyncBytes  (unsigned int  uFunc, 
                            unsigned int  uHwAddr, 
                            unsigned char *pData, 
                            unsigned int  uLen, 
                            unsigned int  bMore);
                           
int sdioDrv_WriteSyncBytes (unsigned int  uFunc, 
                            unsigned int  uHwAddr, 
                            unsigned char *pData, 
                            unsigned int  uLen, 
                            unsigned int  bMore);

void sdioDrv_register_pm(int (*wlanDrvIf_Start)(void),
						int (*wlanDrvIf_Stop)(void));



#endif/* _MX25_SDIODRV_H */
