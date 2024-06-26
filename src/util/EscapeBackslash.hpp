// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include <string>

/**
 * Parses the special characters (cr, lf, back slash) in the old_string and
 * returns the parsed new_string
 * @param old_string The old string with (or without) special characters
 * @return The new parsed string
 */
std::string_view::pointer
UnescapeBackslash(std::string_view old_string) noexcept;
