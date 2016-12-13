#include <gl/glew.h>
#include <gl/glu.h>
#include <QDir>
#include "Scene.h"
#include "glm/ext.hpp"
#include "objmodeler/SceneObject.h"
#include "objmodeler/GroupObject.h"
#include "objmodeler/PrimitiveObject.h"
#include "objmodeler/delete_ptr.h"
#include "Settings.h"
#include <gl/glew.h>
#define LIGHT_NEAR_CLIP    0.01f
#define LIGHT_FAR_CLIP     5.5f
Scene::Scene() :
   m_glTextMap(QMap<QString, QOpenGLTexture *>()),
   m_viewMat(glm::mat4x4()),
   m_projectMat(glm::mat4x4()),
   m_modelMat(glm::mat4x4()),
   m_eyeWidth(0), m_eyeHeight(0),
   m_leftBuffer(0), m_rightBuffer(0),
   m_objLoader(new ObjLoader),
   m_depthMap(0),
   m_sceneObjs(QVector<SceneObject *>()),
   m_skyBoxes(QVector<SceneObject *>()),
   m_currentSky(0),
   m_controllerObj(nullptr) {
   m_lightProjection = glm::ortho(-2.5f, 2.5f, -2.5f, 2.5f,
                                  LIGHT_NEAR_CLIP, LIGHT_FAR_CLIP);
   m_lightView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.75f),
                             glm::vec3(0.0f, 0.f, 0.f),
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
   if (m_sceneObjs.size() > 0) {
      for (auto obj : m_sceneObjs) {
         if (obj->getObjectType() == PRIMITIVE_OBJECT) {
            delete static_cast<PrimitiveObject *> (obj);
         }
         if (obj->getObjectType() == GROUP_OBJECT) {
            delete static_cast<GroupObject *> (obj);
         }
         if (obj->getObjectType() == SCENE_OBJECT) {
            delete (obj);
         }
      }
   }
   if (m_glTextMap.size() > 0) {
      QMap<QString, QOpenGLTexture *>::iterator i;
      for (i = m_glTextMap.begin(); i != m_glTextMap.end(); i++) {
         QOpenGLTexture *ptr = i.value();
         delete ptr;
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
      std::cout << obj->getName().toStdString() << std::endl;
      if (obj->getName().contains("Sky")) {
         obj->setActive(false);
         m_skyBoxes.push_front(obj);
      }else if (obj->getName().contains("Controller")) {
         obj->setActive(false);
         m_controllerObj.reset(obj);
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


void Scene::pickUp(bool& pickStatus, glm::mat4x4& mat) {
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
         m_pickedObj.reset(obj);
         m_pickedObj->setReferenceMatrx(mat);

         std::cout << "this is reference matrix" << std::endl;
         std::cout << glm::to_string(mat) << std::endl;
         m_pickedObj->updateModelMatrixFromReference(mat);
         m_pickedObj->setIsPicked(true);
         pickStatus = true;
         break;
      }
   }
}


void Scene::putDown(bool& pickStatus) {
   m_pickedObj->resetModelMatrix();
   m_pickedObj->resetReferenceMatrx();
   m_pickedObj->setIsPicked(false);
   //m_pickedObj.reset();
   pickStatus = false;
}


void Scene::updatePickedObjPos(glm::mat4x4& mat) {
   m_pickedObj->updateModelMatrixFromReference(mat);
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

   m_phongShader.setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("vertex");

   m_phongShader.setAttributeBuffer("texCoord", GL_FLOAT, 3 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("texCoord");

   m_phongShader.setAttributeBuffer("normal", GL_FLOAT, 5 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
   m_phongShader.enableAttributeArray("normal");

//   m_shader.release(); // FM :+
//   m_vao.release();    // FM :+
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
   depthBuffFormat.setAttachment(QOpenGLFramebufferObject::Depth);
   depthBuffFormat.setInternalTextureFormat(GL_RGBA8);
   depthBuffFormat.setSamples(settings.SAMPLES);
   m_shadowMapBuffer = new QOpenGLFramebufferObject(1024, 1024, depthBuffFormat);
}


void Scene::renderShawdowMap(vr::Hmd_Eye eye) {
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glEnable(GL_MULTISAMPLE);
   glViewport(0, 0, m_width, m_height);
   m_shadowMapBuffer->bind();
   m_shadowShader.bind();
   renderEye(eye, m_shadowShader);
   m_shadowMapBuffer->release();
   if (m_depthMap) {
      delete m_depthMap;
   }
   m_depthMap = new QOpenGLTexture(m_shadowMapBuffer->toImage());
   m_depthMap->setDepthStencilMode(QOpenGLTexture::DepthMode);
   m_shadowShader.release();
//   GLint id  = m_shadowMapBuffer->texture();
//   GLint loc = m_phongShader.attributeLocation("shadowMap");
   m_phongShader.bind();
//   glBindTexture(GL_TEXTURE_2D, id);
   m_phongShader.setUniformValue("shadowMap", 4);
   m_depthMap->bind(4);
}


void Scene::renderLeft() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glViewport(0, 0, m_eyeWidth, m_eyeHeight);

   glEnable(GL_MULTISAMPLE);
   m_leftBuffer->bind();
   renderEye(vr::Eye_Left, m_phongShader);
   m_leftBuffer->release();

   QRect sourceRect(0, 0, m_eyeWidth, m_eyeHeight);
   QRect targetLeft(0, 0, m_eyeWidth, m_eyeHeight);
   QOpenGLFramebufferObject::blitFramebuffer(m_resolveBuffer, targetLeft,
                                             m_leftBuffer, sourceRect);
}


void Scene::renderRight() {
   glEnable(GL_MULTISAMPLE);
   m_rightBuffer->bind();
   renderEye(vr::Eye_Right, m_phongShader);
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
   renderEye(vr::Eye_Right, m_phongShader);
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
   //  }
}


void Scene::renderEye(vr::Hmd_Eye eye, QOpenGLShaderProgram& shader) {
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
   //m_shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
   shader.setUniformValue("lightSpaceMatrix", m_lightSpaceMatrix);
   shader.setUniformValue("v", helper.mat4x4ToQMatrix4x4(m_viewMat));
   shader.setUniformValue("p", helper.mat4x4ToQMatrix4x4(m_projectMat));
   shader.setUniformValue("leftEye", eye == vr::Eye_Left);
   shader.setUniformValue("overUnder", settings.windowMode == OverUnder);
   shader.setUniformValue("light", settings.lightOn);

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
