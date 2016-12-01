#ifndef MOIBUSFACE_H
#define MOIBUSFACE_H

#include "Surface.h"

/**
 * @class MobiusFace
 * The face vertices of a mobius. It is a mobius strip since it only has one face.
 */

class MobiusFace : public Surface
{
public:
   MobiusFace(Paramater para, const std::string name);
   MobiusFace(Paramater para, const std::string name, const glm::mat4& mat);
   MobiusFace(const MobiusFace& mf);
   // I don't expect it's going to be a "deep clone"
   MobiusFace& operator=(const MobiusFace& other) = default;
   ~MobiusFace();
   // compute the surfaces
   virtual void computeVertices() override;
};

#endif
