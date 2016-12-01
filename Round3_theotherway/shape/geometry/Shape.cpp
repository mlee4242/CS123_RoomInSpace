#include "shape/geometry/Shape.h"
#include <iostream>
#include <memory.h>
using namespace CS123::GL;

/**
 * @brief Shape::Shape, constructor function without transformation
 * @param para the parameters setting of this shape, such as radius, width, p1 etc
 * @param name
 */
Shape::Shape(Paramater para, const std::string name) :
   m_para(para),
   m_name(name),
   m_faces({}),
   m_transMat(glm::mat4()),
   m_ifTrans(false),
   m_ifFacesInited(false)
{}

/**
 * @brief Shape::Shape, constructor function with a transformation
 * @param para the parameters setting of this shape, such as radius, width, p1 etc
 * @param name
 * @param mat the transformation matrix
 */
Shape::Shape(Paramater para, const std::string name, const glm::mat4& mat) :
   m_para(para),
   m_name(name),
   m_faces({}),
   m_transMat(mat),
   m_ifTrans(true),
   m_ifFacesInited(false)
{}

Shape& Shape::operator=(const Shape& other) {
   return *this;
}

/**
 * @brief Shape::draw
 * Draw the shape simply by drawing all the surfaces it contains
 * The function is called by a scene object
 */
void Shape::draw() {
   for (int i = 0; i < m_faces.size(); i++) {
      m_faces[i]->draw();
   }
}


void Shape::sentToVAO(bool nofill) {
   for (int i = 0; i < m_faces.size(); i++) {
      m_faces[i]->sentToVAO(nofill);
   }
}


/**
 * @brief Shape::setParameters
 * @param para
 * change the parameters setting of the shape by passing it to all the surfaces
 */
void Shape::setParameters(Paramater para) {
   m_para = para;
   if (!m_ifFacesInited) {
      initFaces();
      m_ifFacesInited = true;
   }
   for (int i = 0; i < m_faces.size(); i++) {
      m_faces[i]->setParameters(m_para);
   }
}


/**
 * @brief Shape::getData
 * @return all the surfaces that the shape contains
 */
std::vector<std::shared_ptr<Surface> > Shape::getData() const {
   return m_faces;
}
