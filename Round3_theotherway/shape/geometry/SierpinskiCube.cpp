#include "SierpinskiCube.h"
#include <iostream>
#include <shape/geometry/Cube.h>
#include <shape/surfaces/FakeFace.h>

SierpinskiCube::SierpinskiCube(Paramater para, const std::string name)
   : Shape(para, name),
   m_allVertices(std::vector<GLfloat>()) {}


SierpinskiCube::SierpinskiCube(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat),
   m_allVertices(std::vector<GLfloat>()) {
   m_ifTrans = true;  // it has been initialized. this is to change the value.
}


SierpinskiCube::~SierpinskiCube() {}

/**
 * @brief SierpinskiCube::initFaces
 * The SierpinskiCube has multiple cubes.But if I just store all the surfaces of all the cubes,
 * it will eat a lot of resources when drawing.
 * I made a fake face to store all the vertices in the fake surface
 */
void SierpinskiCube::setParameters(Paramater para) {
   m_para = para;
   initFaces();
}


void SierpinskiCube::initFaces() {
   std::shared_ptr<FakeFace> fakeFace = std::shared_ptr<FakeFace>(new FakeFace(m_para, "Sierpinski"));

   m_allVertices.clear();
   m_faces.clear();

   // spread the recursion depth...
   int recursionLevel = m_para.p3;
   recursionLevel /= 14;
   recursionLevel += 1;

   // scale it a little bit
   m_para.radius *= 2;

   // recursively find eachcube
   SierpinskiCubeRecursion(recursionLevel, glm::mat4());

   //set up the fake face
   fakeFace->setData(m_allVertices);

   // store the fake surface
   m_faces.push_back(fakeFace);
}


/**
 * @brief SierpinskiCube::SierpinskiCubeRecursion
 * @param level the level in the recursion, 1 is leaf
 * @param mat the transformation matrix for current level
 */
void SierpinskiCube::SierpinskiCubeRecursion(int level, const glm::mat4& mat) {
   if (level == 1) { // if reaches the leaf  cube
      // pull all the faces of this cube
      std::shared_ptr<Shape> cube = std::shared_ptr<Shape>(new Cube(m_para, "Sphere", mat));
      cube->initFaces();
      std::vector<std::shared_ptr<Surface> > faces = cube->getData();

      for (int i = 0; i < faces.size(); i++) {
         // store all the vertices on each faces
         std::vector<GLfloat> vertices = faces[i]->getData();
         for (int j = 0; j < vertices.size(); j++) {
            m_allVertices.push_back(vertices[j]);
         }
      }
      return;
   }else{
      // if not a leaf, makes four cubes based on current location
      glm::mat4 scaleMat     = glm::scale(glm::vec3(0.5, 0.5, 0.5));
      glm::mat4 matTransTop1 = glm::translate(glm::vec3(0.5 * m_para.radius, 0.5 * m_para.radius, 0.5 * m_para.radius));
      glm::mat4 matTop1      = matTransTop1 * scaleMat * mat;

      glm::mat4 matTransTop2 = glm::translate(glm::vec3(-0.5 * m_para.radius, 0.5 * m_para.radius, -0.5 * m_para.radius));
      glm::mat4 matTop2      = matTransTop2 * scaleMat * mat;

      glm::mat4 matTransBottom1 = glm::translate(glm::vec3(-0.5 * m_para.radius, -0.5 * m_para.radius, 0.5 * m_para.radius));
      glm::mat4 matBottom1      = matTransBottom1 * scaleMat * mat;

      glm::mat4 matTransBottom2 = glm::translate(glm::vec3(0.5 * m_para.radius, -0.5 * m_para.radius, -0.5 * m_para.radius));
      glm::mat4 matBottom2      = matTransBottom2 * scaleMat * mat;

      //if not a leaf, then recurse on each of the four cubes
      SierpinskiCubeRecursion(level - 1, matTop1);
      SierpinskiCubeRecursion(level - 1, matTop2);
      SierpinskiCubeRecursion(level - 1, matBottom1);
      SierpinskiCubeRecursion(level - 1, matBottom2);
   }
}
