#include "Circle.h"
#include "gl/util.h"
#include <iostream>
#include <math.h>

Circle::Circle(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


Circle::Circle(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
Circle::Circle(const Circle& r) : Surface(r) {
   m_para     = r.m_para;
   m_name     = r.m_name;
   m_transMat = r.m_transMat;
   m_ifTrans  = r.m_ifTrans;
   m_vertices = r.m_vertices;
}


Circle::~Circle() {}

/**
 * @brief Circle::computeVertices
 * the equation of a circle on XZ plane at center is
 * x = r * sin theta
 * y = 0
 * z = r * cos theta
 * x and z are swrapped here
 */
void Circle::computeVertices() {
   int p1 = m_para.p1;

   // limit the minimum of p2 to 3, otherwise the circle may lose the general shape
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float step_p1 = (m_para.radius + 0.0) / p1;
   float step_p2 = 2.0 * PI / p2;

   float xn = 0, yn = -1, zn = 0;           // normals

   for (int cp1 = 1; cp1 <= p1; cp1++) {    // current position given the parameter p1
      for (int cp2 = 0; cp2 <= p2; cp2++) { // current position given the parameter p2
         float cradius = step_p1 * cp1,     //↓ from top to down
               nradius = step_p1 * (cp1 - 1),
               theta   = step_p2 * cp2;
         float x1      = cradius * sin(theta),  // top vertex
               z1      = cradius * cos(theta),
               y1      = 0;
         float x2      = nradius * sin(theta),  // bottom vertex
               z2      = nradius * cos(theta),
               y2      = 0;

         // save vertices and their normals
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(xn, yn, zn, 0),
              glm::vec4(x2, y2, z2, 1), glm::vec4(xn, yn, zn, 0));
      }
   }
}
