QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    deviceModel/bodyFrameCfgWidget.cpp \
    deviceModel/bodyFrameItem.cpp \
    deviceModel/dataFrame.cpp \
    deviceModel/deviceModelScene.cpp \
    deviceModel/hardwareModel.cpp \
    dialog/stylesheetdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    monitor/monitorWidget.cpp \
    monitor/qcustomplot.cpp \
    newprojectdialog.cpp \
    project/proj659.cpp \
    simulation/faultwidget.cpp \
    simulation/simulinkwidget.cpp \
    tools/myxml.cpp

HEADERS += \
    deviceModel/bodyFrameCfgWidget.h \
    deviceModel/bodyFrameItem.h \
    deviceModel/dataFrame.h \
    deviceModel/deviceModelScene.h \
    deviceModel/hardwareModel.h \
    dialog/stylesheetdialog.h \
    mainwindow.h \
    monitor/monitorWidget.h \
    monitor/qcustomplot.h \
    newprojectdialog.h \
    project/proj659.h \
    simulation/faultwidget.h \
    simulation/simulinkwidget.h \
    tools/myxml.h

FORMS += \
    deviceModel/bodyFrameCfgWidget.ui \
    deviceModel/dataFrame.ui \
    deviceModel/hardwareModel.ui \
    dialog/stylesheetdialog.ui \
    mainwindow.ui \
    monitor/monitorWidget.ui \
    newprojectdialog.ui \
    simulation/faultwidget.ui \
    simulation/simulinkwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
