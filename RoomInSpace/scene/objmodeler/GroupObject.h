#ifndef GROUPOBJECT_H
#define GROUPOBJECT_H

#include <QVector>
// using Qt library to avoid compatibility issues
#include "PrimitiveObject.h"

class GroupObject : public SceneObject
{
public:
   GroupObject();
protected:
   QVector<PrimitiveObject> m_primObjects;
};

#endif // GROUPOBJECT_H
