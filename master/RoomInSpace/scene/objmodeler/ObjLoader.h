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
   ~ObjLoader();
   // load an obj file
   void loadObj(const QString&          target,
                QVector<SceneObject *>& results,
                QVector<GLfloat>&       cVerts);

   // get the texture map for the loaded obj files
   void getTextureMap(QVector<QString>& textMap);

private:
   // load the material file into m_materialMap
   void loadMaterials(const QString& target);

   // load the .obj file and parse all vertices into cVerts
   void parseVertices(const QString& target, QVector<GLfloat>& cVerts);

   // build the group object
   void buildGroups(QVector<SceneObject *>& results);

   // set up which object is pickable
   void setUpPickableList();

   QMap<QString, Material> m_materialMap;
   QVector<PrimitiveObject *> m_allObjs; // will be freed by
};

#endif // OBJLOADER_H
