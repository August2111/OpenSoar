// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "SystemLoad.hpp"

#ifdef _WIN32

#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <cstdint>

OptionalPercent
SystemLoadCPU() noexcept
{
  static unsigned userTime_last= 0;
  static unsigned kernelTime_last= 0;
  static unsigned tick_last = 0;

  uint64_t  creationTime, exitTime, kernelTime, userTime;
  if (!GetProcessTimes(GetCurrentProcess(),
                       (FILETIME *)&creationTime, (FILETIME *)&exitTime,
                       (FILETIME *)&kernelTime, (FILETIME *)&userTime))
    return std::nullopt;

  unsigned tick = ::GetTickCount();

  userTime /= 10000;
  kernelTime /= 10000;

  OptionalPercent retval = std::nullopt;
  if (tick && (tick_last>0)) {
    unsigned dt_user = userTime-userTime_last;
    unsigned dt_kernel = kernelTime-kernelTime_last;
    unsigned dt = tick-tick_last;
    if (dt)
      retval = (100 * (dt_user + dt_kernel)) / dt;
    else
      retval = 0;
  }

  tick_last = tick;
  userTime_last = userTime;
  kernelTime_last = kernelTime;

  return retval;
}

#elif defined(__linux__)

#include "system/FileUtil.hpp"

#include <algorithm>

struct cpu {
  long busy, idle;
};

OptionalPercent
SystemLoadCPU() noexcept
{
  char line[256];
  if (!File::ReadString(Path("/proc/stat"), line, sizeof(line)))
    return std::nullopt;

  static constexpr unsigned HISTORY_LENGTH = 5;
  static cpu history[HISTORY_LENGTH];

  cpu current;
  long user, nice, system;
  int n = sscanf(line, "cpu  %ld %ld %ld %ld ", &user, &nice, &system,
                 &current.idle);
  if (n != 4)
    return std::nullopt;

  current.busy = user + nice + system;

  const cpu last = history[0];
  std::copy(&history[1], &history[HISTORY_LENGTH], &history[0]);
  history[HISTORY_LENGTH - 1] = current;

  if (last.idle == 0)
    /* first run */
    return std::nullopt;

  const cpu diff = {
    current.busy - last.busy,
    current.idle - last.idle,
  };

  long total = diff.busy + diff.idle;
  if (total <= 0)
    return std::nullopt;

  return diff.busy * 100 / total;
}

#else /* !_WIN32 */

///@todo implement for non-win32
OptionalPercent
SystemLoadCPU() noexcept
{
    return std::nullopt;
}

#endif /* !_WIN32 */
