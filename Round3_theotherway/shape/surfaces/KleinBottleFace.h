#ifndef KLEINBOTTLEFACE_H
#define KLEINBOTTLEFACE_H

#include "Surface.h"

/**
 * @class KleinBottleFace
 * The face vertices of a Klein Bottle. Namely, it is a Klein Bottle..
 */

class KleinBottleFace : public Surface
{
public:
   KleinBottleFace(Paramater para, const std::string name);
   KleinBottleFace(Paramater para, const std::string name, const glm::mat4& mat);
   KleinBottleFace(const KleinBottleFace& kbf);
   // I don't expect it's going to be a "deep clone"
   KleinBottleFace& operator=(const KleinBottleFace& other) = default; // copy move assignment
   ~KleinBottleFace();
   // compute the surfaces
   virtual void computeVertices() override;
};

#endif // KLEINBOTTLEFACE_H
