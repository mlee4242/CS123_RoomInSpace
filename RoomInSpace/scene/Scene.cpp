#include "Scene.h"
#include "ui/modelformats.h"
#include "glm/ext.hpp"

Scene::Scene() : m_vertCount(0), m_texture(0),
   m_viewMat(glm::mat4x4()), m_projectMat(glm::mat4x4()), m_modelMat(glm::mat4x4()),
   m_eyeWidth(0), m_eyeHeight(0), m_leftBuffer(0), m_rightBuffer(0)
{}


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
}


void Scene::initScene() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);

   // compile our shader
   compileShader(m_shader, ":/shaders/phong.vert", ":/shaders/phong.frag");

   // build out sample geometry
   m_vao.create();
   m_vao.bind();

   QVector<GLfloat> points = readObj(":/models/teapot/2teapot.obj");
   m_vertCount = points.length();
   qDebug() << "loaded" << m_vertCount << "verts";

   m_vertexBuffer.create();
   m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
   m_vertexBuffer.bind();

   m_vertexBuffer.allocate(points.data(), points.length() * sizeof(GLfloat));

   m_shader.bind();

   m_shader.setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
   m_shader.enableAttributeArray("vertex");

   m_shader.setAttributeBuffer("texCoord", GL_FLOAT, 3 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));
   m_shader.enableAttributeArray("texCoord");

   m_shader.setAttributeBuffer("normal", GL_FLOAT, 5 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
   m_shader.enableAttributeArray("normal");

   m_shader.setUniformValue("diffuse", 0);

   m_shader.release(); // FM :+
   m_vao.release();    // FM :+

   m_texture = new QOpenGLTexture(QImage(":/textures/uvmap.png"));
}


void Scene::initVRScene() {
   QOpenGLFramebufferObjectFormat buffFormat;
   buffFormat.setAttachment(QOpenGLFramebufferObject::Depth);
   buffFormat.setInternalTextureFormat(GL_RGBA8);
   buffFormat.setSamples(4);

   m_leftBuffer  = new QOpenGLFramebufferObject(m_eyeWidth, m_eyeHeight, buffFormat);
   m_rightBuffer = new QOpenGLFramebufferObject(m_eyeWidth, m_eyeHeight, buffFormat);

   QOpenGLFramebufferObjectFormat resolveFormat;
   resolveFormat.setInternalTextureFormat(GL_RGBA8);
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


void Scene::renderLeft() {
   glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
   glViewport(0, 0, m_eyeWidth, m_eyeHeight);

   glEnable(GL_MULTISAMPLE);
   m_leftBuffer->bind();
   renderEye(vr::Eye_Left);
   m_leftBuffer->release();
}


void Scene::renderRight() {
   QRect sourceRect(0, 0, m_eyeWidth, m_eyeHeight);
   QRect targetLeft(0, 0, m_eyeWidth, m_eyeHeight);
   QOpenGLFramebufferObject::blitFramebuffer(m_resolveBuffer, targetLeft,
                                             m_leftBuffer, sourceRect);

   glEnable(GL_MULTISAMPLE);
   m_rightBuffer->bind();
   renderEye(vr::Eye_Right);
   m_rightBuffer->release();
   QRect targetRight(m_eyeWidth, 0, m_eyeWidth, m_eyeHeight);
   QOpenGLFramebufferObject::blitFramebuffer(m_resolveBuffer, targetRight,
                                             m_rightBuffer, sourceRect);
}


void Scene::renderComp() {
   glClearColor(0.25f, 0.25f, 0.28f, 1.0f);
   glViewport(0, 0, m_width, m_height);
   glDisable(GL_MULTISAMPLE);
   renderEye(vr::Eye_Right);
}


void *Scene::getResolveTexture() {
   return (void *)m_resolveBuffer->texture();
}


void Scene::renderEye(vr::Hmd_Eye eye) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   m_vao.bind();
   m_shader.bind();
   m_texture->bind(0);

   m_shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
   m_shader.setUniformValue("v", helper.mat4x4ToQMatrix4x4(m_viewMat));
   m_shader.setUniformValue("p", helper.mat4x4ToQMatrix4x4(m_projectMat));
   m_shader.setUniformValue("leftEye", eye == vr::Eye_Left);
   m_shader.setUniformValue("overUnder", settings.windowMode == OverUnder);
   glDrawArrays(GL_TRIANGLES, 0, m_vertCount);
   m_shader.release(); // FM+ :
   m_vao.release();    // FM+ :
}
