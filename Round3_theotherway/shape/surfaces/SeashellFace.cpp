#include "SeashellFace.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

SeashellFace::SeashellFace(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


SeashellFace::SeashellFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
SeashellFace::SeashellFace(const SeashellFace& sf) : Surface(sf) {
   m_para     = sf.m_para;
   m_name     = sf.m_name;
   m_transMat = sf.m_transMat;
   m_ifTrans  = sf.m_ifTrans;
   m_vertices = sf.m_vertices;
}


SeashellFace::~SeashellFace() {}

/**
 * @brief SeashellFace::computeVertices
 * The equation of a seashell is at
 * http://mathworld.wolfram.com/Seashell.html
 */
void SeashellFace::computeVertices() {
   // limit the minimum of p1 p2 to 3, otherwise the seashell may lose its general shape
   int p1 = m_para.p1 < 3 ? 3 : m_para.p1;
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float step_p1 = 2.0 * PI / p1;
   float step_p2 = 2.0 * PI / p2;

   for (int cp2 = 0; cp2 < p2; cp2++) {
      for (int cp1 = 0; cp1 <= p1; cp1++) {
         float cv = step_p1 * cp1, cu = step_p2 * cp2,
               nv = step_p1 * cp1, nu = step_p2 * (cp2 + 1);

         // vertex 1
         float x1 = 2 * (1 - exp(cu / (6 * PI))) * cos(cu) * cos(cv / 2) * cos(cv / 2),
               y1 = 2 * (-1 + exp(cu / (6 * PI))) * sin(cu) * cos(cv / 2) * cos(cv / 2),
               z1 = 1 - exp(cu / (3 * PI)) - sin(cv) + exp(cu / (6 * PI)) * sin(cv),

         // vertex 2
               x2 = 2 * (1 - exp(nu / (6 * PI))) * cos(nu) * cos(nv / 2) * cos(nv / 2),
               y2 = 2 * (-1 + exp(nu / (6 * PI))) * sin(nu) * cos(nv / 2) * cos(nv / 2),
               z2 = 1 - exp(nu / (3 * PI)) - sin(nv) + exp(nu / (6 * PI)) * sin(nv),

         // Partial derivative dx / du for vertex 1 and 2
               x1u = -1.0 / (3 * PI) * (exp(cu / (6 * PI)) * cos(cu) * cos(cv / 2) * cos(cv / 2)) -
                     2 * (1 - exp(cu / (6 * PI))) * cos(cv / 2) * cos(cv / 2) * sin(cu),
               x2u = -1.0 / (3 * PI) * (exp(nu / (6 * PI)) * cos(nu) * cos(nv / 2) * cos(nv / 2)) -
                     2 * (1 - exp(nu / (6 * PI))) * cos(nv / 2) * cos(nv / 2) * sin(nu),

         // Partial derivative dx / dv for vertex 1 and 2
               x1v = -2 * (1 - exp(cu / (6 * PI))) * cos(cu) * cos(cv / 2) * sin(cv / 2),
               x2v = -2 * (1 - exp(nu / (6 * PI))) * cos(nu) * cos(nv / 2) * sin(nv / 2),

         // Partial derivative dy / du for vertex 1 and 2
               y1u = 2 * (-1 + exp(cu / (6 * PI))) * cos(cu) * cos(cv / 2) * cos(cv / 2) +
                     1 / (3 * PI) * exp(cu / (6 * PI)) * cos(cv / 2) * cos(cv / 2) * sin(cu),
               y2u = 2 * (-1 + exp(nu / (6 * PI))) * cos(nu) * cos(nv / 2) * cos(nv / 2) +
                     1 / (3 * PI) * exp(nu / (6 * PI)) * cos(nv / 2) * cos(nv / 2) * sin(nu),

         // Partial derivative dy / dv for vertex 1 and 2
               y1v = -2 * (-1 + exp(cu / (6 * PI))) * cos(cv / 2) * sin(cu) * sin(cv / 2),
               y2v = -2 * (-1 + exp(nu / (6 * PI))) * cos(nv / 2) * sin(nu) * sin(nv / 2),

         // Partial derivative dz / du for vertex 1 and 2
               z1u = -1 / (3 * PI) * exp(cu / (3 * PI)) + 1 / (6 * PI) * exp(cu / (6 * PI)) * sin(cv),
               z2u = -1 / (3 * PI) * exp(nu / (3 * PI)) + 1 / (6 * PI) * exp(nu / (6 * PI)) * sin(nv),

         // Partial derivative dz / dv for vertex 1 and 2
               z1v = -1 * cos(cv) + exp(cu / (6 * PI)) * cos(cv),
               z2v = -1 * cos(nv) + exp(nu / (6 * PI)) * cos(nv);

         // normal of vertex 1 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv)
         glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
         glm::vec3 n1 = glm::cross(v2, v1);

         // normal of vertex 2 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv
         glm::vec3 v3 = glm::vec3(x2u, y2u, z2u), v4 = glm::vec3(x2v, y2v, z2v);
         glm::vec3 n2 = glm::cross(v4, v3);

         glm::vec4 vertex1 = glm::vec4(x1, y1, z1, 1);
         glm::vec4 vertex2 = glm::vec4(x2, y2, z2, 1);

         // if this is the back side
         if (static_cast<int>(m_name.find("Flip")) > 0) {
            n1 *= -1;
            n2 *= -1;
         }

         // pre-store a vertex if it is at the beginning
         if (cp1 == 0) {
            push(vertex1, glm::vec4(n1, 0));
         }

         // save vertices and their normals
         if (static_cast<int>(m_name.find("Flip")) > 0) {
            push(vertex2, glm::vec4(n2, 0), vertex1, glm::vec4(n1, 0));
         }else {
            push(vertex1, glm::vec4(n1, 0), vertex2, glm::vec4(n2, 0));
         }

         // store a vertex again if it is at the end
         if (cp1 == p1) {
            push(vertex2, glm::vec4(n2, 0));
         }
      }
   }
}
