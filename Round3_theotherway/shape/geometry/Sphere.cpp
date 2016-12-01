#include "Sphere.h"
#include <memory>
#include <iostream>
#include <shape/surfaces/SphereFace.h>
Sphere::Sphere(Paramater para, const std::string name)
   : Shape(para, name) {}


Sphere::Sphere(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


Sphere::~Sphere() {}

void Sphere::initFaces() {
   if (!m_ifTrans) {
      // if not need to transform the sphere as a whole
      m_faces.push_back(std::shared_ptr<SphereFace>(new SphereFace(m_para, "Sphere")));
   } else{
      m_faces.push_back(std::shared_ptr<SphereFace>(new SphereFace(m_para, "Sphere", m_transMat)));
   }
}
