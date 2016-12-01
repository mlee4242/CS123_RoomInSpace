#ifndef FAKEFACE_H
#define FAKEFACE_H

#include "shape/surfaces/Surface.h"

/**
 * @class FakeFace
 * This class is for holding all the faces as one to save the resources.
 * In theory, I don't need this class at all. It breaks the design somehow.
 * However, in the recursion of computing Sierpinski Shape, I could not
 * send thousands of objects to GPU...
 */

class FakeFace : public Surface
{
public:
   FakeFace(Paramater para, const std::string name);
   FakeFace(Paramater para, const std::string name, const glm::mat4& mat);
   FakeFace(const FakeFace& b);         // copy constructor

   // I don't expect it's going to be a "deep clone"
   FakeFace& operator=(const FakeFace& other) = default;         // copy move assignment

   ~FakeFace();
   // compute the surfaces
   virtual void computeVertices() override;
};
#endif // FAKEFACE_H
