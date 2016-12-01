#ifndef TORUSFACE_H
#define TORUSFACE_H

#include "shape/surfaces/Surface.h"

/**
 * @class TorusFace
 * The face vertices of a torus. It is a torus already since the surface is a closed shape.
 */
class TorusFace : public Surface
{
public:
   TorusFace(Paramater para, const std::string name);
   TorusFace(Paramater para, const std::string name, const glm::mat4& mat);
   TorusFace(const TorusFace& tf);
   // I don't expect it's going to be a "deep clone"
   TorusFace& operator=(const TorusFace& other) = default;

   ~TorusFace();
   // compute the surfaces
   virtual void computeVertices() override;
};

#endif         // BARRELCONE_H
