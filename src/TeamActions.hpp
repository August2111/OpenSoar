// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once


class FlarmId;

namespace TeamActions {

/**
 * Track the specified FLARM peer.
 */
void
TrackFlarm(FlarmId id, const char *callsign=nullptr) noexcept;

};
