#include "Cone.h"

#include <iostream>
#include <shape/surfaces/BarrelCone.h>

/**
 * @brief Cone::Cone
 * @param para the parameters setting of the cone
 * @param name
 */
Cone::Cone(Paramater para, const std::string name)
   : Shape(para, name) {}


Cone::~Cone() {}


/**
 * @brief Cone::Cone
 * @param para
 * @param name
 * @param mat the transformation matrix if we need to transform the cone
 */

Cone::Cone(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true; // it has been initialized. this is to change the value.
}


void Cone::initFaces() {
   if (!m_ifTrans) {
      // transform the base to a lower position
      glm::mat4 mat = glm::translate(glm::vec3(0, -0.5 * m_para.height, 0));

      // the base
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_bottom", mat)));

      // the barrel of the cone
      m_faces.push_back(std::shared_ptr<BarrelCone>(new BarrelCone(m_para, "BarrelCone")));
   }else{
      // transform the base to a lower position
      glm::mat4 mat = glm::translate(glm::vec3(0, -0.5 * m_para.height, 0));

      // the base
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_bottom", m_transMat * mat)));

      // the barrel of the cone
      m_faces.push_back(std::shared_ptr<BarrelCone>(new BarrelCone(m_para, "BarrelCone", m_transMat)));
   }
}
