#include "Helpers.h"
#include <iostream>
#include <math.h>
#include "glm/ext.hpp"
Helpers helper;

/**
 * @brief Helpers::mat4x4ToQMatrix4x4
 * @param mat
 * @return
 */
QMatrix4x4 Helpers::mat4x4ToQMatrix4x4(glm::mat4x4 mat) const {
   return QMatrix4x4(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                     mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                     mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                     mat[0][3], mat[1][3], mat[2][3], mat[3][3]
                     );
}

/**
 * @brief Helpers::between
 * @param e
 * @param a
 * @param b
 * @return
 */
bool Helpers::between(float e, float a, float b) {
   return e >= std::fmin(a, b) && e <= std::fmax(a, b);
}


/**
 * @brief Helpers::vrMatrixToGlmMatrixEyeHead
 * @param mat
 * @return
 */
glm::mat4x4 Helpers::vrMatrixToGlmMatrixEyeHead(const vr::HmdMatrix44_t& mat) {
   return glm::mat4x4(
      mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
      mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
      mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
      mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
      );
}

/**
 * @brief Helpers::vrMatrixToGlmMatrixEyeHead
 * @param mat
 * @return
 */
glm::mat4x4 Helpers::vrMatrixToGlmMatrixEyeHead(const vr::HmdMatrix34_t& mat) {
   return glm::mat4x4(
      mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
      mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
      mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
      mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
      );
}

/**
 * @brief Helpers::vrMatrixToGlmMatrixPose
 * @param mat
 * @return
 */
glm::mat4x4 Helpers::vrMatrixToGlmMatrixPose(const vr::HmdMatrix34_t& mat) {
   return glm::mat4x4(
      mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
      mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
      mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
      mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
      );
}

/**
 * @brief Helpers::vec3ToQVector3D
 * @param v
 * @return
 */
QVector3D Helpers::vec3ToQVector3D(glm::vec3 v) const {
   return QVector3D(v[0], v[1], v[2]);
}
