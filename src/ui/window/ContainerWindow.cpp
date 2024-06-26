// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "ContainerWindow.hpp"

unsigned ContainerWindow::exit_value = 0;

void
ContainerWindow::ScrollTo(const PixelRect &rc) noexcept
{
  /* forward the request to the client */
  if (auto *parent = GetParent())
    parent->ScrollTo(ToParentCoordinates(rc));
}
