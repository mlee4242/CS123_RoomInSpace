#ifndef CMAINAPPLICATION_H
#define CMAINAPPLICATION_H

#include <GL/glew.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include <openvr.h>

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include "shared/lodepng.h"
#include "shared/Matrices.h"
#include "shared/pathtools.h"
#include "CGLRenderModel.h"

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

//-----------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
class CMainApplication :  public QWindow, protected QOpenGLFunctions
{
   Q_OBJECT
public:
   // start of qopenglwindow
   explicit CMainApplication(QWindow *parent = 0);
   virtual void render(QPainter *painter);

   virtual void render() {}
   void setAnimating(bool animating);

public slots:
   void renderLater();
   void renderNow();

protected:
   bool event(QEvent *event) Q_DECL_OVERRIDE;
   void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
   bool m_update_pending;
   bool m_animating;

   QOpenGLContext *m_context;
   QOpenGLPaintDevice *m_device;
// end of qopenglwindow

public:
   CMainApplication(int argc, char *argv[], QWindow *parent = 0);
   virtual ~CMainApplication();

   bool BInit();
   bool BInitGL();
   bool BInitCompositor();

   void SetupRenderModels();

   void Shutdown();
   void RunMainLoop();
   bool HandleInput();
   void ProcessVREvent(const vr::VREvent_t& event);
   void RenderFrame();

   bool SetupTexturemaps();

   void SetupScene();
   void AddCubeToScene(Matrix4 mat, std::vector<float>& vertdata);
   void AddCubeVertex(float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float>& vertdata);

   void RenderControllerAxes();

   bool SetupStereoRenderTargets();
   void SetupCompanionWindow();
   void SetupCameras();

   void RenderStereoTargets();
   void RenderCompanionWindow();
   void RenderScene(vr::Hmd_Eye nEye);

   Matrix4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
   Matrix4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
   Matrix4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
   void UpdateHMDMatrixPose();

   Matrix4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose);

   GLuint CompileGLShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader);
   bool CreateAllShaders();

   void SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
   CGLRenderModel *FindOrLoadRenderModel(const char *pchRenderModelName);

private:
   bool m_bDebugOpenGL;
   bool m_bVerbose;
   bool m_bPerf;
   bool m_bVblank;
   bool m_bGlFinishHack;

   vr::IVRSystem *m_pHMD;
   vr::IVRRenderModels *m_pRenderModels;
   std::string m_strDriver;
   std::string m_strDisplay;
   vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
   Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
   bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];

private: // former SDL bookkeeping
   uint32_t m_nCompanionWindowWidth;
   uint32_t m_nCompanionWindowHeight;

   //SDL_GLContext m_pContext;

private: // OpenGL bookkeeping
   int m_iTrackedControllerCount;
   int m_iTrackedControllerCount_Last;
   int m_iValidPoseCount;
   int m_iValidPoseCount_Last;
   bool m_bShowCubes;

   std::string m_strPoseClasses;                             // what classes we saw poses for this frame
   char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];      // for each device, a character representing its class

   int m_iSceneVolumeWidth;
   int m_iSceneVolumeHeight;
   int m_iSceneVolumeDepth;
   float m_fScaleSpacing;
   float m_fScale;

   int m_iSceneVolumeInit;                                   // if you want something other than the default 20x20x20

   float m_fNearClip;
   float m_fFarClip;

   GLuint m_iTexture;

   unsigned int m_uiVertcount;

   GLuint m_glSceneVertBuffer;
   GLuint m_unSceneVAO;
   GLuint m_unCompanionWindowVAO;
   GLuint m_glCompanionWindowIDVertBuffer;
   GLuint m_glCompanionWindowIDIndexBuffer;
   unsigned int m_uiCompanionWindowIndexSize;

   GLuint m_glControllerVertBuffer;
   GLuint m_unControllerVAO;
   unsigned int m_uiControllerVertcount;

   Matrix4 m_mat4HMDPose;
   Matrix4 m_mat4eyePosLeft;
   Matrix4 m_mat4eyePosRight;

   Matrix4 m_mat4ProjectionCenter;
   Matrix4 m_mat4ProjectionLeft;
   Matrix4 m_mat4ProjectionRight;

   struct VertexDataScene
   {
      Vector3 position;
      Vector2 texCoord;
   };

   struct VertexDataWindow
   {
      Vector2 position;
      Vector2 texCoord;

      VertexDataWindow(const Vector2& pos, const Vector2 tex) :  position(pos), texCoord(tex) {    }
   };

   GLuint m_unSceneProgramID;
   GLuint m_unCompanionWindowProgramID;
   GLuint m_unControllerTransformProgramID;
   GLuint m_unRenderModelProgramID;

   GLint m_nSceneMatrixLocation;
   GLint m_nControllerMatrixLocation;
   GLint m_nRenderModelMatrixLocation;

   struct FramebufferDesc
   {
      GLuint m_nDepthBufferId;
      GLuint m_nRenderTextureId;
      GLuint m_nRenderFramebufferId;
      GLuint m_nResolveTextureId;
      GLuint m_nResolveFramebufferId;
   };

   FramebufferDesc leftEyeDesc;
   FramebufferDesc rightEyeDesc;

   bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc);

   uint32_t m_nRenderWidth;
   uint32_t m_nRenderHeight;

   std::vector<CGLRenderModel *> m_vecRenderModels;
   CGLRenderModel *m_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];
};


#endif // CMAINAPPLICATION_H
