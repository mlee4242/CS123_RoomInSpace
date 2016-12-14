#include <QDir>
#include <QDateTime>
#include "Scene.h"
#include "glm/ext.hpp"
#include "objmodeler/SceneObject.h"
#include "objmodeler/GroupObject.h"
#include "objmodeler/PrimitiveObject.h"
#include "objmodeler/delete_ptr.h"
#include "Settings.h"
#define LIGHT_NEAR_CLIP    0.1f
#define LIGHT_FAR_CLIP     5.5f
#define SHADOWMAP_SIZE     2048
Scene::Scene() :
   m_glTextMap(QMap<QString, QOpenGLTexture *>()),
   m_sceneObjs(QVector<SceneObject *>()),
   m_skyBoxes(QVector<SceneObject *>()),
   m_viewMat(glm::mat4x4()),
   m_projectMat(glm::mat4x4()),
   m_modelMat(glm::mat4x4()),
   m_eyeWidth(0), m_eyeHeight(0),
   m_leftBuffer(0), m_rightBuffer(0),
   m_objLoader(new ObjLoader),
   m_controllerObj(nullptr),
   m_pickedObj(nullptr),
   m_depthMap(nullptr),
   m_currentSky(0),
   m_pointLightPos(QVector3D(0, 1.8, 0)) {
   m_lightProjection = glm::ortho(-5.5f, 5.5f, -5.5f, 5.5f,
                                  LIGHT_NEAR_CLIP, LIGHT_FAR_CLIP);
   m_dirLightDir = glm::vec3(1.0f, 3.0f, 1.0f);
   m_lightView   = glm::lookAt(m_dirLightDir,
                               glm::vec3(0.0f, 0.0f, -2.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
   m_lightSpaceMatrix = helper.mat4x4ToQMatrix4x4(m_lightProjection * m_lightView);
}


Scene::~Scene() {
   m_vertexBuffer.destroy();
   m_vao.destroy();
   if (m_leftBuffer) {
      delete m_leftBuffer;
   }
   if (m_rightBuffer) {
      delete m_rightBuffer;
   }
   if (m_resolveBuffer) {
      delete m_resolveBuffer;
   }
   if (m_shadowMapBuffer) {
      delete m_shadowMapBuffer;
   }
   if (m_glTextMap.size() > 0) {
      QMap<QString, QOpenGLTexture *>::iterator i;
      for (i = m_glTextMap.begin(); i != m_glTextMap.end(); i++) {
         QOpenGLTexture *ptr = i.value();
//         delete ptr;
      }
   }
}


void Scene::generateTextureMap(const QVector<QString>& textures) {
   for (QString txt : textures) {
      m_glTextMap[txt] = new QOpenGLTexture(QImage(QDir::currentPath() + "/" + settings.path + txt));
   }
}


void Scene::categorizeSceneObjects(QVector<SceneObject *>& objects) {
   for (SceneObject *obj : objects) {
//      std::cout << obj->getName().toStdString() << std::endl;
      if (obj->getName().contains("Sky")) {
         obj->setActive(false);
         m_skyBoxes.push_front(obj);
      }else if (obj->getName().contains("Controller")) {
         obj->setActive(false);
         m_controllerObj = obj;
      }else{
         m_sceneObjs.push_back(obj);
      }
   }
}


void Scene::printControllerBoundingBox() {
   BoundingBox box;
   m_controllerObj->getBox(box);
   box.printVertices();
}


void Scene::pickedUp(glm::mat4x4& mat) {
   // need to check if m_pickedObj is nothing
   if (m_pickedObj) {
      m_pickedObj->updateModelMatrixFromReference(mat);
   }
}


bool Scene::pickUp(glm::mat4x4& mat) {
   BoundingBox controllerBox;
   BoundingBox objBox;
   bool        collide;
   m_controllerObj->getBox(controllerBox);
   for (SceneObject *obj : m_sceneObjs) {
      if (obj->isPickable()) {
         std::cout << "this object is pickable" << std::endl;
         obj->getBox(objBox);
         collide = objBox.overlap(controllerBox);
         if (collide == 0) {
            continue;
         }
         std::cout << "collide value is " << std::endl;
         std::cout << collide << std::endl;
         m_pickedObj = obj;
         m_pickedObj->setReferenceMatrx(mat);

         std::cout << "this is reference matrix" << std::endl;
         std::cout << glm::to_string(mat) << std::endl;
         m_pickedObj->updateModelMatrixFromReference(mat);
         m_pickedObj->setIsPicked(true);
         return true;
      }
   }
   return false;
}


void Scene::putDown() {
   m_pickedObj->resetModelMatrix();
   m_pickedObj->resetReferenceMatrx();
   m_pickedObj->setIsPicked(false);
   m_pickedObj = nullptr;
}


void Scene::initScene() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);

   QVector<QString>       textures;
   QVector<GLfloat>       points;
   QVector<SceneObject *> objects;
   m_objLoader->loadObj(settings.path + settings.target, objects, points);
   m_objLoader->getTextureMap(textures);
   categorizeSceneObjects(objects);
   generateTextureMap(textures);

   // compile our shader
   compileShader(m_phongShader, QDir::currentPath() + "/shaders/phong.vert", QDir::currentPath() + "/shaders/phong.frag");
   compileShader(m_shadowShader, QDir::currentPath() + "/shaders/shadow.vert", QDir::currentPath() + "/shaders/shadow.frag");

   // build out sample geometry
   m_vao.create();
   m_vao.bind();

   m_vertexBuffer.create();
   m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
   m_vertexBuffer.bind();

   m_vertexBuffer.allocate(points.data(), points.length() * sizeof(GLfloat));

   m_phongShader.bind();

   m_phongShader.setAttributeBuffer("vertex", GL_FLOAT, 0, 3, settings.VERTEX_UNIT_SIZE * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("vertex");

   m_phongShader.setAttributeBuffer("texCoord", GL_FLOAT, 3 * sizeof(GLfloat), 2, settings.VERTEX_UNIT_SIZE * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("texCoord");

   m_phongShader.setAttributeBuffer("normal", GL_FLOAT, 5 * sizeof(GLfloat), 3, settings.VERTEX_UNIT_SIZE * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("normal");

//   m_shader.release(); // FM :+
   m_vao.release();    // FM :+
   m_vertexBuffer.release();
}


void Scene::initVRScene() {
   QOpenGLFramebufferObjectFormat buffFormat;
   buffFormat.setAttachment(QOpenGLFramebufferObject::Depth);
   buffFormat.setInternalTextureFormat(GL_RGBA8);
   buffFormat.setSamples(settings.SAMPLES);

   m_leftBuffer  = new QOpenGLFramebufferObject(m_eyeWidth, m_eyeHeight, buffFormat);
   m_rightBuffer = new QOpenGLFramebufferObject(m_eyeWidth, m_eyeHeight, buffFormat);

   QOpenGLFramebufferObjectFormat resolveFormat;
   resolveFormat.setInternalTextureFormat(GL_RGBA);
   buffFormat.setSamples(0);

   m_resolveBuffer = new QOpenGLFramebufferObject(m_eyeWidth * 2, m_eyeHeight, resolveFormat);
}


void Scene::setEyeDimension(uint32_t w, uint32_t h) {
   m_eyeWidth  = w;
   m_eyeHeight = h;
}


void Scene::setDimension(uint32_t w, uint32_t h) {
   m_width  = w;
   m_height = h;
}


bool Scene::compileShader(QOpenGLShaderProgram& shader, const QString& vertexShaderPath, const QString& fragmentShaderPath) {
   bool result = shader.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
   if (!result) {
      qCritical() << shader.log();
   }

   result = shader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);
   if (!result) {
      qCritical() << shader.log();
   }

   result = shader.link();
   if (!result) {
      qCritical() << "Could not link shader program:" << shader.log();
   }

   return result;
}


void Scene::setMatrices(const glm::mat4x4& m, const glm::mat4x4& v, const glm::mat4x4& p) {
   m_modelMat = m, m_viewMat = v, m_projectMat = p;
}


void Scene::setMatrices(const glm::mat4x4& v, const glm::mat4x4& p) {
   m_viewMat = v, m_projectMat = p;
}


void Scene::initShadowMap() {
   QOpenGLFramebufferObjectFormat depthBuffFormat;
   depthBuffFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
   depthBuffFormat.setInternalTextureFormat(GL_RGBA8);
   depthBuffFormat.setSamples(settings.SAMPLES);
   m_shadowMapBuffer = new QOpenGLFramebufferObject(SHADOWMAP_SIZE, SHADOWMAP_SIZE, depthBuffFormat);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE);
   glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
   m_shadowMapBuffer->bind();
   m_shadowShader.bind();
   renderEye(m_shadowShader);
   m_shadowMapBuffer->release();

   m_depthMap = new QOpenGLTexture(m_shadowMapBuffer->toImage(false));
}


void Scene::bindShadowMap() {
   m_phongShader.bind();
   m_phongShader.setUniformValue("shadowMap", 0);
   m_depthMap->bind(0);
}


void Scene::renderLeft() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glViewport(0, 0, m_eyeWidth, m_eyeHeight);

   glEnable(GL_MULTISAMPLE);
   m_leftBuffer->bind();
   renderEye(m_phongShader);
   m_leftBuffer->release();

   QRect sourceRect(0, 0, m_eyeWidth, m_eyeHeight);
   QRect targetLeft(0, 0, m_eyeWidth, m_eyeHeight);
   QOpenGLFramebufferObject::blitFramebuffer(m_resolveBuffer, targetLeft,
                                             m_leftBuffer, sourceRect);
}


void Scene::renderRight() {
   glEnable(GL_MULTISAMPLE);
   m_rightBuffer->bind();
   renderEye(m_phongShader);
   m_rightBuffer->release();

   QRect sourceRect(0, 0, m_eyeWidth, m_eyeHeight);
   QRect targetRight(m_eyeWidth, 0, m_eyeWidth, m_eyeHeight);
   QOpenGLFramebufferObject::blitFramebuffer(m_resolveBuffer, targetRight,
                                             m_rightBuffer, sourceRect);
}


void Scene::renderComp() {
   glClearColor(0.25f, 0.25f, 0.28f, 1.0f);
   glViewport(0, 0, m_width, m_height);
   glDisable(GL_MULTISAMPLE);
   renderEye(m_phongShader);
}


void *Scene::getResolveTexture() {
   return (void *)m_resolveBuffer->texture();
}


void Scene::nextSky() {
   m_currentSky = (m_currentSky + 1) % m_skyBoxes.length();
}


void Scene::activeController() {
   m_controllerObj->setActive(true);
}


void Scene::inactiveController() {
   m_controllerObj->setActive(false);
}


void Scene::updateController(glm::mat4x4& mat) {
   //  if (m_controllerObj->isActive()) {
   //std::cerr << glm::to_string(mat) << std::endl;
   m_controllerObj->setActive(true);
   m_controllerObj->setModelMatrix(mat);
   //}
}


void Scene::renderEye(QOpenGLShaderProgram& shader) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   if (settings.SAMPLES == 0) {
      glDisable(GL_MULTISAMPLE);
   } else{
      glEnable(GL_MULTISAMPLE);
   }
   m_vao.bind();
   shader.bind();
   //added passing model matrix and the controller disappeared
   shader.setUniformValue("pointLightPosition", m_pointLightPos);
   shader.setUniformValue("lightSpaceMatrix", m_lightSpaceMatrix);
   shader.setUniformValue("v", helper.mat4x4ToQMatrix4x4(m_viewMat));
   shader.setUniformValue("p", helper.mat4x4ToQMatrix4x4(m_projectMat));
   shader.setUniformValue("lightOn", settings.lightOn);
   shader.setUniformValue("shadowOn", settings.shadowOn);

   for (auto obj : m_sceneObjs) {
      obj->draw(shader, m_glTextMap);
   }
   if (m_controllerObj->isActive()) {
      m_controllerObj->draw(shader, m_glTextMap);
   }
   m_skyBoxes[m_currentSky]->draw(shader, m_glTextMap);
//   m_shader.release();   // FM+ :
//   m_vao.release();      // FM+ :
}
