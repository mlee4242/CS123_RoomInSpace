#ifndef SEASHELLFACE_H
#define SEASHELLFACE_H

#include "shape/surfaces/Surface.h"

/**
 * @class SeashellFace
 * The face vertices of a seashell. It is a seashell shape since the surface is a closed shape.
 */

class SeashellFace : public Surface
{
public:
   SeashellFace(Paramater para, const std::string name);
   SeashellFace(Paramater para, const std::string name, const glm::mat4& mat);
   SeashellFace(const SeashellFace& sf);
   // I don't expect it's going to be a "deep clone"
   SeashellFace& operator=(const SeashellFace& other) = default;

   ~SeashellFace();
   // compute the surfaces
   virtual void computeVertices() override;
};

#endif // SEASHELLFACE_H
