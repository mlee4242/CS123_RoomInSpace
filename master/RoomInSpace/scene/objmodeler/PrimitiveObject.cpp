#include "PrimitiveObject.h"
#include <iostream>
#include <QOpenGLShaderProgram>
#include <QDateTime>
#include "Settings.h"
#include "Helpers.h"
#include <time.h>
#include <chrono>
#include "glm/ext.hpp"
PrimitiveObject::PrimitiveObject() : m_setted(0)
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
   glm::mat4x4 modelMat = m_modelMat;

   if (m_name.contains("UFO")) {
      shader.setUniformValue("move", QVector2D(1.f, float((m_timer.nsecsElapsed() / 3000000000.f))));
   }else{
      shader.setUniformValue("move", QVector2D(0.f, 0.f));
   }

//   if(m_name.contains("Controller")){
//       std::cout << "in draw " << glm::to_string(m_modelMat) << std::endl;
//   }

   shader.setUniformValue("m", helper.mat4x4ToQMatrix4x4(modelMat));
   shader.setUniformValue("isSky", m_name.contains("Sky"));
   shader.setUniformValue("isInside", !m_name.contains("Sky") && !m_name.contains("UFO"));
   shader.setUniformValue("pickable", m_pickable);
   shader.setUniformValue("diffuse", helper.vec3ToQVector3D(m_material.Kd));

   if (m_material.map_Kd != "") {
      shader.setUniformValue("useTex", 1);
      txtMap[m_material.map_Kd]->bind(0);
   }else{
      shader.setUniformValue("useTex", 0);
   }

   if (m_material.map_bump != "") {
//      GLint location = shader.uniformLocation("bumpMap");
      shader.setUniformValue("useBump", 1);
//      txtMap[m_material.map_bump]->bind(1);
   }else{
      shader.setUniformValue("useBump", 0);
   }

   if (m_material.map_normal != "") {
//      GLint location = shader.uniformLocation("normalMap");
      shader.setUniformValue("useNormal", 1);
//      txtMap[m_material.map_normal]->bind(2);
   }else{
      shader.setUniformValue("useNormal", 0);
   }
   glDrawArrays(GL_TRIANGLES, m_offset / 8, m_numVertices / 8);
}
