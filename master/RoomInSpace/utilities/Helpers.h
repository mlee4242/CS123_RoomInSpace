#ifndef HELPERS_H
#define HELPERS_H
#include <QMatrix4x4>
#include "glm/glm.hpp"
#include "openvr_mingw.hpp"
struct Helpers
{
   QMatrix4x4  mat4x4ToQMatrix4x4(glm::mat4x4 mat) const;

   QVector3D   vec3ToQVector3D(glm::vec3 v) const;

   glm::mat4x4 vrMatrixToGlmMatrixEyeHead(const vr::HmdMatrix44_t& mat);

   glm::mat4x4 vrMatrixToGlmMatrixEyeHead(const vr::HmdMatrix34_t& mat);

   glm::mat4x4 vrMatrixToGlmMatrixPose(const vr::HmdMatrix34_t& mat);

   bool        between(float e, float a, float b);
};

extern Helpers helper;
#endif // HELPERS_H
