#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QString>
#include <string.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include "BoundingBox.h"
class SceneObject
{
   friend class ObjLoader;
public:
   SceneObject();

   QString getName();
   glm::mat4x4 getModelMatrix();
   void updateBox(const glm::vec3& p);
   void setName(const QString& str);
   void setModelMatrix(glm::mat4x4& mat);

   virtual void draw(QOpenGLShaderProgram& shader,
                     QMap<QString, QOpenGLTexture *>& txtMap) {}

   virtual int getNumVertices() {
      return 0;
   }

protected:
   QString m_name;
   BoundingBox m_box;
   glm::mat4x4 m_modelMat;
};

#endif // SCENEOBJECT_H
