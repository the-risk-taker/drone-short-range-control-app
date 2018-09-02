/* 
* Protocol.h
*
* Created: 07.07.2018 19:21:51
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

#ifndef __PROTOCOL_BYTES_H__
#define __PROTOCOL_BYTES_H__

#include <stdint.h>
#include "SerialHandler.h"
#include "BluetoothHandler.h"
#include <QObject>

//--------------------------------------------------------------------
//-- PROTOCOL BYTES DEBUG
//--------------------------------------------------------------------
#define PROTOCOL_BYTES_INPUT_DATA_SNIFF				0x00	// Enable = 0x01, Disable = 0x00
#define PROTOCOL_BYTES_DEBUG                        0x00	// Enable = 0x01, Disable = 0x00

//--------------------------------------------------------------------
//-- PROTOCOL BYTES DEFINES
//--------------------------------------------------------------------
#define PROTOCOL_BYTES_SERIAL_BAUDRATE				115200UL
#define PROTOCOL_BYTES_MAX_RX_BUFFER_LENGTH			128
#define PROTOCOL_BYTES_MAX_TX_BUFFER_LENGTH			PROTOCOL_BYTES_MAX_RX_BUFFER_LENGTH

//--------------------------------------------------------------------
//-- PROTOCOL BYTES ENUMS
//--------------------------------------------------------------------
typedef enum eBufferType
{
	RX_BUFFER,
	TX_BUFFER
} eBufferType;

typedef enum eConnectionType
{
    NO_TYPE = 0,
    BLUETOOTH,
    SERIAL,
} eConnectionType;

//--------------------------------------------------------------------
//-- PROTOCOL BYTES CLASS
//--------------------------------------------------------------------
class ProtocolBytesProcessing : public QObject
{
    Q_OBJECT

public:
    ProtocolBytesProcessing();
    SerialHandler *serialHandler;
    BluetoothHandler *bluetoothHandler;

	// Buffers and buffer's cursors methods:
    void writeBytes();
	uint8_t getRxBufferCursor();
	uint8_t* getBuffer(eBufferType bufferType);
    void setTxBufferCursor(uint8_t value);
    void copyBytesOnTxBuffer(uint8_t* source, const uint8_t howMany);

    void setConnectionType(eConnectionType type);
	
	// Utilities:
#if(PROTOCOL_INPUT_DATA_SNIFF == 0x01)
	void printBuffer(uint8_t* buffer, const uint8_t length);
#endif

private slots:
    void getRxBuffer(uint8_t *buffer, const uint8_t howMany);

private:
	uint8_t rxBufferCursor;
	uint8_t rxBuffer[PROTOCOL_BYTES_MAX_RX_BUFFER_LENGTH];
    uint8_t txBufferCursor;
    uint8_t txBuffer[PROTOCOL_BYTES_MAX_TX_BUFFER_LENGTH];

    eConnectionType connectionType;

Q_SIGNALS:
    void message(QString);
};

#endif //__PROTOCOL_BYTES_H__
