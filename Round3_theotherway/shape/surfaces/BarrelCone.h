#ifndef BARRELCONE_H
#define BARRELCONE_H

#include "shape/surfaces/Surface.h"

/**
 * @class BarrelCone
 * The "wall" of a cone
 */

class BarrelCone : public Surface
{
public:
   BarrelCone(Paramater para, const std::string name);
   BarrelCone(Paramater para, const std::string name, const glm::mat4& mat);
   BarrelCone(const BarrelCone& bc);
   // I don't expect it's going to be a "deep clone"
   BarrelCone& operator=(const BarrelCone& other) = default; // copy move assignment

   ~BarrelCone();
   //compute all the vertices
   virtual void computeVertices() override;
};

#endif // BARRELCONE_H
