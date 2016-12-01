#include "Barrel.h"
#include "gl/util.h"
#include <iostream>
#include <math.h>

Barrel::Barrel(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


Barrel::Barrel(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
Barrel::Barrel(const Barrel& b) : Surface(b) {
   m_para     = b.m_para;
   m_name     = b.m_name;
   m_transMat = b.m_transMat;
   m_ifTrans  = b.m_ifTrans;
   m_vertices = b.m_vertices;
}


Barrel::~Barrel() {}

/**
 * @brief Barrel::computeVertices computer vertices on this barrel
 * The equation of a cylinder barrel is
 * x = r cos theta
 * y = r sin theta
 * z = a
 * a in [0, h], theta in {0, 2Pi]
 */
void Barrel::computeVertices() {
   int p1 = m_para.p1;

   // limit the minimum of p2 to 3, otherwise the cylinder may lose the general shape
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float step_p1 = m_para.height / p1;
   float step_p2 = 2.0 * PI / p2;
   float r = m_para.radius, h = m_para.height;

   for (int cp1 = 1; cp1 <= p1; cp1++) {      // current position given the parameter p1
      for (int cp2 = 0; cp2 <= p2; cp2++) {   // current position given the parameter p2
         float cheight = step_p1 * cp1,       // the height used in current point
               nheight = step_p1 * (cp1 - 1), // the height used in next point
               theta   = step_p2 * (cp2);

         // vertex 1
         float z1 = r * cos(theta),
               y1 = -0.5 * h + cheight,
               x1 = r * sin(theta);

         // vertex 2
         float z2 = r * cos(theta),
               y2 = -0.5 * h + nheight,
               x2 = r * sin(theta);

         // normals
         float x1n = x1, y1n = 0, z1n = z1,
               x2n = x2, y2n = 0, z2n = z2;

         // push these two vertices and their normals
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(x1n, y1n, z1n, 0),
              glm::vec4(x2, y2, z2, 1), glm::vec4(x2n, y2n, z2n, 0));
      }
   }
}
