#include "vrview.h"
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <QKeyEvent>
#include <QApplication>
#include <QDateTime>
#include <iostream>
#include <math.h>
#include "glm/glm.hpp"
#include "Settings.h"
#include "glm/ext.hpp"
#define SCENE_NEAR_CLIP    0.01f
#define SCENE_FAR_CLIP     100.0f
VRView::VRView(QWidget *parent) : QOpenGLWidget(parent),
   m_hmd(0), m_camera(new OrbitingCamera()),
   m_scene(new Scene()),
   m_isDragging(false),
   m_hasPicked(false),
   m_preClickTime(0),
   m_curClickTime(0) {
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
   if (settings.frames > 0) {
      emit framesPerSecond(settings.frames);
   }
   settings.frames = 0;
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
//   m_logger = new QOpenGLDebugLogger(this);

//   connect(m_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
//           this, SLOT(debugMessage(QOpenGLDebugMessage)), Qt::DirectConnection);

//   if (m_logger->initialize()) {
//      m_logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
//      m_logger->enableMessages();
//   }
#endif
   m_camera->updateMatrices();

   if (settings.VRMode == false) {
      std::cerr << " ============================================" << std::endl;
      std::cerr << " This is the non-VR mode" << std::endl;
      std::cerr << " ============================================" << std::endl;
   }else {
      std::cerr << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
      std::cerr << " This is the VR mode" << std::endl;
      std::cerr << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   }

   m_scene->initScene();
   m_eyeWidth = width(), m_eyeHeight = height();
   if (settings.VRMode) {
      initVR();
   }
   m_scene->setDimension(width(), height());
   m_scene->initShadowMap();
}


void VRView::paintGL() {
   if (m_hmd) {
      // FM switched these two statements: input and poses
      updatePoses();
      updateInput();
      setMatrices(vr::Eye_Left);
      m_scene->bindShadowMap();
      m_scene->renderLeft();

      setMatrices(vr::Eye_Right);
      m_scene->renderRight();
   }else {
      setMatrices(vr::Eye_Right);
      m_scene->bindShadowMap();
   }
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

   settings.frames++;
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
   case Qt::Key_S:
      settings.shadowOn = !settings.shadowOn;
      break;
   case Qt::Key_0:
      settings.SAMPLES = 0;
      break;

   case Qt::Key_4:
      settings.SAMPLES = 4;
      break;

   case Qt::Key_N:
      m_scene->nextSky();
      break;

   case Qt::Key_L:
      settings.lightOn = !settings.lightOn;
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
   m_rightProjection = helper.vrMatrixToGlmMatrixEyeHead(m_hmd->GetProjectionMatrix(vr::Eye_Right, SCENE_NEAR_CLIP, SCENE_FAR_CLIP, vr::API_OpenGL));
   m_rightPose       = glm::inverse(helper.vrMatrixToGlmMatrixEyeHead(m_hmd->GetEyeToHeadTransform(vr::Eye_Right)));

   m_leftProjection = helper.vrMatrixToGlmMatrixEyeHead(m_hmd->GetProjectionMatrix(vr::Eye_Left, SCENE_NEAR_CLIP, SCENE_FAR_CLIP, vr::API_OpenGL));
   m_leftPose       = glm::inverse(helper.vrMatrixToGlmMatrixEyeHead(m_hmd->GetEyeToHeadTransform(vr::Eye_Left)));

   QString ident;
   ident.append("Room In Space - ");
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
         m_matrixDevicePose[i] = helper.vrMatrixToGlmMatrixPose(m_trackedDevicePose[i].mDeviceToAbsoluteTracking);
         // use the last one
         if (m_hmd->GetControllerRoleForTrackedDeviceIndex(i) == vr::TrackedControllerRole_RightHand) {
            m_scene->updateController(m_matrixDevicePose[i]);
            if (m_hasPicked == true) {
               m_scene->pickedUp(m_matrixDevicePose[i]);
            }
            //m_scene->printControllerBoundingBox();
         }
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
      ProcessVREvent(event);
   }

   m_curClickTime = QDateTime::currentMSecsSinceEpoch() / 1000;
   float diff = m_curClickTime - m_preClickTime;

   for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
      vr::VRControllerState_t state;
//    if (m_hmd->GetControllerState(i, &state, sizeof(state))) { //msvc and openvr.h
      if (m_hmd->GetControllerState(i, &state)) {  // mingw and openvr_mingw.hpp
         if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) {
            if (std::fabs(diff) > 0.2) {
               settings.lightOn = !settings.lightOn;
               m_preClickTime   = m_curClickTime;
            }
         } // end of light on

         diff = m_curClickTime - m_preClickTime;
         if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger ))  {
            if (m_hasPicked == false) {
               std::cout << "this should be false when you pick it up: " << m_hasPicked << std::endl;
               m_hasPicked = m_scene->pickUp(m_matrixDevicePose[i]);
               std::cout << "Pick up something? " << m_hasPicked << std::endl;
               m_preClickTime = m_curClickTime;
               break;
            }else{
              m_scene->putDown();
              m_hasPicked    = false;
            }

         }//testing

//            if ((m_hasPicked == true) && (std::fabs(diff) > 0.2)) {
//               //std::cout << "this should be true when you put it down" << std::endl;
//               std::cout << "there is a picked obj. update its matrix." << std::endl;
//               m_scene->pickedUp(m_matrixDevicePose[i]);
//               //std::cout << "now it should be false " << m_hasPicked << std::endl;

//            }
        // } // end of grap

//         diff = m_curClickTime - m_preClickTime;
//         if (!(state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger)) &&
//             ((m_hasPicked == true) && (std::fabs(diff) > 0.2))) {
//            m_scene->putDown();
//            m_hasPicked    = false;
//            m_preClickTime = m_curClickTime;
//            std::cout << "put back" << std::endl;
//         } // end of put down
//        if (m_hasPicked == true){
//          if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger)) {
//                  m_scene->putDown();
//                  m_hasPicked    = false;
//              }
//          }

         diff = m_curClickTime - m_preClickTime;
         if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) {
            m_curClickTime = QDateTime::currentMSecsSinceEpoch() / 1000;
            if (std::fabs(diff) > 0.2) {
               m_scene->nextSky();
               m_preClickTime = m_curClickTime;
            }
         } // end of next sky
      }    // end of get state
   }       // end of for
}          // end of function


//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void VRView::ProcessVREvent(const vr::VREvent_t& event) {
   switch (event.eventType)
   {
   case vr::VREvent_TrackedDeviceActivated: {
         m_scene->activeController();
         std::cout << "Device " << int(event.trackedDeviceIndex) << " attached. Set it visible." << std::endl;
      }
      break;

   case vr::VREvent_TrackedDeviceDeactivated: {
         m_scene->inactiveController();
         std::cout << "Device " << int(event.trackedDeviceIndex) << " deattached. Invisiable." << std::endl;
      }
      break;

   case vr::VREvent_TrackedDeviceUpdated: {
         std::cout << "Device " << int(event.trackedDeviceIndex) << " updated." << std::endl;
      }
      break;
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
      }else {
         return m_rightPose * m_hmdPose;
      }
   }else {
      return m_camera->getViewMatrix();
   }
}


glm::mat4x4 VRView::getProjMatrix(vr::Hmd_Eye eye) {
   if (settings.VRMode) {
      if (eye == vr::Eye_Left) {
         return m_leftProjection;
      }else {
         return m_rightProjection;
      }
   }else {
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
