// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "TimeFormatter.hpp"
#include "time/BrokenDateTime.hpp"
#include "time/DateTime.hpp"
#include "Math/Util.hpp"
#include "util/StringCompare.hxx"
#include "util/StaticString.hxx"

#include <stdio.h>
#include <stdlib.h>

void
FormatISO8601(char *buffer, const BrokenDate &date) noexcept
{
  sprintf(buffer, "%04u-%02u-%02u",
          date.year, date.month, date.day);
}

void
FormatISO8601(char *buffer, const BrokenDateTime &stamp) noexcept
{
  sprintf(buffer, "%04u-%02u-%02uT%02u:%02u:%02uZ",
          stamp.year, stamp.month, stamp.day,
          stamp.hour, stamp.minute, stamp.second);
}

void
FormatISO8601(char *buffer, const time_t &stamp) noexcept
{
  strncpy(buffer, DateTime::time_str(stamp, "%Y-%m-%dT%H:%M:%SZ").c_str(), 21);
}

void
FormatTime(char *buffer, FloatDuration _time) noexcept
{
  if (_time.count() < 0) {
    *buffer++ = '-';
    _time = -_time;
  }

  const BrokenTime time = BrokenTime::FromSinceMidnightChecked(_time);
  sprintf(buffer, "%02u:%02u:%02u",
            time.hour, time.minute, time.second);
}

void
FormatTimeLong(char *buffer, FloatDuration _time) noexcept
{
  if (_time.count() < 0) {
    *buffer++ = '-';
    _time = -_time;
  }

  const BrokenTime time = BrokenTime::FromSinceMidnightChecked(_time);

  _time -= FloatDuration{trunc(_time.count())};
  unsigned millisecond = uround(_time.count() * 1000);

  sprintf(buffer, "%02u:%02u:%02u.%03u",
            time.hour, time.minute, time.second, millisecond);
}

void
FormatSignedTimeHHMM(char *buffer, std::chrono::seconds _time) noexcept
{
  if (_time.count() < 0) {
    *buffer++ = '-';
    _time = -_time;
  }

  const BrokenTime time = BrokenTime::FromSinceMidnightChecked(_time);
  sprintf(buffer, "%02u:%02u", time.hour, time.minute);
}

void
FormatTimeTwoLines(char *buffer1, char *buffer2, std::chrono::seconds _time) noexcept
{
  if (_time >= std::chrono::hours{24}) {
    strcpy(buffer1, ">24h");
    buffer2[0] = '\0';
    return;
  }
  if (_time <= -std::chrono::hours{24}) {
    strcpy(buffer1, "<-24h");
    buffer2[0] = '\0';
    return;
  }
  if (_time.count() < 0) {
    *buffer1++ = '-';
    _time = -_time;
  }

  const BrokenTime time = BrokenTime::FromSinceMidnight(_time);

  if (time.hour > 0) { // hh:mm, ss
    // Set Value
    sprintf(buffer1, "%02u:%02u", time.hour, time.minute);
    sprintf(buffer2, "%02u", time.second);
  } else { // mm'ss
    sprintf(buffer1, "%02u'%02u", time.minute, time.second);
    buffer2[0] = '\0';
  }
}

static void
CalculateTimespanComponents(unsigned timespan, unsigned &days, unsigned &hours,
                            unsigned &minutes, unsigned &seconds) noexcept
{
  if (timespan >= 24u * 60u * 60u) {
    days = timespan / (24u * 60u * 60u);
    timespan -= days * (24u * 60u * 60u);
  } else
    days = 0;

  if (timespan >= 60u * 60u) {
    hours = timespan / (60u * 60u);
    timespan -= hours * (60u * 60u);
  } else
    hours = 0;

  if (timespan >= 60u) {
    minutes = timespan / 60u;
    timespan -= minutes * 60u;
  } else
    minutes = 0;

  seconds = timespan;
}

void
FormatTimespanSmart(char *buffer, std::chrono::seconds timespan,
                    unsigned max_tokens,
                    const char *separator) noexcept
{
  assert(max_tokens > 0 && max_tokens <= 4);

  unsigned days, hours, minutes, seconds;
  CalculateTimespanComponents(std::abs(timespan.count()),
                              days, hours, minutes, seconds);

  unsigned token = 0;
  bool show_days = false, show_hours = false;
  bool show_minutes = false, show_seconds = false;

  // Days
  if (days != 0) {
    show_days = true;
    token++;
  }

  // Hours
  if (token < max_tokens) {
    if (hours != 0) {
      show_hours = true;
      token++;
    } else if (token != 0) {
      if (token + 1 < max_tokens && minutes != 0)
        show_hours = true;
      else if (token + 2 < max_tokens && seconds != 0)
        show_hours = true;

      token++;
    }
  }

  // Minutes
  if (token < max_tokens) {
    if (minutes != 0 ) {
      show_minutes = true;
      token++;
    } else if (token != 0) {
      if (token + 1 < max_tokens && seconds != 0)
        show_minutes = true;

      token++;
    }
  }

  // Seconds
  if (token < max_tokens && (seconds != 0 || token == 0))
    show_seconds = true;

  // Output
  if (timespan.count() < 0) {
    *buffer = '-';
    buffer++;
  }

  *buffer = '\0';

  StaticString<16> component_buffer;

  if (show_days) {
    component_buffer.Format("%u days", days);
    strcat(buffer, component_buffer);
  }

  if (show_hours) {
    if (!StringIsEmpty(buffer))
      strcat(buffer, separator);

    component_buffer.Format("%u h", hours);
    strcat(buffer, component_buffer);
  }

  if (show_minutes) {
    if (!StringIsEmpty(buffer))
      strcat(buffer, separator);

    component_buffer.Format("%u min", minutes);
    strcat(buffer, component_buffer);
  }

  if (show_seconds) {
    if (!StringIsEmpty(buffer))
      strcat(buffer, separator);

    component_buffer.Format("%u sec", seconds);
    strcat(buffer, component_buffer);
  }
}
