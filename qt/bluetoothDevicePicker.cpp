/*
* BluetoothDevicePicker.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include "BluetoothDevicePicker.h"
#include "ui_bluetoothDevicePicker.h"
#include <QDebug>

BluetoothDevicePicker::BluetoothDevicePicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BluetoothDevicePicker)
{
    ui->setupUi(this);

    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    this->discoveryAgent->start();

#if(BLUETOOTH_DEVICE_PICKER_DEBUG == 0x01)

    qDebug() << "Connect status between DiscoveryAgent::deviceDiscovered() and BluetoothDevicePicker::deviceDiscovered() = "
             << connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                        this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    qDebug() << "Connect status between DiscoveryAgent::finished() and BluetoothDevicePicker::restartSearching() = "
             << connect(this->discoveryAgent, SIGNAL(finished()),
                        this, SLOT(restartSearching()));
#else
    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    connect(this->discoveryAgent, SIGNAL(finished()),
            this, SLOT(restartSearching()));
#endif

    ui->buttonAccept->setEnabled(false);
}

BluetoothDevicePicker::~BluetoothDevicePicker()
{
    delete ui;
}

void BluetoothDevicePicker::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    ui->listDevice->addItem(device.name() + " " + device.address().toString());
}

void BluetoothDevicePicker::on_listDevice_itemClicked(QListWidgetItem *item)
{
    for(int i = 0; i < ui->listDevice->count(); i++) {ui->listDevice->item(i)->setBackgroundColor(QColor(0, 0, 0, 1));}
    ui->listDevice->currentItem()->setBackgroundColor(QColor(0, 0, 255, 127));

    QStringList listDevice = item->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);

    this->selectedDeviceName = listDevice.first();
    this->selectedDeviceAddress = listDevice.last();

    ui->buttonAccept->setEnabled(true);
}

void BluetoothDevicePicker::on_buttonAccept_released()
{
    // emit signal
    emit btDeviceSelected(this->selectedDeviceName, this->selectedDeviceAddress);
    emit(accept());
    this->close();
}

void BluetoothDevicePicker::on_buttonRefresh_released()
{
    this->restartSearching();
}

void BluetoothDevicePicker::on_buttonCancel_released()
{
    emit(reject());
    this->close();
}

void BluetoothDevicePicker::restartSearching()
{
    this->discoveryAgent->stop();
    ui->listDevice->clear();
    this->discoveryAgent->start();

    ui->buttonAccept->setEnabled(false);
}
