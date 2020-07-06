QT += network

DEFINES += QT_DEPRECATED_WARNINGS

unix {
    INCLUDEPATH += /usr/include/quazip5
    LIBS += -L/usr/lib -lquazip5
}

SOURCES += \
    main.cpp \
    program.cpp

HEADERS += \
    program.h
