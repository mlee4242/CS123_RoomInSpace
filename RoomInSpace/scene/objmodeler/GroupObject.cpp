#include "GroupObject.h"

GroupObject::GroupObject() : m_primObjects(QVector<PrimitiveObject *>())
{}

void GroupObject::addPrimitiveObject(PrimitiveObject *obj) {
   m_box.updateBox(obj->m_box);
   m_primObjects.push_back(obj);
}
