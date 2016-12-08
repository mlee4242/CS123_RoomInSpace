#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QMap>
#include <QOpenGLTexture>
#include "material.h"
#include "PrimitiveObject.h"
#include "SceneObject.h"
class ObjLoader
{
public:
   ObjLoader();
   void loadObj(const QString& target, QVector<SceneObject *>& results, QVector<GLfloat>& cVerts);
   void getTextureMap(QVector<QString>& textMap);

private:
   void loadMaterials(const QString& target);
   void parseVertices(const QString& target, QVector<GLfloat>& cVerts);
   void buildGroups(QVector<SceneObject *>& results);
   void setUpPickableList();

   bool m_ignoreGroup = false;
   QMap<QString, Material> m_materialMap;
   QVector<PrimitiveObject *> m_allObjs;
};

#endif // OBJLOADER_H
