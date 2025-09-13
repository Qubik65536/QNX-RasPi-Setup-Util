#ifndef SETUP_UTILS_HPP
#define SETUP_UTILS_HPP

#include "config-editor.hpp"
#include <iostream>

class SetupUtils
{
private:
    /**
     * @brief Path to the configuration file.
     */
    std::string path;

    /**
     * @brief Configuration editor instance for managing the config file.
     */
    ConfigEditor configEditor;

public:
    /**
     * @brief Constructor that initializes the setup utility with a configuration file path.
     * @param configFilePath The path to the configuration file.
     */
    explicit SetupUtils(const std::string &configFilePath);

    /**
     * @brief Destructor
     */
    ~SetupUtils() = default;

    /**
     * @brief Save the configuration changes.
     * @return true if the configuration was saved successfully, false otherwise.
     */
    bool saveConfig();

    /**
     * @brief Set the system hostname.
     * @param hostname The desired hostname.
     * @return The set hostname.
     */
    static std::string setHostname(const std::string &hostname)
    {
        // For QNX, simply write "HOSTNAME=new_hostname" to /boot/network
        std::string networkConfigPath = "/boot/network";
        FILE *file = fopen(networkConfigPath.c_str(), "a");
        if (file)
        {
            fprintf(file, "HOSTNAME=%s\n", hostname.c_str());
            fclose(file);
            return hostname;
        }
        else
        {
            std::cerr << "Error: Unable to open network configuration file: " << networkConfigPath << std::endl;
            exit(1);
        }
    }

    /**
     * @brief Get a list of available keyboard layouts.
     * @return A vector of strings representing available keyboard layouts.
     */
    std::vector<std::string> getAvailableKeyboardLayouts() const
    {
        return {
            "cs_CZ_102",
            "da_DK_102",
            "de_CH_102",
            "de_DE_102",
            "en_CA_101",
            "en_CA_101_dvorak",
            "en_GB_102",
            "en_US_101",
            "en_US_101_dvorak",
            "es_ES_102",
            "fr_BE_102",
            "fr_CA_102",
            "fr_CH_102",
            "fr_FR_102",
            "hr_HR_102",
            "it_IT_102",
            "ja_JP_106",
            "nl_NL_102",
            "no_NO_102",
            "pl_PL_102",
            "pt_PT_102",
            "se_SE_102",
            "sk_SK_102"};
    }

    /**
     * @brief Set the keyboard layout in the configuration.
     * @param layout The keyboard layout to set (e.g., `en_CA_101`, `fr_CA_102`).
     * @return std::string The set keyboard layout.
     */
    std::string setKeyboardLayout(const std::string &layout);

    /**
     * @brief Set the display configuration.
     * @param width The display width in pixels (e.g., 1920).
     * @param height The display height in pixels (e.g., 1080).
     * @param refreshRate The display refresh rate in Hz (e.g., 60).
     * @param stackSize The stack size for the display server (default: 65536).
     * @param forceComposition Whether to force composition (default: true).
     * @param cursor Whether to enable the cursor (default: true, which is to be 'on' in the actual configuration).
     * @return std::string The set display configuration.
     */
    std::string setDisplay(
        const int width, const int height, const int refreshRate,
        const int stackSize = 65536, const bool forceComposition = true, const bool cursor = true
    );
};

#endif // SETUP_UTILS_HPP
