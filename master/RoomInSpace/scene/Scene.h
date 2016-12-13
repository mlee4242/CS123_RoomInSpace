#ifndef SCENE_H
#define SCENE_H
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLDebugMessage>
#include <QOpenGLDebugLogger>
#include <QOpenGLTexture>
#include <QElapsedTimer>

#include <memory>
#include "openvr_mingw.hpp"
#include "glm/glm.hpp"
#include "Settings.h"
#include "Helpers.h"
#include "objmodeler/ObjLoader.h"

class Scene
{
public:
   Scene();
   ~Scene();
   void initScene();
   void initVRScene();
   void initShadowMap();
   void setEyeDimension(uint32_t w, uint32_t h);
   void setDimension(uint32_t w, uint32_t h);
   void setMatrices(const glm::mat4x4& m, const glm::mat4x4& v, const glm::mat4x4& p);
   void setMatrices(const glm::mat4x4& v, const glm::mat4x4& p);
   void activeController();
   void inactiveController();
   void updateController(glm::mat4x4& mat);

   void renderLeft();
   void renderRight();
   void renderComp();
   void renderShawdowMap(vr::Hmd_Eye eye);
   void nextSky();
   void printControllerBoundingBox();
   void *getResolveTexture();

   void pickUp(bool& pickStatus, glm::mat4x4& mat);
   void putDown(bool& pickStatus);
   void updatePickedObjPos(glm::mat4x4& mat);

private:
   void renderEye(vr::Hmd_Eye eye, QOpenGLShaderProgram& shader);
   void generateTextureMap(const QVector<QString>& textures);
   void categorizeSceneObjects(QVector<SceneObject *>& objects);
   bool compileShader(QOpenGLShaderProgram& shader,
                      const QString&        vertexShaderPath,
                      const QString&        fragmentShaderPath);

   // QOpenGLShaderProgram m_shader;
   QOpenGLShaderProgram m_shadowShader;
   QOpenGLShaderProgram m_phongShader;
   QOpenGLBuffer m_vertexBuffer;
   QOpenGLVertexArrayObject m_vao;
   QMap<QString, QOpenGLTexture *> m_glTextMap;
   std::unique_ptr<ObjLoader> m_objLoader;
   QVector<SceneObject *> m_sceneObjs;
   QVector<SceneObject *> m_skyBoxes;
   int m_currentSky;
   std::unique_ptr<SceneObject> m_controllerObj;
   std::unique_ptr<SceneObject> m_pickedObj;

   QOpenGLFramebufferObject *m_leftBuffer;
   QOpenGLFramebufferObject *m_rightBuffer;
   QOpenGLFramebufferObject *m_resolveBuffer;
   QOpenGLFramebufferObject *m_shadowMapBuffer;
   QOpenGLTexture *m_depthMap;
   glm::mat4x4 m_modelMat;
   glm::mat4x4 m_viewMat;
   glm::mat4x4 m_projectMat;
   uint32_t m_eyeWidth, m_eyeHeight;
   int m_width, m_height;

   glm::mat4        m_lightProjection;
   glm::mat4        m_lightView;
   QMatrix4x4       m_lightSpaceMatrix;
};

#endif // SCENE_H
