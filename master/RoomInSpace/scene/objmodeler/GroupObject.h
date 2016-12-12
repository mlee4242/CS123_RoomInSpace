#ifndef GROUPOBJECT_H
#define GROUPOBJECT_H

#include <QVector>
#include "PrimitiveObject.h"

/**
 * @brief The GroupObject class
 *
 */
class GroupObject : public SceneObject
{
   friend class ObjLoader;
public:
   GroupObject();
   ~GroupObject();
   void addPrimitiveObject(PrimitiveObject *obj);
   void draw(QOpenGLShaderProgram& shader,
             QMap<QString, QOpenGLTexture *>& txtMap) override;

private:
   QVector<PrimitiveObject *> m_primObjects;
};

#endif // GROUPOBJECT_H
