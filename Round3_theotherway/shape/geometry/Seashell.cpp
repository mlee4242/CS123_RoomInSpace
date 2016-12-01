#include "Seashell.h"

#include <iostream>
#include <shape/surfaces/SeashellFace.h>

Seashell::Seashell(Paramater para, const std::string name)
   : Shape(para, name) {}


Seashell::Seashell(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


Seashell::~Seashell() {}

void Seashell::initFaces() {
   // the seashell face, I rotate it a little bit
   glm::mat4 mat = glm::translate(glm::vec3(0, m_para.radius, 0)) *
                   glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));

   // if need to transform this seashell as a whole
   if (m_ifTrans) { mat = m_transMat * mat; }

   // the front side
   m_faces.push_back(std::shared_ptr<SeashellFace>(new SeashellFace(m_para, m_name, mat)));

   // the back side
   m_faces.push_back(std::shared_ptr<SeashellFace>(new SeashellFace(m_para, m_name + "Flip", mat)));

}
