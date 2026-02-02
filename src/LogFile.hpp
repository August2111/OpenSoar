// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "util/Compiler.h"

// #if 1 // w.o. fmt/core.h not available
# include <fmt/core.h>
// # if !defined(FMT_VERSION) || (FMT_VERSION >= 80000 && FMT_VERSION < 90000)
// #   include <fmt/format.h>
// # endif
// #else
// #endif
# include <fmt/format.h>

#include <exception>
#include <string_view>

void
LogVFmt(fmt::string_view format_str, fmt::format_args args) noexcept;

template<typename S, typename... Args>
#ifdef _WIN32
void
LogFmt(const S &format_str, Args&&... args) noexcept
{
// #if FMT_VERSION >= 90000
	LogVFmt(format_str,
		       fmt::make_format_args(args...));
// #else
// 	return LogVFmt(fmt::to_string_view(format_str),
// 		       fmt::make_args_checked<Args...>(format_str,
// 						       args...));
// #endif
}
#else
// do nothing -> TODO(August2111): !!!!!
void
LogFmt([[maybe_unused]] const S &format_str, 
  [[maybe_unused]] Args&&... args) noexcept
{
}
#endif

/**
 * Write a line to the log file.
 *
 * @param s the line, which must not contain newline or carriage
 * return characters
 */
void
LogString(std::string_view s) noexcept;

/**
 * Write a formatted line to the log file.
 *
 * @param fmt the format string, which must not contain newline or
 * carriage return characters
 */
gcc_printf(1, 2)
void
LogFormat(const char *fmt, ...) noexcept;

#if !defined(NDEBUG)

#define LogDebug(...) LogFmt(__VA_ARGS__)

#else /* NDEBUG */

/* not using an empty inline function here because we don't want to
   evaluate the parameters */
#define LogDebug(...) do {} while (false)

#endif /* NDEBUG */

void
LogError(std::exception_ptr e) noexcept;

void
LogError(std::exception_ptr e, const char *msg) noexcept;
