// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "IGCFilenameFormatter.hpp"
#include "time/BrokenDate.hpp"
#include "util/StringFormat.hpp"

#ifdef _UNICODE
#include <algorithm>
#endif

#include <cassert>
#include <string.h>

static TCHAR
NumToIGCChar(unsigned num)
{
  assert(num <= 35);

  if (num < 10)
    return _T('1') + (num - 1);

  return _T('A') + (num - 10);
}

void
FormatIGCFilename(TCHAR* buffer, const BrokenDate &date,
                  TCHAR manufacturer, const TCHAR *logger_id,
                  unsigned flight_number)
{
  assert(logger_id != NULL);
  assert(_tcslen(logger_id) == 3);

  TCHAR cyear = NumToIGCChar(date.year % 10);
  TCHAR cmonth = NumToIGCChar(date.month);
  TCHAR cday = NumToIGCChar(date.day);
  TCHAR cflight = NumToIGCChar(flight_number);

#ifdef _UNICODE
  StringFormatUnsafe(buffer, _T("%lc%lc%lc%lc%ls%c.igc"),
                     cyear, cmonth, cday,
                     manufacturer, logger_id, cflight);
#else
  StringFormatUnsafe(buffer, _T("%c%c%c%c%s%c.igc"),
                     cyear, cmonth, cday,
                     manufacturer, logger_id, cflight);
#endif
}

void
FormatIGCFilenameLong(TCHAR* buffer, const BrokenDate &date,
                      const TCHAR *manufacturer, const TCHAR *logger_id,
                      unsigned flight_number)
{
  // 2003-12-31-XYZ-987-01.igc
  // long filename form of IGC file.
  // XYZ represents manufacturer code

  assert(manufacturer != NULL);
  assert(_tcslen(manufacturer) == 3);

  assert(logger_id != NULL);
  assert(_tcslen(logger_id) == 3);

#ifdef _UNICODE
  StringFormatUnsafe(buffer, _T("%04u-%02u-%02u-%ls-%ls-%02u.igc"),
#else
  StringFormatUnsafe(buffer, _T("%04u-%02u-%02u-%s-%s-%02u.igc"),
#endif
                     date.year, date.month, date.day,
                     manufacturer, logger_id, flight_number);
}

#ifdef _UNICODE

void
FormatIGCFilename(TCHAR* buffer, const BrokenDate &date,
                  char manufacturer, const char *logger_id,
                  unsigned flight_number)
{
  assert(logger_id != NULL);
  assert(strlen(logger_id) == 3);

  TCHAR logger_id_t[4];
  /* poor man's char->TCHAR converted; this works because we know
     we're dealing with ASCII only */
  std::copy_n(logger_id, 4, logger_id_t);

  FormatIGCFilename(buffer, date, (TCHAR)manufacturer, logger_id_t,
                    flight_number);
}

void
FormatIGCFilenameLong(TCHAR* buffer, const BrokenDate &date,
                      const char *manufacturer, const char *logger_id,
                      unsigned flight_number)
{
  assert(manufacturer != NULL);
  assert(strlen(manufacturer) == 3);

  assert(logger_id != NULL);
  assert(strlen(logger_id) == 3);

  TCHAR manufacturer_t[4], logger_id_t[4];
  /* poor man's char->TCHAR converted; this works because we know
     we're dealing with ASCII only */
  std::copy_n(manufacturer, 4, manufacturer_t);
  std::copy_n(logger_id, 4, logger_id_t);

  FormatIGCFilenameLong(buffer, date, manufacturer_t, logger_id_t,
                        flight_number);
}

#endif
