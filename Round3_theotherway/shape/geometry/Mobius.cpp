#include "Mobius.h"

#include <iostream>
#include <shape/surfaces/MobiusFace.h>

Mobius::Mobius(Paramater para, const std::string name)
   : Shape(para, name) {}


Mobius::Mobius(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


Mobius::~Mobius() {}

void Mobius::initFaces() {
   // the first mobius. I scale it a little bit
   glm::mat4 mat1 = glm::translate(glm::vec3(0, -0.5 * m_para.radius, 0)) *
                    glm::rotate(glm::radians(45.0f), glm::vec3(1, 0, 0));

   // if we need to transform these two mobius strips as a whole, transform the first one
   if (m_ifTrans) { mat1 = m_transMat * mat1; }

   // the front side
   m_faces.push_back(std::shared_ptr<MobiusFace>(new MobiusFace(m_para, m_name, mat1)));

   // the back side
   m_faces.push_back(std::shared_ptr<MobiusFace>(new MobiusFace(m_para, m_name + " Flip 1", mat1)));

   // the second mobius. I scale it a little bit
   glm::mat4 mat2 = glm::translate(glm::vec3(m_para.height * 0.75, -0.5 * m_para.radius, 0)) *
                    glm::rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));

   // if we need to transform these two mobius strips as a whole, transform the second one
   if (m_ifTrans) { mat2 = m_transMat * mat2; }

   // the front side
   m_faces.push_back(std::shared_ptr<MobiusFace>(new MobiusFace(m_para, m_name, mat2)));

   // the back side
   m_faces.push_back(std::shared_ptr<MobiusFace>(new MobiusFace(m_para, m_name + " Flip 2", mat2)));
}
