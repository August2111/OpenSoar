// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include "util/StringBuffer.hxx"

#if 1 // w.o. fmt/core.h not available
# include <fmt/core.h>
# if !defined(FMT_VERSION) || (FMT_VERSION >= 80000 && FMT_VERSION < 90000)
#   include <fmt/format.h>
# endif
#else
# include <fmt/format.h>
#endif

template<std::size_t size>
StringBuffer<size> &
VFmtToBuffer(StringBuffer<size> &buffer,
	     fmt::string_view format_str, fmt::format_args args) noexcept
{
	auto [p, _] = fmt::vformat_to_n(buffer.begin(), buffer.capacity() - 1,
					format_str, args);
	*p = 0;
	return buffer;
}

template<std::size_t size>
[[nodiscard]] [[gnu::pure]]
auto
VFmtBuffer(fmt::string_view format_str, fmt::format_args args) noexcept
{
	StringBuffer<size> buffer;
	return VFmtToBuffer(buffer, format_str, args);
}

template<std::size_t size, typename S, typename... Args>
StringBuffer<size> &
FmtToBuffer(StringBuffer<size> &buffer,
	    const S &format_str, Args&&... args) noexcept
{
#if FMT_VERSION >= 90000
	return VFmtToBuffer(buffer, format_str,
			    fmt::make_format_args(args...));
#else
	return VFmtToBuffer(buffer, fmt::to_string_view(format_str),
			    fmt::make_args_checked<Args...>(format_str,
							    args...));
#endif
}

template<std::size_t size, typename S, typename... Args>
[[nodiscard]] [[gnu::pure]]
auto
FmtBuffer(const S &format_str, Args&&... args) noexcept
{
#if FMT_VERSION >= 90000
	return VFmtBuffer<size>(format_str,
				fmt::make_format_args(args...));
#else
	return VFmtBuffer<size>(fmt::to_string_view(format_str),
				fmt::make_args_checked<Args...>(format_str,
								args...));
#endif
}
