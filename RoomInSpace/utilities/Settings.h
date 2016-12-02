#ifndef SETTINGS_H
#define SETTINGS_H

#include "glm/glm.hpp"

// Enumeration values for the Brush types from which the user can choose in the GUI.
enum RenderMode {
   VR_MODE,
   COMPUTER_MODE
};

struct Settings
{
   float cameraFov;              // The camera's field of view, which is twice the height angle.
   float cameraNear;             // The distance from the camera to the near clipping plane.
   float cameraFar;              // The distance from the camera to the far clipping plane.

   // Loads settings from disk, or fills in default values if no saved settings exist.
   void  loadSettingsOrDefaults();

   // Saves the current settings to disk.
   void  saveSettings();

   int   renderMode;
};
extern Settings settings;
#endif // SETTINGS_H
