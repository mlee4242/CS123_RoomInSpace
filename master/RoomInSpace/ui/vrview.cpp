#include "vrview.h"
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QKeyEvent>
#include <QApplication>
#include <iostream>
#include "glm/glm.hpp"
#include "Settings.h"
#include "glm/ext.hpp"

#define NEAR_CLIP    0.01f
#define FAR_CLIP     100.0f

VRView::VRView(QWidget *parent) : QOpenGLWidget(parent),
   m_hmd(0), m_frames(0), m_camera(new OrbitingCamera()),
   m_scene(new Scene()), m_isDragging(false) {
   memset(m_inputNext, 0, sizeof(m_inputNext));
   memset(m_inputNext, 0, sizeof(m_inputPrev));

   QSizePolicy size;
   size.setHorizontalPolicy(QSizePolicy::Expanding);
   size.setVerticalPolicy(QSizePolicy::Expanding);
   setSizePolicy(size);

   QTimer *fpsTimer = new QTimer(this);
   connect(fpsTimer, &QTimer::timeout, this, &VRView::updateFramerate);
   fpsTimer->start(1000);

   grabKeyboard();
}


VRView::~VRView() {
   shutdown();
}


QSize VRView::minimumSizeHint() const {
   return QSize(1, 1);
}


void VRView::updateFramerate() {
   if (m_frames > 0) {
      emit framesPerSecond(m_frames);
   }
   m_frames = 0;
}


void VRView::shutdown() {
   makeCurrent();

   if (m_hmd) {
      vr::VR_Shutdown();
      m_hmd = 0;
   }

   qDebug() << "Shutdown";
   doneCurrent();
}


void VRView::debugMessage(QOpenGLDebugMessage message) {
   qDebug() << message;
}


void VRView::initializeGL() {
   initializeOpenGLFunctions();

#ifdef QT_DEBUG
   m_logger = new QOpenGLDebugLogger(this);

   connect(m_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
           this, SLOT(debugMessage(QOpenGLDebugMessage)), Qt::DirectConnection);

   if (m_logger->initialize()) {
      m_logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
      m_logger->enableMessages();
   }
#endif
   m_scene->initScene();

   if (settings.VRMode) {
      initVR();
   }
}


void VRView::paintGL() {
   if (m_hmd) {
      updatePoses();
      updateInput();
      setMatrices(vr::Eye_Left);
      m_scene->renderLeft();
      setMatrices(vr::Eye_Right);
      m_scene->renderRight();
   }else{
      setMatrices(vr::Eye_Right);
   }
   m_scene->setDimension(width(), height());
   m_scene->renderComp();
   if (m_hmd) {
      void *resolveTex = m_scene->getResolveTexture();
      vr::VRTextureBounds_t leftRect  = { 0.0f, 0.0f, 0.5f, 1.0f };
      vr::VRTextureBounds_t rightRect = { 0.5f, 0.0f, 1.0f, 1.0f };
      vr::Texture_t         composite = { resolveTex, vr::API_OpenGL, vr::ColorSpace_Gamma };

      vr::VRCompositor()->Submit(vr::Eye_Left, &composite, &leftRect);
      vr::VRCompositor()->Submit(vr::Eye_Right, &composite, &rightRect);
   }

   //vr::VRCompositor()->PostPresentHandoff();

   m_frames++;

   update();
}


void VRView::setMatrices(vr::Hmd_Eye eye) {
   glm::mat4x4 v = getViewMatrix(eye);
   glm::mat4x4 p = getProjMatrix(eye);
   m_scene->setMatrices(v, p);
}


void VRView::resizeGL(int, int) {
   // do nothing
}


void VRView::keyPressEvent(QKeyEvent *event) {
   switch (event->key())
   {
   case Qt::Key_Left:
   case Qt::Key_Backspace:
   case Qt::Key_Right:
   case Qt::Key_Space:
   case Qt::Key_Escape:
      break;

   default:
      event->ignore();
   }
}


void VRView::initVR() {
   vr::EVRInitError error = vr::VRInitError_None;
   m_hmd = vr::VR_Init(&error, vr::VRApplication_Scene);

   if (error != vr::VRInitError_None) {
      m_hmd = 0;

      QString message = vr::VR_GetVRInitErrorAsEnglishDescription(error);
      qCritical() << message;
      QMessageBox::critical(this, "Unable to init VR", message);
      return;
   }

   // get eye matrices
   m_rightProjection = helper.vrMatrixToGlmMatrix(m_hmd->GetProjectionMatrix(vr::Eye_Right, NEAR_CLIP, FAR_CLIP, vr::API_OpenGL));
   m_rightPose       = glm::inverse(helper.vrMatrixToGlmMatrix(m_hmd->GetEyeToHeadTransform(vr::Eye_Right)));

   m_leftProjection = helper.vrMatrixToGlmMatrix(m_hmd->GetProjectionMatrix(vr::Eye_Left, NEAR_CLIP, FAR_CLIP, vr::API_OpenGL));
   m_leftPose       = glm::inverse(helper.vrMatrixToGlmMatrix(m_hmd->GetEyeToHeadTransform(vr::Eye_Left)));

   QString ident;
   ident.append("QVRViewer - ");
   ident.append(getTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String));
   ident.append(" ");
   ident.append(getTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String));
   emit deviceIdentifier(ident);

   // setup frame buffers for eyes
   m_hmd->GetRecommendedRenderTargetSize(&m_eyeWidth, &m_eyeHeight);
   m_scene->setEyeDimension(m_eyeWidth, m_eyeHeight);
   m_scene->initVRScene();


   // turn on compositor
   if (!vr::VRCompositor()) {
      QString message = "Compositor initialization failed. See log file for details";
      qCritical() << message;
      QMessageBox::critical(this, "Unable to init VR", message);
      return;
   }

#ifdef QT_DEBUG
   vr::VRCompositor()->ShowMirrorWindow();
#endif
}


void VRView::updatePoses() {
   vr::VRCompositor()->WaitGetPoses(m_trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

   for (unsigned int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
      if (m_trackedDevicePose[i].bPoseIsValid) {
         m_matrixDevicePose[i] = helper.vrMatrixToGlmMatrix(m_trackedDevicePose[i].mDeviceToAbsoluteTracking);
      }
   }

   if (m_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
      m_hmdPose = glm::inverse(m_matrixDevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
   }
}


void VRView::updateInput() {
   vr::VREvent_t event;
   while (m_hmd->PollNextEvent(&event, sizeof(event)))
   {
      //ProcessVREvent( event );
   }

   for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
      vr::VRControllerState_t state;
      if (m_hmd->GetControllerState(i, &state, sizeof(state))) {
         if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) {
            if (!m_inputNext[i]) {
               // this is for next image, need to change
               // loadImageRelative(1);
               m_inputNext[i] = true;
            }
         }else if (m_inputNext[i]) {
            m_inputNext[i] = false;
         }

         if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) {
            if (!m_inputPrev[i]) {
               // loadImageRelative(-1);
               m_inputPrev[i] = true;
            }
         }else if (m_inputPrev[i]) {
            m_inputPrev[i] = false;
         }
      }
   }
}


void VRView::glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
   glUniformMatrix4fv(location, count, transpose, value);
}


void VRView::glUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
   glUniformMatrix4dv(location, count, transpose, value);
}


glm::mat4x4 VRView::getViewMatrix(vr::Hmd_Eye eye) {
   if (settings.VRMode) {
      if (eye == vr::Eye_Left) {
         return m_leftPose * m_hmdPose;
      } else{
         return m_rightPose * m_hmdPose;
      }
   }else{
      return m_camera->getViewMatrix();
   }
}


glm::mat4x4 VRView::getProjMatrix(vr::Hmd_Eye eye) {
   if (settings.VRMode) {
      if (eye == vr::Eye_Left) {
         return m_leftProjection;
      } else{
         return m_rightProjection;
      }
   }else{
      return m_camera->getProjectionMatrix();
   }
}


QString VRView::getTrackedDeviceString(vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *error) {
   uint32_t len = m_hmd->GetStringTrackedDeviceProperty(device, prop, NULL, 0, error);
   if (len == 0) {
      return "";
   }

   char *buf = new char[len];
   m_hmd->GetStringTrackedDeviceProperty(device, prop, buf, len, error);

   QString result = QString::fromLocal8Bit(buf);
   delete [] buf;

   return result;
}


void VRView::mousePressEvent(QMouseEvent *event) {
   if (event->button() == Qt::RightButton) {
      m_camera->mouseDown(event->x(), event->y());
      m_isDragging = true;
      update();
   }
}


void VRView::mouseMoveEvent(QMouseEvent *event) {
   if (m_isDragging) {
      m_camera->mouseDragged(event->x(), event->y());
      update();
   }
}


void VRView::mouseReleaseEvent(QMouseEvent *event) {
   if (m_isDragging && (event->button() == Qt::RightButton)) {
      m_camera->mouseUp(event->x(), event->y());
      m_isDragging = false;
      update();
   }
}


void VRView::wheelEvent(QWheelEvent *event) {
   m_camera->mouseScrolled(event->delta());
   update();
}
