// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "Id.hpp"
#include "util/StaticString.hxx"
#include "util/StaticArray.hxx"

#include <cassert>

class FlarmNameDatabase {
public:
  struct Record {
    FlarmId id;
    StaticString<21> name;

    Record() = default;
    Record(FlarmId _id, const char *_name) noexcept
      :id(_id), name(_name) {}
  };

private:
  using Array = StaticArray<Record, 200>;
  using iterator = Array::iterator;

  Array data;

public:
  using const_iterator = Array::const_iterator;

  [[gnu::pure]]
  const_iterator begin() const noexcept {
    return data.begin();
  }

  [[gnu::pure]]
  const_iterator end() const noexcept {
    return data.end();
  }

  [[gnu::pure]]
  const char *Get(FlarmId id) const noexcept;

  [[gnu::pure]]
  FlarmId Get(const char *name) const noexcept;

  /**
   * Look up all records with the specified name.
   *
   * @param max the maximum size of the given buffer
   * @return the number of items copied to the given buffer
   */
  unsigned Get(const char *name,
               FlarmId *buffer, unsigned max) const noexcept;

  bool Set(FlarmId id, const char *name) noexcept;

  bool Remove(FlarmId id) noexcept;

protected:
  [[gnu::pure]]
  int Find(FlarmId id) const noexcept;

  [[gnu::pure]]
  int Find(const char *name) const noexcept;
};
