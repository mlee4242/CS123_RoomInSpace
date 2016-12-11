#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <QVector>

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

   glm::mat4x4 getModelMatrix();
   glm::vec3 getUpper();
   glm::vec3 getLower();
   glm::vec3 getTranUpper();
   glm::vec3 getTranLower();

   // test if another bounding hits this box
   bool overlap(const BoundingBox& box);
   bool inside(const glm::vec4& x);
   void updateBox(const BoundingBox& box);
   void setModelMatrix(glm::mat4x4& mat);
   void updateBox(const glm::vec3&);
   void updateBox(const glm::vec4&);
   void updateUpper(const glm::vec3&);
   void updateLower(const glm::vec3&);
   void updateTranformedBounds();

   // print the upper and lower bound
   void print();
   void printCorners();

   // check if this bounding box satifies its definition
   void check();

protected:
   float hit(const glm::vec4& p, const glm::vec4& d);

   glm::vec3 m_upper;
   glm::vec3 m_lower;
   QVector<glm::vec4> m_corners;
   glm::mat4x4 m_modelMat;
};

#endif // BOUNDINGBOX_H
