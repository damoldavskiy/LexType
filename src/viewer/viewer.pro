QT += widgets

INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/lib -lpoppler-qt5

SOURCES += \
    ../editor/styler.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    ../editor/styler.h \
    mainwindow.h \

