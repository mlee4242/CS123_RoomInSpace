 #include "KleinBottleFace.h"
#include "gl/util.h"
#include <iostream>
#include <math.h>

KleinBottleFace::KleinBottleFace(Paramater para, const std::string name)
   : Surface(para, name) {
   computeVertices();
}


KleinBottleFace::KleinBottleFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   computeVertices();
}


// copy constructor
KleinBottleFace::KleinBottleFace(const KleinBottleFace& mf) : Surface(mf) {
   m_para     = mf.m_para;
   m_name     = mf.m_name;
   m_transMat = mf.m_transMat;
   m_ifTrans  = mf.m_ifTrans;
   m_vertices = mf.m_vertices;
}

KleinBottleFace::~KleinBottleFace() {}

/**
 * @brief KleinBottleFace::computeVertices
 * The equation of "The figure 8 immersion" and a Klein Bottle is at
 * https://en.wikipedia.org/wiki/Klein_bottle
 */
void KleinBottleFace::computeVertices() {
   // limit the minimum of p1 p2 to 4, otherwise the klein shape may lose the general shape
   // 4 because this is a 4D shape projected on 3D space
   int p1 = m_para.p1 < 4 ? 4 : m_para.p1;
   int p2 = m_para.p2 < 4 ? 4 : m_para.p2;

   // the figure 8 immersion, a type of klein bottle
   if (static_cast<int> (m_name.find("Bottle")) < 0) {
      float step_p1 = 2.0 * PI / p1;
      float step_p2 = 2.0 * PI / p2;
      float a       = 4 * m_para.radius;

      for (int cp2 = 0; cp2 < p2; cp2++) {
         for (int cp1 = 0; cp1 <= p1; cp1++) {
            float cv = step_p1 * cp1, cu = step_p2 * cp2,       // the top u, v paramater
                  nv = step_p1 * cp1, nu = step_p2 * (cp2 + 1); // the bottom top u, v paramater

            //! I could pre-compute the repeat veriable to avoid extra computation
            //! But I think C++'s complier could do that?

            // vertex 1
            float x1 = (a + cos(cu / 2) * sin(cv) - sin(cu / 2) * sin(2 * cv)) * cos(cu),
                  y1 = (a + cos(cu / 2) * sin(cv) - sin(cu / 2) * sin(2 * cv)) * sin(cu),
                  z1 = sin(cu / 2) * sin(cv) + cos(cu / 2) * sin(2 * cv);

            // vertex 2
            float x2 = (a + cos(nu / 2) * sin(nv) - sin(nu / 2) * sin(2 * nv)) * cos(nu),
                  y2 = (a + cos(nu / 2) * sin(nv) - sin(nu / 2) * sin(2 * nv)) * sin(nu),
                  z2 = sin(nu / 2) * sin(nv) + cos(nu / 2) * sin(2 * nv);

            // ∂x / ∂u, ∂y / ∂u, ∂z / ∂u for vertex 1
            float x1u = cos(cu) * (-1.0 / 2.0 * sin(cu / 2) * sin(cv) - 1.0 / 2.0 * cos(cu / 2) * sin(2 * cv)) -
                        sin(cu) * (a + cos(cu / 2) * sin(cv) - sin(cu / 2) * sin(2 * cv)),
                  y1u = sin(cu) * (-(1.0 / 2.0) * sin(cu / 2) * sin(cv) - 1.0 / 2.0 * cos(cu / 2) * sin(2 * cv)) +
                        cos(cu) * (a + cos(cu / 2) * sin(cv) - sin(cu / 2) * sin(2 * cv)),
                  z1u = 1.0 / 2.0 * cos(cu / 2) * sin(cv) - 1.0 / 2.0 * sin(cu / 2) * sin(2 * cv);

            // ∂x / ∂v, ∂y / ∂v, ∂z / ∂v for vertex 1
            float x1v = cos(cu) * (cos(cu / 2) * cos(cv) - 2 * cos(2 * cv) * sin(cu / 2)),
                  y1v = (cos(cu / 2) * cos(cv) - 2 * cos(2 * cv) * sin(cu / 2)) * sin(cu),
                  z1v = 2 * cos(cu / 2) * cos(2 * cv) + cos(cv) * sin(cu / 2);

            // ∂x / ∂u, ∂y / ∂u, ∂z / ∂u for vertex 2
            float x2u = cos(nu) * (-1.0 / 2.0 * sin(nu / 2) * sin(nv) - 1.0 / 2.0 * cos(nu / 2) * sin(2 * nv)) -
                        sin(nu) * (a + cos(nu / 2) * sin(nv) - sin(nu / 2) * sin(2 * nv)),
                  y2u = sin(nu) * (-(1.0 / 2.0) * sin(nu / 2) * sin(nv) - 1.0 / 2.0 * cos(nu / 2) * sin(2 * nv)) +
                        cos(nu) * (a + cos(nu / 2) * sin(nv) - sin(nu / 2) * sin(2 * nv)),
                  z2u = 1.0 / 2.0 * cos(nu / 2) * sin(nv) - 1.0 / 2.0 * sin(nu / 2) * sin(2 * nv);

            // ∂x / ∂v, ∂y / ∂v, ∂z / ∂v for vertex 2
            float x2v = cos(nu) * (cos(nu / 2) * cos(nv) - 2 * cos(2 * nv) * sin(nu / 2)),
                  y2v = (cos(nu / 2) * cos(nv) - 2 * cos(2 * nv) * sin(nu / 2)) * sin(nu),
                  z2v = 2 * cos(nu / 2) * cos(2 * nv) + cos(nv) * sin(nu / 2);

            // normal of vertex 1 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
            glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
            glm::vec3 n1 = glm::cross(v1, v2);

            // normal of vertex 2 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
            glm::vec3 v3 = glm::vec3(x2u, y2u, z2u), v4 = glm::vec3(x2v, y2v, z2v);
            glm::vec3 n2 = glm::cross(v3, v4);

            glm::vec4 vertex1 = glm::vec4(x1, y1 + 4 * m_para.radius, z1, 1);
            glm::vec4 vertex2 = glm::vec4(x2, y2 + 4 * m_para.radius, z2, 1);

            if (static_cast<int>(m_name.find("Flip")) < 0) {
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
   } else{
      // A Klein bottle with a bottle shape

      // This has to be an even number
      p1 = p1 / 2 * 2;

      float step_p1 = 2.0 * PI / p1;
      float step_p2 = PI / p2;

      for (int cp2 = 0; cp2 < p2; cp2++) {
         for (int cp1 = 0; cp1 <= p1; cp1++) {
            float cv = step_p1 * cp1, cu = step_p2 * cp2,
                  nv = step_p1 * cp1, nu = step_p2 * (cp2 + 1);

            // precompute some repeated terms
            float coscu = cos(cu), sincu = sin(cu), coscv = cos(cv), sincv = sin(cv),
                  cosnv = cos(nv), sinnv = sin(nv), cosnu = cos(nu), sinnu = sin(nu);

            // vertex 1
            float x1 = -2.0 / 15.0 * coscu * (3 * coscv - 30 * sincu + 90 * pow(coscu, 4) * sincu
                                              - 60 * pow(coscu, 6) * sincu + 5 * coscu * coscv * sincu),
                  y1 = -1.0 / 15.0 * sincu * (3 * coscv - 3 * pow(coscu, 2) * coscv - 48 * pow(coscu, 4) * coscv + 48 * pow(coscu, 6) * coscv -
                                              60 * sincu + 5 * coscu * coscv * sincu - 5 * pow(coscu, 2) * coscv * sincu - 80 *
                                              pow(coscu, 4) * coscu * coscv * sincu + 80 * pow(coscu, 6) * coscu * coscv * sincu),
                  z1 = 2.0 / 15.0 * (3.0 + 5.0 * coscu * sincu) * sincv;

            // vertex 2
            float x2 = -2.0 / 15.0 * cosnu * (3 * cosnv - 30 * sinnu + 90 * pow(cosnu, 4) * sinnu -
                                              60 * pow(cosnu, 6) * sinnu + 5 * cosnu * cosnv * sinnu),
                  y2 = -1.0 / 15.0 * sinnu * (3 * cosnv - 3 * pow(cosnu, 2) * cosnv - 48 * pow(cosnu, 4) * cosnv + 48 * pow(cosnu, 6) * cosnv -
                                              60 * sinnu + 5 * cosnu * cosnv * sinnu - 5 * pow(cosnu, 2) * cosnv * sinnu - 80 *
                                              pow(cosnu, 4) * cosnu * cosnv * sinnu + 80 * pow(cosnu, 6) * cosnu * cosnv * sinnu),
                  z2 = 2.0 / 15.0 * (3.0 + 5.0 * cosnu * sinnu) * sinnv;

            // ∂x / ∂u, ∂y / ∂u, ∂z / ∂u for vertex 1
            float x1u = 2.0 / 15.0 * sincu * (3 * coscv - 30 * sincu + 90 * pow(coscu, 4) * sincu -
                                              60 * pow(coscu, 6) * sincu + 5 * coscu * coscv * sincu) -
                        2.0 / 15.0 * coscu * (-30 * coscu + 90 * pow(coscu, 5) - 60 * pow(coscu, 7) +
                                              5 * pow(coscu, 2) * coscv - 360 * pow(coscu, 3) * pow(sincu, 2) +
                                              360 * pow(coscu, 5) * pow(sincu, 2) - 5 * coscv * pow(sincu, 2)),

                  y1u = -1.0 / 15.0 * coscu * (3 * coscv - 3 * pow(coscu, 2) * coscv - 48 * pow(coscu, 4) * coscv +
                                               48 * pow(coscu, 6) * coscv - 60 * sincu + 5 * coscu * coscv * sincu -
                                               5 * pow(coscu, 3) * coscv * sincu - 80 * pow(coscu, 5) * coscv * sincu +
                                               80 * pow(coscu, 7) * coscv * sincu) -
                        1.0 / 15.0 * sincu * (-60 * coscu + 5 * pow(coscu, 2) * coscv - 5 * pow(coscu, 4) * coscv -
                                              80 * pow(coscu, 6) * coscv + 80 * pow(coscu, 8) * coscv +
                                              6 * coscu * coscv * sincu + 192 * pow(coscu, 3) * coscv * sincu -
                                              288 * pow(coscu, 5) * coscv * sincu - 5 * coscv * pow(sincu, 2) +
                                              15 * pow(coscu, 2) * coscv * pow(sincu, 2) + 400 * pow(coscu, 4) * coscv * pow(sincu, 2) -
                                              560 * pow(coscu, 6) * coscv * pow(sincu, 2)),

                  z1u = 2.0 / 15.0 * (5 * pow(coscu, 2) - 5 * pow(sincu, 2)) * sincv;


            // ∂x / ∂v, ∂y / ∂v, ∂z / ∂v for vertex 1
            float x1v = (-2.0 / 15.0) * coscu * (-3 * sincv - 5 * coscu * sincu * sincv),
                  y1v = (-1.0 / 15.0) * sincu * (-3 * sincv + 3 * pow(coscu, 2) * sincv + 48 * pow(coscu, 4) * sincv -
                                                 48 * pow(coscu, 6) * sincv - 5 * coscu * sincu * sincv +
                                                 5 * pow(coscu, 3) * sincu * sincv + 80 * pow(coscu, 5) * sincu * sincv -
                                                 80 * pow(coscu, 7) * sincu * sincv),
                  z1v = 2.0 / 15.0 * coscv * (3 + 5 * coscu * sincu);

            // ∂x / ∂u, ∂y / ∂u, ∂z / ∂u for vertex 2
            float x2u = 2.0 / 15.0 * sinnu * (3 * cosnv - 30 * sinnu + 90 * pow(cosnu, 4) * sinnu -
                                              60 * pow(cosnu, 6) * sinnu + 5 * cosnu * cosnv * sinnu) -
                        2.0 / 15.0 * cosnu * (-30 * cosnu + 90 * pow(cosnu, 5) - 60 * pow(cosnu, 7) +
                                              5 * pow(cosnu, 2) * cosnv - 360 * pow(cosnu, 3) * pow(sinnu, 2) +
                                              360 * pow(cosnu, 5) * pow(sinnu, 2) - 5 * cosnv * pow(sinnu, 2)),
                  y2u = (-1.0 / 15.0) * cosnu * (3 * cosnv - 3 * pow(cosnu, 2) * cosnv - 48 * pow(cosnu, 4) * cosnv +
                                                 48 * pow(cosnu, 6) * cosnv - 60 * sinnu + 5 * cosnu * cosnv * sinnu -
                                                 5 * pow(cosnu, 3) * cosnv * sinnu - 80 * pow(cosnu, 5) * cosnv * sinnu +
                                                 80 * pow(cosnu, 7) * cosnv * sinnu) -
                        1.0 / 15.0 * sinnu * (-60 * cosnu + 5 * pow(cosnu, 2) * cosnv - 5 * pow(cosnu, 4) * cosnv -
                                              80 * pow(cosnu, 6) * cosnv + 80 * pow(cosnu, 8) * cosnv +
                                              6 * cosnu * cosnv * sinnu + 192 * pow(cosnu, 3) * cosnv * sinnu -
                                              288 * pow(cosnu, 5) * cosnv * sinnu - 5 * cosnv * pow(sinnu, 2) +
                                              15 * pow(cosnu, 2) * cosnv * pow(sinnu, 2) + 400 * pow(cosnu, 4) * cosnv * pow(sinnu, 2) -
                                              560 * pow(cosnu, 6) * cosnv * pow(sinnu, 2)),

                  z2u = 2.0 / 15.0 * (5 * pow(cosnu, 2) - 5 * pow(sinnu, 2)) * sinnv;

            // ∂x / ∂v, ∂y / ∂v, ∂z / ∂v for vertex 2
            float x2v = (-2.0 / 15.0) * cosnu * (-3 * sinnv - 5 * cosnu * sinnu * sinnv),
                  y2v = (-1.0 / 15.0) * sinnu * (-3 * sinnv + 3 * pow(cosnu, 2) * sinnv + 48 * pow(cosnu, 4) * sinnv -
                                                 48 * pow(cosnu, 6) * sinnv - 5 * cosnu * sinnu * sinnv +
                                                 5 * pow(cosnu, 3) * sinnu * sinnv + 80 * pow(cosnu, 5) * sinnu * sinnv -
                                                 80 * pow(cosnu, 7) * sinnu * sinnv),
                  z2v = 2.0 / 15.0 * cosnv * (3 + 5 * cosnu * sinnu);

            // normal of vertex 1 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
            glm::vec3 v1 = glm::vec3(x1u, y1u, z1u), v2 = glm::vec3(x1v, y1v, z1v);
            glm::vec3 n1 = glm::cross(v2, v1);

            // normal of vertex 2 using the cross production of (∂x/ ∂u, ∂y/ ∂u, ∂z/ ∂u) and (∂x/ ∂v, ∂y/ ∂v, ∂z/ ∂v)
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
}
