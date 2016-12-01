#ifndef MOIBUS_H
#define MOIBUS_H


#include "shape/geometry/Shape.h"
#include "shape/surfaces/MobiusFace.h"

/**
 * @class Mobius
 * A Mobius includes two Mobius strips here.
 */
class Mobius : public Shape
{
public:
   Mobius(Paramater para, const std::string name);
   Mobius(Paramater para, const std::string name, const glm::mat4& mat);
   Mobius(const Mobius& mf);
   // I don't expect it's going to be a "deep clone"
   Mobius& operator=(const Mobius& other) = default;
   ~Mobius();
   void initFaces() override;
};


#endif // MOIBUS_H
