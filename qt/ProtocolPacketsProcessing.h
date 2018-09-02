/* 
* ProtocolPacketsProcessing.h
*
* Created: 09.07.2018 15:55:04
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/


#ifndef __PROTOCOLPACKETS_H__
#define __PROTOCOLPACKETS_H__

#include "ProtocolBytesProcessing.h"
#include <QObject>

//--------------------------------------------------------------------
//-- PROTOCOL PACKETS CLASS DEBUG
//--------------------------------------------------------------------
#define PROTOCOL_PACKETS_DEBUG                          0x00	// Enable = 0x01, Disable = 0x00

//--------------------------------------------------------------------
//-- PROTOCOL PACKETS CLASS DEFINES
//--------------------------------------------------------------------
#define PROTOCOL_PACKETS_MAX_CIRCULAR_BUFFER_LENGTH		5 * PROTOCOL_BYTES_MAX_RX_BUFFER_LENGTH
#define PROTOCOL_PACKETS_MAX_DATA_LENGTH				4 * sizeof(float)
#define PROTOCOL_PACKETS_PACKET_ZERO_LENGTH				0
#define PROTOCOL_PACKETS_MAX_NUMBER_OF_PACKETS			10

#define PROTOCOL_PACKETS_PREAMBLE_FIRST_SIGN			'D'
#define PROTOCOL_PACKETS_PREAMBLE_SECOND_SIGN			'R'

//--------------------------------------------------------------------
//-- PROTOCOL PACKETS CLASS ENUMS
//--------------------------------------------------------------------
typedef enum eProcessBytesStateMachine
{
	PREAMBLE = 0,
	CODE,
	LENGTH,
	DATA,
	CRC
}eProcessBytesStateMachine;

typedef enum ePreambleStateMachine
{
	FIRST_SIGN,		// D
	SECOND_SIGN		// R
} ePreambleStateMachine;

typedef enum eCrcStateMachine
{
	CRC_LSB,
	CRC_MSB
} eCrcStateMachine;

//--------------------------------------------------------------------
//-- PROTOCOL PACKETS CLASS STRUCTURES
//--------------------------------------------------------------------
typedef struct sProtocolPacket
{
	uint8_t commandCode;
	uint8_t length;
	uint8_t data[PROTOCOL_PACKETS_MAX_DATA_LENGTH];
	uint16_t crc;
} sProtocolPacket;

//--------------------------------------------------------------------
//-- PROTOCOL PACKETS CLASS STRUCTURES
//--------------------------------------------------------------------
class ProtocolPacketsProcessing : public QObject
{
    Q_OBJECT

public:
	ProtocolPacketsProcessing();
	ProtocolBytesProcessing *protocolBytes;
	
	// Main methods:
	void processInputBytes();
	sProtocolPacket* getNewPacketsTable();
	uint8_t getNewPacketsNumber();
	void resetNewPacketsNumber();
	
	// Utilities:
	void printPacket(sProtocolPacket packet);
	void rewritePacketToBytes(sProtocolPacket &outcomingPacket);
	
private:
	uint16_t circularBufferDataEndCursor;
	uint16_t circularBufferDataStartCursor;
	uint8_t circularBuffer[PROTOCOL_PACKETS_MAX_CIRCULAR_BUFFER_LENGTH];
	
	sProtocolPacket newPacketsTable[PROTOCOL_PACKETS_MAX_NUMBER_OF_PACKETS];
	uint8_t newPacketsNumber;
	
	void checkBufferCursor(uint16_t &position, const uint16_t bufferLength);
	void clearPacket(sProtocolPacket &packet);
	void copyRxBufferToCircualBuffer();

Q_SIGNALS:
    void message(QString);

};
#endif //__PROTOCOLPACKETS_H__
