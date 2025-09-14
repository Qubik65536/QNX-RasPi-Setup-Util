#include "setup-utils.hpp"
#include <iostream>
#include <fstream>

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

bool SetupUtils::updateWifiConfig(const std::string &configPath,
                      const std::string &newSSID,
                      const std::string &newKeyMgmt,
                      const std::string &newPSK)
{

    std::ifstream inFile(configPath);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Cannot open config file: " << configPath << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    bool inNetworkBlock = false;
    bool foundNetwork = false;

    // Read all lines from the file
    while (std::getline(inFile, line))
    {
        lines.push_back(line);
    }
    inFile.close();

    // Process lines to update network configuration
    for (size_t i = 0; i < lines.size(); ++i)
    {
        std::string &currentLine = lines[i];

        // Check if we're entering a network block
        if (currentLine.find("network={") != std::string::npos)
        {
            inNetworkBlock = true;
            foundNetwork = true;
            continue;
        }

        // Check if we're exiting a network block
        if (inNetworkBlock && currentLine.find("}") != std::string::npos)
        {
            inNetworkBlock = false;
            continue;
        }

        // If we're in a network block, update the relevant fields
        if (inNetworkBlock)
        {
            // Update SSID
            if (currentLine.find("ssid=") != std::string::npos)
            {
                currentLine = "\tssid=\"" + newSSID + "\"";
            }
            // Update key_mgmt
            else if (currentLine.find("key_mgmt=") != std::string::npos)
            {
                currentLine = "\tkey_mgmt=" + newKeyMgmt;
            }
            // Update PSK
            else if (currentLine.find("psk=") != std::string::npos)
            {
                currentLine = "\tpsk=\"" + newPSK + "\"";
            }
        }
    }

    // If no network block was found, add one
    if (!foundNetwork)
    {
        lines.push_back("network={");
        lines.push_back("\tssid=\"" + newSSID + "\"");
        lines.push_back("\tkey_mgmt=" + newKeyMgmt);
        lines.push_back("\tpsk=\"" + newPSK + "\"");
        lines.push_back("}");
    }

    // Write the modified content back to the file
    std::ofstream outFile(configPath);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Cannot write to config file: " << configPath << std::endl;
        return false;
    }

    for (const auto &line : lines)
    {
        outFile << line << std::endl;
    }
    outFile.close();

    return true;
}
