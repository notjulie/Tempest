/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Header for usbd_cdc_vcp.c file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"

#include "usbd_cdc_core.h"
#include "usbd_conf.h"
#include <stdint.h>

extern class AbstractTempestStream &USBStream;


class VirtualComPort
{
public:
	VirtualComPort(void);

	bool WasBreakReceived(void) const { return breakReceived; }
	void ClearBreak(void) { breakReceived = false; }

	void Service(void);

public:
	// the low level ops
	uint16_t Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);

private:
	bool breakReceived;
};

extern VirtualComPort VCP;

bool GetUSBReceiveHeartbeat();

#endif /* __USBD_CDC_VCP_H */

