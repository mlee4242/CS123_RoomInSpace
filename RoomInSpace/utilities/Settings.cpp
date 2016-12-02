#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
 * Loads the application settings, or, if no saved settings are available, loads default values for
 * the settings. You can change the defaults here.
 */
void Settings::loadSettingsOrDefaults() {
   QSettings s("Room in Space", "Room in Space");
   renderMode = s.value("renderMode", COMPUTER_MODE).toInt();
   cameraFov  = s.value("cameraFov", 150).toDouble();
   cameraNear = s.value("cameraNear", 0.1).toDouble();
   cameraFar  = s.value("cameraFar", -50).toDouble();
}


void Settings::saveSettings() {
   QSettings s("Room in Space", "Room in Space");

   // Brush
   s.setValue("renderMode", renderMode);
   s.setValue("cameraFov", cameraFov);
   s.setValue("cameraNear", cameraNear);
   s.setValue("cameraFar", cameraFar);
}
