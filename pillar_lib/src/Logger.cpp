#include "pillar/Logger.hpp"
#include "pillar/Assert.hpp"
#include "pillar/XDGBaseDirectories.hpp"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace
{

struct LoggerSettings
{
  std::size_t maxSize{1024 * 1024 * 5}; // MB
  std::size_t maxFiles{3};
  spdlog::level::level_enum loglevel{spdlog::level::trace};
};

} // namespace

namespace pillar
{

static LoggerSettings& logger_settings()
{
  static LoggerSettings settings;
  return settings;
}

std::size_t get_max_log_file_size()
{
  return logger_settings().maxSize;
}

void set_max_log_file_size(std::size_t maxFileSize)
{
  logger_settings().maxSize = maxFileSize;
}

std::size_t get_max_log_files()
{
  return logger_settings().maxFiles;
}

void set_max_log_files(std::size_t maxFiles)
{
  logger_settings().maxFiles = maxFiles;
}

spdlog::level::level_enum get_log_level()
{
  return logger_settings().loglevel;
}

void set_log_level(spdlog::level::level_enum level)
{
  logger_settings().loglevel = level;
}

std::shared_ptr<spdlog::logger> create_logger(const std::filesystem::path& logFilePath, const std::string& loggerName)
{
  if (PILLAR_ASSERT(!loggerName.empty()))
  {
    return nullptr;
  }

  if (PILLAR_ASSERT(std::filesystem::exists(logFilePath.parent_path())))
  {
    return nullptr;
  }

  const auto loglevel = logger_settings().loglevel;

  const std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] [pid %P] %v";

  auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  consoleSink->set_level(loglevel);
  consoleSink->set_pattern(pattern);

  auto fileSink =
      std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFilePath.string(), logger_settings().maxSize, logger_settings().maxFiles);

  fileSink->set_level(loglevel);
  fileSink->set_pattern(pattern);

  auto logger = std::make_shared<spdlog::logger>(loggerName, spdlog::sinks_init_list{consoleSink, fileSink});
  logger->set_level(loglevel);

  return logger;
}

} // namespace pillar