/*
* BluetoothHandler.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include "BluetoothHandler.h"
#include <QBluetoothLocalDevice>
#include <QDebug>

BluetoothHandler::BluetoothHandler()
{
    /* BT SOCKET:
     *
     * QBluetoothServiceInfo::UnknownProtocol	0	The service uses an unknown socket protocol.
     * QBluetoothServiceInfo::L2capProtocol     1	The service uses the L2CAP socket protocol. This protocol is not supported for direct socket connections on Android.
     * QBluetoothServiceInfo::RfcommProtocol	2	The service uses the RFCOMM socket protocol.
    */
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

#if(BLUETOOTH_HANDLER_DEBUG == 0x01)
    qDebug() << "Connect status between socket::connected() and BluetoothHandler::connectionEstablished() = "
             << QObject::connect(this->socket, SIGNAL(connected()),
                                 this, SLOT(connectionEstablished()));
    qDebug() << "Connect status between socket::disconnected() and BluetoothHandler::connectionInterrupted() = "
             << QObject::connect(this->socket, SIGNAL(disconnected()),
                                 this, SLOT(connectionInterrupted()));
    qDebug() << "Connect status between socket::readyRead() and BluetoothHandler::readBytes() = "
             << QObject::connect(this->socket, SIGNAL(readyRead()),
                                 this, SLOT(readBytes()));
    qDebug() << "Connect status between socket::error() and BluetoothHandler::handleError() = "
             << QObject::connect(this->socket, SIGNAL(error(QBluetoothSocket::SocketError)),
                                 this, SLOT(handleError(QBluetoothSocket::SocketError)));
#else
    QObject::connect(this->socket, SIGNAL(connected()),
                     this, SLOT(connectionEstablished()));
    QObject::connect(this->socket, SIGNAL(disconnected()),
                     this, SLOT(connectionInterrupted()));
    QObject::connect(this->socket, SIGNAL(readyRead()),
                     this, SLOT(readBytes()));
    QObject::connect(this->socket, SIGNAL(error(QBluetoothSocket::SocketError)),
                     this, SLOT(handleError(QBluetoothSocket::SocketError)));
#endif

}

bool BluetoothHandler::checkLocalBluetoothDevice()
{
    QBluetoothLocalDevice localDevice;

    if(localDevice.isValid())
    {
        localDevice.powerOn();

        this->localDeviceName = localDevice.name();
        this->localDeviceAddress = localDevice.address().toString();

    #if(BLUETOOTH_HANDLER_DEBUG == 0x01)
        qDebug() << this->localDeviceName;
        qDebug() << this->localDeviceAddress;
    #endif

        return true;
    }
    else
    {
        return false;
    }
}
void BluetoothHandler::connect()
{
    /*
    * Hint: If you are connecting to a Bluetooth serial board then try using the well-known
    * SPP UUID 00001101-0000-1000-8000-00805F9B34FB. However if you are connecting to an Android
    * peer then please generate your own unique UUID.
    */

    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));

    this->socket->connectToService(QBluetoothAddress(this->socketAddress),
                                   QBluetoothUuid(serviceUuid),
                                   QIODevice::ReadWrite);
#if(BLUETOOTH_HANDLER_DEBUG == 0x01)
    qDebug() << "Rozpoczęto łączenie z urządzeniem o nazwie: " + this->socketName + " i adresie: " + this->socketAddress;
#endif
}
void BluetoothHandler::disconnect()
{
    emit(disconnected());
    this->socket->disconnectFromService();
}

QString BluetoothHandler::getLocalDeviceName()
{
    return this->localDeviceName;
}
QString BluetoothHandler::getLocalDeviceAddress()
{
    return this->localDeviceAddress;
}
QString BluetoothHandler::getSocketName()
{
    return this->socketName;
}
QString BluetoothHandler::getSocketAddress()
{
    return this->socketAddress;
}
void BluetoothHandler::setSocketName(QString name)
{
    this->socketName = name;
}
void BluetoothHandler::setSocketAddres(QString address)
{
    this->socketAddress = address;
}

void BluetoothHandler::handleError(QBluetoothSocket::SocketError error)
{
    emit(message(this->socket->errorString()));
}
void BluetoothHandler::connectionEstablished()
{
    emit(connected());
}
void BluetoothHandler::connectionInterrupted()
{
    emit(disconnected());
    this->disconnect();
}
void BluetoothHandler::readBytes()
{
    QByteArray response;
    response.append(this->socket->readAll());

    uint8_t rxBuffer[response.size()];
    for(uint8_t i = 0; i < response.size(); i++)
    {
        rxBuffer[i] = (uint8_t)response.at(i);
    }

    // Emit data to higher layer
    emit(getRxBuffer(rxBuffer, response.size()));
}

void BluetoothHandler::write(const QString message)
{
    this->socket->write(message.toUtf8());
}
void BluetoothHandler::writeBytes(char *buffer, uint8_t howMany)
{
    char bytesTable[howMany];
    for(uint8_t i = 0; i < howMany; i++)
    {
        bytesTable[i] = (char)buffer[i];
    }

    this->socket->write(bytesTable, howMany);
}
