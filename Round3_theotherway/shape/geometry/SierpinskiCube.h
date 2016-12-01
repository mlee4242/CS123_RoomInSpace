#ifndef SIERPINSKICUBE_H
#define SIERPINSKICUBE_H


#include "shape/geometry/Shape.h"
#include "shape/geometry/Cube.h"

/**
 * @class SierpinskiCube
 * A SierpinskiCube includes multiple cubes manipulated by Sierpinski's rule.
 * Each cube's faces are stored as the faces of the Sierpinski shape to save resources.
 * Each vertex in all the faces is stored in the same list to save resources.
 */

class SierpinskiCube : public Shape
{
public:
   SierpinskiCube(Paramater para, const std::string name);
   SierpinskiCube(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   SierpinskiCube& operator=(const SierpinskiCube& other) = default;

   ~SierpinskiCube();

   void initFaces() override;
   void setParameters(Paramater para) override;

   // the recursion to compute each cube
   void SierpinskiCubeRecursion(int level, const glm::mat4& m_transMat);

private:
   std::vector<GLfloat> m_allVertices;
};

#endif // SIERPINSKICUBE_H
