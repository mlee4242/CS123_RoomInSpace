#ifndef SUPPORTCANVAS3D_H
#define SUPPORTCANVAS3D_H

#include <memory>

#include "GL/glew.h"
#include <QGLWidget>

#include "glm/glm.hpp"

class BGRA;
class Camera;
class OpenGLScene;
class ShapesScene;
//class SceneviewScene;
class OrbitingCamera;
class CamtransCamera;
class CS123XmlSceneParser;

/**
 * @class  SupportCanvas3D
 *
 * The SupportCanvas3D class holds a single active OpenGLScene, and either
 * calls upon that scene to draw itself using OpenGL or draws the scene
 * by directly calling upon OpenGL (getting the scene-specific information
 * from the OpenGLScene object). The details of the implementation are left
 * to the student; neither way is better than the other.
 *
 * The SupportCanvas3D also contains a default camera which can be used in
 * case the loaded scene does not specify a camera.
 */

class SupportCanvas3D : public QGLWidget {
   Q_OBJECT
public:
   SupportCanvas3D(QGLFormat format, QWidget *parent);

   virtual ~SupportCanvas3D();

   Camera *getCamera();
   OrbitingCamera *getOrbitingCamera();
   CamtransCamera *getCamtransCamera();

   // This function will be called by the UI when the settings have changed.
   virtual void settingsChanged();

signals:
   void aspectRatioChanged();

protected:
   // Overridden from QGLWidget
   virtual void initializeGL() override;
   virtual void paintGL() override;

   // Overridden from QWidget
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void mouseMoveEvent(QMouseEvent *event) override;
   virtual void mouseReleaseEvent(QMouseEvent *event) override;
   virtual void wheelEvent(QWheelEvent *event) override;
   virtual void resizeEvent(QResizeEvent *event) override;

private:

   void initializeGlew();
   void initializeOpenGLSettings();
   void initializeScenes();
   void setSceneFromSettings();
   void setSceneToShapes();

   bool m_isDragging;
   glm::vec4 m_cameraEye;
   bool m_settingsDirty;
   std::unique_ptr<OrbitingCamera> m_defaultOrbitingCamera;
   OpenGLScene *m_currentScene;
   std::unique_ptr<ShapesScene> m_shapesScene;
};

#endif // SUPPORTCANVAS3D_H
