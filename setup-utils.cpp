#include "setup-utils.hpp"
#include <iostream>

SetupUtils::SetupUtils(const std::string &configFilePath)
{
    path = configFilePath;   
    if (!configEditor.loadFile(path))
    {
        std::cerr << "Error: Unable to load configuration file: " << path << std::endl;
        exit(1);
    }
}

bool SetupUtils::saveConfig()
{
    if (!configEditor.saveFile(path))
    {
        std::cerr << "Error: Unable to save configuration file: " << path << std::endl;
        return false;
    }
    return true;
}

std::string SetupUtils::setKeyboardLayout(const std::string &layout){
    bool result = configEditor.setValue({"winmgr", "globals"}, "keymap", layout);
    if (!result)
    {
        std::cerr << "Error: Unable to set keyboard layout in configuration." << std::endl;
        exit(1);
    }
    return layout;
}
