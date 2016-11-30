QT += opengl xml
TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14

HEADERS += shared/pathtools.h \
   shared/lodepng.h \
   shared/Matrices.h

SOURCES += main.cpp\
           shared/pathtools.cpp \
           shared/lodepng.cpp \
           shared/Matrices.cpp

INCLUDEPATH += headers
DEPENDPATH += headers

unix:!macx {
    LIBS += -lGLU
}

win32{
  DEFINES += GLEW_STATIC
  LIBS += -lopengl32 -lglu32
  LIBS += -L$$PWD/lib/win32 -lopenvr_api
}


win64{
  LIBS += -L$$PWD/lib/win64 -lopenvr_api
}

unix{
  LIBS += -L$$PWD/lib/linux64 -lopenvr_api
}

macx{
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
    LIBS += -L lib/osx32 -lopenvr_api
}
