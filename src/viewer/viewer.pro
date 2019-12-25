QT += widgets

INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/lib -lpoppler-qt5

SOURCES += \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    mainwindow.h \

