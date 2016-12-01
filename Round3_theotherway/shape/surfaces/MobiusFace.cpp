#include "MobiusFace.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

MobiusFace::MobiusFace(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


MobiusFace::MobiusFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
MobiusFace::MobiusFace(const MobiusFace& mf) : Surface(mf) {
   m_para     = mf.m_para;
   m_name     = mf.m_name;
   m_transMat = mf.m_transMat;
   m_ifTrans  = mf.m_ifTrans;
   m_vertices = mf.m_vertices;
}


MobiusFace::~MobiusFace() {}

/**
 * @brief MobiusFace::computeVertices
 * The equation of a mobius is
 * x = (1 + s * cos u/2) * cos u
 * y = (1 + s * cos u/2) * sin u
 * z =  s  * sin u/2
 */
void MobiusFace::computeVertices() {
   int p1 = m_para.p1;
   // limit the minimum of p1 to 3, otherwise the mobius shape may lose its general shape
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float step_p1 = 2.0 * (m_para.radius + 0.0) / p1;
   float step_p2 = 2.0 * PI / p2;


   for (int cp1 = 0; cp1 < p1; cp1++) {
      for (int cp2 = 0; cp2 <= p2; cp2++) {
         float cv = step_p1 * cp1 - m_para.radius, cu = step_p2 * cp2,       // the top u, v paramater
               nv = step_p1 * (cp1 + 1) - m_para.radius, nu = step_p2 * cp2, // the bottom u, v paramater
               cs = cv / 2, ns = nv / 2, R = 1;
         // vertex 1
         float x1 = (R + cs * cos(cu / 2)) * cos(cu),
               y1 = (R + cs * cos(cu / 2)) * sin(cu),
               z1 = cs * sin(cu / 2);

         // vertex 2
         float x2 = (R + ns * cos(nu / 2)) * cos(nu),
               y2 = (R + ns * cos(nu / 2)) * sin(nu),
               z2 = ns * sin(nu / 2);

         // Partial derivative dx / du, dy / du, dz / du for vertex 1
         float x1u = -1.0 / 4.0 * cv * cos(cu) * sin(cu / 2) - (1 + 1.0 / 2.0 * cv * cos(cu / 2)) * sin(cu),
               y1u = (1 + 1.0 / 2.0 * cv * cos(cu / 2)) * cos(cu) - 1.0 / 4.0 * cv * sin(cu / 2) * sin(cu),
               z1u = 1.0 / 4.0 * cv * cos(cu / 2);

         // Partial derivative dx / dv, dy / dv, dz / dv for vertex 1
         float x1v = 1.0 / 2.0 * cos(cu / 2) * cos(cu),
               y1v = 1.0 / 2.0 * cos(cu / 2) * sin(cu),
               z1v = sin(cu / 2) * 0.5;

         // Partial derivative dx / du, dy / du, dz / du for vertex 2
         float x2u = -1.0 / 4.0 * nv * cos(nu) * sin(nu / 2) - (1 + 1.0 / 2.0 * nv * cos(nu / 2)) * sin(nu),
               y2u = (1 + 1.0 / 2.0 * nv * cos(nu / 2)) * cos(nu) - 1.0 / 4.0 * nv * sin(nu / 2) * sin(nu),
               z2u = nv / 2.0 * cos(nu / 2) * 0.5;

         // Partial derivative dx / dv, dy / dv, dz / dv for vertex 2
         float x2v = 1.0 / 2.0 * cos(nu / 2) * cos(nu),
               y2v = 1.0 / 2.0 * cos(nu / 2) * sin(nu),
               z2v = sin(nu / 2) * 0.5;

         glm::vec4 vertex1 = glm::vec4(x1, y1, z1, 1), vertex2 = glm::vec4(x2, y2, z2, 1);


         // normal of vertex 1 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv)
         glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
         glm::vec3 n1 = glm::cross(v1, v2);


         // normal of vertex 2 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv)
         glm::vec3 v3 = glm::vec3(x2u, y2u, z2u), v4 = glm::vec3(x2v, y2v, z2v);
         glm::vec3 n2 = glm::cross(v3, v4);

         // if this is the back side
         if (static_cast<int>(m_name.find("Flip")) > 0) {
            n1 *= -1;
            n2 *= -1;
         }else{
            std::swap(vertex1, vertex2);
            std::swap(n1, n2);
         }

         // save vertices and their normals
         push(vertex1, glm::vec4(n1, 0), vertex2, glm::vec4(n2, 0));
      }
   }
}
