#include "timezone-helper.hpp"
#include <string>
#include <fstream>
#include <regex>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <algorithm>

bool TimezoneHelper::internal::fileExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}

bool TimezoneHelper::internal::isTimezoneFile(const std::string &filepath)
{
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    // Check for TZif magic bytes
    char magic[4];
    file.read(magic, 4);
    return (file.gcount() == 4 &&
            magic[0] == 'T' && magic[1] == 'Z' &&
            magic[2] == 'i' && magic[3] == 'f');
}

bool TimezoneHelper::isValidTimezone(const std::string &timezone)
{
    // Handle empty or excessively long strings
    if (timezone.empty() || timezone.length() > 100)
    {
        return false;
    }

    // Check for path traversal attempts
    if (timezone.find("..") != std::string::npos ||
        timezone.find("/./") != std::string::npos ||
        timezone[0] == '/' || timezone[0] == '.')
    {
        return false;
    }

    // Special cases that are always valid
    if (timezone == "UTC" || timezone == "GMT")
    {
        return true;
    }

    // Check if timezone file exists in system zoneinfo directory
    const std::string zoneinfoPath = "/usr/share/zoneinfo/" + timezone;

    if (TimezoneHelper::internal::fileExists(zoneinfoPath) && TimezoneHelper::internal::isTimezoneFile(zoneinfoPath))
    {
        return true;
    }

    // Validate UTC offset format (±HH:MM, ±HHMM, ±HH)
    std::regex offsetPattern("^[+-](?:0[0-9]|1[0-4])(?::[0-5][0-9]|[0-5][0-9])?$");
    if (std::regex_match(timezone, offsetPattern))
    {
        return true;
    }

    // Validate GMT offset format (GMT±HH:MM, GMT±HHMM, GMT±HH)
    std::regex gmtPattern("^GMT[+-](?:0[0-9]|1[0-4])(?::[0-5][0-9]|[0-5][0-9])?$");
    if (std::regex_match(timezone, gmtPattern))
    {
        return true;
    }

    return false;
}

void TimezoneHelper::internal::scanDirectory(const std::string &basePath, const std::string &currentPath,
                                             std::vector<std::string> &timezones)
{
    std::string fullPath = basePath + "/" + currentPath;
    DIR *dir = opendir(fullPath.c_str());
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // Skip . and .. and hidden files

        std::string entryName = entry->d_name;
        std::string entryPath = fullPath + "/" + entryName;
        std::string relativePath = currentPath.empty() ? entryName : currentPath + "/" + entryName;

        struct stat statbuf;
        if (stat(entryPath.c_str(), &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                // Skip certain directories
                if (entryName != "posix" && entryName != "right")
                {
                    scanDirectory(basePath, relativePath, timezones);
                }
            }
            else if (S_ISREG(statbuf.st_mode))
            {
                // Skip certain files
                if (entryName != "iso3166.tab" &&
                    entryName != "zone.tab" &&
                    entryName != "zone1970.tab" &&
                    entryName != "tzdata.zi" &&
                    entryName != "leapseconds")
                {

                    if (isTimezoneFile(entryPath))
                    {
                        timezones.push_back(relativePath);
                    }
                }
            }
        }
    }
    closedir(dir);
}

std::vector<std::string> TimezoneHelper::getAvailableTimezones()
{
    std::vector<std::string> timezones;
    const std::string zoneinfoDir = "/usr/share/zoneinfo";

    TimezoneHelper::internal::scanDirectory(zoneinfoDir, "", timezones);
    std::sort(timezones.begin(), timezones.end());
    return timezones;
}

bool TimezoneHelper::isValidTimezoneNoRegex(const std::string &timezone)
{
    // Handle empty or excessively long strings
    if (timezone.empty() || timezone.length() > 100)
    {
        return false;
    }

    // Check for path traversal attempts
    if (timezone.find("..") != std::string::npos ||
        timezone.find("/./") != std::string::npos ||
        timezone[0] == '/' || timezone[0] == '.')
    {
        return false;
    }

    // Special cases
    if (timezone == "UTC" || timezone == "GMT")
    {
        return true;
    }

    // Check system timezone file
    const std::string zoneinfoPath = "/usr/share/zoneinfo/" + timezone;
    if (TimezoneHelper::internal::fileExists(zoneinfoPath) && TimezoneHelper::internal::isTimezoneFile(zoneinfoPath))
    {
        return true;
    }

    // Manual validation of UTC offset format
    if (timezone.length() >= 3 && (timezone[0] == '+' || timezone[0] == '-'))
    {
        // Simple offset validation without regex
        std::string offset = timezone.substr(1);
        if (offset.length() == 2 || offset.length() == 4 || offset.length() == 5)
        {
            // Basic hour validation
            if (offset.length() >= 2)
            {
                int hour = (offset[0] - '0') * 10 + (offset[1] - '0');
                if (hour <= 14)
                {
                    return true;
                }
            }
        }
    }

    // GMT offset validation
    if (timezone.length() >= 6 && timezone.substr(0, 3) == "GMT" &&
        (timezone[3] == '+' || timezone[3] == '-'))
    {
        std::string offset = timezone.substr(4);
        if (offset.length() == 2 || offset.length() == 4 || offset.length() == 5)
        {
            if (offset.length() >= 2)
            {
                int hour = (offset[0] - '0') * 10 + (offset[1] - '0');
                if (hour <= 14)
                {
                    return true;
                }
            }
        }
    }

    return false;
}
