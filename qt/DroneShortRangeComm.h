/*
* DroneShortRangeComm.h
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#ifndef DRONESHORTRANGECOMM_H
#define DRONESHORTRANGECOMM_H

#include <QMainWindow>

///
/// \brief My includes
///
#include "BluetoothDevicePicker.h"
#include "SerialPortPicker.h"
#include <QSerialPort>
#include <ProtocolAPI.h>

///
/// \brief My defines
///
#define DRONE_SHORT_RANGE_COMM_DEBUG            0x00 // 0x01 = enabled, 0x00 = disabled
#define PROTOCOL_PROCESS_BYTES_INTERVAL         25
#define PROTOCOL_PROCESS_PACKETS_INTERVAL       25

#define GUI_REFREASH_RATE_INTERVAL              100

///
/// \brief My enums
///
typedef enum eMessageType
{
    NORMAL = 0,
    WARNING,
    ERROR
} eMessageType;

///
/// \brief ui namespace
///
namespace Ui
{
    class DroneShortRangeComm;
}

///
/// \brief The DroneShortRangeComm class
///
class DroneShortRangeComm : public QMainWindow
{
    Q_OBJECT

public:
    explicit DroneShortRangeComm(QWidget *parent = 0);
    ~DroneShortRangeComm();

private slots:
    //---- bt
    void on_buttonSearchBTDevices_released();
    void setBluetoothSocketNameAndAddress(QString name, QString address);
    void bluetoothConnected();
    void bluetoothDisconnected();

    //---- bt & serial
    void on_buttonDisconnect_released();

    //---- serial
    void on_buttonSearchSerialDevices_released();
    void portSelected(QString portName, QString portDescription);
    void serialConnected();
    void serialDisconnected();

    //---- console
    void passErrorInfoToConsole(QString data);

    //---- communication
    void on_sendInitPacket_released();
    void runProcessPackets();
    void runProcessBytes();
    void runEditGUI();

    void on_PdoubleSpinBox_valueChanged(double arg1);
    void on_IdoubleSpinBox_valueChanged(double arg1);
    void on_DdoubleSpinBox_valueChanged(double arg1);

    void on_tunningEnable_stateChanged(int arg1);
    void on_pitchEnable_stateChanged(int arg1);
    void on_rollEnable_stateChanged(int arg1);

    void on_frontRightOffset_editingFinished();
    void on_frontLeftOffset_editingFinished();
    void on_rearRightOffset_editingFinished();
    void on_rearLeftOffset_editingFinished();
    void on_enableOffset_stateChanged(int arg1);

private:
    // VARIABLES:

    //TODO bool isLocalBluetoothDeviceValid;

    // OBJECTS:

    Ui::DroneShortRangeComm *ui;
    //---- bt
    BluetoothDevicePicker *btDevicePicker;      // do stuff with Bluetooth socket (slave device)
    //---- serial
    SerialPortPicker *serialDevicePicker;       // do stuff with Serial port (chose device)
    //---- protocol
    ProtocolAPI *myProtocolAPI;

    // METHODS:

    //---- bt
    void runBluetoothDevicePickerDialog();

    //---- serial
    void runSerialDevicePickerDialog();

    //---- utilites
    void addTooProgramConsole(QString message, eMessageType eMessageType = NORMAL);
    void useConsole(QListWidget *list, QString message, eMessageType eMessageType);

    void chooseMotor(uint8_t motorID, double offsetValue);
};

#endif // DRONESHORTRANGECOMM_H
