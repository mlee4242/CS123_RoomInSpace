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
   cameraFov  = s.value("cameraFov", 180).toDouble();
   cameraNear = s.value("cameraNear", -100).toDouble();
   cameraFar  = s.value("cameraFar", 100).toDouble();
   windowMode = s.value("windowMode", OverUnder).toInt();
}


void Settings::saveSettings() {
   QSettings s("Room in Space", "Room in Space");

   // Brush
   s.setValue("renderMode", renderMode);
   s.setValue("cameraFov", cameraFov);
   s.setValue("cameraNear", cameraNear);
   s.setValue("cameraFar", cameraFar);
   s.setValue("windowMode", windowMode);
}
