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
    lib/ResourceLoader.cpp \
    scenegraph/ShapesScene.cpp \
    scenegraph/Scene.cpp \
    scenegraph/OpenGLScene.cpp \
    ui/SupportCanvas3D.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp \
    main.cpp \
    glew-1.10.0/src/glew.c \
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
    lib/BGRA.cpp \
    gl/util/FullScreenQuad.cpp \
    gl/shaders/GBufferShader.cpp \
    gl/textures/RenderBuffer.cpp \
    gl/textures/DepthBuffer.cpp \
    shape/OpenGLShape.cpp \
    shape/surfaces/Circle.cpp \
    shape/geometry/Cube.cpp \
    shape/geometry/Cylinder.cpp \
    shape/surfaces/Rectangle.cpp \
    shape/geometry/Shape.cpp \
    shape/surfaces/Surface.cpp \
    shape/surfaces/Barrel.cpp \
    shape/geometry/Cone.cpp \
    shape/surfaces/BarrelCone.cpp \
    shape/surfaces/SphereFace.cpp \
    shape/geometry/Sphere.cpp \
    shape/geometry/Mobius.cpp \
    shape/surfaces/MobiusFace.cpp \
    shape/surfaces/KleinBottleFace.cpp \
    shape/geometry/KleinBottle.cpp \
    shape/geometry/Torus.cpp \
    shape/surfaces/TorusFace.cpp \
    shape/surfaces/SeashellFace.cpp \
    shape/geometry/Seashell.cpp \
    shape/geometry/SierpinskiCube.cpp \
    shape/surfaces/FakeFace.cpp


HEADERS += brush/SmudgeBrush.h \
    brush/QuadraticBrush.h \
    brush/LinearBrush.h \
    brush/ConstantBrush.h \
    brush/Brush.h \
    camera/OrbitingCamera.h \
    camera/CamtransCamera.h \
    camera/Camera.h \
    lib/ResourceLoader.h \
    scenegraph/ShapesScene.h \
    scenegraph/SceneviewScene.h \
    scenegraph/Scene.h \
    scenegraph/RayScene.h \
    scenegraph/OpenGLScene.h \
    ui/SupportCanvas3D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    ui_mainwindow.h \
    glew-1.10.0/include/GL/glew.h \
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
    lib/BGRA.h \
    gl/util/FullScreenQuad.h \
    gl/shaders/GBufferShader.h \
    gl/textures/RenderBuffer.h \
    gl/textures/DepthBuffer.h \
    gl/util/spheretest.h \
    shape/OpenGLShape.h \
    gl/util.h \
    shape/Parameter.h \
    shape/surfaces/Circle.h \
    shape/geometry/Cube.h \
    shape/geometry/Cylinder.h \
    shape/surfaces/Rectangle.h \
    shape/geometry/Shape.h \
    shape/surfaces/Surface.h \
    shape/surfaces/Barrel.h \
    shape/geometry/Cone.h \
    shape/surfaces/SphereFace.h \
    shape/surfaces/BarrelCone.h \
    shape/geometry/Sphere.h \
    shape/surfaces/MobiusFace.h \
    shape/geometry/Mobius.h \
    shape/surfaces/KleinBottleFace.h \
    shape/geometry/KleinBottle.h \
    shape/geometry/Torus.h \
    shape/surfaces/TorusFace.h \
    shape/surfaces/SeashellFace.h \
    shape/geometry/Seashell.h \
    shape/geometry/SierpinskiCube.h \
    shape/surfaces/FakeFace.h

FORMS += ui/mainwindow.ui
INCLUDEPATH += glm camera lib scenegraph ui glew-1.10.0/include
DEPENDPATH += glm camera lib scenegraph ui glew-1.10.0/include
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

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
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
