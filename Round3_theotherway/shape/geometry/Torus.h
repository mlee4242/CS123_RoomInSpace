#ifndef TORUS_H
#define TORUS_H

#include "shape/geometry/Shape.h"
#include "shape/surfaces/TorusFace.h"

/**
 * @class Torus
 * A Torus contains one Torus Face
 */
class Torus : public Shape
{
public:
   Torus(Paramater para, const std::string name);
   Torus(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   Torus& operator=(const Torus& other) = default;

   ~Torus();
   void initFaces() override;
};


#endif // TORUS_H
