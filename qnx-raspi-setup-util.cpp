#include "config-editor.hpp"
#include "first-run-utils.hpp"
#include "setup-utils.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <unistd.h>

const bool TESTING_MODE = false;

const std::string GRAPHICS_CONFIG_PATH = "/system/lib/graphics/rpi4-drm/graphics-rpi4.conf";
const std::string TEST_GRAPHICS_CONFIG_PATH = "test-graphics-rpi4.conf";

int main()
{
    // This program can only be run as root.
    if (!TESTING_MODE && geteuid() != 0)
    {
        std::cerr << "This program must be run as root. Please use su to switch to root user." << std::endl;
        return 1;
    }

    if (TESTING_MODE)
    {
        std::cout << "Running in TESTING MODE" << std::endl;
    }

    if (TESTING_MODE || FirstRunUtils::isFirstRun())
    {
        std::cout << "First time run detected. Performing initial setup..." << std::endl;

        // Set up hostname.
        std::string hostname = FirstRunUtils::firstTimeSetupHostname();
        std::cout << "Hostname set to: " << hostname << std::endl;

        // Initialize SetupUtils for configuring graphics config.
        SetupUtils setupUtils(TESTING_MODE ? TEST_GRAPHICS_CONFIG_PATH : GRAPHICS_CONFIG_PATH);
        
        // Set up keyboard layout.
        std::string keyboardLayout = FirstRunUtils::firstTimeSetupKeyboardLayout(setupUtils);
        // Set up display configuration.
        std::string displayConfig = FirstRunUtils::firstTimeSetupDisplay(setupUtils);

        // Save configuration changes.
        if (!setupUtils.saveConfig())
        {
            std::cerr << "Error: Failed to save configuration." << std::endl;
            exit(1);
        }

        // Set up timezone.
        std::string timezone = FirstRunUtils::firstTimeSetupTimezone();

        // Set up Wi-Fi configuration.
        std::string wifiConfig = FirstRunUtils::firstTimeSetupWifi();

        // TODO: Handle first-time setup tasks here.

        if (TESTING_MODE) exit(0); // Exit after first run setup in testing mode.

        // Reboot the RasPi to apply changes.
        std::cout << "You need to reboot the system for changes to take effect." << std::endl;
        std::cout << "Enter 'y' to reboot now, or any other key to exit without rebooting: ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            std::cout << "Rebooting..." << std::endl;
            // QNX uses 'shutdown' to power off the system.
            // The Raspberry Pi reboots automatically after shutdown.
            system("/system/bin/shutdown");
            exit(0);
        }
        else
        {
            std::cout << "Exiting without reboot. Please remember to reboot later." << std::endl;
        }
    }

    using namespace ftxui;

    Element document = hbox({
        text("left") | border,
        text("middle") | border | flex,
        text("right") | border,
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}
