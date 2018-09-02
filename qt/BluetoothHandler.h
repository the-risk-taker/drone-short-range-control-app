/*
* BluetoothHandler.h
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#ifndef BLUETOOTHHANDLER_H
#define BLUETOOTHHANDLER_H

#include <QBluetoothSocket>
#include <QObject>

#define BLUETOOTH_HANDLER_DEBUG                 0x00 // 0x01 = enabled, 0x00 = disabled

class BluetoothHandler : public QObject
{
    Q_OBJECT
public:
    BluetoothHandler();

    // OBJECTS:
    QBluetoothSocket *socket;

    // METHODS:
    bool checkLocalBluetoothDevice();
    void connect();
    void disconnect();

    // GETERS:
    QString getLocalDeviceName();
    QString getLocalDeviceAddress();
    QString getSocketName();
    QString getSocketAddress();

    // SETERS:
    void setSocketName(QString name);
    void setSocketAddres(QString address);

Q_SIGNALS:
    void connected();
    void disconnected();
    void message(QString message);
    void getRxBuffer(uint8_t *rxBuffer, uint8_t howMany);

private slots:
    void handleError(QBluetoothSocket::SocketError error);
    void connectionEstablished();
    void connectionInterrupted();
    void readBytes();

public slots:
    void write(const QString message);
    void writeBytes(char *buffer, uint8_t howMany);

private:
    // VARIABLES:
    QString localDeviceName,
            localDeviceAddress,

            socketName,
            socketAddress;
};

#endif // BLUETOOTHHANDLER_H
