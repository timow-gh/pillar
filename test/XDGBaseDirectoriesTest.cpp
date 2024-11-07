#include <gtest/gtest.h>
#include <pillar/XDGBaseDirectories.hpp>

class XDGBaseDirectoriesTest : public ::testing::Test {
protected:
  void remove_test_dir(std::filesystem::path testDir)
  {
    std::filesystem::remove_all(testDir);
    ASSERT_FALSE(std::filesystem::exists(testDir));
  }

  // cppcheck-suppress-begin unusedStructMember
  std::string m_testAuthor{"testAuthor"};
  std::string m_testAppName{"pillar_test_app"};
  // cppcheck-suppress-end unusedStructMember
};

TEST_F(XDGBaseDirectoriesTest, get_config_home)
{
  auto path = pillar::xdg::get_config_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(XDGBaseDirectoriesTest, get_data_home)
{
  auto path = pillar::xdg::get_data_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(XDGBaseDirectoriesTest, get_cache_home)
{
  auto path = pillar::xdg::get_cache_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(XDGBaseDirectoriesTest, get_state_home)
{
  auto path = pillar::xdg::get_state_home(m_testAuthor, m_testAppName);
  remove_test_dir(path);
}

TEST_F(XDGBaseDirectoriesTest, get_runtime_dir)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_runtime_dir();
}

TEST_F(XDGBaseDirectoriesTest, get_config_dirs)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_config_dirs();
}

TEST_F(XDGBaseDirectoriesTest, get_data_dirs)
{
  [[maybe_unused]] auto paths = pillar::xdg::get_data_dirs();
}
