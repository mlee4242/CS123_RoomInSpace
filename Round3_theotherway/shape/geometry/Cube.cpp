#include "Cube.h"
#include <iostream>

Cube::Cube(Paramater para, const std::string name)
   : Shape(para, name) {}


/**
 * @brief Cube::Cube
 * @param para
 * @param name
 * @param mat the transformation matrix if we need to transform the cone
 */
Cube::Cube(Paramater para, const std::string name, const glm::mat4& mat)
   : Shape(para, name, mat) {
   m_ifTrans = true;  // it has been initialized. this is to change the value.
}


Cube::~Cube() {}

/**
 * @brief Cube::initialize all six rectangles on this cube
 */
void Cube::initFaces() {
   // the front face by moving forward
   glm::mat4 mat1 = glm::translate(glm::vec3(0, 0, m_para.radius));
   // if the cube itself needs to be transformed, same to all the following statements
   if (m_ifTrans) { mat1 = m_transMat * mat1; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "front", mat1)));

   // the back face by flipping and moving backward
   glm::mat4 mat2 = glm::translate(glm::vec3(0, 0, -1 * m_para.radius)) * glm::rotate(glm::radians(180.0f), glm::vec3(1, 0, 0));
   if (m_ifTrans) { mat2 = m_transMat * mat2; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "back", mat2)));

   // the right face by rotating and moving right
   glm::mat4 mat3 = glm::translate(glm::vec3(m_para.radius, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
   if (m_ifTrans) { mat3 = m_transMat * mat3; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "right", mat3)));

   // the left face by rotating and moving left
   glm::mat4 mat4 = glm::translate(glm::vec3(-1 * m_para.radius, 0, 0)) * glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
   if (m_ifTrans) { mat4 = m_transMat * mat4; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "left", mat4)));

   // the bottom face by rotating and moving up
   glm::mat4 mat5 = glm::translate(glm::vec3(0, -1 * m_para.radius, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
   if (m_ifTrans) { mat5 = m_transMat * mat5; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "top", mat5)));

   // the top face by rotating and moving bottom
   glm::mat4 mat6 = glm::translate(glm::vec3(0, m_para.radius, 0)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
   if (m_ifTrans) { mat6 = m_transMat * mat6; }
   m_faces.push_back(std::shared_ptr<Rectangle>(new Rectangle(m_para, "down", mat6)));
}
