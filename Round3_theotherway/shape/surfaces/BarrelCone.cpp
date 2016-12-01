#include "BarrelCone.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

BarrelCone::BarrelCone(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


BarrelCone::BarrelCone(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
BarrelCone::BarrelCone(const BarrelCone& bc) : Surface(bc) {
   m_para     = bc.m_para;
   m_name     = bc.m_name;
   m_transMat = bc.m_transMat;
   m_ifTrans  = bc.m_ifTrans;
   m_vertices = bc.m_vertices;
}


BarrelCone::~BarrelCone() {}

/**
 * @brief BarrelCone::computeVertices
 * The equation of a cone based on XZ plane is
 * x = (h - u) / h * r * cos theta
 * z = (h - u) / h * r * sin theta
 * y = u
 * u in [0, h], theta in [0, 2Pi)
 */
void BarrelCone::computeVertices() {
   int p1 = m_para.p1;

   // limit the minimum of p2 to 3, otherwise the barrel may lose the general shape
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float r = m_para.radius, h = m_para.height;

   float step_p1 = m_para.height / p1;
   float step_p2 = 2.0 * PI / p2;

   for (int cp1 = 1; cp1 <= p1; cp1++) {                               // current position given the parameter p1
      for (int cp2 = 0; cp2 <= p2; cp2++) {                            // current position given the parameter p2
         float cheight = step_p1 * cp1, nheight = step_p1 * (cp1 - 1), //↓ from top to down
               theta = step_p2 * cp2;
         float x1    = r * (h - cheight) / h * sin(theta),             // top vertex
               y1    = -0.5 * h + cheight,                             // move down a little bit
               z1    = r * (h - cheight) / h * cos(theta);

         float x2 = r * (h - nheight) / h * sin(theta),                // bottom vertex
               y2 = -0.5 * h + nheight,
               z2 = r * (h - nheight) / h * cos(theta);

         // Hack the normals at the tip by using the previous ones. They are the same
         if (cp1 == p1) {
            nheight -= step_p1;
            cheight -= step_p1;
         }
         float z1u = (r / h) * -1 * cos(theta),                 // top vertex
               y1u = 1,                                         // move down a little bit
               x1u = (r / h) *  -1 * sin(theta);

         float z2u = (r / h) * -1 * cos(theta),                 // bottom vertex
               y2u = 1,
               x2u = (r / h) * -1 * sin(theta);

         float z1v = (r / h) * -1 * (h - cheight) * sin(theta), // top vertex
               y1v = 0,                                         // move down a little bit
               x1v = (r / h) * (h - cheight) * cos(theta);

         float z2v = (r / h) * -1 * (h - nheight) * sin(theta),  // bottom vertex
               y2v = 0,
               x2v = (r / h) * (h - nheight) * cos(theta);

         // normal of vertex 1 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
         glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
         glm::vec3 n1 = glm::cross(v2, v1);

         // normal of vertex 2 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
         glm::vec3 v3 = glm::vec3(x2u, y2u, z2u), v4 = glm::vec3(x2v, y2v, z2v);
         glm::vec3 n2 = glm::cross(v4, v3);


         // save the two vertices
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(n1, 0),
              glm::vec4(x2, y2, z2, 1), glm::vec4(n2, 0));
      }
   }
}
