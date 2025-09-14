#ifndef UTF8_TUI_HPP
#define UTF8_TUI_HPP

namespace UTF8TUI
{
    /**
     * @brief Run the UTF-8 TUI. Show the dashboard interface to start.
     * @return int Status Code (0 for success, non-zero for errors).
     */
    int run();

    /**
     * @brief The dashboard interface for TUI in UTF-8 supported terminal.
     * @return int Status Code (0 for success, non-zero for errors).
     */
    int dashboard();
}

#endif // UTF8_TUI_HPP
