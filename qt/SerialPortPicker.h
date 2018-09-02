/*
* SerialPortPicker.h
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#ifndef SERIALPORTPICKER_H
#define SERIALPORTPICKER_H

#include <QDialog>
#include <QListWidget>

// MY DEFINES:
#define SERIAL_PORT_PICKER_DEBUG        0x00 // 0x00 = disabled, 0x01 = enabled

// MY INCLUDES:
#include <QSerialPortInfo>
#if(0x01 == SERIAL_PORT_PICKER_DEBUG)
#include <QDebug>
#endif

namespace Ui {
class SerialPortPicker;
}

class SerialPortPicker : public QDialog
{
    Q_OBJECT

public:
    SerialPortPicker(QWidget *parent = 0);
    ~SerialPortPicker();

Q_SIGNALS:
    void portSelected(QString portName, QString portDescription);

private slots:
    void on_listSerialDevices_itemClicked(QListWidgetItem *item);

    void on_buttonAccept_released();
    void on_buttonRefresh_released();
    void on_buttonCancel_released();  

private:
    Ui::SerialPortPicker *ui;

    // VARIABLES:
    QString selectedPortName;
    QString selectedPortDescription;

    // METHODS:
    void findConnectedDevices(void);
};

#endif // SERIALPORTPICKER_H
