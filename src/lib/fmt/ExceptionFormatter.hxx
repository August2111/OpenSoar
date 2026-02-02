// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#ifndef EXCEPTION_FORMATTER_HXX
#define EXCEPTION_FORMATTER_HXX

#include "util/Exception.hxx"

#if 0 // w.o. fmt/core.h not available
# include <fmt/core.h>
# if FMT_VERSION >= 80000 && FMT_VERSION < 90000
#   include <fmt/format.h>
# endif
#else
# include <fmt/format.h>
#endif

template<>
struct fmt::formatter<std::exception_ptr> : formatter<string_view>
{
	template<typename FormatContext>
	auto format(std::exception_ptr e, FormatContext &ctx) {
		return formatter<string_view>::format(GetFullMessage(e), ctx);
	}
};

#endif
