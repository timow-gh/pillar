#include "pillar/xdgdirectories.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace pillar::xdg
{

static std::filesystem::path get_env_path(const char* envVar, const std::filesystem::path& defaultPath)
{
  const char* envVal = std::getenv(envVar);
  if (envVal && std::filesystem::path(envVal).is_absolute())
  {
    return std::filesystem::path(envVal);
  }
  else
  {
    return defaultPath;
  }
}

std::filesystem::path get_config_home([[maybe_unused]] const std::string& appAuthor, const std::string& appName)
{
#ifdef _WIN32
  char* appdata = nullptr;
  size_t len = 0;
  _dupenv_s(&appdata, &len, "APPDATA");
  std::filesystem::path configDir;
  if (appdata)
  {
    configDir = std::filesystem::path(appdata) / appAuthor / appName;
    free(appdata);
  }
  else
  {
    // Fallback to user's home directory
    const char* home = std::getenv("USERPROFILE");
    if (home)
    {
      configDir = std::filesystem::path(home) / "AppData" / "Local" / appAuthor / appName;
    }
    else
    {
      // Last resort to current directory
      configDir = "config";
    }
  }
  return configDir;
#else
  // XDG_CONFIG_HOME or default to ~/.config
  std::filesystem::path defaultPath = std::filesystem::path(std::getenv("HOME")) / ".config" / appName;
  return get_env_path("XDG_CONFIG_HOME", defaultPath);
#endif
}

std::filesystem::path get_data_home([[maybe_unused]] const std::string& appAuthor, const std::string& appName)
{
#ifdef _WIN32
  char* local_appdata = nullptr;
  size_t len = 0;
  _dupenv_s(&local_appdata, &len, "LOCALAPPDATA");
  std::filesystem::path dataDir;
  if (local_appdata)
  {
    dataDir = std::filesystem::path(local_appdata) / appAuthor / appName;
    free(local_appdata);
  }
  else
  {
    // Fallback to user's home directory
    const char* home = std::getenv("USERPROFILE");
    if (home)
    {
      dataDir = std::filesystem::path(home) / "AppData" / "Local" / appAuthor / appName;
    }
    else
    {
      // Last resort to current directory
      dataDir = "data";
    }
  }
  return dataDir;
#else
  // XDG_DATA_HOME or default to ~/.local/share
  std::filesystem::path defaultPath = std::filesystem::path(std::getenv("HOME")) / ".local" / "share" / appName;
  return get_env_path("XDG_DATA_HOME", defaultPath);
#endif
}

std::filesystem::path get_cache_home([[maybe_unused]] const std::string& appAuthor, const std::string& appName)
{
#ifdef _WIN32
  char* local_appdata = nullptr;
  size_t len = 0;
  _dupenv_s(&local_appdata, &len, "LOCALAPPDATA");
  std::filesystem::path cacheDir;
  if (local_appdata)
  {
    cacheDir = std::filesystem::path(local_appdata) / appAuthor / appName / "Cache";
    free(local_appdata);
  }
  else
  {
    // Fallback to user's home directory
    const char* home = std::getenv("USERPROFILE");
    if (home)
    {
      cacheDir = std::filesystem::path(home) / "AppData" / "Local" / appAuthor / appName / "Cache";
    }
    else
    {
      // Last resort to current directory
      cacheDir = "cache";
    }
  }
  return cacheDir;
#else
  // XDG_CACHE_HOME or default to ~/.cache
  std::filesystem::path defaultPath = std::filesystem::path(std::getenv("HOME")) / ".cache" / appName;
  return get_env_path("XDG_CACHE_HOME", defaultPath);
#endif
}

std::filesystem::path get_state_home([[maybe_unused]] const std::string& appAuthor, const std::string& appName)
{
#ifdef _WIN32
  // Windows does not have a direct equivalent to XDG_STATE_HOME
  // Use LOCALAPPDATA as a fallback
  char* local_appdata = nullptr;
  size_t len = 0;
  _dupenv_s(&local_appdata, &len, "LOCALAPPDATA");
  std::filesystem::path stateDir;
  if (local_appdata)
  {
    stateDir = std::filesystem::path(local_appdata) / appAuthor / appName / "State";
    free(local_appdata);
  }
  else
  {
    // Fallback to user's home directory
    const char* home = std::getenv("USERPROFILE");
    if (home)
    {
      stateDir = std::filesystem::path(home) / "AppData" / "Local" / appAuthor / appName / "State";
    }
    else
    {
      // Last resort to current directory
      stateDir = "state";
    }
  }
  return stateDir;
#else
  // XDG_STATE_HOME or default to ~/.local/state
  std::filesystem::path defaultPath = std::filesystem::path(std::getenv("HOME")) / ".local" / "state" / appName;
  return get_env_path("XDG_STATE_HOME", defaultPath);
#endif
}

std::filesystem::path get_runtime_dir()
{
#ifdef _WIN32
  std::filesystem::path runtimeDir;
  // Windows does not have a direct equivalent to XDG_RUNTIME_DIR
  // Use LOCALAPPDATA as a fallback
  char* local_appdata = nullptr;
  size_t len = 0;
  _dupenv_s(&local_appdata, &len, "LOCALAPPDATA");

  if (local_appdata)
  {
    runtimeDir = std::filesystem::path(local_appdata) / "YourAppName" / "Runtime";
    free(local_appdata);
  }
  else
  {
    // Fallback to user's home directory
    const char* home = std::getenv("USERPROFILE");
    if (home)
    {
      runtimeDir = std::filesystem::path(home) / "AppData" / "Local" / "YourAppName" / "Runtime";
    }
    else
    {
      // Last resort to current directory
      runtimeDir = "runtime";
    }
  }
  return runtimeDir;
#else
  const char* runtimeDirEnv = std::getenv("XDG_RUNTIME_DIR");
  if ((runtimeDirEnv != nullptr) && std::filesystem::path(runtimeDirEnv).is_absolute())
  {
    return runtimeDirEnv;
  }

  return std::filesystem::temp_directory_path();
#endif
}

std::vector<std::filesystem::path> get_config_dirs()
{
  std::vector<std::filesystem::path> configDirs;

#ifdef _WIN32
  // On Windows, system-wide config directories can be hardcoded or configured
  // Typically, /etc/xdg is not used on Windows, so this can be left empty or include specific paths
#else
  // XDG_CONFIG_DIRS or default to /etc/xdg
  const char* xdgConfigDirs = std::getenv("XDG_CONFIG_DIRS");
  if ((xdgConfigDirs != nullptr) && strlen(xdgConfigDirs) > 0)
  {
    std::string dirsStr(xdgConfigDirs);
    size_t pos = 0;
    while ((pos = dirsStr.find(':')) != std::string::npos)
    {
      std::string dir = dirsStr.substr(0, pos);
      configDirs.emplace_back(dir);
      dirsStr.erase(0, pos + 1);
    }
    if (!dirsStr.empty())
    {
      configDirs.emplace_back(dirsStr);
    }
  }
  else
  {
    configDirs.emplace_back("/etc/xdg");
  }
#endif
  return configDirs;
}

std::vector<std::filesystem::path> get_data_dirs()
{
  std::vector<std::filesystem::path> dataDirs;

#ifdef _WIN32
  // On Windows, system-wide data directories can be hardcoded or configured
  // Typically, /usr/share is not used on Windows, so this can be left empty or include specific paths
#else
  // XDG_DATA_DIRS or default to /usr/local/share:/usr/share
  const char* xdgDataDirs = std::getenv("XDG_DATA_DIRS");
  if ((xdgDataDirs != nullptr) && strlen(xdgDataDirs) > 0)
  {
    std::string dirsStr(xdgDataDirs);
    size_t pos = 0;
    while ((pos = dirsStr.find(':')) != std::string::npos)
    {
      std::string dir = dirsStr.substr(0, pos);
      dataDirs.emplace_back(dir);
      dirsStr.erase(0, pos + 1);
    }
    if (!dirsStr.empty())
    {
      dataDirs.emplace_back(dirsStr);
    }
  }
  else
  {
    dataDirs.emplace_back("/usr/local/share");
    dataDirs.emplace_back("/usr/share");
  }
#endif
  return dataDirs;
}

} // namespace pillar::xdg
