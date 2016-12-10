#include "DeviceModels.h"
#include "glm/glm.hpp"
#include <iostream>
#include <cstring>
#include <QString>
#include <glm/ext.hpp>

DeviceModels::DeviceModels():m_matrixDevicePosePtr(0)
{}

void DeviceModels::shutdown() {
   for (std::vector<CGLRenderModel *>::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++) {
      delete (*i);
   }
   m_vecRenderModels.clear();
}


void DeviceModels::initRenderModel(vr::IVRSystem *hmd) {
   vr::EVRInitError eError = vr::VRInitError_None;
   m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
   if (!m_pRenderModels) {
      std::cout << "Unable to get render model interface" << std::endl;
      exit(1);
   }
   glewExperimental = GL_TRUE;
   GLenum nGlewError = glewInit();
   if (nGlewError != GLEW_OK) {
      std::cout << "Error initializing GLEW!" << std::endl;
   }
   glGetError(); // to clear the error caused deep in GLEW
   compileShaderForRenderModel();
   memset(m_rTrackedDeviceToRenderModel, 0, sizeof(m_rTrackedDeviceToRenderModel));
   m_hmd = hmd;
   for (unsigned int i = vr::k_unTrackedDeviceIndex_Hmd + 1; i < vr::k_unMaxTrackedDeviceCount; i++) {
      if (!m_hmd->IsTrackedDeviceConnected(i)) {
         continue;
      }
      setupRenderModelForTrackedDevice(i);
   }
}


void DeviceModels::setMatrices(const glm::mat4x4& m,  glm::mat4x4 * matrixDevicePose) {
   m_tranMat = m;
   m_matrixDevicePosePtr = matrixDevicePose;
}

void DeviceModels::drawRenderModelForDevice() {
   glUseProgram(m_unRenderModelProgramID);

   for (unsigned int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
      if (!m_rTrackedDeviceToRenderModel[i] || !m_rbShowTrackedDevice[i]) {
         continue;
      }

      glm::mat4x4 deviceMatrix = *(m_matrixDevicePosePtr + i);
      glm::mat4x4 finalMatrix  = m_tranMat * deviceMatrix;
      //glUniformMatrix4fv(m_nRenderModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));
      m_rTrackedDeviceToRenderModel[i]->render();
   }

   glUseProgram(0);
}


//-----------------------------------------------------------------------------
// Purpose: Finds a render model we've already loaded or loads a new one
//-----------------------------------------------------------------------------
CGLRenderModel *DeviceModels::findOrLoadRenderModel(const char *pchRenderModelName) {
   CGLRenderModel *pRenderModel = NULL;
   for (std::vector<CGLRenderModel *>::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++) {
      if (!stricmp((*i)->getName().c_str(), pchRenderModelName)) {
         pRenderModel = *i;
         break;
      }
   }

   // load the model if we didn't find one
   if (!pRenderModel) {
      vr::RenderModel_t       *pModel;
      vr::EVRRenderModelError error;
      while (1)
      {
         error = vr::VRRenderModels()->LoadRenderModel_Async(pchRenderModelName, &pModel);
         if (error != vr::VRRenderModelError_Loading) {
            break;
         }

         // ThreadSleep(1);
      }

      if (error != vr::VRRenderModelError_None) {
         std::cout << "Unable to load render model" << std::endl; // %s - %s\n", pchRenderModelName, vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error));
         return NULL;                                             // move on to the next tracked device
      }

      vr::RenderModel_TextureMap_t *pTexture;
      while (1)
      {
         error = vr::VRRenderModels()->LoadTexture_Async(pModel->diffuseTextureId, &pTexture);
         if (error != vr::VRRenderModelError_Loading) {
            break;
         }

         // ThreadSleep(1);
      }

      if (error != vr::VRRenderModelError_None) {
         std::cout << "Unable to load render texture id" << std::endl; //
         vr::VRRenderModels()->FreeRenderModel(pModel);
         return NULL;                                                  // move on to the next tracked device
      }

      pRenderModel = new CGLRenderModel(pchRenderModelName);
      if (!pRenderModel->init(*pModel, *pTexture)) {
         std::cout << "Unable to create GL model from render model" << std::endl;  //
         delete pRenderModel;
         pRenderModel = NULL;
      }else {
         m_vecRenderModels.push_back(pRenderModel);
      }
      vr::VRRenderModels()->FreeRenderModel(pModel);
      vr::VRRenderModels()->FreeTexture(pTexture);
   }
   return pRenderModel;
}


QString DeviceModels::getTrackedDeviceString(vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *error) {
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


//-----------------------------------------------------------------------------
// Purpose: Create/destroy GL a Render Model for a single tracked device
//-----------------------------------------------------------------------------
void DeviceModels::setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex) {
   if (unTrackedDeviceIndex >= vr::k_unMaxTrackedDeviceCount) {
      return;
   }
   // try to find a model we've already set up
   QString        sRenderModelName = getTrackedDeviceString(unTrackedDeviceIndex, vr::Prop_RenderModelName_String);
   CGLRenderModel *pRenderModel    = findOrLoadRenderModel(sRenderModelName.toStdString().c_str());
   if (!pRenderModel) {
      QString sTrackingSystemName = getTrackedDeviceString(unTrackedDeviceIndex, vr::Prop_TrackingSystemName_String);
      std::cout << "Unable to load render model for tracked device" << std::endl;
   }else {
      m_rTrackedDeviceToRenderModel[unTrackedDeviceIndex] = pRenderModel;
      m_rbShowTrackedDevice[unTrackedDeviceIndex]         = true;
   }
}


//-----------------------------------------------------------------------------
// Purpose: Create/destroy GL Render Models
//-----------------------------------------------------------------------------
void DeviceModels::compileShaderForRenderModel() {
   m_unRenderModelProgramID = CompileGLShader(
      "render model",

      // vertex shader
      "#version 410\n"
      "uniform mat4 matrix;\n"
      "layout(location = 0) in vec4 position;\n"
      "layout(location = 1) in vec3 v3NormalIn;\n"
      "layout(location = 2) in vec2 v2TexCoordsIn;\n"
      "out vec2 v2TexCoord;\n"
      "void main()\n"
      "{\n"
      "	v2TexCoord = v2TexCoordsIn;\n"
      "	gl_Position = matrix * vec4(position.xyz, 1);\n"
      "}\n",

      //fragment shader
      "#version 410 core\n"
      "uniform sampler2D diffuse;\n"
      "in vec2 v2TexCoord;\n"
      "out vec4 outputColor;\n"
      "void main()\n"
      "{\n"
      "   outputColor = texture( diffuse, v2TexCoord);\n"
      "}\n"

      );
   m_nRenderModelMatrixLocation = glGetUniformLocation(m_unRenderModelProgramID, "matrix");
}


//-----------------------------------------------------------------------------
// Purpose: Compiles a GL shader program and returns the handle. Returns 0 if
//			the shader couldn't be compiled for some reason.
//-----------------------------------------------------------------------------
GLuint DeviceModels::CompileGLShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader) {
   GLuint unProgramID = glCreateProgram();

   GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(nSceneVertexShader, 1, &pchVertexShader, NULL);
   glCompileShader(nSceneVertexShader);

   GLint vShaderCompiled = GL_FALSE;
   glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
   if (vShaderCompiled != GL_TRUE) {
      std::cout << "Unable to compile vertex shader" << std::endl;
      glDeleteProgram(unProgramID);
      glDeleteShader(nSceneVertexShader);
      return 0;
   }
   glAttachShader(unProgramID, nSceneVertexShader);
   glDeleteShader(nSceneVertexShader);      // the program hangs onto this once it's attached

   GLuint nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(nSceneFragmentShader, 1, &pchFragmentShader, NULL);
   glCompileShader(nSceneFragmentShader);

   GLint fShaderCompiled = GL_FALSE;
   glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
   if (fShaderCompiled != GL_TRUE) {
      std::cout << "Unable to compile fragment shader" << std::endl;
      glDeleteProgram(unProgramID);
      glDeleteShader(nSceneFragmentShader);
      return 0;
   }

   glAttachShader(unProgramID, nSceneFragmentShader);
   glDeleteShader(nSceneFragmentShader);      // the program hangs onto this once it's attached

   glLinkProgram(unProgramID);

   GLint programSuccess = GL_TRUE;
   glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
   if (programSuccess != GL_TRUE) {
      std::cout << "Error linking program" << std::endl;
      glDeleteProgram(unProgramID);
      return 0;
   }

   glUseProgram(unProgramID);
   glUseProgram(0);

   return unProgramID;
}
