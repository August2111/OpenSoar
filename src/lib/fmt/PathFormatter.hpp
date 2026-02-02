// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "system/Path.hpp"

#if 1 // w.o. fmt/core.h not available
# include <fmt/core.h>
# if FMT_VERSION >= 80000 && FMT_VERSION < 90000
#   include <fmt/format.h>
# endif
#else
# include <fmt/format.h>
#endif

template<>
struct fmt::formatter<Path> : formatter<string_view>
{
  template <typename FormatContext>
  // auto format(Path path, FormatContext &ctx) const
  auto format(const Path path, FormatContext &ctx) const
  {
    return formatter<string_view>::format(path.ToUTF8(), ctx);
  }
};

template<>
struct fmt::formatter<AllocatedPath> : formatter<Path> {};
