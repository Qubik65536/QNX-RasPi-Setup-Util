#include "config-editor.hpp"
#include "first-run-utils.hpp"
#include "setup-utils.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

const bool TESTING_MODE = true;

const std::string GRAPHICS_CONFIG_PATH = "/system/lib/graphics/rpi4-drm/graphics-rpi4.conf";
const std::string TEST_GRAPHICS_CONFIG_PATH = "test-graphics-rpi4.conf";

int main()
{
    if (TESTING_MODE)
    {
        std::cout << "Running in TESTING MODE" << std::endl;
    }

    if (TESTING_MODE || FirstRunUtils::isFirstRun())
    {
        std::cout << "First time run detected. Performing initial setup..." << std::endl;

        // Create a temp folder to hold conf file.
        std::string temp_dir = "/tmp/qnx-raspi-setup";

        // Initialize SetupUtils for configuring graphics config.
        SetupUtils setupUtils(TESTING_MODE ? TEST_GRAPHICS_CONFIG_PATH : GRAPHICS_CONFIG_PATH);
        
        // Set up keyboard layout.
        std::string keyboardLayout = FirstRunUtils::firstTimeSetupKeyboardLayout(setupUtils);

        // Save configuration changes.
        if (!setupUtils.saveConfig())
        {
            std::cerr << "Error: Failed to save configuration." << std::endl;
            exit(1);
        }

        // TODO: Handle first-time setup tasks here.

        if (TESTING_MODE) exit(0); // Exit after first run setup in testing mode.
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
