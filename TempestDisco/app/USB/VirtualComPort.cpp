/**
 ******************************************************************************
 * @file    usbd_cdc_vcp.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    22-July-2011
 * @brief   Generic media access Layer.
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

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "TempestDisco.h"

#include "TempestIO/TempestMemoryStream.h"

#include "SystemError.h"
#include "SystemTime.h"

#include "VirtualComPort.h"


// our global memory stream between the USB port and the app
static TempestMemoryStream memoryStream;
AbstractTempestStream &USBStream = *memoryStream.GetLeftSide();

// our global VirtualComPort instance
VirtualComPort VCP;

static bool newDataReceived = false;


/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);


extern "C" {

	/* These are external variables imported from CDC core to be used for IN
	 transfer management. */
	extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
	 These data will be sent over USB IN endpoint
	 in the CDC core functions. */
	extern uint32_t APP_Rx_ptr_in; /* Increment this pointer or roll it back to
	 start address when writing received data
	 in the buffer APP_Rx_Buffer. */
	extern uint32_t APP_Rx_ptr_out;


	CDC_IF_Prop_TypeDef VCP_fops = {VCP_Init, VCP_DeInit, VCP_Ctrl, VCP_DataTx, VCP_DataRx };


};


VirtualComPort::VirtualComPort(void)
{
	breakReceived = false;
}


void VirtualComPort::Service(void)
{
	AbstractTempestStream *stream = memoryStream.GetRightSide();

	// empty out our send buffer
    for (;;)
    {
    	 // make sure we have room in the output buffer
    	 int newInPointer = APP_Rx_ptr_in + 1;
    	 if (newInPointer >= APP_RX_DATA_SIZE)
    	    newInPointer = 0;
    	 if (newInPointer == (int)APP_Rx_ptr_out)
    		 break;

    	 // write if we have something to write
    	 int b = stream->Read();
    	 if (b < 0)
    		 break;

       APP_Rx_Buffer[APP_Rx_ptr_in] = (uint8_t)b;
       APP_Rx_ptr_in = newInPointer;
    }
}


/**
 * @brief  VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
uint16_t VirtualComPort::Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
   switch (Cmd) {
       /* Not  needed for this driver, AT modem commands */
      case SEND_ENCAPSULATED_COMMAND:
      case GET_ENCAPSULATED_RESPONSE:
         break;

      // Not needed for this driver
      case SET_COMM_FEATURE:
      case GET_COMM_FEATURE:
      case CLEAR_COMM_FEATURE:
         break;


      //Note - hw flow control on UART 1-3 and 6 only
      case SET_LINE_CODING:
         break;


      case GET_LINE_CODING:
         break;


      case SET_CONTROL_LINE_STATE:
         /* Not  needed for this driver */
         //RSW - This tells how to set RTS and DTR
         break;

      case SEND_BREAK:
      	breakReceived = true;
         break;

      default:
         break;
	}

   return USBD_OK;
}





/* Private functions ---------------------------------------------------------*/



/**
 * @brief  VCP_Init
 *         DeInitializes the Media on the STM32
 * @param  None
 * @retval Result of the operation (USBD_OK in all cases)
 */
static uint16_t VCP_Init(void)
{
   return USBD_OK;
}

/**
 * @brief  VCP_DeInit
 *         DeInitializes the Media on the STM32
 * @param  None
 * @retval Result of the operation (USBD_OK in all cases)
 */
static uint16_t VCP_DeInit(void)
{
   return USBD_OK;
}




/**
 * @brief  VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
	return VCP.Ctrl(Cmd, Buf, Len);
}




/**
 * @brief  VCP_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
	return USBD_OK;
}



/**
 * @brief  VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         until exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
 */

static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len)
{
	// note that we got some data
	if (Len > 0)
		newDataReceived = true;

    // send the data to the stream
	AbstractTempestStream *stream = memoryStream.GetRightSide();
    while (Len-- > 0)
    	stream->Write(*Buf++);

    return USBD_OK;
}


bool GetUSBReceiveHeartbeat()
{
	static bool flashOn = false;
	static uint32_t lastTick = 0;

	// get the current millisecond count
	uint32_t millisecondCount = GetMillisecondCount();

	// our clock is 20Hz so that we flash at most 10Hz
	uint32_t tickCount = millisecondCount / 50;

	// never mind of the tick count hasn't changed
	if (tickCount == lastTick)
		return flashOn;
	lastTick = tickCount;

	// update
	if (flashOn)
	{
		flashOn = false;
	}
	else
	{
		if (newDataReceived)
		{
			newDataReceived = false;
			flashOn = true;
		}
	}

	return flashOn;
}
