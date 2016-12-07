#ifndef PRIMITIVEOBJECT_H
#define PRIMITIVEOBJECT_H

#include <QVector>
#include <QGL>
#include <QString>
#include <QOpenGLTexture>
#include <memory>
#include "material.h"
#include "SceneObject.h"
#include "glm/glm.hpp"
class PrimitiveObject : public SceneObject
{
   friend class GroupObject;
public:
   PrimitiveObject();
   void setVertices(const QVector<GLfloat>& verts);
   void setMaterial(const Material& mtl);
   void setOffset(int offset);
   void setNumVertices(int num);
   int getNumVertices() override;
   void draw(QOpenGLShaderProgram& shader,
             QMap<QString, QOpenGLTexture *>& txtMap) override;

protected:
   QVector<GLfloat> m_vertices;
   Material m_material;
   int m_offset;
   int m_numVertices;

private:
   int m_setted;
   glm::mat4x4 m_modelMat;
};

#endif // PRIMITIVEOBJECT_H
