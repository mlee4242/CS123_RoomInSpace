#ifndef BARREL_H
#define BARREL_H

#include "shape/surfaces/Surface.h"

/**
 * @class Barrel
 * The "wall" of a cylinder
 */

class Barrel : public Surface
{
public:
   Barrel(Paramater para, const std::string name);
   Barrel(Paramater para, const std::string name, const glm::mat4& mat);
   Barrel(const Barrel& b); // copy constructor

   // I don't expect it's going to be a "deep clone"
   Barrel& operator=(const Barrel& other) = default; // copy move assignment
   ~Barrel();

   // compute the surfaces
   virtual void computeVertices() override;
};

#endif // BARREL_H
