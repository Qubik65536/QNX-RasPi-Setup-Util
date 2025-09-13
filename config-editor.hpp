#ifndef CONFIG_EDITOR_HPP
#define CONFIG_EDITOR_HPP

#include <string>
#include <vector>
#include <utility>

/**
 * @brief A class for editing hierarchical configuration files with begin/end blocks
 *
 * This class provides functionality to load, modify, and save configuration files
 * that use a hierarchical structure with "begin" and "end" keywords to define sections.
 * It preserves formatting and indentation while allowing safe modifications.
 */
class ConfigEditor
{
private:
    std::vector<std::string> lines;

    /**
     * @brief Remove leading and trailing whitespace from a string
     * @param str Input string
     * @return Trimmed string
     */
    std::string trim(const std::string &str);

    /**
     * @brief Check if a line is a comment (starts with # or is empty)
     * @param line Input line
     * @return true if line is a comment, false otherwise
     */
    bool isComment(const std::string &line);

    /**
     * @brief Parse a key-value pair from a configuration line
     * @param line Input line containing key = value
     * @return Pair of key and value strings
     */
    std::pair<std::string, std::string> parseKeyValue(const std::string &line);

    /**
     * @brief Find the line index for a specific key within a section
     * @param sectionPath Vector of section names representing the path
     * @param key Key to search for
     * @return Line index if found, -1 if not found
     */
    int findKeyInSection(const std::vector<std::string> &sectionPath, const std::string &key);

    /**
     * @brief Find the end line of a section for insertion purposes
     * @param sectionPath Vector of section names representing the path
     * @return Line index of section end, -1 if section not found
     */
    int findSectionEnd(const std::vector<std::string> &sectionPath);

public:
    /**
     * @brief Default constructor
     */
    ConfigEditor() = default;

    /**
     * @brief Constructor that loads a file immediately
     * @param filename Path to the configuration file
     */
    explicit ConfigEditor(const std::string &filename);

    /**
     * @brief Destructor
     */
    ~ConfigEditor() = default;

    // Copy constructor and assignment operator
    ConfigEditor(const ConfigEditor &other) = default;
    ConfigEditor &operator=(const ConfigEditor &other) = default;

    // Move constructor and assignment operator
    ConfigEditor(ConfigEditor &&other) noexcept = default;
    ConfigEditor &operator=(ConfigEditor &&other) noexcept = default;

    /**
     * @brief Load a configuration file into memory
     * @param filename Path to the configuration file
     * @return true if successful, false if file could not be opened
     */
    bool loadFile(const std::string &filename);

    /**
     * @brief Save the current configuration to a file
     * @param filename Path where to save the configuration
     * @return true if successful, false if file could not be written
     */
    bool saveFile(const std::string &filename);

    /**
     * @brief Set or update a configuration value in a specific section
     * @param sectionPath Vector of section names representing the hierarchical path
     * @param key Configuration key name
     * @param value Configuration value
     * @return true if successful, false if section path not found
     *
     * @example
     * editor.setValue({"winmgr", "display 1"}, "video-mode", "1920 x 1080 @ 60");
     */
    bool setValue(const std::vector<std::string> &sectionPath,
                  const std::string &key,
                  const std::string &value);

    /**
     * @brief Get a configuration value from a specific section
     * @param sectionPath Vector of section names representing the hierarchical path
     * @param key Configuration key name
     * @return Configuration value if found, empty string if not found
     *
     * @example
     * std::string mode = editor.getValue({"winmgr", "display 1"}, "video-mode");
     */
    std::string getValue(const std::vector<std::string> &sectionPath, const std::string &key);

    /**
     * @brief Comment out a configuration line by adding # at the beginning
     * @param sectionPath Vector of section names representing the hierarchical path
     * @param key Configuration key name to comment out
     * @return true if successful, false if key not found
     */
    bool commentLine(const std::vector<std::string> &sectionPath, const std::string &key);

    /**
     * @brief Uncomment a configuration line by removing # from the beginning
     * @param sectionPath Vector of section names representing the hierarchical path
     * @param key Configuration key name to uncomment
     * @return true if successful, false if key not found
     */
    bool uncommentLine(const std::vector<std::string> &sectionPath, const std::string &key);

    /**
     * @brief Check if a key exists in a specific section
     * @param sectionPath Vector of section names representing the hierarchical path
     * @param key Configuration key name
     * @return true if key exists, false otherwise
     */
    bool keyExists(const std::vector<std::string> &sectionPath, const std::string &key);

    /**
     * @brief Get all keys in a specific section
     * @param sectionPath Vector of section names representing the hierarchical path
     * @return Vector of key names found in the section
     */
    std::vector<std::string> getKeysInSection(const std::vector<std::string> &sectionPath);

    /**
     * @brief Check if a section exists
     * @param sectionPath Vector of section names representing the hierarchical path
     * @return true if section exists, false otherwise
     */
    bool sectionExists(const std::vector<std::string> &sectionPath);

    /**
     * @brief Get the number of lines in the configuration
     * @return Number of lines
     */
    size_t getLineCount() const;

    /**
     * @brief Get a specific line by index
     * @param index Line index (0-based)
     * @return Line content if index is valid, empty string otherwise
     */
    std::string getLine(size_t index) const;

    /**
     * @brief Clear all loaded configuration data
     */
    void clear();

    /**
     * @brief Print the current configuration to stdout (for debugging)
     * @param showLineNumbers If true, show line numbers
     */
    void printConfig(bool showLineNumbers = false) const;
};

#endif // CONFIG_EDITOR_HPP
