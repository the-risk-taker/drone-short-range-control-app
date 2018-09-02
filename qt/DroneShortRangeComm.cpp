/*
* DroneShortRangeComm.cpp
*
* Author: Mateusz Patyk
* Email: matpatyk@gmail.com
*/

#include "DroneShortRangeComm.h"
#include "ui_DroneShortRangeComm.h"

///
/// \brief My includes
///
#include "BluetoothDevicePicker.h"
#include <QTime>
#include <QMessageBox>
#include "ProtocolBytesProcessing.h"
#include <QTimer>

///
/// \brief DroneShortRangeComm::DroneShortRangeComm
/// \param parent
///
DroneShortRangeComm::DroneShortRangeComm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DroneShortRangeComm)
{
    ui->setupUi(this);

    // ProtocolAPI
    this->myProtocolAPI = new ProtocolAPI();

    // Check local Bluetooth device
    this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->checkLocalBluetoothDevice();

#if(DRONE_SHORT_RANGE_COMM_DEBUG == 0x01)

    // --- serial
    qDebug() << "Connect status between ProtocolAPI->...->serialHandler::connected() and DroneShortRangeComm::serialConnected() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(connected()),
                        this, SLOT(serialConnected()));
    qDebug() << "Connect status between ProtocolAPI->...->serialHandler::disconnected() and DroneShortRangeComm::serialDisconnected() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(disconnected()),
                        this, SLOT(serialDisconnected()));
    // TODO refactor, message not always can be an error message
    qDebug() << "Connect status between ProtocolAPI->...->serialHandler::message() and DroneShortRangeComm::passErrorInfoToConsole() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(message(QString)),
                        this, SLOT(passErrorInfoToConsole(QString)));

    // --- bluetooth
    qDebug() << "Connect status between ProtocolAPI->...->bluetoothHandler::connected() and DroneShortRangeComm::serialConnected() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(connected()),
                        this, SLOT(bluetoothConnected()));
    qDebug() << "Connect status between ProtocolAPI->...->bluetoothHandler::disconnected() and DroneShortRangeComm::bluetoothDisconnected() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(disconnected()),
                        this, SLOT(bluetoothDisconnected()));
    // TODO refactor, message not always can be an error message
    qDebug() << "Connect status between ProtocolAPI->...->bluetoothHandler::message() and DroneShortRangeComm::passErrorInfoToConsole() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(message(QString)),
                        this, SLOT(passErrorInfoToConsole(QString)));

    // TODO refactor, message not always can be an error message
    // --- protocol bytes
    qDebug() << "Connect status between ProtocolAPI->...->protocolBytes::message() and DroneShortRangeComm::passErrorInfoToConsole() = "
             << connect(this->myProtocolAPI->protocolPackets->protocolBytes, SIGNAL(message(QString)),
                        this, SLOT(passErrorInfoToConsole(QString)));
    // --- protocol packets
    qDebug() << "Connect status between ProtocolAPI->protocolPackets::message() and DroneShortRangeComm::passErrorInfoToConsole() = "
             << connect(this->myProtocolAPI->protocolPackets, SIGNAL(message(QString)),
                        this, SLOT(passErrorInfoToConsole(QString)));
    // --- protocol API
    qDebug() << "Connect status between ProtocolAPI::message() and DroneShortRangeComm::passErrorInfoToConsole() = "
             << connect(this->myProtocolAPI, SIGNAL(message(QString)),
                        this, SLOT(passErrorInfoToConsole(QString)));
#else
    // --- serial
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(connected()),
            this, SLOT(serialConnected()));
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(disconnected()),
            this, SLOT(serialDisconnected()));
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler, SIGNAL(message(QString)),
            this, SLOT(passErrorInfoToConsole(QString)));

    // --- bluetooth
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(connected()),
                            this, SLOT(bluetoothConnected()));
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(disconnected()),
                            this, SLOT(bluetoothDisconnected()));
    connect(this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler, SIGNAL(message(QString)),
                            this, SLOT(passErrorInfoToConsole(QString)));

    // --- protocol bytes
    connect(this->myProtocolAPI->protocolPackets->protocolBytes, SIGNAL(message(QString)),
            this, SLOT(passErrorInfoToConsole(QString)));
    // --- protocol packets
    connect(this->myProtocolAPI->protocolPackets, SIGNAL(message(QString)),
            this, SLOT(passErrorInfoToConsole(QString)));
    // --- protocol API
    connect(this->myProtocolAPI, SIGNAL(message(QString)),
            this, SLOT(passErrorInfoToConsole(QString)));
#endif

    QTimer *timerProcessBytes = new QTimer(this);
    connect(timerProcessBytes, SIGNAL(timeout()),
            this, SLOT(runProcessBytes()));
    timerProcessBytes->start(PROTOCOL_PROCESS_BYTES_INTERVAL);

    QTimer *timerProcessPackets = new QTimer(this);
    connect(timerProcessPackets, SIGNAL(timeout()),
            this, SLOT(runProcessPackets()));
    timerProcessPackets->start(PROTOCOL_PROCESS_PACKETS_INTERVAL);

    QTimer *timerEditGUI = new QTimer(this);
    connect(timerEditGUI, SIGNAL(timeout()),
            this, SLOT(runEditGUI()));
    timerEditGUI->start(GUI_REFREASH_RATE_INTERVAL);
}

///
/// \brief DroneShortRangeComm::~DroneShortRangeComm
///
DroneShortRangeComm::~DroneShortRangeComm()
{
    delete ui;
}

///
/// \brief slots BT:
///
void DroneShortRangeComm::on_buttonSearchBTDevices_released()
{
    this->runBluetoothDevicePickerDialog();
}
void DroneShortRangeComm::setBluetoothSocketNameAndAddress(QString name, QString address)
{
    this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->setSocketAddres(address);
    this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->setSocketName(name);
}
void DroneShortRangeComm::bluetoothConnected()
{
    this->addTooProgramConsole("Connected with " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->getSocketName() +
                               " " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->getSocketAddress());

    this->myProtocolAPI->sendPacket(INIT_COMM);
    ui->connection->setText("connected");
    ui->connection->setStyleSheet("color: #41f45c");
}
void DroneShortRangeComm::bluetoothDisconnected()
{
    this->addTooProgramConsole("Disconected from " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->getSocketName() +
                               " " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->getSocketAddress());

    ui->connection->setText("disconnected");
    ui->connection->setStyleSheet("color: #f44242");
}

///
/// \brief slots bt & serial:
///
void DroneShortRangeComm::on_buttonDisconnect_released()
{
    this->myProtocolAPI->sendPacket(STOP_COMM);
    this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->disconnect();
    this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->disconnect();
}

///
/// \brief slots serial:
///
void DroneShortRangeComm::on_buttonSearchSerialDevices_released()
{
    this->runSerialDevicePickerDialog();
}
void DroneShortRangeComm::portSelected(QString portName, QString portDescription)
{
    this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->setPortName(portName);
    this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->setPortDescription(portDescription);

    this->addTooProgramConsole("Wybrano port: " + portName + " " + portDescription);
}
void DroneShortRangeComm::serialConnected()
{
    this->addTooProgramConsole("Connected with " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->getPortName() +
                               " " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->getPortDescription());

    ui->connection->setText("connected");
    ui->connection->setStyleSheet("color: rgb(47, 237, 56)");
}
void DroneShortRangeComm::serialDisconnected()
{
    this->addTooProgramConsole("Disconected from " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->getPortName() +
                               " " +
                               this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->getPortDescription());

    ui->connection->setText("disconnected");
    ui->connection->setStyleSheet("color: rgb(206, 0, 0);");
}

///
/// \brief slots console:
///
void DroneShortRangeComm::passErrorInfoToConsole(QString data)
{
    this->addTooProgramConsole(data, ERROR);
}

///
/// \brief slots communication:
///
void DroneShortRangeComm::on_sendInitPacket_released()
{
    this->myProtocolAPI->sendPacket(INIT_COMM);
}
void DroneShortRangeComm::runProcessPackets()
{
    this->myProtocolAPI->processPackets();   
}
void DroneShortRangeComm::runProcessBytes()
{
    this->myProtocolAPI->protocolPackets->processInputBytes();
}

void DroneShortRangeComm::runEditGUI()
{
    // TODO zwracanie danych z protocolAPI do mainWindow
    //ui->doubleSpinBoxPitch->setValue(this->myProtocolAPI->pitch);
   // ui->doubleSpinBoxRoll->setValue(this->myProtocolAPI->roll);
}

///
/// \brief methods bt:
///
void DroneShortRangeComm::runBluetoothDevicePickerDialog()
{
    this->btDevicePicker = new BluetoothDevicePicker();

#if(DRONE_SHORT_RANGE_COMM_DEBUG == 0x01)
    // Connect signalItemClicked with slot to catch device name and address:
    qDebug() << "Connect status between btDevicePicker::btDeviceSelected() and DroneShortRangeComm::setBTNameAndAddress() = "
             << connect(this->btDevicePicker, SIGNAL(btDeviceSelected(QString, QString)),
                        this, SLOT(setBluetoothSocketNameAndAddress(QString, QString)));
#else
    connect(this->btDevicePicker, SIGNAL(btDeviceSelected(QString, QString)),
            this, SLOT(setBluetoothSocketNameAndAddress(QString, QString)));
#endif
    // Execute Bluetooth slave picker dialog app:
    if( this->btDevicePicker->exec() == QDialog::Accepted)
    {
        // And if device selected, connect with device:
        this->myProtocolAPI->protocolPackets->protocolBytes->bluetoothHandler->connect();
        this->myProtocolAPI->protocolPackets->protocolBytes->setConnectionType(BLUETOOTH);
    }

    delete this->btDevicePicker;
}

///
/// \brief methods serial:
///
void DroneShortRangeComm::runSerialDevicePickerDialog()
{
    this->serialDevicePicker = new SerialPortPicker(this);
#if(DRONE_SHORT_RANGE_COMM_DEBUG == 0x01)

    qDebug() << "Connect status between serialDevicePicker::portSelected() and DroneShortRangeComm::portSelected() = "
             << connect(this->serialDevicePicker, SIGNAL(portSelected(QString, QString)),
                        this, SLOT(portSelected(QString, QString)));
#else
    connect(this->serialDevicePicker, SIGNAL(portSelected(QString, QString)),
            this, SLOT(portSelected(QString, QString)));
#endif
    // Execute Serial port picker dialog app:
    if( this->serialDevicePicker->exec() == QDialog::Accepted)
    {
        // And if device selected, connect to port:
        this->myProtocolAPI->protocolPackets->protocolBytes->serialHandler->connect();
        this->myProtocolAPI->protocolPackets->protocolBytes->setConnectionType(SERIAL);
    }

    delete this->serialDevicePicker;
}

///
/// \brief methods utilites:
///
void DroneShortRangeComm::addTooProgramConsole(QString message, eMessageType messageType)
{
    this->useConsole(ui->listConsole, message, messageType);
}
void DroneShortRangeComm::useConsole(QListWidget *list, QString message, eMessageType messageType)
{
    switch (messageType) {
    case NORMAL:
        list->addItem((QTime::currentTime().toString() + "\t" + message));
        list->item(list->count() - 1)->setTextColor(QColor(2, 66, 244, 255));
        break;
    case WARNING:
        list->addItem((QTime::currentTime().toString() + "\t" + message));
        list->item(list->count() - 1)->setTextColor(QColor(255, 170, 0, 255));
        break;
    case ERROR:
        list->addItem((QTime::currentTime().toString() + "\t" + message));
        list->item(list->count() - 1)->setTextColor(QColor(255, 0, 0, 255));
        break;
    default:
        break;
    }

    list->scrollToBottom();
}

void DroneShortRangeComm::on_PdoubleSpinBox_valueChanged(double arg1)
{
    float value = (float)arg1;
    this->myProtocolAPI->sendPacket(PID_P_VALUE, &value, sizeof(value));
}

void DroneShortRangeComm::on_IdoubleSpinBox_valueChanged(double arg1)
{
    float value = (float)arg1;
    this->myProtocolAPI->sendPacket(PID_I_VALUE, &value, sizeof(value));
}

void DroneShortRangeComm::on_DdoubleSpinBox_valueChanged(double arg1)
{
    float value = (float)arg1;
    this->myProtocolAPI->sendPacket(PID_D_VALUE, &value, sizeof(value));
}

void DroneShortRangeComm::on_tunningEnable_stateChanged(int arg1)
{
    ui->PdoubleSpinBox->setEnabled(arg1);
    ui->IdoubleSpinBox->setEnabled(arg1);
    ui->DdoubleSpinBox->setEnabled(arg1);
}

void DroneShortRangeComm::on_pitchEnable_stateChanged(int arg1)
{
    uint8_t value = (uint8_t)arg1;
    this->myProtocolAPI->sendPacket(ENABLE_PITCH_PID, &value, sizeof(value));
}

void DroneShortRangeComm::on_rollEnable_stateChanged(int arg1)
{
    uint8_t value = (uint8_t)arg1;
    this->myProtocolAPI->sendPacket(ENABLE_ROLL_PID, &value, sizeof(value));
}

#define FRONT_RIGHT_MOTOR   0
#define FRONT_LEFT_MOTOR    1
#define REAR_RIGHT_MOTOR    2
#define REAR_LEFT_MOTOR     3

void DroneShortRangeComm::chooseMotor(uint8_t motorID, double offsetValue)
{
    uint16_t table[2];

    table[0] = motorID;
    table[1] = (uint16_t)offsetValue;

    this->myProtocolAPI->sendPacket(CHANGE_MOTOR_OFFSET, &table, sizeof(table));

    this->addTooProgramConsole(QString::number(motorID) + " " + QString::number(offsetValue));
}

void DroneShortRangeComm::on_frontRightOffset_editingFinished()
{
    this->chooseMotor(FRONT_RIGHT_MOTOR, ui->frontRightOffset->value());
}

void DroneShortRangeComm::on_frontLeftOffset_editingFinished()
{
    this->chooseMotor(FRONT_LEFT_MOTOR, ui->frontLeftOffset->value());
}

void DroneShortRangeComm::on_rearRightOffset_editingFinished()
{
    this->chooseMotor(REAR_RIGHT_MOTOR, ui->rearRightOffset->value());
}

void DroneShortRangeComm::on_rearLeftOffset_editingFinished()
{
    this->chooseMotor(REAR_LEFT_MOTOR, ui->rearLeftOffset->value());
}

void DroneShortRangeComm::on_enableOffset_stateChanged(int arg1)
{
    uint8_t value = (uint8_t)arg1;
    this->myProtocolAPI->sendPacket(ENABLE_OFFSET, &value, sizeof(value));
}
