#include "setup-utils.hpp"
#include "utf8-tui.hpp"
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/util/ref.hpp>

int UTF8TUI::run() { return dashboard(); }

int UTF8TUI::dashboard()
{
    using namespace ftxui;

    std::string hostname = SetupUtils::getHostname();
    std::string username = getenv("USER");

    std::string option;
    Component inputOption = Input(&option, "option");
    // If Enter is pressed, we consider the input complete.
    inputOption |= CatchEvent([&](Event event)
                              { if (event == Event::Return) {
                                        switch (option[0])
                                        {
                                        case '1':
                                            break;
                                        case '2':
                                            break;
                                        case '3':
                                            break;
                                        case '4':
                                            break;
                                        case 'q':
                                        case 'Q':
                                            ScreenInteractive::TerminalOutput().ExitLoopClosure();
                                            std::cout << std::endl << std::endl << std::endl <<
                                                "Thank you for using the QNX Raspberry Pi Setup Utility!" << std::endl;
                                            exit(0);
                                        }
                                        return true;
                                        }
                                        return false; });

    // The component tree.
    auto component = Container::Horizontal({
        inputOption,
    });

    auto renderer = Renderer(component, [&]
                             { return vbox({vbox({
                                                text("Welcome back to " + hostname + ", " + username + "!") | bold,
                                                separator(),
                                                text("This is the dashboard of the QNX Raspberry Pi Setup Utility."),
                                                text("Use the menu below to navigate through different setup options."),
                                                separator(),
                                                text("1. Network Settings"),
                                                text("2. Display Settings"),
                                                text("3. Keyboard Layout"),
                                                text("4. Timezone Configuration"),
                                                separator(),
                                                text("Press 'q' to exit the setup utility.") | dim,
                                            }) | border,
                                            hbox({
                                                text("Select an option (1-4) or 'q' to quit: ") | bold,
                                                inputOption->Render(),
                                            }) | border}); });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}
