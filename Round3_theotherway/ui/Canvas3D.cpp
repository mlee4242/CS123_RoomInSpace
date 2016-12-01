#include "SupportCanvas3D.h"

#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QtCore/QCoreApplication>

#include "BGRA.h"
#include "CamtransCamera.h"
#include "OrbitingCamera.h"
#include "SceneviewScene.h"
#include "Settings.h"
#include "ShapesScene.h"

#include <iostream>
#include "gl/GLDebug.h"
#include "CS123XmlSceneParser.h"

Canvas3D::Canvas3D(QGLFormat format, QWidget *parent) :
   QGLWidget(format, parent)
   , m_update_pending(false)
   , m_animating(false)
   , m_context(0)
   , m_device(0)
   , m_isDragging(false)
   , m_defaultOrbitingCamera(new OrbitingCamera()) {}


Canvas3D::~Canvas3D() {
   delete m_device;
}


Camera *Canvas3D::getCamera() {
   return m_defaultOrbitingCamera.get();
}


void Canvas3D::render(QPainter *painter) {
   Q_UNUSED(painter);
}


OrbitingCamera *Canvas3D::getOrbitingCamera() {
   return m_defaultOrbitingCamera.get();
}


void Canvas3D::initializeGL() {
   initializeGlew();
   initializeOpenGLSettings();
   initializeScenes();
   settingsChanged();
}


void Canvas3D::initializeGlew() {
   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   glGetError();  // Clear errors after call to glewInit
   if (GLEW_OK != err) {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error initializing glew: %s\n", glewGetErrorString(err));
   }
}


void Canvas3D::initializeOpenGLSettings() {
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


void Canvas3D::initializeScenes() {
   m_shapesScene  = std::make_unique<ShapesScene>(width(), height());
   m_currentScene = m_shapesScene.get();
}


void Canvas3D::paintGL() {
   getCamera()->setAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
   float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
   glViewport(0, 0, width() * ratio, height() * ratio);
   m_currentScene->render(this);
}


void Canvas3D::settingsChanged() {
   settings.settingChanged = true;
   update();
}


void Canvas3D::mousePressEvent(QMouseEvent *event) {
   if (event->button() == Qt::RightButton) {
      getCamera()->mouseDown(event->x(), event->y());
      m_isDragging = true;
   }
   update();
}


void Canvas3D::mouseMoveEvent(QMouseEvent *event) {
   if (m_isDragging) {
      getCamera()->mouseDragged(event->x(), event->y());
   }
   update();
}


void Canvas3D::mouseReleaseEvent(QMouseEvent *event) {
   if (m_isDragging && (event->button() == Qt::RightButton)) {
      getCamera()->mouseUp(event->x(), event->y());
      m_isDragging = false;
   }
   update();
}


void Canvas3D::wheelEvent(QWheelEvent *event) {
   getCamera()->mouseScrolled(event->delta());
   update();
}


void Canvas3D::resizeEvent(QResizeEvent *event) {
   emit aspectRatioChanged();
   update();
}
