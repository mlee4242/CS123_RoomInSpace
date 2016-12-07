#include "GroupObject.h"

GroupObject::GroupObject() : m_primObjects(QVector<PrimitiveObject *>())
{}

void GroupObject::addPrimitiveObject(PrimitiveObject *obj) {
   m_box.updateBox(obj->m_box);
   m_primObjects.push_back(obj);
}


void GroupObject::draw(QOpenGLShaderProgram& shader,
                       QMap<QString, QOpenGLTexture *>& txtMap) {
   for (PrimitiveObject *primObj : m_primObjects) {
      primObj->setModelMatrix(m_modelMat);
      primObj->draw(shader, txtMap);
   }
}
