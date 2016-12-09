#ifndef SETTINGS_H
#define SETTINGS_H
#include <QString>
#include <QVector>
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
   float            cameraFov;        // The camera's field of view, which is twice the height angle.
   float            cameraNear;       // The distance from the camera to the near clipping plane.
   float            cameraFar;        // The distance from the camera to the far clipping plane.

   // Loads settings from disk, or fills in default values if no saved settings exist.
   void             loadSettingsOrDefaults();

   // Saves the current settings to disk.
   void             saveSettings();

   int              renderMode;
   int              windowMode;

   QString          path    = "models/room/";
   QString          target  = "roominspace";
   bool             VRMode  = true;
   bool             lightOn = true;

   QVector<QString> pickableList;
   long int         frames = 0;
};
extern Settings settings;
#endif // SETTINGS_H
