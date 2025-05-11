#include "pillar/xdgdirectories.hpp"
#include <gtest/gtest.h>

class test_xdgdirectories : public ::testing::Test {
protected:
  static void remove_test_dir(const std::filesystem::path& testDir)
  {
    std::filesystem::remove_all(testDir);
    ASSERT_FALSE(std::filesystem::exists(testDir));
  }

  // cppcheck-suppress-begin unusedStructMember
  std::string m_testAuthor{"testAuthor"};
  std::string m_testAppName{"pillar_test_app"};
  // cppcheck-suppress-end unusedStructMember
};

TEST_F(test_xdgdirectories, get_config_home)
{
  auto path = pillar::xdg::get_config_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(test_xdgdirectories, get_data_home)
{
  auto path = pillar::xdg::get_data_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(test_xdgdirectories, get_cache_home)
{
  auto path = pillar::xdg::get_cache_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(test_xdgdirectories, get_state_home)
{
  auto path = pillar::xdg::get_state_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(test_xdgdirectories, get_runtime_dir)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_runtime_dir();
}

TEST_F(test_xdgdirectories, get_config_dirs)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_config_dirs();
}

TEST_F(test_xdgdirectories, get_data_dirs)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_data_dirs();
}
