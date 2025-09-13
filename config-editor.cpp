#include "config-editor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

ConfigEditor::ConfigEditor(const std::string &filename)
{
    loadFile(filename);
}

std::string ConfigEditor::trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool ConfigEditor::isComment(const std::string &line)
{
    std::string trimmed = trim(line);
    return trimmed.empty() || trimmed[0] == '#';
}

std::pair<std::string, std::string> ConfigEditor::parseKeyValue(const std::string &line)
{
    size_t pos = line.find('=');
    if (pos == std::string::npos)
    {
        return {"", ""};
    }
    std::string key = trim(line.substr(0, pos));
    std::string value = trim(line.substr(pos + 1));
    return {key, value};
}

int ConfigEditor::findKeyInSection(const std::vector<std::string> &sectionPath, const std::string &key)
{
    std::vector<std::string> currentPath;
    bool inTargetSection = false;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        std::string line = trim(lines[i]);

        if (isComment(line))
            continue;

        // Check for begin statement
        if (line.find("begin ") == 0)
        {
            std::string sectionName = trim(line.substr(6));
            currentPath.push_back(sectionName);
            inTargetSection = (currentPath == sectionPath);
        }
        // Check for end statement
        else if (line.find("end ") == 0)
        {
            if (!currentPath.empty())
            {
                currentPath.pop_back();
                inTargetSection = (currentPath == sectionPath);
            }
        }
        // Check for key-value pair
        else if (inTargetSection && line.find('=') != std::string::npos)
        {
            auto kvPair = parseKeyValue(line);
            if (kvPair.first == key)
            {
                return static_cast<int>(i);
            }
        }
    }

    return -1; // Key not found
}

int ConfigEditor::findSectionEnd(const std::vector<std::string> &sectionPath)
{
    std::vector<std::string> currentPath;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        std::string line = trim(lines[i]);

        if (isComment(line))
            continue;

        if (line.find("begin ") == 0)
        {
            std::string sectionName = trim(line.substr(6));
            currentPath.push_back(sectionName);
        }
        else if (line.find("end ") == 0)
        {
            if (currentPath == sectionPath)
            {
                return static_cast<int>(i); // Return the line just before "end"
            }
            if (!currentPath.empty())
            {
                currentPath.pop_back();
            }
        }
    }

    return -1; // Section not found
}

bool ConfigEditor::loadFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    lines.clear();
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();
    return true;
}

bool ConfigEditor::saveFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not write to file " << filename << std::endl;
        return false;
    }

    for (const auto &line : lines)
    {
        file << line << std::endl;
    }

    file.close();
    return true;
}

bool ConfigEditor::setValue(const std::vector<std::string> &sectionPath,
                            const std::string &key,
                            const std::string &value)
{

    int lineIndex = findKeyInSection(sectionPath, key);

    if (lineIndex != -1)
    {
        // Key exists, update it
        std::string originalLine = lines[lineIndex];
        std::string indent = "";

        // Preserve original indentation
        for (char c : originalLine)
        {
            if (c == ' ' || c == '\t')
            {
                indent += c;
            }
            else
            {
                break;
            }
        }

        lines[lineIndex] = indent + key + " = " + value;
        return true;
    }
    else
    {
        // Key doesn't exist, add it to the section
        int sectionEndIndex = findSectionEnd(sectionPath);
        if (sectionEndIndex != -1)
        {
            // Determine indentation by looking at other lines in the section
            std::string indent = "    "; // Default indentation

            // Try to match existing indentation in the section
            for (int i = sectionEndIndex - 1; i >= 0; --i)
            {
                std::string line = lines[i];
                if (!isComment(line) && line.find('=') != std::string::npos)
                {
                    indent = "";
                    for (char c : line)
                    {
                        if (c == ' ' || c == '\t')
                        {
                            indent += c;
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            }

            std::string newLine = indent + key + " = " + value;
            lines.insert(lines.begin() + sectionEndIndex, newLine);
            return true;
        }
    }

    std::cerr << "Error: Could not find section path" << std::endl;
    return false;
}

std::string ConfigEditor::getValue(const std::vector<std::string> &sectionPath, const std::string &key)
{
    int lineIndex = findKeyInSection(sectionPath, key);
    if (lineIndex != -1)
    {
        auto kvPair = parseKeyValue(lines[lineIndex]);
        return kvPair.second;
    }
    return ""; // Key not found
}

bool ConfigEditor::commentLine(const std::vector<std::string> &sectionPath, const std::string &key)
{
    int lineIndex = findKeyInSection(sectionPath, key);
    if (lineIndex != -1)
    {
        std::string &line = lines[lineIndex];
        if (!isComment(line))
        {
            // Find the first non-whitespace character and add # before it
            size_t firstChar = line.find_first_not_of(" \t");
            if (firstChar != std::string::npos)
            {
                line.insert(firstChar, "# ");
            }
        }
        return true;
    }
    return false;
}

bool ConfigEditor::uncommentLine(const std::vector<std::string> &sectionPath, const std::string &key)
{
    int lineIndex = findKeyInSection(sectionPath, key);
    if (lineIndex != -1)
    {
        std::string &line = lines[lineIndex];
        size_t hashPos = line.find('#');
        if (hashPos != std::string::npos)
        {
            // Remove # and following space if present
            size_t removeEnd = hashPos + 1;
            if (removeEnd < line.length() && line[removeEnd] == ' ')
            {
                removeEnd++;
            }
            line.erase(hashPos, removeEnd - hashPos);
        }
        return true;
    }
    return false;
}

bool ConfigEditor::keyExists(const std::vector<std::string> &sectionPath, const std::string &key)
{
    return findKeyInSection(sectionPath, key) != -1;
}

std::vector<std::string> ConfigEditor::getKeysInSection(const std::vector<std::string> &sectionPath)
{
    std::vector<std::string> keys;
    std::vector<std::string> currentPath;
    bool inTargetSection = false;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        std::string line = trim(lines[i]);

        if (isComment(line))
            continue;

        // Check for begin statement
        if (line.find("begin ") == 0)
        {
            std::string sectionName = trim(line.substr(6));
            currentPath.push_back(sectionName);
            inTargetSection = (currentPath == sectionPath);
        }
        // Check for end statement
        else if (line.find("end ") == 0)
        {
            if (!currentPath.empty())
            {
                currentPath.pop_back();
                inTargetSection = (currentPath == sectionPath);
            }
        }
        // Check for key-value pair
        else if (inTargetSection && line.find('=') != std::string::npos)
        {
            auto kvPair = parseKeyValue(line);
            if (!kvPair.first.empty())
            {
                keys.push_back(kvPair.first);
            }
        }
    }

    return keys;
}

bool ConfigEditor::sectionExists(const std::vector<std::string> &sectionPath)
{
    return findSectionEnd(sectionPath) != -1;
}

size_t ConfigEditor::getLineCount() const
{
    return lines.size();
}

std::string ConfigEditor::getLine(size_t index) const
{
    if (index < lines.size())
    {
        return lines[index];
    }
    return "";
}

void ConfigEditor::clear()
{
    lines.clear();
}

void ConfigEditor::printConfig(bool showLineNumbers) const
{
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (showLineNumbers)
        {
            std::cout << i << ": ";
        }
        std::cout << lines[i] << std::endl;
    }
}
