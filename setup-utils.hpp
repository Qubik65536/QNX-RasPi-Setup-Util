#ifndef SETUP_UTILS_HPP
#define SETUP_UTILS_HPP

#include "config-editor.hpp"
#include <string>
#include <vector>

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
            "sk_SK_102"
        };
    }

    /**
     * @brief Set the keyboard layout in the configuration.
     * @param layout The keyboard layout to set (e.g., `en_CA_101`, `fr_CA_102`).   
     * @return std::string The set keyboard layout.
     */
    std::string setKeyboardLayout(const std::string &layout);
};

#endif // SETUP_UTILS_HPP
