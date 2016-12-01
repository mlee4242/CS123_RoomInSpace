#ifndef KLEINBOTTLE_H
#define KLEINBOTTLE_H

#include "Shape.h"
#include "shape/surfaces/KleinBottleFace.h"


/**
 * @class KleinBottle
 * A Klein Bottle includes surfaces that makes of this bottle, called Klein Bottle Face
 */
class KleinBottle : public Shape
{
public:
   KleinBottle(Paramater para, const std::string name);
   KleinBottle(Paramater para, const std::string name, const glm::mat4& m_transMat);
   // I don't expect it's going to be a "deep clone"
   KleinBottle& operator=(const KleinBottle& other) = default;
   ~KleinBottle();
   void initFaces() override;
};


#endif // KLEINBOTTLE_H
