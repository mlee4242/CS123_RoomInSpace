# -------------------------------------------------
# Project created by QtCreator 2010-08-22T14:12:19
# -------------------------------------------------
QT += opengl xml
TARGET = Round3_RVS
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    camera/OrbitingCamera.cpp \
    camera/CamtransCamera.cpp \
    utilities/ResourceLoader.cpp \
    scenegraph/ShapesScene.cpp \
    scenegraph/Scene.cpp \
    scenegraph/OpenGLScene.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp\
    main.cpp \
    thirdparty/glew-1.10.0/src/glew.c \
    gl/shaders/Shader.cpp \
    gl/GLDebug.cpp \
    gl/textures/Texture.cpp \
    gl/datatype/FBO.cpp \
    gl/datatype/VBO.cpp \
    gl/datatype/VAO.cpp \
    gl/datatype/VBOAttribMarker.cpp \
    gl/shaders/CS123Shader.cpp \
    gl/datatype/IBO.cpp \
    gl/textures/Texture2D.cpp \
    gl/textures/TextureParameters.cpp \
    gl/textures/TextureParametersBuilder.cpp \
    utilities/BGRA.cpp \
    gl/util/FullScreenQuad.cpp \
    gl/shaders/GBufferShader.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/DepthBuffer.cpp \
    shape/OpenGLShape.cpp \
    shape/geometry/Shape.cpp \
    shape/surfaces/Surface.cpp \
    shape/surfaces/SphereFace.cpp \
    shape/geometry/Sphere.cpp \
    ui/Canvas3D.cpp


HEADERS += camera/OrbitingCamera.h \
    camera/Camera.h \
    utilities/ResourceLoader.h \
    scenegraph/ShapesScene.h \
    scenegraph/Scene.h \
    scenegraph/OpenGLScene.h \
    ui/SupportCanvas3D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    thirdparty/glew-1.10.0/include/GL/glew.h \
    gl/shaders/Shader.h \
    gl/GLDebug.h \
    gl/textures/Texture.h \
    gl/datatype/FBO.h \
    gl/datatype/VBO.h \
    gl/datatype/VAO.h \
    gl/datatype/VBOAttribMarker.h \
    gl/shaders/ShaderAttribLocations.h \
    gl/shaders/CS123Shader.h \
    gl/datatype/IBO.h \
    gl/textures/Texture2D.h \
    gl/textures/TextureParameters.h \
    gl/textures/TextureParametersBuilder.h \
    utilities/BGRA.h \
    gl/util/FullScreenQuad.h \
    gl/shaders/GBufferShader.h \
    gl/textures/RenderBuffer.h \
    gl/textures/DepthBuffer.h \
    gl/util/spheretest.h \
    shape/OpenGLShape.h \
    gl/util.h \
    shape/Parameter.h \
    shape/geometry/Shape.h \
    shape/surfaces/Surface.h \
    shape/surfaces/SphereFace.h \
    shape/geometry/Sphere.h

FORMS += \
    mainwindow.ui
INCLUDEPATH += thirdparty/glm camera ui scenegraph utilities thirdparty/glew-1.10.0/include
DEPENDPATH += thirdparty/glm camera ui scenegraph utilities thirdparty/glew-1.10.0/include
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
OTHER_FILES += shaders/shader.frag \
    shaders/shader.vert \
    shaders/wireframe/wireframe.vert \
    shaders/wireframe/wireframe.frag \
    shaders/normals/normals.vert \
    shaders/normals/normals.gsh \
    shaders/normals/normals.frag \
    shaders/normals/normalsArrow.vert \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/deferredlighting/gbuffer/gbuffer.frag \
    shaders/deferredlighting/gbuffer/gbuffer.vert \
    shaders/deferredlighting/lighting/lighting.frag \
    shaders/deferredlighting/lighting/lighting.vert \
    shaders/deferredlighting/compositing/compositing.frag \
    shaders/deferredlighting/compositing/compositing.vert

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain
unix:!macx {
    LIBS += -lGLU
}

win32 {
    ## Windows common build here
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglut -lglu32
    #LIBS += -loleaut64 -luuid -lodbc64 -lodbccp64 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32
    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        ## Windows x86 (32bit) specific build here
        INCLUDEPATH +=  lib/win32 bin/win32
        DEPENDPATH +=  lib/win32 bin/win32

        LIBS += -L$$PWD/lib/win32 -lopenvr_api

    } else {
        message("x86_64 build")
        ## Windows x64 (64bit) specific build here

        INCLUDEPATH +=  lib/win64 bin/win64
        DEPENDPATH +=  lib/win64 bin/win64

        LIBS += -L$$PWD/lib/win64 -lopenvr_api
        #DLLDESTDIR += bin/win64
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

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/normals/normals.vert \
    shaders/normals/normals.frag \
    shaders/normals/normals.gsh \
    shaders/normals/normalsArrow.gsh \
    shaders/normals/normalsArrow.frag \
    shaders/normals/normalsArrow.vert
