#ifndef DEVICEMODELS_H
#define DEVICEMODELS_H
#include <vector>
#include <QString>
#include "CGLRenderModel.h"
#include "glm/glm.hpp"

class DeviceModels
{
public:
   DeviceModels();
   void initRenderModel(vr::IVRSystem *hmd);
   void drawRenderModelForDevice(const glm::mat4x4(&matrixDevicePose)[vr::k_unMaxTrackedDeviceCount]);

   void shutdown();
   void setMatrice(const glm::mat4x4& m);

private:
   void setupRenderModels();

   void compileShaderForRenderModel();
   QString getTrackedDeviceString(vr::TrackedDeviceIndex_t  device,
                                  vr::TrackedDeviceProperty prop,
                                  vr::TrackedPropertyError  *error = 0);
   GLuint CompileGLShader(const char *pchShaderName, const char *pchVertexShader,
                          const char *pchFragmentShader);

   void setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
   CGLRenderModel *findOrLoadRenderModel(const char *pchRenderModelName);

   vr::IVRSystem *m_hmd;
   std::vector<CGLRenderModel *> m_vecRenderModels;
   vr::IVRRenderModels *m_pRenderModels;
   CGLRenderModel *m_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];
   bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];
   GLuint m_unRenderModelProgramID;
   GLint m_nRenderModelMatrixLocation;

   glm::mat4x4 m_tranMat;
};

#endif // DEVICEMODELS_H
