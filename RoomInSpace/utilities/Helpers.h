#ifndef HELPERS_H
#define HELPERS_H
#include <QMatrix4x4>

struct Helpers
{
   QMatrix4x4 mat4x4ToQMatrix4x4(glm::mat4x4 mat) {
      return QMatrix4x4(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
                        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
                        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
                        mat[0][3], mat[1][3], mat[2][3], mat[3][3]
                        );
   }
};

Helpers helper;
#endif // HELPERS_H
