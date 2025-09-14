#include "utf8-tui.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int UTF8TUI::run() { return dashboard(); }

int UTF8TUI::dashboard()
{
    using namespace ftxui;

    Element document = hbox({
        text("left") | border,
        text("middle") | border | flex,
        text("right") | border,
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    return 0;
}
