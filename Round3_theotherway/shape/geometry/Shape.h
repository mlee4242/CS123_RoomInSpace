#ifndef SHAPE_H
#define SHAPE_H
#include <vector>
#include <string>
#include "shape/surfaces/Surface.h"

/**
 * @class Shape: a base calss
 * A shape is a geometry in 3D space. In general, a shape contains a couple of surfaces.
 * The number of surfaces could be one.
 * All the shape without transformation are centered at (0, 0, 0)
 */

class Shape {
public:
   Shape(Paramater para, const std::string name);

   // if transform the shape
   Shape(Paramater para, const std::string name, const glm::mat4& mat);

   // deconstructor
   virtual ~Shape() = default;

   // I don't expect it's going to be a "deep clone"
   virtual Shape& operator=(const Shape& other);

   // initialize all the faces that consists this shape
   virtual void initFaces() = 0;

   // draw the shape by drawing all the surfaces it has
   // the function is called by a scene object
   void draw();

   void sentToVAO(bool nofill);

   // change the settings of paramaters and compute all the faces again.
   virtual void setParameters(Paramater para);

   // get all the surfaces
   std::vector<std::shared_ptr<Surface> > getData() const;

protected:
   std::vector<std::shared_ptr<Surface> > m_faces; // all the surfaces the shape has
   std::string m_name;
   Paramater   m_para;                             // the parameters, such as length and width
   glm::mat4   m_transMat;                         // the tranfomation matrix if we need to transform the shape
   bool        m_ifTrans;                          // the flag of if we need to transform the shape
   bool        m_ifFacesInited;                    // if all the surfaces have been initialized
};


#endif // SHAPE_H
