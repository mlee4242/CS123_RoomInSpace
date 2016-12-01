#ifndef CIRCLE_H
#define CIRCLE_H
#include "Surface.h"

/**
 * @class Circle
 * A circle used as the base of a cone and a cylinder
 */

class Circle : public Surface
{
public:
   Circle(Paramater para, const std::string name);
   Circle(Paramater para, const std::string name, const glm::mat4& mat);
   Circle(const Circle& c);
   // I don't expect it's going to be a "deep clone"
   Circle& operator=(const Circle& other) = default; // copy move assignment

   ~Circle();
   virtual void computeVertices() override;
};

#endif // CIRCLE_H
