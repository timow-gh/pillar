#include <fstream>
#include <gtest/gtest.h>
#include <pillar/Logger.hpp>
#include <pillar/XDGBaseDirectories.hpp>

TEST(Logger, file_size)
{
  std::size_t maxFileSize = pillar::get_max_log_file_size();
  EXPECT_EQ(1024 * 1024 * 5, maxFileSize);

  maxFileSize = 1;
  pillar::set_max_log_file_size(maxFileSize);
  EXPECT_EQ(maxFileSize, pillar::get_max_log_file_size());
}

TEST(Logger, max_log_files)
{
  std::size_t maxLogFiles = pillar::get_max_log_files();
  EXPECT_EQ(3, maxLogFiles);

  maxLogFiles = 1;
  pillar::set_max_log_files(maxLogFiles);
  EXPECT_EQ(maxLogFiles, pillar::get_max_log_files());
}

TEST(Logger, log_level)
{
  pillar::log_level_enum level = pillar::get_log_level();
  EXPECT_EQ(pillar::log_level_enum::trace, level);

  level = pillar::log_level_enum::info;
  pillar::set_log_level(level);
  EXPECT_EQ(level, pillar::get_log_level());
}

TEST(Logger, create_logger)
{
  auto loggerDir = pillar::xdg::get_data_home("timow", "pillar_log_test");
  EXPECT_NO_THROW(std::filesystem::remove_all(loggerDir));
  EXPECT_NO_THROW(std::filesystem::create_directories(loggerDir));

  {
    auto logFile = loggerDir / "testLog.txt";
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

  auto parentDir = loggerDir.parent_path();
  ASSERT_TRUE(std::filesystem::is_directory(parentDir));
  ASSERT_TRUE(parentDir.filename() == "timow");
  EXPECT_NO_THROW(std::filesystem::remove_all(parentDir));
}
