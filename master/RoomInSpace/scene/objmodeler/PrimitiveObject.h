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
   ~PrimitiveObject();
   // this is not used in this program because we don't have vertices for each object
   void setVertices(const QVector<GLfloat>& verts);
   void setMaterial(const Material& mtl);
   // the offset from the first in the big vertices array
   void setOffset(int offset);
   // set how many tuples this object have (actually one vertice, 3 coordinates + 2 uv + 3 normals + ...)
   void setNumVertices(int num);
   int getNumVertices() override;
   // draw the object using the provide shader, textmap
   void draw(QOpenGLShaderProgram& shader,
             QMap<QString, QOpenGLTexture *>& txtMap) override;

protected:
   QVector<GLfloat> m_vertices;
   Material m_material;
   int m_offset;
   int m_numVertices;
private:
   int m_setted;
};

#endif // PRIMITIVEOBJECT_H
