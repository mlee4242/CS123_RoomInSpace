#ifndef SPHEREFACE_H
#define SPHEREFACE_H

#include "shape/surfaces/Surface.h"

/**
 * @class SphereFace
 * The face vertices of a seashell. It is a seashell shape since the surface is a closed shape.
 */

class SphereFace : public Surface
{
public:
   SphereFace(Paramater para, const std::string name);
   SphereFace(Paramater para, const std::string name, const glm::mat4& mat);
   SphereFace(const SphereFace& sf);
   // I don't expect it's going to be a "deep clone"
   SphereFace& operator=(const SphereFace& other) = default;

   ~SphereFace();
   // compute the surfaces
   virtual void computeVertices() override;
};
#endif // SPHEREFACE_H
