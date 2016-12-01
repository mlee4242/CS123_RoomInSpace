#ifndef SUPPORTCANVAS3D_H
#define SUPPORTCANVAS3D_H

#include <memory>

#include "GL/glew.h"
#include <QGLWidget>

#include "glm.hpp"

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
class BGRA;
class Camera;
class OpenGLScene;
class ShapesScene;
class OrbitingCamera;
class CS123XmlSceneParser;
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class Canvas3D :  public QGLWidget {
   Q_OBJECT
public:
   Canvas3D(QGLFormat format, QWidget *parent);
   virtual ~Canvas3D();
   // Overridden from QGLWidget

   virtual void render(QPainter *painter);

   Camera *getCamera();
   OrbitingCamera *getOrbitingCamera();

// This function will be called by the UI when the settings have changed.
   virtual void settingsChanged();

signals:
   void aspectRatioChanged();


protected:
// Overridden from QWidget
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void mouseMoveEvent(QMouseEvent *event) override;
   virtual void mouseReleaseEvent(QMouseEvent *event) override;
   virtual void wheelEvent(QWheelEvent *event) override;
   virtual void resizeEvent(QResizeEvent *event) override;
   void initializeGL() override;
   void paintGL() override;

private:

   void initializeGlew();
   void initializeOpenGLSettings();
   void initializeScenes();

   bool m_isDragging;
   glm::vec4 m_cameraEye;

   std::unique_ptr<OrbitingCamera> m_defaultOrbitingCamera;
   OpenGLScene *m_currentScene;
   std::unique_ptr<ShapesScene> m_shapesScene;
   bool m_update_pending;
   bool m_animating;

   QOpenGLContext *m_context;
   QOpenGLPaintDevice *m_device;
};

#endif // SUPPORTCANVAS3D_H
