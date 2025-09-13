#ifndef TIMEZONE_HELPER_HPP
#define TIMEZONE_HELPER_HPP

#include <string>
#include <vector>

namespace TimezoneHelper
{
    /**
     * @brief Checks if a timezone identifier is valid
     *
     * This function validates timezone identifiers by:
     * 1. Checking against system timezone files in /usr/share/zoneinfo/
     * 2. Validating TZif magic bytes in timezone files
     * 3. Supporting UTC/GMT offset formats (e.g., "+05:30", "GMT-08:00")
     * 4. Preventing path traversal attacks
     *
     * @param timezone The timezone identifier to validate (e.g., "America/Toronto", "UTC", "+05:30")
     * @return true if the timezone is valid, false otherwise
     *
     * @example
     * @code
     * bool isValid = TimezoneValidator::isValidTimezone("America/Toronto"); // returns true
     * bool isInvalid = TimezoneValidator::isValidTimezone("Invalid/Zone");   // returns false
     * @endcode
     */
    bool isValidTimezone(const std::string &timezone);

    /**
     * @brief Alternative timezone validation without regex dependency
     *
     * This function provides the same validation as isValidTimezone() but without
     * using std::regex, making it compatible with older C++ compilers or systems
     * where regex support is not available.
     *
     * @param timezone The timezone identifier to validate
     * @return true if the timezone is valid, false otherwise
     */
    bool isValidTimezoneNoRegex(const std::string &timezone);

    /**
     * @brief Retrieves all available timezone identifiers from the system
     *
     * Scans the system timezone database (/usr/share/zoneinfo/) and returns
     * a sorted list of all valid timezone identifiers. This function recursively
     * searches through subdirectories and validates each file by checking for
     * TZif magic bytes.
     *
     * @return std::vector<std::string> containing all valid timezone identifiers
     *
     * @note This function may be slow on first call as it scans the entire
     *       timezone database. Consider caching the results if called frequently.
     *
     * @example
     * @code
     * std::vector<std::string> zones = TimezoneValidator::getAvailableTimezones();
     * for (const auto& zone : zones) {
     *     std::cout << zone << std::endl;
     * }
     * @endcode
     */
    std::vector<std::string> getAvailableTimezones();

    /**
     * @brief Checks if a timezone identifier represents a UTC offset
     *
     * Determines if the given timezone string represents a fixed UTC offset
     * (e.g., "+05:30", "-08:00", "GMT+01:00") rather than a named timezone.
     *
     * @param timezone The timezone identifier to check
     * @return true if it's a UTC offset format, false otherwise
     *
     * @example
     * @code
     * bool isOffset1 = TimezoneValidator::isUTCOffset("+05:30");        // returns true
     * bool isOffset2 = TimezoneValidator::isUTCOffset("GMT-08:00");     // returns true
     * bool isOffset3 = TimezoneValidator::isUTCOffset("America/Denver"); // returns false
     * @endcode
     */
    bool isUTCOffset(const std::string &timezone);

    /**
     * @brief Gets the system's default timezone
     *
     * Attempts to determine the system's current timezone by checking:
     * 1. The /etc/localtime symlink (on most Linux systems)
     * 2. The TZ environment variable
     * 3. Other system-specific methods
     *
     * @return std::string containing the system's default timezone identifier,
     *         or "UTC" if unable to determine
     *
     * @example
     * @code
     * std::string systemTz = TimezoneValidator::getSystemTimezone();
     * std::cout << "System timezone: " << systemTz << std::endl;
     * @endcode
     */
    std::string getSystemTimezone();

    /**
     * @brief Exception class for timezone validation errors
     */
    class TimezoneValidationError : public std::exception
    {
    private:
        std::string message;

    public:
        explicit TimezoneValidationError(const std::string &msg) : message(msg) {}
        virtual const char *what() const throw()
        {
            return message.c_str();
        }
    };

    // Internal helper functions (not part of public API)
    namespace internal
    {
        /**
         * @brief Checks if a file exists and is a regular file
         * @param path The file path to check
         * @return true if file exists and is regular, false otherwise
         */
        bool fileExists(const std::string &path);

        /**
         * @brief Validates if a file is a proper timezone file by checking TZif magic bytes
         * @param filepath The path to the file to validate
         * @return true if file contains valid timezone data, false otherwise
         */
        bool isTimezoneFile(const std::string &filepath);

        /**
         * @brief Recursively scans directory for timezone files
         * @param basePath The base directory path (/usr/share/zoneinfo)
         * @param currentPath The current relative path being scanned
         * @param timezones Vector to store found timezone identifiers
         */
        void scanDirectory(const std::string &basePath, const std::string &currentPath,
                           std::vector<std::string> &timezones);
    }

} // namespace TimezoneHelper

#endif // TIMEZONE_HELPER_HPP
