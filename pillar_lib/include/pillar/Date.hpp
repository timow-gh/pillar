#ifndef PILLAR_DATE_HPP
#define PILLAR_DATE_HPP

#include <expected>
#include <filesystem>
#include <string>

/**
 * Resources for working with dates and times.
 *
 * - https://www.cl.cam.ac.uk/~mgk25/iso-time.html
 * - https://en.cppreference.com/w/cpp/chrono
 * - https://github.com/HowardHinnant/date
 * - https://howardhinnant.github.io/date/date.html
 */

namespace pillar::date
{

/** @brief A class representing a UTC time string in ISO 8601 format. */
class UTCTimeString {
  std::string m_time;

public:
  UTCTimeString() = delete;
  explicit UTCTimeString(std::string time)
      : m_time(std::move(time))
  {
  }

  [[nodiscard]] const std::string& get_string() const { return m_time; }
  operator std::string() const { return m_time; }
};

enum class DateErrors : std::uint8_t
{
  SUCCESS = 0,
  PATH_IS_EMPTY,
  PATH_IS_NOT_A_DIRECTORY,
  PATH_ALREADY_EXISTS,
  TZDB_DIRECTORY_NOT_SET
};

/** @brief Returns the global time zone database directory. */
auto get_time_zone_db_directory() -> std::expected<std::filesystem::path, DateErrors>;

/** @brief Download the time zone data base to the given directory. */
DateErrors set_time_zone_db_directory(const std::filesystem::path& tzDbLocation);

/** @brief Returns the current UTC time in ISO 8601 format. */
UTCTimeString get_utc_time_string();

/** @brief Converts a local time point to a UTC time string in ISO 8601 format. */
UTCTimeString to_utc_iso8601(const std::chrono::system_clock::time_point& localTime);

/**
 * Converts a local time specified by date and time components to a UTC time string in ISO 8601 format.
 *
 * @param year  The year (e.g., 2024).
 * @param month The month (1-12).
 * @param day   The day of the month (1-31).
 * @param hour  The hour of the day (0-23).
 * @param min   The minute of the hour (0-59).
 * @param sec   The second of the minute (0-60, allowing for leap seconds).
 * @return A string representing the UTC time in ISO 8601 format.
 */
UTCTimeString to_utc_iso8601(int year, unsigned month, unsigned day, unsigned hour, unsigned min, unsigned sec);

} // namespace pillar::date

#endif // PILLAR_DATE_HPP
