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


int PrimitiveObject::getNumVertices() {
   return m_numVertices;
}


void PrimitiveObject::draw(QOpenGLShaderProgram& shader,
                           QMap<QString, QOpenGLTexture *>& txtMap) {
   shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
   if (m_material.map_Kd != "") {
      txtMap[m_material.map_Kd]->bind();
      shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
      glDrawArrays(GL_TRIANGLES, m_offset, m_numVertices);
      txtMap[m_material.map_Kd]->release();
   }else{
       shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
       glDrawArrays(GL_TRIANGLES, m_offset, m_numVertices);
   }
}
