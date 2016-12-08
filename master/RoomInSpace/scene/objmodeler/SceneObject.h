#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QString>
#include <string.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include <QElapsedTimer>
#include "BoundingBox.h"
enum ObjectType {
   SCENE_OBJECT,
   PRIMITIVE_OBJECT,
   GROUP_OBJECT,
   PRIMITIVE_IN_GROUP
};
class SceneObject
{
   friend class ObjLoader;
public:
   SceneObject();

   QString getName();
   glm::mat4x4 getModelMatrix();
   ObjectType getObjectType();

   void getBox(BoundingBox& box);
   void updateBox(const glm::vec3& p);
   void setName(const QString& str);
   void setModelMatrix(glm::mat4x4& mat);
   void setObjectType(ObjectType t);
   void setPickable(bool b);
   bool getPickable();

   virtual void free() {}

   virtual void draw(QOpenGLShaderProgram& shader,
                     QMap<QString, QOpenGLTexture *>& txtMap) {}

   virtual int getNumVertices() {
      return 0;
   }

protected:
   QString m_name;
   BoundingBox m_box;
   glm::mat4x4 m_modelMat;
   ObjectType m_type;
   bool m_pickable;
   QElapsedTimer m_timer;
};

#endif // SCENEOBJECT_H
