// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Password.hpp"

#include <algorithm>

const char *
PasswordDataField::GetAsDisplayString() const noexcept
{
  const char *obfuscated = _T("********************************");
  const size_t obfuscated_length = strlen(obfuscated);
  size_t length = std::min(strlen(GetAsString()), obfuscated_length);
  return obfuscated + (obfuscated_length - length);
}
