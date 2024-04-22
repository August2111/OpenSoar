// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "UTF8.hpp"

#define _W(text) (text)
#define _A(text) (text)

#include "StringPointer.hxx"

#include <cassert>

/**
 * Convert a UTF-8 string to a TCHAR string.  The source buffer passed
 * to the constructor must be valid as long as this object is being
 * used.
 */
class UTF8ToWideConverter {
  typedef StringPointer<> Value;
  typedef typename Value::const_pointer const_pointer;

  Value value;

public:
  UTF8ToWideConverter(const_pointer _value) noexcept
    :value(_value)
  {
    assert(_value != nullptr);
  }

  UTF8ToWideConverter(const UTF8ToWideConverter &other) = delete;
  UTF8ToWideConverter &operator=(const UTF8ToWideConverter &other) = delete;

  [[gnu::pure]]
  bool IsValid() const noexcept {
    assert(value != nullptr);

    return ValidateUTF8(value.c_str());
  }

  const_pointer c_str() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }

  operator const_pointer() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }
};

/**
 * Convert a TCHAR string to UTF-8.  The source buffer passed to the
 * constructor must be valid as long as this object is being used.
 */
class WideToUTF8Converter {
  typedef StringPointer<> Value;
  typedef typename Value::const_pointer const_pointer;

  Value value;

public:
  WideToUTF8Converter(const_pointer _value) noexcept
    :value(_value)
  {
    assert(_value != nullptr);
  }

  WideToUTF8Converter(const WideToUTF8Converter &other) = delete;
  WideToUTF8Converter &operator=(const WideToUTF8Converter &other) = delete;

  [[gnu::pure]]
  bool IsValid() const noexcept {
    assert(value != nullptr);

    return true;
  }

  const_pointer c_str() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }

  operator const_pointer() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }
};

/**
 * Convert a TCHAR string to ACP (Windows ANSI code page).  The source
 * buffer passed to the constructor must be valid as long as this
 * object is being used.
 */
class WideToACPConverter {
  typedef StringPointer<> Value;
  typedef typename Value::const_pointer const_pointer;

  Value value;

public:
  WideToACPConverter(const_pointer _value) noexcept
    :value(_value)
  {
    assert(_value != nullptr);
  }

  WideToACPConverter(const WideToACPConverter &other) = delete;
  WideToACPConverter &operator=(const WideToACPConverter &other) = delete;

  [[gnu::pure]]
  bool IsValid() const noexcept {
    assert(value != nullptr);

    return true;
  }

  const_pointer c_str() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }

  operator const_pointer() const noexcept {
    assert(value != nullptr);

    return value.c_str();
  }
};
