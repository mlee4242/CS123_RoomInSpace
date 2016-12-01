#ifndef CONE_H
#define CONE_H


#include "shape/geometry/Shape.h"
#include "shape/surfaces/Circle.h"

/**
 * @class Cone
 * A cone include one barrel and one base
 * The barrel is specific to a cone, called BarrelCone
 * The base is a circle
 */

class Cone : public Shape
{
public:
   Cone(Paramater para, const std::string name);
   Cone(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   Cone& operator=(const Cone& other) = default;
   ~Cone();
   void initFaces() override;
};

#endif // CONE_H
