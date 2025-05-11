#include "pillar/logger.hpp"
#include "pillar/xdgdirectories.hpp"
#include <gtest/gtest.h>
#include <fstream>

class test_logger : public ::testing::Test {
public:
  test_logger() = default;
  explicit test_logger(std::filesystem::path loggerDir)
      : m_loggerDir(std::move(loggerDir))
  {
  }

protected:
  static std::filesystem::path init_logger_test_dir(const std::string& appAuthor, const std::string& appName)
  {
    auto loggerDir = pillar::xdg::get_data_home(appAuthor, appName);
    std::filesystem::remove_all(loggerDir);
    std::filesystem::create_directories(loggerDir);
    return loggerDir;
  }

  // cppcheck-suppress unusedFunction
  void SetUp() override { m_loggerDir = init_logger_test_dir(m_appAuthor, m_appName); }
  // cppcheck-suppress unusedFunction
  void TearDown() override { m_loggerDir = init_logger_test_dir(m_appAuthor, m_appName); }

  std::string m_appAuthor = "timow";
  std::string m_appName = "pillar_log_test";
  std::filesystem::path m_loggerDir;
};

TEST_F(test_logger, file_size)
{
  std::size_t maxFileSize = pillar::get_max_log_file_size();
  EXPECT_EQ(1024 * 1024 * 5, maxFileSize);

  maxFileSize = 1;
  pillar::set_max_log_file_size(maxFileSize);
  EXPECT_EQ(maxFileSize, pillar::get_max_log_file_size());
}

TEST_F(test_logger, max_log_files)
{
  std::size_t maxLogFiles = pillar::get_max_log_files();
  EXPECT_EQ(3, maxLogFiles);

  maxLogFiles = 1;
  pillar::set_max_log_files(maxLogFiles);
  EXPECT_EQ(maxLogFiles, pillar::get_max_log_files());
}

TEST_F(test_logger, log_level)
{
  pillar::log_level_enum level = pillar::get_log_level();
  EXPECT_EQ(pillar::log_level_enum::trace, level);

  level = pillar::log_level_enum::info;
  pillar::set_log_level(level);
  EXPECT_EQ(level, pillar::get_log_level());
}

TEST_F(test_logger, create_logger)
{
  auto logFile = m_loggerDir / "testLog.txt";
  auto logger = pillar::create_logger(logFile, "MainLogger");
  const std::string testString = "Test log.";
  logger->info(testString);
  logger->flush();

  std::ifstream ifstream;
  ifstream.open(logFile.c_str());

  std::string line;
  std::getline(ifstream, line);

  EXPECT_TRUE(line.contains(testString));
}
