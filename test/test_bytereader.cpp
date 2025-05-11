#include "pillar/bytereader.hpp"
#include <gtest/gtest.h>
#include <array>
#include <cstddef>
#include <cstdint>


class test_bytereader : public testing::Test {
protected:
  std::array<const std::uint32_t, 3> m_uint32s = {1, 2, 3};
  std::array<const std::byte, 4> m_bytes = {std::byte{0x01}, std::byte{0x10}, std::byte{0x00}, std::byte{0x01}}; // NOLINT
};

TEST_F(test_bytereader, read)
{
  pillar::bytereader byteReader{m_uint32s};

  auto first = byteReader.readLE<std::uint32_t>();
  EXPECT_EQ(first, 1);
  EXPECT_EQ(byteReader.get_position(), 4);
  auto second = byteReader.readLE<std::uint32_t>();
  EXPECT_EQ(second, 2);
  EXPECT_EQ(byteReader.get_position(), 8);

  auto firstBE = byteReader.readBE<std::uint32_t>();
  EXPECT_EQ(firstBE, 50331648);
  EXPECT_EQ(byteReader.get_position(), 12);
}

TEST_F(test_bytereader, skip)
{
  pillar::bytereader byteReader{m_bytes};
  byteReader.skip<std::uint32_t>();
  std::size_t pos = byteReader.get_position();
  EXPECT_EQ(pos, 4);
  EXPECT_THROW(byteReader.skip(2), std::out_of_range);

  byteReader = pillar::bytereader{m_uint32s};
  byteReader.skip<std::uint32_t>(2);
  EXPECT_EQ(byteReader.get_position(), 8);
  EXPECT_THROW(byteReader.skip<std::uint32_t>(2), std::out_of_range);
}
