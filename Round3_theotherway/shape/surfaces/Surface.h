#ifndef SURFACE_H
#define SURFACE_H

#include "shape/OpenGLShape.h"
#include "shape/Parameter.h"
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate"

/**
 * @class Surface: a based class
 * This class models a surface. Intuitively, it means a plane or a surface with no volume.
 * However, a closed face, such as seashell, klein bottle can also be considered as a surface.
 * So it could be anything that can be described by 2 parameter in 3D space.
 * All the surfaces without transformation are centered at (0, 0, 0)
 */

class Surface : public OpenGLShape {
public:
   Surface(Paramater para, const std::string name);

   // if the surface needs to be transformed
   Surface(Paramater para, const std::string name, const glm::mat4& mat);

   // copy constructor
   Surface(const Surface& sf);

   // deconstructor
   virtual ~Surface() = default;

   // copy move assignment
   Surface& operator=(const Surface& other);

   // draw all the vertices on this surface in order
   void sentToVAO(bool nofill);

   // draw all the vertices on this surface in order
   void draw();

   // get all the vertices on this surface
   std::vector<GLfloat> getData();

   // set all the vertices on this surface
   void setData(const std::vector<GLfloat>& m_vertices);

   // set the parameters
   virtual void setParameters(const Paramater& para);

protected:
   // push two vertices who are next to each other with their normals
   void push(glm::vec4 v1, glm::vec4 n1, glm::vec4 v2, glm::vec4 n2);

   // push one vertex with its normal
   void push(glm::vec4 v, glm::vec4 n);

   // compute all the vertices on this surface in order
   virtual void computeVertices() {}

   Paramater            m_para;     // the parameters of this surface
   std::string          m_name;
   std::vector<GLfloat> m_vertices;
   glm::mat4            m_transMat; // the transformation matrix of this surface, if applys
   bool                 m_ifTrans;  // the flag of ifdoing transformation
};

#endif // SURFACE_H
