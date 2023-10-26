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
    data/bodyframe.cpp \
    data/dataframe.cpp \
    data/framewindow.cpp \
    data/module.cpp \
    deviceModel/arrow.cpp \
    deviceModel/bodyFrameCfgWidget.cpp \
    deviceModel/bodyFrameGraphicsItem.cpp \
    deviceModel/bodyframetobuslineitem.cpp \
    deviceModel/busgraphicsitem.cpp \
    deviceModel/commandfilewidget.cpp \
    deviceModel/dataFrameCfgWidget.cpp \
    deviceModel/deviceModelScene.cpp \
    deviceModel/modulecfgwidget.cpp \
    dialog/datatransferwindowcfgdialog.cpp \
    dialog/frameswitchwindowcfgdialog.cpp \
    dialog/freewindowcfgdialog.cpp \
    dialog/interruptionwindowcfgdialog.cpp \
    dialog/jumpwindowcfgdialog.cpp \
    dialog/longsyncwindowcfgdialog.cpp \
    dialog/stylesheetdialog.cpp \
    dialog/subframecallwindowdialog.cpp \
    dialog/versiondetectionwindowcfgdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    monitor/monitorWidget.cpp \
    monitor/qcustomplot.cpp \
    newprojectdialog.cpp \
    project/proj659.cpp \
    simulation/faultwidget.cpp \
    simulation/simulinkwidget.cpp \
    tools/codegenerate.cpp \
    tools/coecodecompile.cpp \
    tools/commandfile.cpp \
    tools/globalfunc.cpp \
    tools/gramcheck.cpp \
    tools/labelscan.cpp \
    tools/mcscodecompile.cpp \
    tools/myxml.cpp \
    tools/preprocess.cpp \
    validator/stringidvalidator.cpp

HEADERS += \
    data/bodyframe.h \
    data/dataframe.h \
    data/framewindow.h \
    data/module.h \
    deviceModel/arrow.h \
    deviceModel/bodyFrameCfgWidget.h \
    deviceModel/bodyFrameGraphicsItem.h \
    deviceModel/bodyframetobuslineitem.h \
    deviceModel/busgraphicsitem.h \
    deviceModel/commandfilewidget.h \
    deviceModel/dataFrameCfgWidget.h \
    deviceModel/deviceModelScene.h \
    deviceModel/modulecfgwidget.h \
    dialog/windowcfgdialog.h \
    dialog/datatransferwindowcfgdialog.h \
    dialog/frameswitchwindowcfgdialog.h \
    dialog/freewindowcfgdialog.h \
    dialog/interruptionwindowcfgdialog.h \
    dialog/jumpwindowcfgdialog.h \
    dialog/longsyncwindowcfgdialog.h \
    dialog/stylesheetdialog.h \
    dialog/subframecallwindowdialog.h \
    dialog/versiondetectionwindowcfgdialog.h \
    mainwindow.h \
    monitor/monitorWidget.h \
    monitor/qcustomplot.h \
    newprojectdialog.h \
    project/proj659.h \
    simulation/faultwidget.h \
    simulation/simulinkwidget.h \
    tools/codegenerate.h \
    tools/coecodecompile.h \
    tools/commandfile.h \
    tools/globalfunc.h \
    tools/gramcheck.h \
    tools/labelscan.h \
    tools/mcscodecompile.h \
    tools/myxml.h \
    tools/preprocess.h \
    validator/stringidvalidator.h

FORMS += \
    deviceModel/bodyFrameCfgWidget.ui \
    deviceModel/commandfilewidget.ui \
    deviceModel/dataFrameCfgWidget.ui \
    deviceModel/modulecfgwidget.ui \
    dialog/datatransferwindowcfgdialog.ui \
    dialog/frameswitchwindowcfgdialog.ui \
    dialog/freewindowcfgdialog.ui \
    dialog/interruptionwindowcfgdialog.ui \
    dialog/jumpwindowcfgdialog.ui \
    dialog/longsyncwindowcfgdialog.ui \
    dialog/stylesheetdialog.ui \
    dialog/subframecallwindowdialog.ui \
    dialog/versiondetectionwindowcfgdialog.ui \
    mainwindow.ui \
    monitor/monitorWidget.ui \
    newprojectdialog.ui \
    simulation/faultwidget.ui \
    simulation/simulinkwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QMAKE_CXXFLAGS += --std=c++11 -fno-elide-constructors
RESOURCES += \
    resources.qrc
