/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_ROUGH_ALTITUDE_HPP
#define XCSOAR_ROUGH_ALTITUDE_HPP

#include "Math/fixed.hpp"

/**
 * Store an rough altitude value, when the exact value is not needed.
 *
 * The accuracy is 1m, and the range is -32768..32767.
 */
class RoughAltitude {
  short value;

public:
  RoughAltitude() {}
  explicit RoughAltitude(short _value):value(_value) {}
  explicit RoughAltitude(int _value):value((short)_value) {}
  RoughAltitude(fixed _value):value(_value) {}

  RoughAltitude &operator=(fixed other) {
    value = other;
    return *this;
  }

  gcc_pure
  operator fixed() const {
    return fixed(value);
  }
};

#endif
