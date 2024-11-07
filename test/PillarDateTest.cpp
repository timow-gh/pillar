#include <fstream>
#include <gtest/gtest.h>
#include <pillar/Date.hpp>
#include <pillar/XDGBaseDirectories.hpp>

class PillarDateTest : public ::testing::Test {
protected:
  // cppcheck-suppress unusedFunction
  void SetUp() override
  {
    remove_test_dir();

    std::error_code error;
    std::filesystem::create_directories(m_testDir, error);
    ASSERT_FALSE(error);
    pillar::date::DateErrors res = pillar::date::set_time_zone_db_directory(m_tzdbDirectory);
    ASSERT_EQ(res, pillar::date::DateErrors::SUCCESS);
  }

  // cppcheck-suppress unusedFunction
  void TearDown() override { remove_test_dir(); }

  void remove_test_dir()
  {
    std::filesystem::remove_all(m_testDir);
    ASSERT_FALSE(std::filesystem::exists(m_testDir));
  }

  std::filesystem::path m_testDir = pillar::xdg::get_data_home("timow", "pillar_test");
  std::filesystem::path m_tzdbDirectory = m_testDir / "tzdata";
};

class PillarDateFailDBTest : public ::testing::Test {

protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(PillarDateFailDBTest, tzdb_get_directory)
{
  const auto& dbDir = pillar::date::get_time_zone_db_directory();
  EXPECT_FALSE(dbDir.has_value());
  EXPECT_EQ(pillar::date::DateErrors::TZDB_DIRECTORY_NOT_SET, dbDir.error());
}

TEST_F(PillarDateTest, tzdb_get_directory)
{
  const auto& dbDir = pillar::date::get_time_zone_db_directory();
  EXPECT_TRUE(dbDir.has_value());
  EXPECT_EQ(dbDir.value(), m_tzdbDirectory);
}

TEST_F(PillarDateTest, set_time_zone_db_directory)
{
  pillar::date::DateErrors res = pillar::date::set_time_zone_db_directory("");
  EXPECT_EQ(pillar::date::DateErrors::PATH_IS_EMPTY, res);

  std::filesystem::path tmpFile = m_testDir / "tmp.txt";
  std::ofstream file(tmpFile);
  file << "test";

  res = pillar::date::set_time_zone_db_directory(tmpFile);
  EXPECT_EQ(pillar::date::DateErrors::PATH_IS_NOT_A_DIRECTORY, res);
}

TEST_F(PillarDateTest, get_utc_time)
{
  pillar::date::UTCTimeString utcTime = pillar::date::get_utc_time_string();
  ASSERT_FALSE(utcTime.get_string().empty());
}
