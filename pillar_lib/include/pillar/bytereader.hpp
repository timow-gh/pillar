#ifndef CPP_LIB_TEMPLATE_LIBRARY_H
#define CPP_LIB_TEMPLATE_LIBRARY_H

#include <bit>
#include <cstddef>
#include <cstring>
#include <span>
#include <stdexcept>

namespace pillar
{

class bytereader
{
public:
  bytereader() = delete;
  explicit bytereader(std::span<const std::byte> bytes)
      : m_bytes(bytes)
  {
  }

  template <typename T, std::size_t D>
  explicit bytereader(const std::array<T, D>& arr)
      : m_bytes(std::as_bytes(std::span<T>(arr.data(), arr.size())))
  {
  }

  template <typename T>
  T readLE()
  {
    std::size_t size = sizeof(T);
    check_overflow(size);
    T value = T{};
    std::memcpy(&value, m_bytes.data() + m_pos, size);
    m_pos += size;
    return value;
  }

  template <typename T>
  T readBE()
  {
    auto value = readLE<T>();
    return std::byteswap(value);
  }

  void skip(std::size_t n)
  {
    check_overflow(n);
    m_pos += n;
  }

  template <typename T>
  void skip()
  {
    std::size_t size = sizeof(T);
    check_overflow(size);
    m_pos += size;
  }

  template <typename T>
  void skip(std::size_t n)
  {
    std::size_t size = sizeof(T) * n;
    check_overflow(size);
    m_pos += size;
  }

  [[nodiscard]] std::size_t get_position() const noexcept { return m_pos; }

private:
  void check_overflow(std::size_t n) const
  {
    if ((m_pos + n) > m_bytes.size())
    {
      throw std::out_of_range("ByteReader: Buffer overflow.");
    }
  }

  std::span<const std::byte> m_bytes;
  std::size_t m_pos{0};
};

} // namespace es

#endif
