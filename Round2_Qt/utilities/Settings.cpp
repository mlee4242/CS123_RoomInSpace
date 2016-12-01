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
}


void Settings::saveSettings() {
   QSettings s("Room in Space", "Room in Space");

   // Brush
   s.setValue("renderMode", renderMode);
}
