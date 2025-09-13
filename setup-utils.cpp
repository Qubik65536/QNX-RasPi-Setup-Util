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

std::string SetupUtils::setDisplay(
    const int width, const int height, const int refreshRate,
    const int stackSize, const bool forceComposition, const bool cursor
)
{
    std::string videoMode = std::to_string(width) + " x " + std::to_string(height) + " @ " + std::to_string(refreshRate);
    bool result = configEditor.setValue({"winmgr", "display 1"}, "video-mode", videoMode);
    if (!result)
    {
        std::cerr << "Error: Unable to set video mode in configuration." << std::endl;
        exit(1);
    }

    result = configEditor.setValue({"winmgr", "display 1"}, "stack-size", std::to_string(stackSize));
    if (!result)
    {
        std::cerr << "Error: Unable to set stack size in configuration." << std::endl;
        exit(1);
    }

    result = configEditor.setValue({"winmgr", "display 1"}, "force-composition", forceComposition ? "true" : "false");
    if (!result)
    {
        std::cerr << "Error: Unable to set force-composition in configuration." << std::endl;
        exit(1);
    }

    // Note: The configuration uses 'on'/'off' for cursor setting.
    result = configEditor.setValue({"winmgr", "display 1"}, "cursor", cursor ? "on" : "off");
    if (!result)
    {
        std::cerr << "Error: Unable to set cursor in configuration." << std::endl;
        exit(1);
    }

    return videoMode + ", stack-size=" + std::to_string(stackSize) +
           ", force-composition=" + (forceComposition ? "true" : "false") +
           ", cursor=" + (cursor ? "on" : "off");
}
