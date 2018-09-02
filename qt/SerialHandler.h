/*
* SerialHandler.h
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QSerialPort>

// DEFINES:
#define USED_OS                 0x00 // 0x00 = Windows, 0x01 = UNIX
#define USING_WINDOWS           0x00
#define USING_UNIX              0x01

#define SERIAL_HANDLER_DEBUG    0x01 // 0x01 = enabled, 0x00 disabled

class SerialHandler : public QObject
{
    Q_OBJECT
public:
    SerialHandler();

    // OBJECTS:
    QSerialPort *serial;

    // METHODS:
    void connect();
    void disconnect();

    // GETERS:
    QString getPortName();
    QString getPortDescription();

    // SETERS:
    void setPortName(QString name);
    void setPortDescription(QString description);

Q_SIGNALS:
    void connected();
    void disconnected();
    void message(QString message);
    void getRxBuffer(uint8_t *rxBuffer, uint8_t howMany);

private slots:
    void handleError(QSerialPort::SerialPortError error);
    void readBytes();

public slots:
    void write(const QString message);
    void writeBytes(char *buffer, uint8_t howMany);

private:
    // VARIABLES:
    QString portName;
    QString portDescription;
    bool deviceAvailable;
};

#endif // SERIALHANDLER_H
