/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#pragma once

#include "WeGlideObjects.hpp"

#include <cstdint>

/**
 * Settings for the WeGlide access for following tasks:
 * - Uploading IGC files to the server
 * - Get the active task from server
 */
struct WeGlideSettings {
  /**
   * Enable the general communication to the WeGlide server
   */
  bool enabled;

  /**
   * Automatic upload IGC file to WeGlide server after downloading an IGC file
   * from logger/
   */
  bool automatic_upload;

  /**
   * hard coded yet, maybe WeGlide change this in the future, then you have to
   * make it settable!
   * The documentation of the WeGlide API you can find:
   * https://api.weglide.org/docs
  */
#if defined(_DEBUG)
  // for test purposes use the alternative dev url
  static constexpr char dev_url[] = "https://api.devs.glidercheck.com/v1";
  static constexpr char weglide_url[] = "https://api.weglide.org/v1";
  static constexpr char default_url[] = "https://api.devs.glidercheck.com/v1";
  // August2111 - not possible: static constexpr char default_url[] = dev_url;
#else
  static constexpr char default_url[] = "https://api.weglide.org/v1";
#endif
  static constexpr char gliderlist_uri[] = "https://raw.githubusercontent.com/"
    "weglide/GliderList/master/gliderlist.csv";

  WeGlide::User pilot;

  void SetDefaults() noexcept {
    pilot.Clear();

    enabled = false;
    automatic_upload = true; // after enabling WeGlide!
  }
};
