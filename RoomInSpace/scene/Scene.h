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
#include <memory>
#include "openvr.h"
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
   void setEyeDimension(uint32_t w, uint32_t h);
   void setDimension(uint32_t w, uint32_t h);
   void setMatrices(const glm::mat4x4& m, const glm::mat4x4& v, const glm::mat4x4& p);
   void setMatrices(const glm::mat4x4& v, const glm::mat4x4& p);

   void renderLeft();
   void renderRight();
   void renderEye(vr::Hmd_Eye eye);
   void renderComp();
   void *getResolveTexture();

private:
   void generateTextureMap(const QVector<QString>& textures);
   bool compileShader(QOpenGLShaderProgram& shader,
                      const QString&        vertexShaderPath,
                      const QString&        fragmentShaderPath);

   QOpenGLShaderProgram m_shader;
   QOpenGLBuffer m_vertexBuffer;
   QOpenGLVertexArrayObject m_vao;
   QMap<QString, QOpenGLTexture *> m_glTextMap;
   std::unique_ptr<ObjLoader> m_objLoader;
   QVector<SceneObject *> m_sceneObjs;

   QOpenGLFramebufferObject *m_leftBuffer;
   QOpenGLFramebufferObject *m_rightBuffer;
   QOpenGLFramebufferObject *m_resolveBuffer;
   glm::mat4x4 m_modelMat;
   glm::mat4x4 m_viewMat;
   glm::mat4x4 m_projectMat;
   QString m_path = "models/room/";
   QString m_target = "roominspace";
   uint32_t m_eyeWidth, m_eyeHeight;
   int m_width, m_height;
   //FBOHandle *m_leftBuffer, *m_rightBuffer;
};

#endif // SCENE_H
