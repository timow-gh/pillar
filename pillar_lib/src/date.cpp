#include "pillar/date.hpp"
#include "pillar/warnings.hpp"
#include <sstream>

DISABLE_ALL_WARNINGS
#include <date/date.h>
#include <date/tz.h>
ENABLE_ALL_WARNINGS

using namespace std::chrono;

namespace pillar::date
{

static auto time_zone_db_location() -> std::filesystem::path&
{
  static std::filesystem::path tzDbLocation;
  return tzDbLocation;
}

auto get_time_zone_db_directory() -> std::expected<std::filesystem::path, DateErrors>
{
  if (auto path = time_zone_db_location(); !path.empty())
  {
    return path;
  }
  return std::unexpected(DateErrors::TZDB_DIRECTORY_NOT_SET);
}

DateErrors set_time_zone_db_directory(const std::filesystem::path& tzDbLocation)
{
  if (tzDbLocation.empty())
  {
    return DateErrors::PATH_IS_EMPTY;
  }

  if (std::filesystem::exists(tzDbLocation))
  {
    if (!std::filesystem::is_directory(tzDbLocation))
    {
      return DateErrors::PATH_IS_NOT_A_DIRECTORY;
    }

    return DateErrors::PATH_ALREADY_EXISTS;
  }

  std::string tzDbLocationStr = tzDbLocation.string();
  ::date::set_install(tzDbLocationStr);
  ::date::reload_tzdb();
  time_zone_db_location() = tzDbLocation;
  return DateErrors::SUCCESS;
}

UTCTimeString get_utc_time_string()
{
  return to_utc_iso8601(system_clock::now());
}

UTCTimeString to_utc_iso8601(const std::chrono::system_clock::time_point& localTime)
{
  assert(time_zone_db_location() != "");

  const ::date::time_zone* localTz = nullptr;
  localTz = ::date::current_zone();
  ::date::zoned_time localZonedTime(localTz, localTime);
  const ::date::zoned_time utcTime = ::date::make_zoned("UTC", localZonedTime.get_sys_time());

  // Format the UTC time according to ISO 8601
  std::ostringstream oss;
  oss << format("%Y-%m-%dT%H:%M:%SZ", utcTime);
  return UTCTimeString{oss.str()};
}

UTCTimeString to_utc_iso8601(int year, unsigned month, unsigned day, unsigned hour, unsigned min, unsigned sec)
{
  assert(time_zone_db_location() != "");

  ::date::year_month_day ymd = ::date::year{year} / ::date::month{month} / ::date::day{day};
  if (!ymd.ok())
  {
    return UTCTimeString{""};
  }

  ::date::sys_days localSysDays = ymd;
  ::date::sys_seconds localTp = localSysDays + hours{hour} + minutes{min} + seconds{sec};
  const auto* localTz = ::date::current_zone();
  ::date::zoned_time localZonedTime(localTz, localTp);
  auto utcTime = ::date::make_zoned("UTC", localZonedTime.get_sys_time());

  // Format the UTC time according to ISO 8601
  std::ostringstream oss;
  oss << ::date::format("%Y-%m-%dT%H:%M:%SZ", utcTime);
  return UTCTimeString{oss.str()};
}

} // namespace pillar::date
