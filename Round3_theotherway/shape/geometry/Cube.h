#ifndef CUBE_H
#define CUBE_H

#include "shape/geometry/Shape.h"
#include "shape/surfaces/Rectangle.h"

/**
 * @class Cube
 * A cone contains six rectangles
 */

class Cube : public Shape {
public:
   Cube(Paramater para, const std::string name);
   Cube(Paramater para, const std::string name, const glm::mat4& mat);
   // I don't expect it's going to be a "deep clone"
   Cube& operator=(const Cube& other) = default;
   ~Cube();
   void initFaces() override;
};

#endif // CUBE_H
