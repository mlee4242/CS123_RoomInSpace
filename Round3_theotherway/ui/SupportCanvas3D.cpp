#include "SupportCanvas3D.h"

#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>

#include "BGRA.h"
#include "CamtransCamera.h"
#include "OrbitingCamera.h"
#include "SceneviewScene.h"
#include "Settings.h"
#include "ShapesScene.h"

#include <iostream>
#include "gl/GLDebug.h"
#include "CS123XmlSceneParser.h"

SupportCanvas3D::SupportCanvas3D(QGLFormat format, QWidget *parent) : QGLWidget(format, parent),
   m_isDragging(false),
   m_settingsDirty(true),
   m_defaultOrbitingCamera(new OrbitingCamera())
{}


SupportCanvas3D::~SupportCanvas3D()
{}


Camera *SupportCanvas3D::getCamera() {
   return m_defaultOrbitingCamera.get();
}


OrbitingCamera *SupportCanvas3D::getOrbitingCamera() {
   return m_defaultOrbitingCamera.get();
}


void SupportCanvas3D::initializeGL() {
   initializeGlew();
   initializeOpenGLSettings();
   initializeScenes();
   setSceneFromSettings();
   settingsChanged();
}


void SupportCanvas3D::initializeGlew() {
   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   glGetError();  // Clear errors after call to glewInit
   if (GLEW_OK != err) {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error initializing glew: %s\n", glewGetErrorString(err));
   }
}


void SupportCanvas3D::initializeOpenGLSettings() {
   // Enable depth testing, so that objects are occluded based on depth instead of drawing order.
   glEnable(GL_DEPTH_TEST);

   // Move the polygons back a bit so lines are still drawn even though they are coplanar with the
   // polygons they came from, which will be drawn before them.
   glEnable(GL_POLYGON_OFFSET_LINE);
   glPolygonOffset(-1, -1);

   // Enable back-face culling, meaning only the front side of every face is rendered.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Specify that the front face is represented by vertices in counterclockwise order (this is
   // the default).
   glFrontFace(GL_CCW);

   // Calculate the orbiting camera matrices.
   getOrbitingCamera()->updateMatrices();
}


void SupportCanvas3D::initializeScenes() {
   m_shapesScene = std::make_unique<ShapesScene>(width(), height());
}


void SupportCanvas3D::paintGL() {
   if (m_settingsDirty) {
      setSceneFromSettings();
   }

   float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
   glViewport(0, 0, width() * ratio, height() * ratio);
   getCamera()->setAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
   m_currentScene->render(this);
}


void SupportCanvas3D::settingsChanged() {
   m_settingsDirty = true;
   update();  /* repaint the scene */
   settings.settingChanged = true;
}


void SupportCanvas3D::setSceneFromSettings() {
   setSceneToShapes();

   m_settingsDirty = false;
}


void SupportCanvas3D::setSceneToShapes() {
   assert(m_shapesScene.get());
   m_currentScene = m_shapesScene.get();
}


void SupportCanvas3D::mousePressEvent(QMouseEvent *event) {
   if (event->button() == Qt::RightButton) {
      getCamera()->mouseDown(event->x(), event->y());
      m_isDragging = true;
      update();
   }
}


void SupportCanvas3D::mouseMoveEvent(QMouseEvent *event) {
   if (m_isDragging) {
      getCamera()->mouseDragged(event->x(), event->y());
      update();
   }
}


void SupportCanvas3D::mouseReleaseEvent(QMouseEvent *event) {
   if (m_isDragging && (event->button() == Qt::RightButton)) {
      getCamera()->mouseUp(event->x(), event->y());
      m_isDragging = false;
      update();
   }
}


void SupportCanvas3D::wheelEvent(QWheelEvent *event) {
   getCamera()->mouseScrolled(event->delta());
   update();
}


void SupportCanvas3D::resizeEvent(QResizeEvent *event) {
   emit aspectRatioChanged();
}
