QT += quick widgets
CONFIG += c++17
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic  -Werror -Wno-error=unused-parameter

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += include external /usr/include/opencv4/

HEADERS += \
    external/freenect.h \
    include/browsermanager.h \
    include/gesturedetector.h \
    include/kinectdevice.h \
    include/kinectimagestream.h \
    include/kinectmanager.h \
    include/pthreadmutex.h

SOURCES += \
    src/browsermanager.cpp \
    src/gesturedetector.cpp \
    src/kinectdevice.cpp \
    src/kinectimagestream.cpp \
    src/kinectmanager.cpp \
    src/pthreadmutex.cpp \
    src/main.cpp

RESOURCES += qml/qml.qrc

LIBS += -lfreenect -lopencv_imgproc -lopencv_core

