#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T18:28:16
#
#-------------------------------------------------

QT       += core gui bluetooth serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DroneShortRangeComm
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        DroneShortRangeComm.cpp \
    ProtocolBytesProcessing.cpp \
    ProtocolAPI.cpp \
    ProtocolPacketsProcessing.cpp \
    BluetoothDevicePicker.cpp \
    BluetoothHandler.cpp \
    SerialHandler.cpp \
    SerialPortPicker.cpp

HEADERS += \
        DroneShortRangeComm.h \
    ProtocolBytesProcessing.h \
    ProtocolAPI.h \
    ProtocolPacketsProcessing.h \
    crc16.h \
    SerialHandler.h \
    SerialPortPicker.h \
    BluetoothHandler.h \
    BluetoothDevicePicker.h

FORMS += \
        DroneShortRangeComm.ui \
    BluetoothDevicePicker.ui \
    SerialPortPicker.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    icon/icon.ico \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    icon/icon.png

RC_ICONS = icon/icon.ico

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
