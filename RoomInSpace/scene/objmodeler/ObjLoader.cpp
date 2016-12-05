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

ObjLoader::ObjLoader() : m_textureMap(QMap<QString, QOpenGLTexture>()),
   m_materialMap(QMap<QString, Material>())
{}

void ObjLoader::loadObj(const QString& target, QVector<SceneObject>& results) {
   QString objPath = ":" + target + ".obj";
   QString mtlPath = ":" + target + ".mtl";
   loadMaterials(target);
}


void ObjLoader::loadMaterials(const QString& target) {
   QString mtlPath = ":" + target + ".mtl";
   QFile   mtlFile(mtlPath);
   if (!mtlFile.exists()) {
      std::cout << "Can't read the texture file (" << mtlPath.toStdString() << std::endl;
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
            QString mtlName = data[1];
            bool    nextMtl = false;
//            while (!nextMtl)
//            {}
         }
      }
   }
}


void ObjLoader::parseVertices(const QString& target, QVector<SceneObject>& results) {}
