#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


Rectangle::Rectangle(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


Rectangle::Rectangle(const Rectangle& r) : Surface(r) {
   m_para     = r.m_para;
   m_name     = r.m_name;
   m_transMat = r.m_transMat;
   m_ifTrans  = r.m_ifTrans;
   m_vertices = r.m_vertices;
}


Rectangle::~Rectangle() {}

/**
 * @brief Rectangle::computeVertices
 * compute the rectangle on XZ plane at the center
 */
void Rectangle::computeVertices() {
   float step = 2 * m_para.radius / m_para.p1;
   int   p1   = m_para.p1;
   float r    = m_para.radius;

   for (int cp1 = 0; cp1 < p1; cp1++) {
      for (int cp2 = 0; cp2 <= p1; cp2++) {
         float x1, x2, y1, y2, z1, z2, xn, yn, zn;
         x1 = -1 * r + cp2 * step, // vertex 1
         y1 = -1 * r + cp1 * step,
         z1 = 0;
         x2 = -1 * r + cp2 * step, // vertex 2
         y2 = -1 * r + (cp1 + 1) * step,
         z2 = 0,
         xn = 0, yn = 0, zn = 1;   // normal
         // pre-store a vertex if it is at the beginning
         if (cp2 == 0) {
            push(glm::vec4(x1, y1, z1, 1), glm::vec4(xn, yn, zn, 0));
         }
         // save vertices and their normals
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(xn, yn, zn, 0),
              glm::vec4(x2, y2, z2, 1), glm::vec4(xn, yn, zn, 0));

         // store a vertex again if it is at the end
         if (cp2 == p1) {
            push(glm::vec4(x2, y2, z2, 1), glm::vec4(xn, yn, zn, 0));
         }
      }
   }
}
