#include "OpenGLScene.h"

#include <GL/glew.h>

#include "Settings.h"

OpenGLScene::~OpenGLScene()
{}


void OpenGLScene::setClearColor() {
   glClearColor(1.0, 1.0, 1.0, 1.0);
   if (settings.drawWireframe || settings.drawNormals) {
//        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
   } else {
//        glClearColor(0, 0, 0, 0);
   }
}
