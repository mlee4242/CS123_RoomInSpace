#ifndef MATERIAL_H
#define MATERIAL_H
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <QString>
#include <iostream>
// using Qt library to avoid compatibility issues
struct Material
{
   QString   name;
   int       id;
   float     Ns;
   float     Ni;
   float     d;
   float     Tr;
   glm::vec3 Tf;
   float     illum;
   glm::vec3 Ka;
   glm::vec3 Kd;
   glm::vec3 Ks;
   glm::vec3 Ke;
   QString   map_Ka;
   QString   map_Kd;
   Material() {
      name   = "", id = -1;
      Ns     = 0, Ni = 0, d = 0, Tr = 0, Tf = glm::vec3(), illum = 0;
      Ka     = glm::vec3(), Kd = glm::vec3(), Ks = glm::vec3(), Ke = glm::vec3();
      map_Ka = "", map_Kd = "";
   }

   Material(QString& name_, int id_) {
      name   = name_, id = id_;
      Ns     = 0, Ni = 0, d = 0, Tr = 0, Tf = glm::vec3(), illum = 0;
      Ka     = glm::vec3(), Kd = glm::vec3(), Ks = glm::vec3(), Ke = glm::vec3();
      map_Ka = "", map_Kd = "";
   }

   Material(QString& name_, int id_, glm::vec3& a, glm::vec3& d, QString& map_a, QString& map_d) {
      name = name_, id = id_;
      Ka   = a, Ka = d, map_Ka = map_a, map_Kd = map_d;
   }

   void      print() {
      std::cout << "name is " << name.toStdString() << ", map_Ka is " << map_Ka.toStdString() << std::endl;
   }
};
#endif // MATERIAL_H
