#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "glm/glm.hpp"
#include <memory>
#include <vector>

/**
 * @brief The BoundingBox class
 */
class BoundingBox
{
public:
   BoundingBox();
   // copy constructor
   BoundingBox(const BoundingBox& box);
   // initialize a bounding box for a set of other bounding boxes
   BoundingBox(const std::vector<std::shared_ptr<BoundingBox> >&);
   // initialize a bounding box from upper and lower bounds
   BoundingBox(const glm::vec3& upper, const glm::vec3& lower);
   void updateBox(const BoundingBox& box);
   glm::vec3 getUpper();
   glm::vec3 getLower();

   // test if another bounding hits this box
   bool hit(const BoundingBox& box);

   // print the upper and lower bound
   void print();

   // check if this bounding box satifies its definition
   void check();
   void updateBox(const glm::vec3&);
   void updateBox(const glm::vec4&);
   void updateUpper(const glm::vec3&);
   void updateLower(const glm::vec3&);

protected:
   glm::vec3 m_upper;
   glm::vec3 m_lower;
};

#endif // BOUNDINGBOX_H
