#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Surface.h"

/**
 * @class Rectangle
 * The face vertices of a rect / square used in cube
 */

class Rectangle : public Surface
{
public:
   Rectangle(Paramater para, const std::string name);
   Rectangle(Paramater para, const std::string name, const glm::mat4& mat);
   Rectangle(const Rectangle& r);
   // I don't expect it's going to be a "deep clone"
   Rectangle& operator=(const Rectangle& other) = default;
   ~Rectangle();
   // compute the surfaces
   virtual void computeVertices() override;
};

#endif // RECTANGLE_H
