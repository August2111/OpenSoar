// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include "SystemClock.hxx"

#include <fileapi.h>
#include <chrono>
#include <cstdint>

constexpr uint_least64_t
ConstructUint64(DWORD lo, DWORD hi) noexcept
{
	return uint_least64_t(lo) | (uint_least64_t(hi) << 32);
}

constexpr uint_least64_t
ToUint64(FILETIME ft) noexcept
{
	return ConstructUint64(ft.dwLowDateTime, ft.dwHighDateTime);
}

constexpr int_least64_t
ToInt64(FILETIME ft) noexcept
{
	return ToUint64(ft);
}

constexpr FILETIME
ToFileTime(uint_least64_t t) noexcept
{
	FILETIME ft{};
	ft.dwLowDateTime = DWORD(t);
	ft.dwHighDateTime = DWORD(t >> 32);
	return ft;
}

constexpr FILETIME
ToFileTime(int_least64_t t) noexcept
{
	return ToFileTime(uint_least64_t(t));
}

/* "A file time is a 64-bit value that represents the number of
   100-nanosecond intervals"
   https://docs.microsoft.com/en-us/windows/win32/sysinfo/file-times */
using FileTimeResolution = std::ratio<1, 10000000>;

using FileTimeDuration = std::chrono::duration<int_least64_t,
					       FileTimeResolution>;

/**
 * Calculate a std::chrono::duration specifying the duration of the
 * FILETIME since its epoch (1601-01-01T00:00).
 */
constexpr auto
FileTimeToChronoDuration(FILETIME ft) noexcept
{
	return FileTimeDuration(ToInt64(ft));
}

/**
 * Calculate a std::chrono::duration specifying the duration between
 * the unix epoch and the given FILETIME.
 */
constexpr auto
FileTimeToUnixEpochDuration(FILETIME ft) noexcept
{
	/**
	 * The number of days between the Windows FILETIME epoch
	 * (1601-01-01T00:00) and the Unix epoch (1970-01-01T00:00).
	 */
	constexpr int_least64_t windows_unix_days = 134774;
	constexpr int_least64_t windows_unix_hours = windows_unix_days * 24;

	constexpr FileTimeDuration windows_unix_delta{std::chrono::hours{windows_unix_hours}};

	return FileTimeToChronoDuration(ft) - windows_unix_delta;
}

inline std::chrono::system_clock::time_point
FileTimeToChrono(FILETIME ft) noexcept
{
	return TimePointAfterUnixEpoch(FileTimeToUnixEpochDuration(ft));
}

constexpr FILETIME
ToFileTime(FileTimeDuration d) noexcept
{
	return ToFileTime(d.count());
}

constexpr FILETIME
UnixEpochDurationToFileTime(FileTimeDuration d) noexcept
{
	/**
	 * The number of days between the Windows FILETIME epoch
	 * (1601-01-01T00:00) and the Unix epoch (1970-01-01T00:00).
	 */
	constexpr int_least64_t windows_unix_days = 134774;
	constexpr int_least64_t windows_unix_hours = windows_unix_days * 24;

	constexpr FileTimeDuration windows_unix_delta{std::chrono::hours{windows_unix_hours}};

	return ToFileTime(d + windows_unix_delta);
}

constexpr FILETIME
UnixEpochTimeToFileTime(time_t t) noexcept
{
	/**
	 * The number of days between the Windows FILETIME epoch
	 * (1601-01-01T00:00) and the Unix epoch (1970-01-01T00:00).
	 */
	constexpr int_least64_t windows_unix_days = 134774;
	constexpr int_least64_t windows_unix_hours = windows_unix_days * 24;

	constexpr FileTimeDuration windows_unix_delta{std::chrono::hours{windows_unix_hours}};

	return ToFileTime(windows_unix_delta + std::chrono::seconds(t));
}

inline FILETIME
ChronoToFileTime(std::chrono::system_clock::time_point tp) noexcept
{
	const auto since_unix_epoch = DurationSinceUnixEpoch(tp);
	const auto ft_since_unix_epoch =
		std::chrono::duration_cast<FileTimeDuration>(since_unix_epoch);

	return UnixEpochDurationToFileTime(ft_since_unix_epoch);
}

constexpr std::chrono::seconds
DeltaFileTimeS(FILETIME a, FILETIME b) noexcept
{
	return std::chrono::duration_cast<std::chrono::seconds>
		(FileTimeToChronoDuration(a) - FileTimeToChronoDuration(b));
}
