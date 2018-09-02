/* 
* ProtocolAPI.h
*
* Created: 09.07.2018 15:41:10
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/


#ifndef __PROTOCOL_API_H__
#define __PROTOCOL_API_H__

#include "ProtocolPacketsProcessing.h"
#include <QObject>
#include <QDebug>

//--------------------------------------------------------------------
//-- PROTOCOL API DEBUG
//--------------------------------------------------------------------
#define PROTOCOL_API_DEBUG                        0x00	// Enable = 0x01, Disable = 0x00

//--------------------------------------------------------------------
//-- PROTOCOL API ENUMS
//-------------------------------------------------------------------
typedef enum
{
    NO_CODE,					// Restricted
    ERROR_CODE,					// Restricted
    WARNING_CODE,				// Restricted
    INIT_COMM,					// Restricted
    STOP_COMM,					// Restricted
    COMM_STARTED,				// Restricted
    COMM_ENDED,					// Restricted
    ACKNOWLEDGEMENT,			// Restricted

    JOYSTICK_DATA = 10,

    PID_P_VALUE = 20,
    PID_I_VALUE,
    PID_D_VALUE,

    PITCH_VALUE = 30,

    ENABLE_PITCH_PID = 40,
    ENABLE_ROLL_PID,
    ENABLE_OFFSET,

    CHANGE_MOTOR_OFFSET = 50

} eCommandCodes;

//--------------------------------------------------------------------
//-- PROTOCOL API CLASS
//-------------------------------------------------------------------
class ProtocolAPI : public QObject
{
    Q_OBJECT

public:
	ProtocolAPI();
	ProtocolPacketsProcessing *protocolPackets;
	
	// Main methods, working constantly:
    void processPackets();
	
	// Main methods, working on events:
	void sendPacket(eCommandCodes command);
	template <typename T>
	void sendPacket(eCommandCodes command, T value);
	template <typename T>
	void sendPacket(eCommandCodes command, T* value, uint8_t howMany);

    // TODO pozbycie sie tych zmiennych z protocolAPI, lub dodanie metod
    float   pitch,
            roll;

Q_SIGNALS:
    void message(QString);

private:
	// Here write your project functionalities:
	void encodePacket(sProtocolPacket packet);
};

template <typename T>
void ProtocolAPI::sendPacket(eCommandCodes command, T value)
{
	sProtocolPacket outcomingPacket;
	
	outcomingPacket.commandCode = (uint8_t)command;
	outcomingPacket.length = sizeof(T);
	memcpy(outcomingPacket.data, &value, sizeof(T));
	
	this->protocolPackets->rewritePacketToBytes(outcomingPacket);
    this->protocolPackets->protocolBytes->writeBytes();
}

template <typename T>
void ProtocolAPI::sendPacket(eCommandCodes command, T* value, uint8_t howManyElements)
{
	sProtocolPacket outcomingPacket;
	
	outcomingPacket.commandCode = (uint8_t)command;
	outcomingPacket.length = sizeof(T)*howManyElements;

	if(outcomingPacket.length > PROTOCOL_PACKETS_MAX_DATA_LENGTH)
	{
    #if(PROTOCOL_API_DEBUG == 0x01)
        qDebug() << "You wanted to send too much data (" + QString::number(outcomingPacket.length) +
                    " bytes) in one packet (command code = " + QString::number(outcomingPacket.commandCode) +
                    "). Increase PROTOCOL_PACKETS_MAX_DATA_LENGTH or send two packets.";
    #endif
        emit(message("You wanted to send too much data (" + QString::number(outcomingPacket.length) +
                     " bytes) in one packet (command code = " + QString::number(outcomingPacket.commandCode) +
                     "). Increase PROTOCOL_PACKETS_MAX_DATA_LENGTH or send two packets."));
		return;
	}

	for(uint8_t i = 0; i < howManyElements; i++)
	{
		memcpy(outcomingPacket.data + i * sizeof(T), (value + i), sizeof(T));
	}
	
    outcomingPacket.crc = 0;

	this->protocolPackets->rewritePacketToBytes(outcomingPacket);
    this->protocolPackets->protocolBytes->writeBytes();
}

#endif //__PROTOCOL_API_H__
