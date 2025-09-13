#include "first-run-utils.hpp"
#include "timezone-helper.hpp"
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

std::string FirstRunUtils::firstTimeSetupHostname()
{
    std::string hostname;
    std::cout << "Enter your preferred hostname (e.g., qnxpi): ";
    std::cin >> hostname;
    SetupUtils::setHostname(hostname);
    std::cout << "Hostname set to: " << hostname << std::endl;
    return hostname;
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

std::string FirstRunUtils::firstTimeSetupDisplay(SetupUtils &setupUtils)
{
    int width, height, refreshRate;
    std::cout << "Enter display width (e.g., 1920): ";
    std::cin >> width;
    std::cout << "Enter display height (e.g., 1080): ";
    std::cin >> height;
    std::cout << "Enter display refresh rate (e.g., 60): ";
    std::cin >> refreshRate;
    
    setupUtils.setDisplay(width, height, refreshRate);
    std::cout << "Display configuration set to: " 
              << width << "x" << height << " @ " << refreshRate << "Hz" << std::endl;
    return std::to_string(width) + "x" + std::to_string(height) + "@" + std::to_string(refreshRate) + "Hz";
}

std::string FirstRunUtils::firstTimeSetupTimezone()
{
    std::string timezone;
    bool valid = false;

    while(!valid)
    {
        std::cout << "Enter your preferred timezone (e.g., America/Toronto, UTC, +05:30): ";
        std::cin >> timezone;
        // Validate the timezone input.
        if (!TimezoneHelper::isValidTimezone(timezone))
        {
            std::cerr << "Invalid timezone format. Please try again." << std::endl;
            continue;
        }
        valid = true; // Mark as valid to exit the loop
    }

    std::string result = SetupUtils::setTimezone(timezone);
    std::cout << "Timezone set to: " << timezone << std::endl;
    return result;
}
