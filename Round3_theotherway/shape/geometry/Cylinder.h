#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape/geometry/Shape.h"
#include "shape/surfaces/Circle.h"

/**
 * @class Cylinder
 * A cylinder includes one circle base and a barrel
 */

class Cylinder : public Shape
{
public:
   Cylinder(Paramater para, const std::string name);
   Cylinder(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   Cylinder& operator=(const Cylinder& other) = default;
   ~Cylinder();
   void initFaces() override;
};

#endif // CYLINDER_H
