#ifndef SEASHELL_H
#define SEASHELL_H


#include "shape/geometry/Shape.h"
#include "shape/surfaces/SeashellFace.h"

/**
 * @class Seashell
 * A Seashell includes one Seashell surface.
 */

class Seashell : public Shape
{
public:
   Seashell(Paramater para, const std::string name);
   Seashell(Paramater para, const std::string name, const glm::mat4& mat);
   // I don't expect it's going to be a "deep clone"
   Seashell& operator=(const Seashell& other) = default;

   ~Seashell();
   void initFaces() override;
};

#endif // SEASHELL_H
