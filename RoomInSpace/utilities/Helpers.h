#ifndef HELPERS_H
#define HELPERS_H
#include <QMatrix4x4>
#include "glm/glm.hpp"
#include "openvr.h"
struct Helpers
{
   QMatrix4x4  mat4x4ToQMatrix4x4(glm::mat4x4 mat) const;

   glm::mat4x4 vrMatrixToGlmMatrix(const vr::HmdMatrix34_t& mat);

   glm::mat4x4 vrMatrixToGlmMatrix(const vr::HmdMatrix44_t& mat);
};

extern Helpers helper;
#endif // HELPERS_H
