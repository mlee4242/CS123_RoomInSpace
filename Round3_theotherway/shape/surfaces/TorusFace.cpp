#include "TorusFace.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

TorusFace::TorusFace(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


TorusFace::TorusFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
TorusFace::TorusFace(const TorusFace& tf) : Surface(tf) {
   m_para     = tf.m_para;
   m_name     = tf.m_name;
   m_transMat = tf.m_transMat;
   m_ifTrans  = tf.m_ifTrans;
   m_vertices = tf.m_vertices;
}


TorusFace::~TorusFace() {}

/**
 * @brief TorusFace::computeVertices
 * The equation of a torus is
 * x = (c + a cos v) cos u
 * y = (c + a cos v) sin u
 * z = a sin v
 */
void TorusFace::computeVertices() {
   // limit the minimum of p1 p2 to 3, otherwise the torus may lose the general shape
   int p1 = m_para.p1 < 3 ? 3 : m_para.p1;
   int p2 = m_para.p2 < 3 ? 3 : m_para.p2;

   float step_p1 = 2.0 * PI / p1;
   float step_p2 = 2.0 * PI / p2;

   float a = m_para.radius, c = 1.5 * sqrt(m_para.radius); // parameters in the equation


   for (int cp2 = 0; cp2 < p2; cp2++) {
      for (int cp1 = 0; cp1 <= p1; cp1++) {
         float cv = step_p1 * cp1, cu = step_p2 * cp2,       // the top u, v value
               nv = step_p1 * cp1, nu = step_p2 * (cp2 + 1); // the bottom u, v value

         // vertex 1
         float x1 = (c + a * cos(cv)) * cos(cu),
               y1 = (c + a * cos(cv)) * sin(cu),
               z1 = a * sin(cv),

         // vertex 2
               x2 = (c + a * cos(nv)) * cos(nu),
               y2 = (c + a * cos(nv)) * sin(nu),
               z2 = a * sin(nv),

         // Partial derivative dx / du, dy / du, dz / du for vertex 1
               x1u = -1 * (c + a * cos(cv)) * sin(cu),
               y1u = cos(cu) * (c + a * cos(cv)),
               z1u = 0,

         // Partial derivative dx / dv, dy / dv, dz / dv for vertex 1
               x1v = -a *cos(cu) * sin(cv),
               y1v = -a *sin(cu) * sin(cv),
               z1v = a * cos(cv),

         // Partial derivative dx / du, dy / du, dz / du for vertex 2
               x2u = -1 * (c + a * cos(nv)) * sin(nu),
               y2u = cos(nu) * (c + a * cos(nv)),
               z2u = 0,

         // Partial derivative dx / dv, dy / dv, dz / dv for vertex 2
               x2v = -a *cos(nu) * sin(nv),
               y2v = -a *sin(nu) * sin(nv),
               z2v = a * cos(nv);

         // normal of vertex 1 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv)
         glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
         glm::vec3 n1 = glm::cross(v1, v2);

         // normal of vertex 2 using the cross production of (dx/ du, dy/ du, dz/ du) and (dx/ dv, dy/ dv, dz/ dv)
         glm::vec3 v3 = glm::vec3(x2u, y2u, z2u), v4 = glm::vec3(x2v, y2v, z2v);
         glm::vec3 n2 = glm::cross(v3, v4);

         // save vertices and their normals
         push(glm::vec4(x1, y1, z1, 1), glm::vec4(n1, 0), glm::vec4(x2, y2, z2, 1), glm::vec4(n2, 0));
      }
   }
}
