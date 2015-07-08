TEMPLATE = app
TARGET   = SimpGCS
CONFIG  += qt
QT      += core gui opengl sql svg network xml declarative
QT      += printsupport widgets

UI_DIR       = ./build
MOC_DIR      = ./build
OBJECTS_DIR  = ./build

SOURCES += \
    ./src/GCS_MainWindow.cpp \
    ./src/qFlightInstruments.cpp \
    ./src/MapWidget.cpp \
    ./src/utils_UART.cpp \
    ./src/utils_GPS.cpp \
    ./src/utils_mavlink.cpp \
    ./src/UAS.cpp \
    ./src/SimpGCS.cpp

HEADERS += \
    ./src/GCS_MainWindow.h \
    ./src/qFlightInstruments.h \
    ./src/MapWidget.h \
    ./src/utils_UART.h \
    ./src/utils_GPS.h \
    ./src/utils_mavlink.h \
    ./src/UAS.h


################################################################################
# Mavlink
################################################################################
INCLUDEPATH += ./libs

################################################################################
# RTK
################################################################################
RTK_DIR = ./libs/rtk++
INCLUDEPATH += $$RTK_DIR/include
LIBS += $$RTK_DIR/lib/librtk_osa.a $$RTK_DIR/lib/librtk_utils.a

################################################################################
# qglviewer
################################################################################
QGLVIEWER_DIR =
LIBS += -lQGLViewer -lGL -lGLU

################################################################################
# opmapcontrol
################################################################################
OPMAPCONTROL_DIR = ./libs/opmapcontrol
INCLUDEPATH +=  $$OPMAPCONTROL_DIR/core \
                $$OPMAPCONTROL_DIR/internals \
                $$OPMAPCONTROL_DIR/mapwidget
LIBS        += $$OPMAPCONTROL_DIR/libopmapwidget.a
RESOURCES   += $$OPMAPCONTROL_DIR/mapwidget/mapresources.qrc
