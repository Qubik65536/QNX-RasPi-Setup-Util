#ifndef FIRST_RUN_UTILS_HPP
#define FIRST_RUN_UTILS_HPP

#include "setup-utils.hpp"
#include <string>

namespace FirstRunUtils
{
    /**
     * @brief Check if this is the first run of the program.
     *
     * This function checks for the existence of a configuration file
     * at ~/.config/qnx-raspi-setup. If the file does not exist,
     * it creates the file and returns true, indicating that this
     * is the first run. If the file exists, it returns false.
     */
    bool isFirstRun();

    /**
     * @brief Perform first-time setup for hostname.
     * 
     * This function allows user to set up their preferred hostname
     * through interactive prompts.
     * 
     * @return std::string The set up hostname.
     */
    std::string firstTimeSetupHostname();

    /**
     * @brief Perform first-time setup for keyboard layout.
     *
     * This function allows user to set up their preferred keyboard layout
     * through interactive prompts.
     * 
     * @param setupUtils Reference to SetupUtils instance for configuration management.
     *
     * @return std::string The set up keyboard layout.
     */
    std::string firstTimeSetupKeyboardLayout(SetupUtils &setupUtils);

    /**
     * @brief Perform first-time setup for display configuration.
     *
     * This function allows user to set up their preferred display settings
     * through interactive prompts.
     * 
     * @param setupUtils Reference to SetupUtils instance for configuration management.
     *
     * @return std::string The set up display configuration.
     */
    std::string firstTimeSetupDisplay(SetupUtils &setupUtils);

    /**
     * @brief Perform first-time setup for timezone.
     *
     * This function allows user to set up their preferred timezone
     * through interactive prompts.
     * 
     * @return std::string The set up timezone.
     */
    std::string firstTimeSetupTimezone();
}

#endif // FIRST_RUN_UTILS_HPP
