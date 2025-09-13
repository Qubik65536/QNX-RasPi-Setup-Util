#include "first-run-utils.hpp"
#include <iostream>

bool FirstRunUtils::isFirstRun()
{
    std::string config_path = "/data/home/root/.config/qnx-raspi-setup";
    FILE *file = fopen(config_path.c_str(), "r");
    if (file)
    {
        // Configuration file exists, not the first run.
        fclose(file);
        return false;
    }
    else
    {
        // Configuration file does not exist, first run.

        // Ensure the .config directory exists.
        std::string config_dir = "/data/home/root/.config";
        std::string mkdir_command = "mkdir -p " + config_dir;
        if (system(mkdir_command.c_str()) != 0)
        {
            std::cerr << "Error: Unable to create directory " << config_dir << std::endl;
            exit(1);
        }

        // Create the configuration file to mark that the program has been run.
        file = fopen(config_path.c_str(), "w");
        if (file)
        {
            fclose(file);
        }
        else
        {
            std::cerr << "Error: Unable to create configuration file at " << config_path << std::endl;
            exit(1);
        }

        return true;
    }
}

std::string FirstRunUtils::firstTimeSetupKeyboardLayout(SetupUtils &setupUtils)
{
    // Display available keyboard layouts.
    std::vector<std::string> layouts = setupUtils.getAvailableKeyboardLayouts();
    std::cout << "Available Keyboard Layouts:" << std::endl;
    for (size_t i = 0; i < layouts.size(); ++i)
    {
        std::cout << i + 1 << ". " << layouts[i] << std::endl;
    }
    std::cout << "Select your preferred keyboard layout (1-" << layouts.size() << "): ";
    size_t choice = 0;
    std::cin >> choice;
    if (choice < 1 || choice > layouts.size())
    {
        std::cerr << "Invalid choice. Please run the setup again." << std::endl;
        std::cerr << "Exiting..." << std::endl;
        exit(1);
    }
    std::string selectedLayout = layouts[choice - 1];
    setupUtils.setKeyboardLayout(selectedLayout);
    std::cout << "Keyboard layout set to: " << selectedLayout << std::endl;
    return selectedLayout;
}
