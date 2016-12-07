#include "PrimitiveObject.h"
#include <iostream>
#include <QOpenGLShaderProgram>
#include "Settings.h"
#include "Helpers.h"
PrimitiveObject::PrimitiveObject() : m_setted(0), m_modelMat(glm::mat4x4())
{}

void PrimitiveObject::setVertices(const QVector<GLfloat>& verts) {
   m_vertices    = verts;
   m_numVertices = verts.size();
   m_setted++;
}


void PrimitiveObject::setMaterial(const Material& mtl) {
   m_material = mtl;
   m_setted++;
}


void PrimitiveObject::setOffset(int offset) {
   m_offset = offset;
}


void PrimitiveObject::setNumVertices(int num) {
   m_numVertices = num;
}


int PrimitiveObject::getNumVertices() {
   return m_numVertices;
}


void PrimitiveObject::draw(QOpenGLShaderProgram& shader,
                           QMap<QString, QOpenGLTexture *>& txtMap) {
   if (m_material.map_Kd != "") {
      shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
      shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
      shader.setUniformValue("useTex", 1);
      txtMap[m_material.map_Kd]->bind();
      glDrawArrays(GL_TRIANGLES, m_offset / 8, m_numVertices / 8);
   }else{
      shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
      shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
      shader.setUniformValue("useTex", 0);
      glDrawArrays(GL_TRIANGLES, m_offset / 8, m_numVertices / 8);
   }
}
