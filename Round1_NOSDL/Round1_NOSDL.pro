QT += opengl xml
TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++14

HEADERS += shared/pathtools.h \
   thirdparty/glew-1.10.0/src/glew.h\
   shared/lodepng.h \
   shared/Matrices.h \
   headers/openvr.h \
   shared/compat.h \
   shared/Vectors.h \
   headers/openvr_capi.h \
   headers/openvr_driver.h

SOURCES += main.cpp\
           thirdparty/glew-1.10.0/src/glew.c \
           shared/pathtools.cpp \
           shared/lodepng.cpp \
           shared/Matrices.cpp

INCLUDEPATH += headers bin thirdparty/glm thirdparty/glew-1.10.0/include
DEPENDPATH += headers bin thirdparty/glm thirdparty/glew-1.10.0/include
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall

unix:macx{
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra
}
# none of them (a guess) works under windows
QMAKE_CXXFLAGS += -g

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

unix:!macx {
    LIBS += -lGLU
}

win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        ## Windows x86 (32bit) specific build here

    } else {
        message("x86_64 build")
        ## Windows x64 (64bit) specific build here
        DEFINES += GLEW_STATIC
        INCLUDEPATH +=  lib/win64 bin/win64 #thirdparty/sdl2-2.0.3/bin/win64  thirdparty/sdl2-2.0.3/include
        DEPENDPATH +=  lib/win64 bin/win64 #thirdparty/sdl2-2.0.3/bin/win64 thirdparty/sdl2-2.0.3/include
        LIBS += -lopengl32 -lglu32
        #LIBS += -loleaut64 -luuid -lodbc64 -lodbccp64 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32
        LIBS += -L$$PWD/lib/win64 -lopenvr_api
        DLLDESTDIR += bin/win64
    }
}


unix{
  LIBS += -L$$PWD/lib/linux64 -lopenvr_api
}

macx{
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
    LIBS += -L$$PWD/lib/osx32 -lopenvr_api
}

DISTFILES += \
    openvrsrc/CMakeLists.txt \
    openvrsrc/README
