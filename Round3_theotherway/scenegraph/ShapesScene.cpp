#include "ShapesScene.h"
#include "Camera.h"
#include <SupportCanvas3D.h>
#include <QFileDialog>
#include "shape/OpenGLShape.h"
#include "gl/util.h"
#include <sstream>
#include "gl/util/spheretest.h"
#include <iostream>
using namespace CS123::GL;
#include "gl/shaders/CS123Shader.h"
#include "gl/shaders/Shader.h"

#include "ResourceLoader.h"

glm::vec4 lightDirection = glm::normalize(glm::vec4(1.f, -1.f, -1.f, 0.f));

ShapesScene::ShapesScene(int width, int height) :
   m_width(width),
   m_height(height),
   m_para(Paramater()),
   m_shape(nullptr),
   m_axes(std::make_unique<OpenGLShape>()),
   m_shapeList({}),
   m_dataAxes(AXES_DATA),
   m_shapeIsReady(false) {

   initializeSceneMaterial();
   initializeSceneLight();
   loadPhongShader();
   loadWireframeShader();
   loadNormalsShader();
   loadNormalsArrowShader();

   initializeAxes();
   initShapes();

   settings.settingChanged = true;
   //TODO: [SHAPES] Allocate any additional memory you need...
}


ShapesScene::~ShapesScene() {
   // Pro-tip: If you use smart pointers properly, this destructor should be empty
}


void ShapesScene::initializeSceneMaterial() {
   // Use a shiny orange material
   m_material.clear();
   m_material.cAmbient.r  = 0.2f;
   m_material.cAmbient.g  = 0.1f;
   m_material.cDiffuse.r  = 1.0f;
   m_material.cDiffuse.g  = 0.5f;
   m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
   m_material.shininess   = 64;
}


void ShapesScene::initializeSceneLight() {
   // Use a white directional light from the upper left corner
   memset(&m_light, 0, sizeof(m_light));
   m_light.type    = LightType::LIGHT_DIRECTIONAL;
   m_light.dir     = lightDirection;
   m_light.color.r = m_light.color.g = m_light.color.b = 1;
   m_light.id      = 0;
}


void ShapesScene::loadPhongShader() {
   std::string vertexSource   = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
   std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
   m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}


void ShapesScene::loadWireframeShader() {
   std::string vertexSource   = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
   std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
   m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}


void ShapesScene::loadNormalsShader() {
   std::string vertexSource   = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
   std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
   std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
   m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}


void ShapesScene::loadNormalsArrowShader() {
   std::string vertexSource   = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
   std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
   std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
   m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}


void ShapesScene::render(SupportCanvas3D *context) {
   // Clear the screen in preparation for the next frame. (Use a gray background instead of a
   // black one for drawing wireframe or normals so they will show up against the background.)
   setClearColor();

   renderPhongPass(context);

   if (settings.drawWireframe) {
      renderWireframePass(context);
   }

   if (settings.drawNormals) {
      renderNormalsPass(context);
   }
}

void ShapesScene::renderPhongPass(SupportCanvas3D *context) {
   m_phongShader->bind();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   clearLights();
   setLights(context->getCamera()->getViewMatrix());
   setPhongSceneUniforms();
   setMatrixUniforms(m_phongShader.get(), context);
   renderGeometryAsFilledPolygons();

   m_phongShader->unbind();
}


void ShapesScene::setPhongSceneUniforms() {
   m_phongShader->setUniform("useLighting", settings.useLighting);
   m_phongShader->setUniform("useArrowOffsets", false);
   m_phongShader->applyMaterial(m_material);
}


void ShapesScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
   shader->setUniform("p", context->getCamera()->getProjectionMatrix());
   shader->setUniform("v", context->getCamera()->getViewMatrix());
   shader->setUniform("m", glm::mat4(1.0f));
}


void ShapesScene::renderGeometryAsFilledPolygons() {
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   renderGeometry();
}


void ShapesScene::renderWireframePass(SupportCanvas3D *context) {
   m_wireframeShader->bind();
   setMatrixUniforms(m_wireframeShader.get(), context);
   renderGeometryAsWireframe();
   m_wireframeShader->unbind();
}


void ShapesScene::renderGeometryAsWireframe() {
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   renderGeometry();
}


void ShapesScene::renderNormalsPass(SupportCanvas3D *context) {
   // Render the lines.
   m_normalsShader->bind();
   setMatrixUniforms(m_normalsShader.get(), context);
   renderGeometryAsWireframe();
   m_normalsShader->unbind();

   // Render the arrows.
   m_normalsArrowShader->bind();
   setMatrixUniforms(m_normalsArrowShader.get(), context);
   renderGeometryAsFilledPolygons();
   m_normalsArrowShader->unbind();
}

/**
 * @brief ShapesScene::initShapes
 * Initialize the shape list to contain a list of shape objects
 */
void ShapesScene::initShapes() {
   m_shapeList["Cube"]        = std::make_shared<Cube>(m_para, "Cube");
   m_shapeList["Sphere"]      = std::make_shared<Sphere>(m_para, "Sphere");
   m_shapeList["Cylinder"]    = std::make_shared<Cylinder>(m_para, "Cylinder");
   m_shapeList["Cone"]        = std::make_shared<Cone>(m_para, "Cone");
   m_shapeList["Torus"]       = std::make_shared<Torus>(m_para, "Torus");
   m_shapeList["Mobius"]      = std::make_shared<Mobius>(m_para, "Mobius");
   m_shapeList["KleinBottle"] = std::make_shared<KleinBottle>(m_para, "Klein Bottle");
   m_shapeList["Figure8"]     = std::make_shared<KleinBottle>(m_para, "Figure 8");
   m_shapeList["Sierpinski"]  = std::make_shared<SierpinskiCube>(m_para, "Sierpinski Cube");
   m_shapeList["Seashell"]    = std::make_shared<Seashell>(m_para, "Seashell");
}

/**
 * @brief ShapesScene::updateShape
 * update the shape or the current shape type
 */
void ShapesScene::updateShape() {
   if ((m_para.shapeType == settings.shapeType) && (m_para.p1 == settings.shapeParameter1) &&
       (m_para.p2 == settings.shapeParameter2) && (std::abs(m_para.p3 - settings.shapeParameter3) < 0.1) &&
       (m_para.noFillShape == settings.noFillShape)) { // if no parameter nor fill flag has changed
      return;
   } else if ((m_para.shapeType == settings.shapeType) && (m_para.p1 == settings.shapeParameter1) &&
              (m_para.p2 == settings.shapeParameter2) && (std::abs(m_para.p3 - settings.shapeParameter3) < 0.1) &&
              (m_para.noFillShape != settings.noFillShape)) { // if only fill flag has changed
      m_para.noFillShape = settings.noFillShape;
      m_shape->sentToVAO(m_para.noFillShape);
      return;
   } // check if need to recompute (they might choose light, axes etc)


   if (m_para.shapeType != settings.shapeType) {
      switch (settings.shapeType)
      {
      case ShapeType::SHAPE_CUBE:
         m_shape        = m_shapeList.at("Cube");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_CYLINDER:
         m_shape        = m_shapeList.at("Cylinder");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_CONE:
         m_shape        = m_shapeList.at("Cone");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_SPHERE:
         m_shape        = m_shapeList.at("Sphere");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_TORUS:
         m_shape        = m_shapeList.at("Torus");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_MOBIUS:
         m_shape        = m_shapeList.at("Mobius");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_KLEIN:
         m_shape        = m_shapeList.at("KleinBottle");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_FIGURE8:
         m_shape        = m_shapeList.at("Figure8");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_SEASHELL:
         m_shape        = m_shapeList.at("Seashell");
         m_shapeIsReady = true;
         break;

      case ShapeType::SHAPE_SIERPINSKI:
         m_shape        = m_shapeList.at("Sierpinski");
         m_shapeIsReady = true;
         break;

      default:
         std::cout << "Shape " << settings.shapeType << " not find" << std::endl;
         m_shapeIsReady = false;
         break;
      }
   }

   // if reaches here, it means parameter 1 | 2 | 3 have been changed.
   m_para.p1          = settings.shapeParameter1;
   m_para.p2          = settings.shapeParameter2;
   m_para.p3          = settings.shapeParameter3;
   m_para.shapeType   = settings.shapeType;
   m_para.height      = 1;//sqrt(settings.shapeParameter3);
   m_para.radius      = 0.5 * m_para.height;
   m_para.noFillShape = settings.noFillShape;

   m_shape->setParameters(m_para);
   m_shape->sentToVAO(m_para.noFillShape);
   m_para.shapeType = settings.shapeType;
}


void ShapesScene::initializeAxes() {
   m_axes->setVertexData(m_dataAxes.data(), m_dataAxes.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINES, m_dataAxes.size() / 2);
   m_axes->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
   m_axes->buildVAO();
}


void ShapesScene::renderGeometry() {
   if (settings.drawAxes) {
      m_axes->drawGeo();
   }

   if (settings.settingChanged) {
      updateShape();
      settings.settingChanged = false;
   }
   if (m_shapeIsReady) {
      m_shape->draw();
   }
}

void ShapesScene::clearLights() {
   for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
      std::ostringstream os;
      os << i;
      std::string indexString = "[" + os.str() + "]";   // e.g. [0], [1], etc.
      m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
   }
}


void ShapesScene::setLights(const glm::mat4 viewMatrix) {
   // YOU DON'T NEED TO TOUCH THIS METHOD, unless you want to do fancy lighting...

   m_light.dir = glm::inverse(viewMatrix) * lightDirection;

   clearLights();
   m_phongShader->setLight(m_light);
}
