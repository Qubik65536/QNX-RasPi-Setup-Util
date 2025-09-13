#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

/********************************************************/
/* Check if this is the first run of the program by    */
/* checking for the existence of a specific.           */
/* configuration file at ~/.config/qnx-raspi-setup     */
bool first_time_run() {
    const char* home = getenv("HOME");
    if (home == nullptr) {
        std::cerr << "Error: HOME environment variable not set." << std::endl;
        return true; // Assume first run if HOME is not set.
    }

    std::string config_path = std::string(home) + "/.config/qnx-raspi-setup";
    FILE* file = fopen(config_path.c_str(), "r");
    if (file) {
        // Configuration file exists, not the first run.
        fclose(file);
        return false;
    } else {
        // Configuration file does not exist, first run.
        
        // Ensure the .config directory exists.
        std::string config_dir = std::string(home) + "/.config";
        std::string mkdir_command = "mkdir -p " + config_dir;
        if (system(mkdir_command.c_str()) != 0) {
            std::cerr << "Error: Unable to create directory " << config_dir << std::endl;
            exit(1);
        }

        // Create the configuration file to mark that the program has been run.
        file = fopen(config_path.c_str(), "w");
        if (file) {
            fclose(file);
        } else {
            std::cerr << "Error: Unable to create configuration file at " << config_path << std::endl;
            exit(1);
        }

        return true;
    }
}

int main() {
  if (first_time_run()) {
      // TODO: Handle first-time setup tasks here.
  }

  using namespace ftxui;
 
  Element document = hbox({
    text("left")   | border,
    text("middle") | border | flex,
    text("right")  | border,
  });
 
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}
