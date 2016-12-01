#ifndef SPHERE_H
#define SPHERE_H


#include "shape/geometry/Shape.h"
#include "shape/surfaces/SphereFace.h"

/**
 * @class Sphere
 * A Sphere contains one Sphere Face
 */

class Sphere : public Shape
{
public:
   Sphere(Paramater para, const std::string name);
   Sphere(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   Sphere& operator=(const Sphere& other) = default;

   ~Sphere();
   void initFaces() override;
};

#endif // SPHERE_H
