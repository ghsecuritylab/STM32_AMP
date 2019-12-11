/*
 * UARTBuffer.cpp
 *
 *  Created on: Dec 9, 2019
 *      Author: piotrek
 */

#include "UARTBuffer.h"

UARTBuffer::UARTBuffer(DMA_HandleTypeDef* hdmaRxUart)
{
	this->hdmaRxUart = hdmaRxUart;
}

UARTBuffer::~UARTBuffer() {
	// TODO Auto-generated destructor stub
}

void UARTBuffer::checkRxBuffer()
{
	headPosition = UART_BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(hdmaRxUart);
}

uint16_t UARTBuffer::getBufferLength()
{
	return UART_BUFFER_LENGTH;
}

uint16_t UARTBuffer::getSize()
{
	uint16_t size;

	if (headPosition >= tailPosition)
	{
		size = headPosition - tailPosition;
	}
	else
	{
		size = UART_BUFFER_LENGTH + headPosition - tailPosition;
	}

	return size;
}

uint16_t UARTBuffer::getData(uint8_t* dataBuffer)
{
	uint16_t dataSize = getSize();

	if(dataSize)
	{
		if(headPosition > tailPosition)
		{
			std::copy_n(&rxBuffer[tailPosition], headPosition - tailPosition, dataBuffer);
//			memcpy(dataBuffer, &rxBuffer[tailPosition], headPosition - tailPosition);
		}
		else
		{
			std::copy_n(&rxBuffer[tailPosition], UART_BUFFER_LENGTH - tailPosition, dataBuffer);
//			memcpy(dataBuffer, &rxBuffer[tailPosition], UART_BUFFER_LENGTH - tailPosition);

			if(headPosition > 0)
			{
				std::copy_n(rxBuffer, headPosition, dataBuffer);
//				memcpy(dataBuffer, rxBuffer, headPosition);
			}
		}

		tailPosition = headPosition;
	}

	if(tailPosition == UART_BUFFER_LENGTH)
	{
		tailPosition = 0;
	}

	return dataSize;
}

uint8_t* UARTBuffer::getDataBuffer()
{
	return rxBuffer;
}
