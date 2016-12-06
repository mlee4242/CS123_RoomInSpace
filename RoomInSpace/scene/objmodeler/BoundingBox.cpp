#include "BoundingBox.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

/**
 * @brief BoundingBox::BoundingBox
 * initialize the bounding box to a infinite box
 */
BoundingBox::BoundingBox() :
   m_upper(glm::vec3(-FLT_MAX)), m_lower(glm::vec3(FLT_MAX)) {}

/**
 * @brief BoundingBox::BoundingBox, copy constructor
 * @param box
 */
BoundingBox::BoundingBox(const BoundingBox& box) :
   m_upper(glm::vec3(-FLT_MAX)), m_lower(glm::vec3(FLT_MAX)) {
   m_upper = box.m_upper;
   m_lower = box.m_lower;
}


BoundingBox::BoundingBox(const glm::vec3& upper, const glm::vec3& lower) :
   m_upper(upper), m_lower(lower) {}


/**
 * @brief BoundingBox::BoundingBox
 * compute the bounding box of a set of bounding boxes
 * @param boxes
 */
BoundingBox::BoundingBox(const std::vector<std::shared_ptr<BoundingBox> >& boxes) :
   m_upper(glm::vec4(-FLT_MAX)), m_lower(glm::vec4(FLT_MAX)) {
   for (auto box : boxes) {
      updateBox(box->m_lower);
      updateBox(box->m_upper);
   }
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
bool BoundingBox::hit(const BoundingBox& box) {
   return false;
}
