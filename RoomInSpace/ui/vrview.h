#ifndef VRVIEW_H
#define VRVIEW_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLDebugMessage>
#include <QOpenGLDebugLogger>
#include <QOpenGLTexture>
#include <openvr.h>
#include <memory>
#include "glm/glm.hpp"
#include "OrbitingCamera.h"
#include "scene/Scene.h"
#include "Settings.h"
#include "Helpers.h"
class VRView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core
{
   Q_OBJECT
public:
   explicit VRView(QWidget *parent = 0);
   virtual ~VRView();

   QSize minimumSizeHint() const;

signals:
   void framesPerSecond(float);
   void deviceIdentifier(const QString&);
   void frameSwap();
   void statusMessage(const QString&);

public slots:

protected slots:
   void updateFramerate();
   void shutdown();
   void debugMessage(QOpenGLDebugMessage message);

protected:
   void initializeGL();
   void paintGL();
   void resizeGL(int w, int h);
   void keyPressEvent(QKeyEvent *event);

   // Overridden from QWidget
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void mouseMoveEvent(QMouseEvent *event) override;
   virtual void mouseReleaseEvent(QMouseEvent *event) override;
   virtual void wheelEvent(QWheelEvent *event) override;

//   virtual void resizeEvent(QResizeEvent *event) override;

private:
   void initVR();

//   void renderEye(vr::Hmd_Eye eye);

   void updatePoses();

   void updateInput();

   void setMatrices(vr::Hmd_Eye eye);

   glm::mat4x4 getViewMatrix(vr::Hmd_Eye eye);

   glm::mat4x4 getProjMatrix(vr::Hmd_Eye eye);

//   bool compileShader(QOpenGLShaderProgram& shader,
//                      const QString&        vertexShaderPath,
//                      const QString&        fragmentShaderPath);

//   glm::mat4x4 vrMatrixToQt(const vr::HmdMatrix34_t& mat);
//   glm::mat4x4 vrMatrixToQt(const vr::HmdMatrix44_t& mat);

   // QMatrix is using qreal, so we need to overload to handle both platform cases
   void glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
   void glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);

   glm::mat4x4 viewProjection(vr::Hmd_Eye eye);

   QString getTrackedDeviceString(vr::TrackedDeviceIndex_t  device,
                                  vr::TrackedDeviceProperty prop,
                                  vr::TrackedPropertyError  *error = 0);

   vr::IVRSystem *m_hmd;
   vr::TrackedDevicePose_t m_trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
   glm::mat4x4 m_matrixDevicePose[vr::k_unMaxTrackedDeviceCount];

   glm::mat4x4 m_leftProjection, m_leftPose;
   glm::mat4x4 m_rightProjection, m_rightPose;
   glm::mat4x4 m_hmdPose;

   QOpenGLDebugLogger *m_logger;

//   QOpenGLShaderProgram m_shader;
//   QOpenGLBuffer m_vertexBuffer;
//   QOpenGLVertexArrayObject m_vao;
//  QOpenGLTexture *m_texture;
//   int m_vertCount;

   uint32_t m_eyeWidth, m_eyeHeight;
//   //FBOHandle *m_leftBuffer, *m_rightBuffer;
//   QOpenGLFramebufferObject *m_leftBuffer;
//   QOpenGLFramebufferObject *m_rightBuffer;
//   QOpenGLFramebufferObject *m_resolveBuffer;

   int m_frames;

   QString m_imageDirectory;
   QString m_currentImage;
   std::unique_ptr<OrbitingCamera> m_camera;
   std::unique_ptr<Scene> m_scene;

   bool m_inputNext[vr::k_unMaxTrackedDeviceCount];
   bool m_inputPrev[vr::k_unMaxTrackedDeviceCount];
   bool m_VRmode = false;
   bool m_isDragging;
};

#endif // VRVIEW_H
