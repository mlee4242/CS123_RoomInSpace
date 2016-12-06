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
   if (m_material.map_Kd != "") {
      shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
      shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
      txtMap[m_material.map_Kd]->bind();
      glDrawArrays(GL_TRIANGLES, m_offset, m_numVertices);

//   txtMap[m_material.map_Kd]->bind(0);
   }else{
      shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(m_modelMat));
      shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));
      std::cout << glm::to_string(m_material.Kd) << std::endl;
      glDrawArrays(GL_TRIANGLES, m_offset, m_numVertices);
   }
}
