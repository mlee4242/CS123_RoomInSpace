#include "KleinBottle.h"

#include <iostream>
#include <shape/surfaces/KleinBottleFace.h>

KleinBottle::KleinBottle(Paramater para, const std::string name)
   : Shape(para, name) {}


KleinBottle::KleinBottle(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;   // it has been initialized. this is to change the value.
}


KleinBottle::~KleinBottle() {}


void KleinBottle::initFaces() {
   glm::mat4 transMat = glm::translate(glm::vec3(0, -4 * m_para.radius, 0));
   glm::mat4 scaleMat = glm::scale(glm::vec3(0.5, 0.5, 0.5));
   transMat = scaleMat * transMat;
   // if we need to transform the whole cylinder
   if (m_ifTrans) {
      transMat = m_transMat * transMat;
   }
   // the front side
   m_faces.push_back(std::shared_ptr<KleinBottleFace>(new KleinBottleFace(m_para, m_name, transMat)));

   // the back side
   m_faces.push_back(std::shared_ptr<KleinBottleFace>(new KleinBottleFace(m_para, m_name + " Flip", transMat)));
}
