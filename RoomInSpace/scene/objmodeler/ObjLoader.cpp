#include "ObjLoader.h"
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QFile>
#include <QOpenGLTexture>
#include <iostream>
#include "SceneObject.h"
#include "PrimitiveObject.h"
#include "GroupObject.h"

ObjLoader::ObjLoader() : m_materialMap(QMap<QString, Material>())
{}

void ObjLoader::loadObj(const QString& target, QVector<SceneObject *>& results, QVector<GLfloat>& points) {
   loadMaterials(target);
   parseVertices(target, points);
   buildGroups(results);
}


void ObjLoader::getTextureMap(QVector<QString>& textMap) {
   for (Material m : m_materialMap) {
      if (!textMap.contains(m.map_Ka) && (m.map_Ka != "")) {
         textMap.push_back(m.map_Ka);
      }
      if (!textMap.contains(m.map_Kd) && (m.map_Kd != "")) {
         textMap.push_back(m.map_Kd);
      }
   }
}


void ObjLoader::loadMaterials(const QString& target) {
   QString mtlPath = ":/" + target + ".mtl";
   QFile   mtlFile(mtlPath);
   if (!mtlFile.exists()) {
      std::cout << "Can't read the texture file (" << mtlPath.toStdString() << ")" << std::endl;
      exit(1);
   }else if (mtlFile.open(QIODevice::ReadOnly)) {
      QTextStream in(&mtlFile);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         // removed whitespace from the start and the end
         // and that has each sequence of internal whitespace replaced with a single space.
         QStringList data = line.simplified().split(" ");
         if ((data.length() > 0) && (data[0] == "newmtl")) {
            QString  mtlName = data[1];
            Material mtl;
            mtl.name = mtlName;
            bool nextMtl = false;
            while (!nextMtl)
            {
               line = in.readLine();
               data = line.simplified().split(" ");

               if ((data.length() <= 0) || (data[0] == "")) { nextMtl = true; continue; }
               if (data[0] == "Ns") { mtl.Ns = data[1].toFloat();  }
               if (data[0] == "Ni") { mtl.Ni = data[1].toFloat(); }
               if (data[0] == "d") { mtl.d = data[1].toFloat();  }
               if (data[0] == "Tr") { mtl.d = data[1].toFloat(); }
               if (data[0] == "Tf") {
                  mtl.Tf = glm::vec3(data[1].toFloat(),
                                     data[2].toFloat(),
                                     data[3].toFloat());

               }
               if (data[0] == "illum") { mtl.illum = data[1].toInt(); continue; }
               if (data[0] == "Ka") {
                  mtl.Ka = glm::vec3(data[1].toFloat(),
                                     data[2].toFloat(),
                                     data[3].toFloat());

               }
               if (data[0] == "Kd") {
                  mtl.Kd = glm::vec3(data[1].toFloat(),
                                     data[2].toFloat(),
                                     data[3].toFloat());

               }
               if (data[0] == "Ks") {
                  mtl.Ks = glm::vec3(data[1].toFloat(),
                                     data[2].toFloat(),
                                     data[3].toFloat());

               }
               if (data[0] == "Ke") {
                  mtl.Ke = glm::vec3(data[1].toFloat(),
                                     data[2].toFloat(),
                                     data[3].toFloat());

               }
               if (data[0] == "map_Ka") { mtl.map_Ka = data[1];  }
               if (data[0] == "map_Kd") { mtl.map_Kd = data[1]; }
            }
            m_materialMap[mtlName] = mtl;
//                 std::cout << mtlName.toStdString() << std::endl;
         }
      }
   }
}


void ObjLoader::parseVertices(const QString& target, QVector<GLfloat>& cVerts) {
   QString objPath = ":/" + target + ".obj";
   QFile   objFile(objPath);
   if (!objFile.exists()) {
      std::cout << "Can't read the obj file (" << objPath.toStdString() << ")" << std::endl;
      exit(1);
   }else if (objFile.open(QIODevice::ReadOnly)) {
      QVector<GLfloat> verts, uvs, ns;
      QTextStream      in(&objFile);
      int              offset = 0;
      while (!in.atEnd())
      {
         QString     line = in.readLine();
         QStringList data = line.simplified().split(" ");
         if ((data.length() > 1) && (data[0] == "#") && (data[1] == "object")) {
            PrimitiveObject *obj = new PrimitiveObject();
            obj->m_offset = offset;
            obj->setName(QString(data[2]));
            bool nextObj = false;
            int count = 0;
            while (!nextObj)
            {
               line = in.readLine();
               data = line.simplified().split(" ");
               // # object o_Chair
               if ((data.length() > 0) && (data[0] == "v")) {
                  float v1 = data[1].toFloat(), v2 = data[2].toFloat(), v3 = data[3].toFloat();
                  verts.append(v1);
                  verts.append(v2);
                  verts.append(v3);
                  obj->updateBox(glm::vec3(v1, v2, v3));

               }else if ((data.length() > 0) && (data[0] == "vt")) {
                  uvs.append(data[1].toFloat());
                  uvs.append(data[2].toFloat());

               }else if ((data.length() > 0) && (data[0] == "vn")) {
                  ns.append(data[1].toFloat());
                  ns.append(data[2].toFloat());
                  ns.append(data[3].toFloat());

               }else if ((data.length() > 0) && (data[0] == "usemtl")) {
                  QString mtlName = data[1];
                  obj->setMaterial(m_materialMap[mtlName]);
               }else if ((data.length() > 0) && (data[0] == "f")) {
                  if (data.length() == 5) { // this is quad
                     int         tris[] = { 0, 1, 2, 0, 2, 3 };
                     QStringList row;

                     for (int i = 0; i < 6; i++) {
                        row = data[tris[i] + 1].split("/");
                        int v  = row[0].toInt() - 1;
                        int vt = row[1].toInt() - 1;
                        int n  = row[2].toInt() - 1;
                        for (int j = 0; j < 3; j++) {
                           cVerts.append(verts.at(v * 3 + j));
                           offset += 3;
                           count += 3;
                        }
                        for (int j = 0; j < 2; j++) {
                           cVerts.append(uvs.at(vt * 2 + j));
                           offset += 2;
                           count += 2;
                        }
                        for (int j = 0; j < 3; j++) {
                           cVerts.append(ns.at(n * 3 + j));
                           offset += 3;
                           count += 3;
                        }
                     }
                  }else {
                     QStringList row;
                     for (int i = 1; i < 4; i++) {
                        row = data[i].split("/");
                        int v  = row[0].toInt() - 1;
                        int vt = row[1].toInt() - 1;
                        int n  = row[2].toInt() - 1;
                        for (int j = 0; j < 3; j++) {
                           cVerts.append(verts.at(v * 3 + j));
                           offset += 3;
                           count += 3;
                        }
                        for (int j = 0; j < 2; j++) {
                           if (vt == -1) {
                              cVerts.append(0);
                           }else{ cVerts.append(uvs.at(vt * 2 + j));
                               offset += 2;
                               count += 2;}
                        }
                        for (int j = 0; j < 3; j++) {
                           if (n == -1) {
                              cVerts.append(0);
                           }else{ cVerts.append(ns.at(n * 3 + j));
                               offset += 3;
                               count += 3;}
                        }
                     }
                  }
                  // end of parsing one line of f ...
               }
               if ((data.length() > 1) && (data[0] == "#") && (data[2] == "faces")) {
                  nextObj = true;
               }
            } // end of parsing one obj
            std::cout << "parsed " << obj->getName().toStdString() << std::endl;
            obj->m_numVertices = count;
            m_allObjs.append(obj);
         }
      }
   }
}


void ObjLoader::buildGroups(QVector<SceneObject *>& results) {
   for (auto ptr : m_allObjs) {
      results.push_back(ptr);
   }
}
