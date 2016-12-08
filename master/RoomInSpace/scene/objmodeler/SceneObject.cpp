#include "SceneObject.h"

SceneObject::SceneObject() : m_name(""),
   m_box(BoundingBox()),
   m_modelMat(glm::mat4x4()),
   m_type(SCENE_OBJECT),
   m_pickable(false),
   m_timer(QElapsedTimer()) {
   m_timer.start();
}


void SceneObject::setName(const QString& str) {
   m_name = str;
}


QString SceneObject::getName() {
   return m_name;
}


void SceneObject::updateBox(const glm::vec3& p) {
   m_box.updateBox(p);
}


void SceneObject::setObjectType(ObjectType t) {
   m_type = t;
}


ObjectType SceneObject::getObjectType() {
   return m_type;
}


glm::mat4x4 SceneObject::getModelMatrix() {
   return m_modelMat;
}


void SceneObject::setModelMatrix(glm::mat4x4& mat) {
   m_modelMat = mat;
}


void SceneObject::getBox(BoundingBox& box) {
   box = m_box;
}


void SceneObject::setPickable(bool b) {
   m_pickable = b;
}


bool SceneObject::getPickable() {
   return m_pickable;
}
