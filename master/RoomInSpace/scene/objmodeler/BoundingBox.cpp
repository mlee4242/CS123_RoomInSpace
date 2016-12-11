#include "BoundingBox.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

/**
 * @brief BoundingBox::BoundingBox
 * initialize the bounding box to a infinite box
 */
BoundingBox::BoundingBox() :
   m_upper(glm::vec3(-FLT_MAX)), m_lower(glm::vec3(FLT_MAX)),
   m_modelMat(glm::mat4x4()) {
   updateTranformedBounds();
}


/**
 * @brief BoundingBox::BoundingBox, copy constructor
 * @param box
 */
BoundingBox::BoundingBox(const BoundingBox& box) : m_modelMat(glm::mat4x4()) {
   m_upper = box.m_upper;
   m_lower = box.m_lower;
   updateTranformedBounds();
}


BoundingBox::BoundingBox(const glm::vec3& upper, const glm::vec3& lower) :
   m_upper(upper), m_lower(lower), m_modelMat(glm::mat4x4()) {
   updateTranformedBounds();
}


/**
 * @brief BoundingBox::BoundingBox
 * compute the bounding box of a set of bounding boxes
 * @param boxes
 */
BoundingBox::BoundingBox(const std::vector<std::shared_ptr<BoundingBox> >& boxes) :
   m_upper(glm::vec4(-FLT_MAX)), m_lower(glm::vec4(FLT_MAX)), m_modelMat(glm::mat4x4()) {
   for (auto box : boxes) {
      updateBox(box->m_lower);
      updateBox(box->m_upper);
   }
}


void BoundingBox::setModelMatrix(glm::mat4x4& mat) {
   m_modelMat = mat;
   updateTranformedBounds();
}


void BoundingBox::updateTranformedBounds() {
   m_corners.clear();
   m_corners.push_back(m_modelMat * glm::vec4(m_upper.x, m_upper.y, m_upper.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_lower.x, m_upper.y, m_upper.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_lower.x, m_lower.y, m_upper.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_upper.x, m_lower.y, m_upper.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_upper.x, m_upper.y, m_lower.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_lower.x, m_upper.y, m_lower.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_lower.x, m_lower.y, m_lower.z, 1.f));
   m_corners.push_back(m_modelMat * glm::vec4(m_upper.x, m_lower.y, m_lower.z, 1.f));
}


glm::mat4x4 BoundingBox::getModelMatrix() {
   return m_modelMat;
}


void BoundingBox::check() {
   assert(m_upper.x >= m_lower.x);
   assert(m_upper.y >= m_lower.y);
   assert(m_upper.z >= m_lower.z);
}


/**
 * @brief BoundingBox::print
 * print the upper and the lower bound for this
 */
void BoundingBox::print() {
   std::cout << "upper is " << glm::to_string(m_upper) << std::endl;
   std::cout << "lower is " << glm::to_string(m_lower) << std::endl;
}


void BoundingBox::printCorners(){
    for (auto corner : m_corners){
     std::cout << "corners are " << glm::to_string(corner) << std::endl;
    }

}

/**
 * @brief BoundingBox::updateBox
 * udpate the bounding box according the input
 * @param p
 */
void BoundingBox::updateBox(const glm::vec3& p) {
   m_upper.x = std::fmax(p.x, m_upper.x);
   m_upper.y = std::fmax(p.y, m_upper.y);
   m_upper.z = std::fmax(p.z, m_upper.z);
   m_lower.x = std::fmin(p.x, m_lower.x);
   m_lower.y = std::fmin(p.y, m_lower.y);
   m_lower.z = std::fmin(p.z, m_lower.z);
   updateTranformedBounds();
   check();
}


/**
 * @brief BoundingBox::updateBox
 * udpate the bounding box according the input
 * @param p
 */
void BoundingBox::updateBox(const BoundingBox& box) {
   updateBox(box.m_lower);
   updateBox(box.m_upper);
}


/**
 * @brief BoundingBox::updateBox
 * udpate the bounding box according the input
 * @param p
 */
void BoundingBox::updateBox(const glm::vec4& p) {
   m_upper.x = std::fmax(p.x, m_upper.x);
   m_upper.y = std::fmax(p.y, m_upper.y);
   m_upper.z = std::fmax(p.z, m_upper.z);
   m_lower.x = std::fmin(p.x, m_lower.x);
   m_lower.y = std::fmin(p.y, m_lower.y);
   m_lower.z = std::fmin(p.z, m_lower.z);
   updateTranformedBounds();
   check();
}


/**
 * @brief BoundingBox::updateLower
 * udpate the lower bound of the the bounding box according the input
 * @param p
 */
void BoundingBox::updateLower(const glm::vec3& p) {
   m_lower.x = std::fmin(p.x, m_lower.x);
   m_lower.y = std::fmin(p.y, m_lower.y);
   m_lower.z = std::fmin(p.z, m_lower.z);
   updateTranformedBounds();
   check();
}


/**
 * @brief BoundingBox::updateUpper
 * udpate the upper bound of the the bounding box according the input
 * @param p
 */
void BoundingBox::updateUpper(const glm::vec3& p) {
   m_upper.x = std::fmax(p.x, m_upper.x);
   m_upper.y = std::fmax(p.y, m_upper.y);
   m_upper.z = std::fmax(p.z, m_upper.z);
   updateTranformedBounds();
   check();
}


/**
 * @brief BoundingBox::getUpper
 * @return
 */
glm::vec3 BoundingBox::getUpper() {
   return m_upper;
}


/**
 * @brief BoundingBox::getLower
 * @return
 */
glm::vec3 BoundingBox::getLower() {
   return m_lower;
}


/**
 * @brief BoundingBox::hit
 * Check if two bounding boxes overlay with each other
 * @param p
 * @param d
 */
bool BoundingBox::overlap(const BoundingBox& box) {
   for (glm::vec4 x : box.m_corners) {
      if (inside(x)) {
         return true;
      }
   }
   return false;
}


bool BoundingBox::inside(const glm::vec4& x) {
   return hit(x, glm::vec4(1.0, 0, 0, 0)) < FLT_MAX &&
          hit(x, glm::vec4(-1.0, 0, 0, 0)) < FLT_MAX;
}


float BoundingBox::hit(const glm::vec4& p, const glm::vec4& d) {
   float t = FLT_MAX;


   // top
   float t1 = (m_upper.y - p.y) / d.y;
   float x  = p.x + t1 * d.x;
   float z  = p.z + t1 * d.z;

   // check if is within the range and update the nearest t
   if ((x <= m_upper.x) && (x >= m_lower.x) && (z <= m_upper.z) && (z >= m_lower.z)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }

   // bottom
   t1 = (m_lower.y - p.y) / d.y;
   x  = p.x + t1 * d.x;
   z  = p.z + t1 * d.z;
   // check if is within the range and update the nearest t
   if ((x <= m_upper.x) && (x >= m_lower.x) && (z <= m_upper.z) && (z >= m_lower.z)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }


   // left
   // x = m_lower, m_lower <= y <= m_upper, m_lower <= z <= m_upper, n = (-1, 0, 0)
   t1 = (m_lower.x - p.x) / d.x;
   float y = p.y + t1 * d.y;
   z = p.z + t1 * d.z;
   // check if is within the range and update the nearest t
   if ((y <= m_upper.y) && (y >= m_lower.y) && (z <= m_upper.z) && (z >= m_lower.z)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }


   // right
   // x = m_upper, m_lower <= y <= m_upper, m_lower <= z <= m_upper, n = (1, 0, 0)
   t1 = (m_upper.x - p.x) / d.x;
   y  = p.y + t1 * d.y;
   z  = p.z + t1 * d.z;
   if ((y <= m_upper.y) && (y >= m_lower.y) && (z <= m_upper.z) && (z >= m_lower.z)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }


   // front
   // z = m_upper, m_lower <= y <= m_upper, m_lower <= x <= m_upper, n = (0, 0, 1)
   t1 = (m_upper.z - p.z) / d.z;
   y  = p.y + t1 * d.y;
   x  = p.x + t1 * d.x;
   // check if is within the range and update the nearest t
   if ((y <= m_upper.y) && (y >= m_lower.y) && (x <= m_upper.x) && (x >= m_lower.x)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }


   // back
   // z = m_lower, m_lower <= y <= m_upper, m_lower <= x <= m_upper, n = (0, 0, -1)
   t1 = (m_lower.z - p.z) / d.z;
   y  = p.y + t1 * d.y;
   x  = p.x + t1 * d.x;
   // check if is within the range and update the nearest t
   if ((y <= m_upper.y) && (y >= m_lower.y) && (x <= m_upper.x) && (x >= m_lower.x)) {
      if ((t1 >= 0) && (t1 < t)) {
         t = t1;
      }
   }

   return t;
}
