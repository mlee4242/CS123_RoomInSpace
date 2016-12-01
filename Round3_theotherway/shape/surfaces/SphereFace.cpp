#include "SphereFace.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

SphereFace::SphereFace(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


SphereFace::SphereFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


SphereFace::SphereFace(const SphereFace& sf) : Surface(sf) {
   m_para     = sf.m_para;
   m_name     = sf.m_name;
   m_transMat = sf.m_transMat;
   m_ifTrans  = sf.m_ifTrans;
   m_vertices = sf.m_vertices;
}


SphereFace::~SphereFace() {}

/**
 * @brief SphereFace::computeVertices
 * The equation of a sphere is
 * x = r * cos theta * sin phi
 * y = r * sin theta * sin phi
 * z = r * cos phi
 */
void SphereFace::computeVertices() {
   // limit the minimum of p1 p2 to 2 3 respectively, otherwise the sphere may lose its general shape
   int p1 = m_para.p1 < 2 ? 2 : m_para.p1;
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float r = m_para.radius;

   float step_p1 = PI / p1;         // "height", where the axis is
   float step_p2 = 2.0 * PI / p2;   // the position on the circle

   for (int cp1 = 0; cp1 < p1; cp1++) {
      for (int cp2 = 0; cp2 <= p2; cp2++) {
         // the sphere is drawn a bar by a bar, both theta of "height" and "position" should change
         float ctheta = step_p2 * cp2, ntheta = step_p2 * cp2,
               cphi = step_p1 * cp1, nphi = step_p1 * (cp1 + 1);

         // vertex 1
         float z1 = r * cos(ctheta) * sin(cphi),
               y1 = r * cos(cphi),
               x1 = r * sin(ctheta) * sin(cphi);

         // vertex 2
         float z2 = r * cos(ntheta) * sin(nphi),
               y2 = r * cos(nphi),
               x2 = r * sin(ntheta) * sin(nphi);

         float x1n = x1, y1n = y1, z1n = z1,
               x2n = x2, y2n = y2, z2n = z2;

         // save vertices and their normals
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(x1n, y1n, z1n, 0),
              glm::vec4(x2, y2, z2, 1), glm::vec4(x2n, y2n, z2n, 0));
      }
   }
}
