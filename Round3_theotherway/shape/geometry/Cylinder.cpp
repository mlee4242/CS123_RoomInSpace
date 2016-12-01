#include "Cylinder.h"
#include <iostream>
#include <shape/surfaces/Barrel.h>

Cylinder::Cylinder(Paramater para, const std::string name)
   : Shape(para, name) {}


Cylinder::Cylinder(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


Cylinder::~Cylinder() {}


void Cylinder::initFaces() {
   // if we don't need to transform the whole cylinder
   if (!m_ifTrans) {
      // the bottom base circle of the cylinder by moving down a circle
      glm::mat4 mat1 = glm::translate(glm::vec3(0, -1 * m_para.radius, 0));
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_bottom", mat1)));

      // the top base circle of the cylinder by moving up a circle
      glm::mat4 mat2 = glm::translate(glm::vec3(0, m_para.radius, 0)) * glm::rotate(glm::radians(-180.0f), glm::vec3(0, 0, 1));
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_top", mat2)));

      // the barrel of the cylinder
      m_faces.push_back(std::shared_ptr<Barrel>(new Barrel(m_para, "Barrel")));
   }else{
      glm::mat4 mat1 = m_transMat * glm::translate(glm::vec3(0, -1 * m_para.radius, 0));
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_bottom", mat1)));

      glm::mat4 mat2 = m_transMat * glm::translate(glm::vec3(0, m_para.radius, 0))
                       * glm::rotate(glm::radians(-180.0f), glm::vec3(0, 0, 1));
      m_faces.push_back(std::shared_ptr<Circle>(new Circle(m_para, "base_top", mat2)));

      m_faces.push_back(std::shared_ptr<Barrel>(new Barrel(m_para, "Barrel", m_transMat)));
   }
}
