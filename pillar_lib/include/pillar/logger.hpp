#ifndef PILLAR_LOGGER_H
#define PILLAR_LOGGER_H

#include <filesystem>
#include <spdlog/spdlog.h>

namespace pillar
{

using log_level_enum = spdlog::level::level_enum;

// Settings used to create loggers using create_logger()
std::size_t get_max_log_file_size();
void set_max_log_file_size(std::size_t maxFileSize);
std::size_t get_max_log_files();
void set_max_log_files(std::size_t maxFiles);
log_level_enum get_log_level();
void set_log_level(log_level_enum level);

std::shared_ptr<spdlog::logger> create_logger(const std::filesystem::path& logFilePath, const std::string& loggerName);

} // namespace pillar

#endif // PILLAR_LOGGER_H
