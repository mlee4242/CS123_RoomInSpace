#ifndef SETTINGS_H
#define SETTINGS_H


// Enumeration values for the Brush types from which the user can choose in the GUI.
enum RenderMode {
   VR_MODE,
   COMPUTER_MODE
};

struct Settings
{
   // Loads settings from disk, or fills in default values if no saved settings exist.
   void loadSettingsOrDefaults();

   // Saves the current settings to disk.
   void saveSettings();

   int  renderMode;
};
extern Settings settings;
#endif // SETTINGS_H
