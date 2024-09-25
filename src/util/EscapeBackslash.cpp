// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "EscapeBackslash.hpp"

#include <string.h>

std::string_view::pointer
UnescapeBackslash(std::string_view old_string) noexcept
{
  char buffer[2048]; // Note - max size of any string we cope with here !

  std::string_view::size_type used = 0;

  for (std::string_view::size_type i = 0; i < old_string.size(); i++) {
    if (used < 2045) {
      if (old_string[i] == '\\') {
        if (old_string[i + 1] == 'r') {
          buffer[used++] = '\r';
          i++;
        } else if (old_string[i + 1] == 'n') {
          buffer[used++] = '\n';
          i++;
        } else if (old_string[i + 1] == '\\') {
          buffer[used++] = '\\';
          i++;
        } else {
          buffer[used++] = old_string[i];
        }
      } else {
        buffer[used++] = old_string[i];
      }
    }
  }

  buffer[used++] = '\0';

  return strdup(buffer);
}
