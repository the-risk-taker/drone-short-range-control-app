/* 
* ProtocolAPI.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
* Copyright © 2018 Mateusz Patyk. All rights reserved.
*/

#include "ProtocolAPI.h"

ProtocolAPI::ProtocolAPI()
{
	protocolPackets = new ProtocolPacketsProcessing();
}

void ProtocolAPI::processPackets()
{
#if(PROTOCOL_API_DEBUG == 0x01)
    qDebug() << "Number of packets to process = " << this->protocolPackets->getNewPacketsNumber();
#endif
	if(this->protocolPackets->getNewPacketsNumber())
	{
		for(uint8_t i = 0; i < this->protocolPackets->getNewPacketsNumber(); i++)
		{
			sProtocolPacket incomingPacket = this->protocolPackets->getNewPacketsTable()[i];
			this->encodePacket(incomingPacket);
		}
		
		this->protocolPackets->resetNewPacketsNumber();
	}
}

void ProtocolAPI::sendPacket(eCommandCodes command)
{
	sProtocolPacket outcomingPacket;
	
	outcomingPacket.commandCode = (uint8_t)command;
	outcomingPacket.length = 0;
	
	this->protocolPackets->rewritePacketToBytes(outcomingPacket);
    this->protocolPackets->protocolBytes->writeBytes();
}

void ProtocolAPI::encodePacket(sProtocolPacket packet)
{
    switch((eCommandCodes)packet.commandCode)
    {
        case INIT_COMM:
        {
            qDebug() << "INIT_COMM";
        }
        break;
        case COMM_STARTED:
        {
            qDebug() << "COMM_STARTED";
        }
        break;
        case PITCH_VALUE:
        {
            float pitch;
            memcpy(&pitch, packet.data, sizeof(pitch));
            qDebug() << pitch;
        }
        break;
        default:
        {
            qDebug() << "Nieznany kod komendy = " << QString::number(packet.commandCode);
        }
            break;
        }
}
