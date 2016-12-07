/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include <iostream>
#include "glm/ext.hpp"

CamtransCamera::CamtransCamera() : m_near(1.f),
   m_angleW(60),
   m_far(30.f),
   m_aspectRatio(1),
   m_angleH(60),
   m_position(glm::vec4(2, 2, 2, 1)),
   m_look(glm::vec4(0, 0, 0, 1) - m_position),
   m_up(glm::vec4(0, 1, 0, 0)) {
   makeCamera();
   updateMatrices();
}


void CamtransCamera::setAspectRatio(float a) {
   m_aspectRatio = a;
   updateMatrices();
}


glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
//   std::cout << "Projection matrix is  " << glm::to_string(m_projectionMatrix) << std::endl;
   return m_projectionMatrix;
}


glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
//   std::cout << "Projection matrix is  " << glm::to_string(m_projectionMatrix) << std::endl;
   return m_perspectiveMatrix;
}


glm::mat4x4 CamtransCamera::getViewMatrix() const {
//   std::cout << "View matrix is  " << glm::to_string(m_viewMatrix) << std::endl;
   return m_viewMatrix;
}


glm::mat4x4 CamtransCamera::getScaleMatrix() const {
   return m_scaleMatrix;
}


glm::vec4 CamtransCamera::getPosition() const {
   return m_position;
}


glm::vec4 CamtransCamera::getLook() const {
   return m_look;
}


glm::vec4 CamtransCamera::getUp() const {
   return m_up;
}


float CamtransCamera::getAspectRatio() const {
   return m_aspectRatio;
}


float CamtransCamera::getHeightAngle() const {
   return m_angleH;
}


void CamtransCamera::orientLook(const glm::vec4& eye, const glm::vec4& look, const glm::vec4& up) {
   m_position = glm::vec4(eye);
   m_look     = glm::vec4(look);
   m_up       = glm::vec4(up);
   makeCamera();
   updateMatrices();
}


void CamtransCamera::makeCamera() {
   m_w    = glm::vec4(glm::normalize(glm::vec3(-1.0f * m_look.x, -1.0f * m_look.y, -1.0f * m_look.z)), 0);
   m_v    = glm::vec4(glm::normalize(glm::vec3(m_up - glm::dot(m_up, m_w) * m_w)), 0);
   m_u    = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0);
   m_look = -1.0f * m_w;
   m_up   = m_v;
}


void CamtransCamera::setHeightAngle(float h) {
   m_angleH = h;
   updateMatrices();
}


void CamtransCamera::translate(const glm::vec4& v) {
   m_position += v;
   updateMatrices();
}


void CamtransCamera::rotateU(float degrees) {
   float theta = glm::radians(degrees);

   glm::vec4 m_w0 = glm::vec4(m_w);
   glm::vec4 m_v0 = glm::vec4(m_v);

   m_v = glm::sin(theta) * m_w0 + glm::cos(theta) * m_v0;
   m_w = glm::cos(theta) * m_w0 - glm::sin(theta) * m_v0;

   updateMatrices();
}


void CamtransCamera::rotateV(float degrees) {
   float theta = glm::radians(degrees);

   glm::vec4 m_w0 = glm::vec4(m_w);
   glm::vec4 m_u0 = glm::vec4(m_u);

   m_u = m_u0 * glm::cos(theta) - m_w0 * glm::sin(theta);
   m_w = m_u0 * glm::sin(theta) + m_w0 * glm::cos(theta);

   updateMatrices();
}


void CamtransCamera::rotateW(float degrees) {
   float theta = glm::radians(-1 * degrees);

   glm::vec4 m_v0 = glm::vec4(m_v);
   glm::vec4 m_u0 = glm::vec4(m_u);

   m_u = m_u0 * glm::cos(theta) - m_v0 * glm::sin(theta);
   m_v = m_u0 * glm::sin(theta) + m_v0 * glm::cos(theta);

   updateMatrices();
}


void CamtransCamera::setClip(float nearPlane, float farPlane) {
   m_near = nearPlane;
   m_far  = farPlane;

   updateMatrices();
}


void CamtransCamera::print() {
   std::cout << "u is " << glm::to_string(m_u) << std::endl;
   std::cout << "v is " << glm::to_string(m_v) << std::endl;
   std::cout << "w is " << glm::to_string(m_w) << std::endl;
}


void CamtransCamera::updateMatrices() {
   updateTransMatrix();
   updateRotMatrix();
   updateScaleMatrix();
   updatePerspectiveMatrix();
   updateProjectionMatrix();
   updateViewMatrix();
}


void CamtransCamera::updateProjectionMatrix() {
   m_projectionMatrix = m_perspectiveMatrix * m_scaleMatrix;
}


void CamtransCamera::updateViewMatrix() {
   m_viewMatrix = m_rotMatrix * m_transMatrix;
}


void CamtransCamera::updateTransMatrix() {
   glm::vec4 pn = m_position + -1.0f * m_near * m_w;
   m_transMatrix = glm::transpose(glm::mat4x4({ 1, 0, 0, -1.0f * pn.x,
                                                0, 1, 0, -1.0f * pn.y,
                                                0, 0, 1, -1.0f * pn.z,
                                                0, 0, 0, 1 }));
}


void CamtransCamera::updateRotMatrix() {
   m_rotMatrix = glm::transpose(glm::mat4x4({ m_u.x, m_u.y, m_u.z, 0,
                                              m_v.x, m_v.y, m_v.z, 0,
                                              m_w.x, m_w.y, m_w.z, 0,
                                              0, 0, 0, 1 }));
}


void CamtransCamera::updateScaleMatrix() {
   float thetaH = glm::radians(m_angleH);

   m_scaleMatrix = glm::transpose(glm::mat4x4({ 1.0f / (m_far * m_aspectRatio * glm::tan(thetaH / 2.0f)), 0, 0, 0,
                                                0, 1.0f / (m_far * glm::tan(thetaH / 2.0f)), 0, 0,
                                                0, 0, 1.0f / m_far, 0,
                                                0, 0, 0, 1 }));
}


void CamtransCamera::updatePerspectiveMatrix() {
   float c = -1.0f * m_near / m_far;

   m_perspectiveMatrix = glm::transpose(glm::mat4x4({ 1, 0, 0, 0,
                                                      0, 1, 0, 0,
                                                      0, 0, -1.0 / (1.0f + c), c / (1.0f + c),
                                                      0, 0, -1.0f, 0 }));
}
