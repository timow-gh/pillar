#ifndef PILLAR_XDGBASEDIRECTORIES_H
#define PILLAR_XDGBASEDIRECTORIES_H

#include <filesystem>
#include <string>
#include <vector>

namespace pillar::xdg
{

/**
 * @brief Manages user-specific application directories based on the XDG Base Directory Specification.
 *
 * This module provides utility functions to retrieve standard directories for configuration,
 * data, cache, state, and runtime files in a cross-platform manner. On Linux and Unix-like systems,
 * it adheres to the [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/latest/),
 * while on Windows, it maps to the appropriate AppData directories.
 *
 * ## Directory Overview
 *
 * The XDG Base Directory Specification defines several environment variables that specify
 * the locations for various types of application data. These directories help organize
 * files in a consistent and standardized manner across different operating systems.
 *
 * clang-format off
 * | **Purpose**                 | **Linux/Unix Path**                                  | **Windows Path**                                                                                               |
 * |-----------------------------|------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
 * | **Configuration Files**     | `$XDG_CONFIG_HOME` (`~/.config/YourAppName/`)        | `%APPDATA%\YourAppName\` (`C:\Users\<Username>\AppData\Roaming\YourAppName\`)                                  |
 * | **Data Files**              | `$XDG_DATA_HOME` (`~/.local/share/YourAppName/`)     | `%LOCALAPPDATA%\YourAppName\` (`C:\Users\<Username>\AppData\Local\YourAppName\`)                               |
 * | **Cache Files**             | `$XDG_CACHE_HOME` (`~/.cache/YourAppName/`)          | `%LOCALAPPDATA%\YourAppName\Cache\` (`C:\Users\<Username>\AppData\Local\YourAppName\Cache\`)                   |
 * | **State Files**             | `$XDG_STATE_HOME` (`~/.local/state/YourAppName/`)    | `%LOCALAPPDATA%\YourAppName\State\` (`C:\Users\<Username>\AppData\Local\YourAppName\State\`)                   |
 * | **Runtime Files**           | `$XDG_RUNTIME_DIR` (`/run/user/<UID>/`)              | `%LOCALAPPDATA%\YourAppName\Runtime\` (`C:\Users\<Username>\AppData\Local\YourAppName\Runtime\`)               |
 * | **System Configuration**    | `$XDG_CONFIG_DIRS` (`/etc/xdg/`)                     | *Not directly applicable*                                                                                      |
 * | **System Data Directories** | `$XDG_DATA_DIRS` (`/usr/local/share/:/usr/share/`)   | *Not directly applicable*                                                                                      |
 * clang-format on
 *
 * **Note:** On Windows, there is no direct equivalent for some XDG directories like `XDG_STATE_HOME` and `XDG_RUNTIME_DIR`. These are
 * mapped to appropriate subdirectories within `%LOCALAPPDATA%`.
 *
 * ## Function Descriptions
 *
 * - `get_config_home`: Retrieves the path for user-specific configuration files.
 * - `get_data_home`: Retrieves the path for user-specific data files.
 * - `get_cache_home`: Retrieves the path for user-specific cache files.
 * - `get_state_home`: Retrieves the path for user-specific state files.
 * - `get_runtime_dir`: Retrieves the path for user-specific runtime files.
 * - `get_config_dirs`: Retrieves the set of directories to search for configuration files.
 * - `get_data_dirs`: Retrieves the set of directories to search for data files.
 *
 * @see [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/latest/)
 */

/**
 * @brief Retrieves the path for user-specific configuration files.
 *
 * Determines the appropriate configuration directory based on the operating system.
 * On Linux/Unix, it follows the XDG Base Directory Specification, defaulting to
 * `$HOME/.config/<AppName>` if `$XDG_CONFIG_HOME` is not set.
 * On Windows, it maps to `%APPDATA%\YourAppAuthor\YourAppName\`.
 *
 * @param appAuthor  The name of the application author or organization.
 * @param appName    The name of the application.
 * @return std::filesystem::path  The path to the configuration directory.
 */
std::filesystem::path get_config_home(const std::string& appAuthor, const std::string& appName);

/**
 * @brief Retrieves the path for user-specific data files.
 *
 * Determines the appropriate data directory based on the operating system.
 * On Linux/Unix, it follows the XDG Base Directory Specification, defaulting to
 * `$HOME/.local/share/<AppName>` if `$XDG_DATA_HOME` is not set.
 * On Windows, it maps to `%LOCALAPPDATA%\YourAppAuthor\YourAppName\`.
 *
 * @param appAuthor  The name of the application author or organization.
 * @param appName    The name of the application.
 * @return std::filesystem::path  The path to the data directory.
 */
std::filesystem::path get_data_home(const std::string& appAuthor, const std::string& appName);

/**
 * @brief Retrieves the path for user-specific cache files.
 *
 * Determines the appropriate cache directory based on the operating system.
 * On Linux/Unix, it follows the XDG Base Directory Specification, defaulting to
 * `$HOME/.cache/<AppName>` if `$XDG_CACHE_HOME` is not set.
 * On Windows, it maps to `%LOCALAPPDATA%\YourAppAuthor\YourAppName\Cache\`.
 *
 * @param appAuthor  The name of the application author or organization.
 * @param appName    The name of the application.
 * @return std::filesystem::path  The path to the cache directory.
 */
std::filesystem::path get_cache_home(const std::string& appAuthor, const std::string& appName);

/**
 * @brief Retrieves the path for user-specific state files.
 *
 * Determines the appropriate state directory based on the operating system.
 * On Linux/Unix, it follows the XDG Base Directory Specification, defaulting to
 * `$HOME/.local/state/<AppName>` if `$XDG_STATE_HOME` is not set.
 * On Windows, it maps to `%LOCALAPPDATA%\YourAppAuthor\YourAppName\State\`.
 *
 * @param appAuthor  The name of the application author or organization.
 * @param appName    The name of the application.
 * @return std::filesystem::path  The path to the state directory.
 */
std::filesystem::path get_state_home(const std::string& appAuthor, const std::string& appName);

/**
 * @brief Retrieves the path for user-specific runtime files.
 *
 * Determines the appropriate runtime directory based on the operating system.
 * On Linux/Unix, it follows the XDG Base Directory Specification using `$XDG_RUNTIME_DIR`.
 * If `$XDG_RUNTIME_DIR` is not set, it falls back to the system's temporary directory.
 * On Windows, it maps to `%LOCALAPPDATA%\YourAppAuthor\YourAppName\Runtime\`.
 *
 * @return std::filesystem::path  The path to the runtime directory.
 */
std::filesystem::path get_runtime_dir();

/**
 * @brief Retrieves the set of directories to search for configuration files.
 *
 * On Linux/Unix, it follows the XDG Base Directory Specification by reading the
 * `$XDG_CONFIG_DIRS` environment variable, which specifies a colon-separated list of
 * directories (e.g., `/etc/xdg`). If `$XDG_CONFIG_DIRS` is not set, it defaults to
 * `/etc/xdg`.
 *
 * On Windows, this function currently does not retrieve additional configuration directories,
 * as the XDG concept does not directly apply. It returns an empty list or can be extended
 * to include system-wide configuration paths if needed.
 *
 * @return std::vector<std::filesystem::path>  A list of configuration directories to search.
 */
std::vector<std::filesystem::path> get_config_dirs();

/**
 * @brief Retrieves the set of directories to search for data files.
 *
 * On Linux/Unix, it follows the XDG Base Directory Specification by reading the
 * `$XDG_DATA_DIRS` environment variable, which specifies a colon-separated list of
 * directories (e.g., `/usr/local/share:/usr/share`). If `$XDG_DATA_DIRS` is not set,
 * it defaults to `/usr/local/share` and `/usr/share`.
 *
 * On Windows, this function currently does not retrieve additional data directories,
 * as the XDG concept does not directly apply. It returns an empty list or can be extended
 * to include system-wide data paths if needed.
 *
 * @return std::vector<std::filesystem::path>  A list of data directories to search.
 */
std::vector<std::filesystem::path> get_data_dirs();

} // namespace pillar::xdg

#endif // PILLAR_XDGBASEDIRECTORIES_H
