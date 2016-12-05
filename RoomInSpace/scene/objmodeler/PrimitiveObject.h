#ifndef PRIMITIVEOBJECT_H
#define PRIMITIVEOBJECT_H

#include <QVector>
#include <QGL>
#include <QString>
#include <QOpenGLTexture>
#include <memory>
#include "material.h"
#include "SceneObject.h"
class PrimitiveObject : public SceneObject
{
public:
   PrimitiveObject();
protected:
   QVector<GLfloat> m_vertices;
   QString m_name;
   Material m_material;
   std::unique_ptr<QOpenGLTexture> m_texture;
};

#endif // PRIMITIVEOBJECT_H
