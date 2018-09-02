/*
* SerialHandler.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include "SerialHandler.h"
#include <QDebug.h>

SerialHandler::SerialHandler()
{
    this->serial = new QSerialPort;

#if(SERIAL_HANDLER_DEBUG == 0x01)
    qDebug() << "Connect status between serial::error(QSerialPort::SerialPortError) and SerialHandler::handleError() = "
             << QObject::connect(this->serial, SIGNAL(error(QSerialPort::SerialPortError)),
                                 this, SLOT(handleError(QSerialPort::SerialPortError)));
#else
    QObject::connect(this->serial, SIGNAL(error(QSerialPort::SerialPortError)),
                     this, SLOT(handleError(QSerialPort::SerialPortError)));
#endif

}

///
/// \brief public methods
///
void SerialHandler::connect()
{
    this->serial->setPortName(this->portName);

    if(this->serial->open(QSerialPort::ReadWrite))
    {
        this->serial->setBaudRate(QSerialPort::Baud115200);
        this->serial->setDataBits(QSerialPort::Data8);
        this->serial->setParity(QSerialPort::NoParity);
    #if(USED_OS == USING_WINDOWS)
        this->serial->setStopBits(QSerialPort::OneAndHalfStop);
    #elif(USED_OS == USING_UNIX)
        this->serial->setStopBits(QSerialPort::OneStop);
    #endif
        this->serial->setFlowControl(QSerialPort::NoFlowControl);

    #if(SERIAL_HANDLER_DEBUG == 0x01)
        if(this->serial->isWritable())
            qDebug() << "Connected with port: " + this->portName;
        else
            qDebug() << "Sending messages disabled. Device is Read Only.";
    #endif
        emit(connected());

    #if(SERIAL_HANDLER_DEBUG == 0x01)
        qDebug() << "Connect status between serial::readyRead() and SerialHandler::readBytes() = "
                 << QObject::connect(this->serial, SIGNAL(readyRead()),
                                     this, SLOT(readBytes()));
    #else
        QObject::connect(this->serial, SIGNAL(readyRead()),
                         this, SLOT(readBytes()));
    #endif
    }
}
void SerialHandler::disconnect()
{
    if(this->serial->isOpen())
    {
        emit(disconnected());
        this->serial->close();
    }
}

///
/// \brief getters and setters
///
QString SerialHandler::getPortName()
{
    return this->portName;
}
QString SerialHandler::getPortDescription()
{
    return this->portDescription;
}
void SerialHandler::setPortName(QString name)
{
    this->portName = name;
}
void SerialHandler::setPortDescription(QString description)
{
    this->portDescription = description;
}

///
/// \brief private slots
///
void SerialHandler::handleError(QSerialPort::SerialPortError error)
{
    if((error != QSerialPort::NoError) && (error != QSerialPort::OpenError))
    {
        this->disconnect();
        emit(message(this->serial->errorString()));

    #if(SERIAL_HANDLER_DEBUG == 0x01)
        qDebug() << this->serial->errorString();
    #endif
    }
    else if(error == QSerialPort::OpenError)
    {
        emit(message(this->serial->errorString()));
    }
}
void SerialHandler::readBytes()
{
    QByteArray response;
    response.append(this->serial->readAll());

    uint8_t rxBuffer[response.size()];
    for(uint8_t i = 0; i < response.size(); i++)
    {
        rxBuffer[i] = (uint8_t)response.at(i);
    }

    // Emit data to higher layer
    emit(getRxBuffer(rxBuffer, response.size()));
}

void SerialHandler::write(const QString message)
{
    this->serial->write(message.toUtf8());
}
void SerialHandler::writeBytes(char *buffer, uint8_t howMany)
{
    char bytesTable[howMany];
    for(uint8_t i = 0; i < howMany; i++)
    {
        bytesTable[i] = (char)buffer[i];
    }

    this->serial->write(bytesTable, howMany);
}
