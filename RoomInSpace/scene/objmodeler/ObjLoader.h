#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QMap>
#include <QOpenGLTexture>
#include "material.h"
#include "SceneObject.h"
class ObjLoader
{
public:
   ObjLoader();
   void loadObj(const QString& target, QVector<SceneObject>& results);

private:
   void loadMaterials(const QString& target);
   void parseVertices(const QString& target, QVector<SceneObject>& results);

   bool m_ignoreGroup = false;
   QMap<QString, QOpenGLTexture> m_textureMap;
   QMap<QString, Material> m_materialMap;
};

#endif // OBJLOADER_H
