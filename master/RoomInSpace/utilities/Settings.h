#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include "glm/glm.hpp"

// Enumeration values for the Brush types from which the user can choose in the GUI.
enum RenderMode {
   VR_MODE,
   COMPUTER_MODE
};

enum WindowMode {
   OverUnder,
   SideBySide,
   NUM_WINDOWS_TYPES
};

struct Settings
{
   float            cameraFov  = 40.f;       // The camera's field of view, which is twice the height angle.
   float            cameraNear = 0.1f;       // The distance from the camera to the near clipping plane.
   float            cameraFar  = 100.f;      // The distance from the camera to the far clipping plane.

   int              renderMode       = COMPUTER_MODE;
   int              windowMode       = OverUnder;
   int              VERTEX_UNIT_SIZE = 8;
   int              SAMPLES          = 4;

   QString          path    = "models/room/";
   QString          target  = "roominspace";
   bool             VRMode  = false;
   bool             lightOn = true;

   QVector<QString> pickableList;
   long int         frames = 0;
};
extern Settings settings;
#endif // SETTINGS_H
