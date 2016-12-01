#include "Torus.h"

#include <iostream>
#include <shape/surfaces/TorusFace.h>

Torus::Torus(Paramater para, const std::string name)
   : Shape(para, name) {}


Torus::Torus(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


Torus::~Torus() {}

void Torus::initFaces() {
   if (!m_ifTrans) {
      // if not need to transform the sphere as a whole
      m_faces.push_back(std::shared_ptr<TorusFace>(new TorusFace(m_para, "Torus")));
   } else {
      m_faces.push_back(std::shared_ptr<TorusFace>(new TorusFace(m_para, "Torus", m_transMat)));
   }
}
