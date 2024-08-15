QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

CONFIG += c++17

QMAKE_PROJECT_DEPTH = 0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/ftableview.cpp \
    common/ftreeview.cpp \
    editrowdialog.cpp \
    fappconfigurator.cpp \
    main.cpp \
    mainwindow.cpp \
    proxy/customproxymodel.cpp

HEADERS += \
    common/ftableview.h \
    common/ftreeview.h \
    editrowdialog.h \
    fappconfigurator.h \
    mainwindow.h \
    proxy/customproxymodel.h \
    proxy/customproxymodel_p.h

FORMS += \
    common/ftableview.ui \
    common/ftreeview.ui \
    editrowdialog.ui \
    fappconfigurator.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
