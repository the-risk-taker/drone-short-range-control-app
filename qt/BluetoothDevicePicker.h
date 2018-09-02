/*
* BluetoothDevicePicker.h
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#ifndef BTDEVICEPICKER_H
#define BTDEVICEPICKER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QBluetoothDeviceDiscoveryAgent>

#define BLUETOOTH_DEVICE_PICKER_DEBUG           0x00 // 0x01 = enabled, 0x00 = disabled

namespace Ui {
class BluetoothDevicePicker;
}

class BluetoothDevicePicker : public QDialog
{
    Q_OBJECT

public:
    explicit BluetoothDevicePicker(QWidget *parent = 0);
    ~BluetoothDevicePicker();

Q_SIGNALS:
    void btDeviceSelected(QString portName, QString portAddress);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void on_listDevice_itemClicked(QListWidgetItem *item);
    void restartSearching();

    void on_buttonAccept_released();
    void on_buttonRefresh_released();
    void on_buttonCancel_released();

private:
    Ui::BluetoothDevicePicker *ui;

    // DISCOVERY AGENT:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    BluetoothDevicePicker *btDevicePicker;

    // VARIABLES:
    QString selectedDeviceAddress;
    QString selectedDeviceName;
};

#endif // BTDEVICEPICKER_H
