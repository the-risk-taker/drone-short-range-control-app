/* 
* Protocol.cpp
*
* Created: 07.07.2018 19:21:51
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

//--------------------------------------------------------------------
//-- PROTOCOL BYTES INCLUDES
//--------------------------------------------------------------------
#include "ProtocolBytesProcessing.h"
#include <QDebug>

//--------------------------------------------------------------------
//-- PROTOCOL BYTES CLASS
//--------------------------------------------------------------------
ProtocolBytesProcessing::ProtocolBytesProcessing()
{
	this->rxBufferCursor = 0;	
	this->txBufferCursor = 0;

    this->serialHandler = new SerialHandler();
    this->bluetoothHandler = new BluetoothHandler();

    connectionType = NO_TYPE;

#if(PROTOCOL_BYTES_DEBUG == 0x01)
    qDebug() << "Connect status between serialHandler::getRxBuffer() and ProtocolBytesProcessing::getRxBuffer() = "
             << QObject::connect(this->serialHandler, SIGNAL(getRxBuffer(uint8_t*, uint8_t)),
                                 this, SLOT(getRxBuffer(uint8_t*, uint8_t)));
#else
    // --- serial
    QObject::connect(this->serialHandler, SIGNAL(getRxBuffer(uint8_t*, uint8_t)),
                     this, SLOT(getRxBuffer(uint8_t*, uint8_t)));
    // --- bluetooth
    QObject::connect(this->bluetoothHandler, SIGNAL(getRxBuffer(uint8_t*, uint8_t)),
                     this, SLOT(getRxBuffer(uint8_t*, uint8_t)));
#endif
};

void ProtocolBytesProcessing::copyBytesOnTxBuffer(uint8_t* source, const uint8_t howMany)
{
	for(uint8_t i = 0; i < howMany; i++)
	{
		this->txBuffer[i] = source[i];
	}
	
    this->txBufferCursor = howMany;
}

void ProtocolBytesProcessing::setConnectionType(eConnectionType type)
{
    this->connectionType = type;
}

void ProtocolBytesProcessing::getRxBuffer(uint8_t *buffer, const uint8_t howMany)
{
#if(PROTOCOL_BYTES_INPUT_DATA_SNIFF == 0x01)
    qDebug() << "How many new bytes = " << howMany;
#endif
    for(uint8_t i = 0; i < howMany; i++)
    {
        this->rxBuffer[this->rxBufferCursor++] = buffer[i];
    #if(PROTOCOL_BYTES_INPUT_DATA_SNIFF == 0x01)
        qDebug() << "buffer[" << this->rxBufferCursor - 1 << "] = " <<  rxBuffer[this->rxBufferCursor - 1];
    #endif
    }
}

void ProtocolBytesProcessing::writeBytes()
{
    char data[this->txBufferCursor];

    for(uint8_t i = 0; i < this->txBufferCursor; i++)
    {
        data[i] = (char)this->txBuffer[i];
    }

    switch(this->connectionType)
    {
        case NO_TYPE:
        {
            emit(message("Connection type was not determined!"));
        }
        break;
        case BLUETOOTH:
        {
            this->bluetoothHandler->writeBytes(data, sizeof(data));
        }
        break;
        case SERIAL:
        {
            this->serialHandler->writeBytes(data, sizeof(data));
        }
        break;
        default:
        {
        #if(PROTOCOL_BYTES_DEBUG == 0x01)
            qDebug() << "Unknown connection type!";
        #endif
        }
        break;
    }
}

uint8_t* ProtocolBytesProcessing::getBuffer(eBufferType bufferType)
{
	uint8_t *bufferPointer = NULL;
	
	switch(bufferType)
	{
		case RX_BUFFER:
		{
			bufferPointer = this->rxBuffer;
		}
		break;
		case TX_BUFFER:
		{
			bufferPointer = this->txBuffer;
		}
	}
	
	return bufferPointer;
}

uint8_t ProtocolBytesProcessing::getRxBufferCursor()
{
	uint8_t tempRxCounter = this->rxBufferCursor;
	this->rxBufferCursor = 0;
	return tempRxCounter;
}

void ProtocolBytesProcessing::setTxBufferCursor(uint8_t value)
{
	this->txBufferCursor = value;
}
