#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T00:52:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoomInSpace
TEMPLATE = app
CONFIG += c++14



SOURCES += ui/main.cpp\
    ui/mainwindow.cpp \
    ui/vrview.cpp \
    camera/OrbitingCamera.cpp \
    camera/CamtransCamera.cpp \
    utilities/Settings.cpp \
    utilities/Helpers.cpp \
    scene/Scene.cpp \
    scene/objmodeler/BoundingBox.cpp \
    scene/objmodeler/SceneObject.cpp \
    scene/objmodeler/PrimitiveObject.cpp \
    scene/objmodeler/GroupObject.cpp \
    scene/objmodeler/ObjLoader.cpp \
    #thirdparty/glew/src/glew.c
    #scene/rendermodel/CGLRenderModel.cpp \
    #scene/rendermodel/DeviceModels.cpp

HEADERS  += ui/mainwindow.h \
    ui/vrview.h \
    # thirdparty/openvr/headers/openvr.h \
    thirdparty/openvr/headers/openvr_mingw.hpp \
    camera/OrbitingCamera.h \
    camera/CamtransCamera.h \
    utilities/Settings.h \
    utilities/Helpers.h \
    scene/Scene.h \
    scene/objmodeler/BoundingBox.h \
    scene/objmodeler/SceneObject.h \
    scene/objmodeler/PrimitiveObject.h \
    scene/objmodeler/GroupObject.h \
    scene/objmodeler/material.h \
    scene/objmodeler/ObjLoader.h \
    scene/objmodeler/delete_ptr.h
    #thirdparty/glew/include/GL/glew.h \
    #scene/rendermodel/CGLRenderModel.h \
    #scene/rendermodel/DeviceModels.h

FORMS    += ui/mainwindow.ui

# from http://stackoverflow.com/a/10058744
# Copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        CONFIG( debug, debug|release ) {
            DDIR = $${OUT_PWD}/debug
        } else {
            DDIR = $${OUT_PWD}/release
        }

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

win32 {
    RC_FILE = win32.rc
    DEFINES += GLEW_STATIC
    contains(QT_ARCH, i386) {
    message("32 bit build")
        LIBS += -L$$PWD/thirdparty/openvr/lib/win32/ \
                -lopenvr_api -lopengl32 -lglu32
        copyToDestdir($${PWD}/thirdparty/openvr/bin/win32/openvr_api.dll)
    } else {
    message("64 bit build")
        LIBS += -L$$PWD/thirdparty/openvr/lib/win64/ \
                -lopenvr_api -lopengl32 -lglu32
        copyToDestdir($${PWD}/thirdparty/openvr/bin/win64/openvr_api.dll)
    }
}

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

INCLUDEPATH += thirdparty thirdparty/openvr/headers thirdparty/glew/include
INCLUDEPATH += utilities shaders models textures camera ui scenegraph utilities thirdparty/include
DEPENDPATH +=  thirdparty thirdparty/openvr/headersthirdparty/glew/include
DEPENDPATH += utilities textures models camera ui scenegraph utilities thirdparty/include

# from http://stackoverflow.com/a/25193580
#isEmpty(TARGET_EXT) {
#    win32 {
#        TARGET_CUSTOM_EXT = .exe
#    }
#    macx {
#        TARGET_CUSTOM_EXT = .app
#    }
#} else {
#    TARGET_CUSTOM_EXT = $${TARGET_EXT}
#}

#win32 {
#    DEPLOY_COMMAND = windeployqt
#}
#macx {
#    DEPLOY_COMMAND = macdeployqt
#}

#CONFIG( debug, debug|release ) {
#    # debug
#    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
#} else {
#    # release
#    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGE‘T_CUSTOM_EXT}))
#}

#QMAKE_POST_LINK += $${DEPLOY_COMMAND} $${DEPLOY_TARGET} $$escape_expand(\\n\\t)

DISTFILES += \
    shaders/phong.frag \
    shaders/phong.vert \
    shaders/phong.frag \
    shaders/phong.vert \
    textures/uvmap.png \
    models/sphere.obj \
    models/cylinder/cylinder.obj \
    models/simpleroom/roomsimple.obj \
    models/simpleroom/roomfull.obj \
    models/teapot/teapot.obj \
    models/isphere/isphere.obj \
    models/monkey.obj \
    models/monkey.mtl \
    models/cube/cube.obj \
    models/rect.obj \
    models/teapot/2teapot.obj \
    models/room/roominspace.obj \
    models/room/roominspace.mtl \
    models/tests/test.obj \
    models/tests/test.mtl \
    models/tests/maps/download.jpg \
    models/tests/maps/download2.jpg \
    models/tests/maps/download3.png \
    models/room/maps/6-091229101607-5-lit.jpg \
    models/room/maps/6-091229101607-7-lit.jpg \
    models/room/maps/6-091229101608-9-lit.jpg \
    models/room/maps/6-091229101608-11-lit.jpg \
    models/room/maps/2457331_093930679000_2.jpg \
    models/room/maps/20100724062419875118.jpg \
    models/room/maps/20100724062419875422.jpg \
    models/room/maps/bed_cover.jpg \
    models/room/maps/beige_wood.jpg \
    models/room/maps/black.jpg \
    models/room/maps/bool_master.jpg \
    models/room/maps/brown_wood.jpg \
    models/room/maps/ceiling.jpg \
    models/room/maps/clock.jpg \
    models/room/maps/clock_body.jpg \
    models/room/maps/earthinspace.jpg \
    models/room/maps/face.png \
    models/room/maps/floor.jpg \
    models/room/maps/hair.jpg \
    models/room/maps/kid_body.jpg \
    models/room/maps/mpm_vol.01_p22_pages_top_diff.jpg \
    models/room/maps/red.jpg \
    models/room/maps/photo_1.jpg \
    models/room/maps/photo_2.jpg \
    models/room/maps/photo_3.jpg \
    models/room/maps/photo_4.jpg \
    models/room/maps/photo_5.jpg \
    models/room/maps/photo_6.jpg \
    models/room/maps/book_page.jpg \
    models/room/maps/red_brown_wood.tif \
    models/room/maps/wall.jpg
