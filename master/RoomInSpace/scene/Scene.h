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

/**
 * @brief The Scene class
 * The scene knows how to draw all the objects
 * It holds all the opengl calls
 */
class Scene
{
public:
   Scene();
   ~Scene();
   // initialize the scene
   void initScene();

   // [VR] initialize the framebuffer for VR Scene
   void initVRScene();

   // initialize shadow map
   void initShadowMap();
   void setEyeDimension(uint32_t w, uint32_t h);
   void setDimension(uint32_t w, uint32_t h);
   void setMatrices(const glm::mat4x4& m, const glm::mat4x4& v, const glm::mat4x4& p);
   void setMatrices(const glm::mat4x4& v, const glm::mat4x4& p);

   // active controller, set it visible
   void activeController();

   // inactive controller, set it invisible
   void inactiveController();

   // update the model matrix for the controller
   void updateController(glm::mat4x4& mat);

   // [VR] render the left eye
   void renderLeft();

   // [VR] render the right eye
   void renderRight();

   // render the [comp]uter / [comp]anion window
   void renderComp();

   // bind shadow map to the shader
   void bindShadowMap();

   // next skybox / skysphere
   void nextSky();

   // print the controller
   void printControllerBoundingBox();

   // get the resolve texture
   void *getResolveTexture();

   // return if there is an object is picked up
   bool pickUp(glm::mat4x4& mat);

   // if an object is picked up, update its model matrix
   void pickedUp(glm::mat4x4& mat);

   // put down objects
   void putDown();

private:
   // render the frame using the current setted matrices and the given shader
   void renderEye(QOpenGLShaderProgram& shader);

   // generate all the textures as QOpenGLTexture
   void generateTextureMap(const QVector<QString>& textures);

   // categorize different objects as scene objects, controller, or skybox
   void categorizeSceneObjects(QVector<SceneObject *>& objects);

   // compiler the shader, mostly phong shader and shadow shader
   bool compileShader(QOpenGLShaderProgram& shader,
                      const QString&        vertexShaderPath,
                      const QString&        fragmentShaderPath);

   // shadow shader, generate the depth map
   QOpenGLShaderProgram m_shadowShader;
   // phong shader, used in the regular rendering
   QOpenGLShaderProgram m_phongShader;
   // vertex buffer for ALL vertices
   QOpenGLBuffer *m_vertexBuffer;
   // VAO
   QOpenGLVertexArrayObject m_vao;
   // path, texture
   QMap<QString, QOpenGLTexture *> m_glTextMap;
   // all the objects on the scene
   QVector<SceneObject *> m_sceneObjs;
   // store all the skyboxes here
   QVector<SceneObject *> m_skyBoxes;
   // an object loader, use once
   std::unique_ptr<ObjLoader> m_objLoader;
   // which skybox / skysphere to show
   int m_currentSky;

   // the object for the controller, seperate from the others
   SceneObject *m_controllerObj;
   // the object that has been picked
   SceneObject *m_pickedObj;

   QOpenGLFramebufferObject *m_leftBuffer;
   QOpenGLFramebufferObject *m_rightBuffer;
   // the combined buffer for left and right one
   QOpenGLFramebufferObject *m_resolveBuffer;
   QOpenGLFramebufferObject *m_shadowMapBuffer;
   // the depth map
   QOpenGLTexture *m_depthMap;

   // the current view matrix, should be set up before drawing a frame
   glm::mat4x4 m_viewMat;
   // the current project matrix, different for different devices / eyes
   // should be updates before drawing a frame
   glm::mat4x4 m_projectMat;
   // the width and height for the current eye buffer
   uint32_t m_eyeWidth, m_eyeHeight;
   // the width and height for the screen framebuffer
   int m_width, m_height;

   // for the shadow, the projection matrix for the directional light
   glm::mat4 m_lightProjection;
   glm::mat4 m_lightView;
   glm::vec3 m_dirLightDir;
   QMatrix4x4 m_lightSpaceMatrix;
   // the position of the point light
   QVector3D m_pointLightPos;
};

#endif // SCENE_H
