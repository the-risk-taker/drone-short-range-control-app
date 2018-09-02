/*
* SerialPortPicker.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include "SerialPortPicker.h"
#include "ui_serialPortPicker.h"

SerialPortPicker::SerialPortPicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortPicker)
{
    ui->setupUi(this);

    ui->buttonAccept->setEnabled(false);

    findConnectedDevices();
}

SerialPortPicker::~SerialPortPicker()
{
    delete ui;
}

void SerialPortPicker::findConnectedDevices(void)
{
    // Searching for connected devices:
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
    #if(0x01 == SERIAL_PORT_PICKER_DEBUG)
        qDebug() << "\nZnaleziono urządzenie. Nazwa portu: " << serialPortInfo.portName();
        qDebug() << "VID: " << QString::number(serialPortInfo.vendorIdentifier());
        qDebug() << "Product ID: " << QString::number(serialPortInfo.productIdentifier());
        qDebug() << "Manufacturer: " << serialPortInfo.manufacturer();
        qDebug() << "Description: " << serialPortInfo.description();
    #endif

        // Anf if found add to list:
        ui->listSerialDevices->addItem(serialPortInfo.portName() + " " + serialPortInfo.description());
    }
}

void SerialPortPicker::on_listSerialDevices_itemClicked(QListWidgetItem *item)
{
    for(int i = 0; i < ui->listSerialDevices->count(); i++) {ui->listSerialDevices->item(i)->setBackgroundColor(QColor(0, 0, 0, 1));}
    ui->listSerialDevices->currentItem()->setBackgroundColor(QColor(0, 0, 255, 127));

    QStringList listSerialDevices = item->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
    this->selectedPortName = listSerialDevices.first();

    for(uint8_t i = 1 ; i < listSerialDevices.count(); i ++)
    {
        this->selectedPortDescription += listSerialDevices[i];
        if(i < listSerialDevices.count() - 1) this->selectedPortDescription += " ";
    }

    ui->buttonAccept->setEnabled(true);

#if(SERIAL_PORT_PICKER_DEBUG == 0x01)
    qDebug() << "Selected serial port = " + this->selectedPortName + " " + this->selectedPortDescription;
#endif
}

void SerialPortPicker::on_buttonAccept_released()
{
    // emit signal
    emit portSelected(this->selectedPortName, this->selectedPortDescription);
    emit(accept());
    this->close();
}

void SerialPortPicker::on_buttonRefresh_released()
{
    ui->listSerialDevices->clear();
    this->findConnectedDevices();
}

void SerialPortPicker::on_buttonCancel_released()
{
    emit(reject());
    this->close();
}
