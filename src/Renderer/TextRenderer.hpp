// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include <string>

struct PixelRect;
class Canvas;
class Font;

/**
 * Render multi-line text.
 */
class TextRenderer {
  bool center = false, vcenter = false;

#ifndef USE_GDI
  bool control = false;
#endif

public:
  constexpr void SetCenter(bool _center=true) noexcept {
    center = _center;
  }

  constexpr void SetVCenter(bool _vcenter=true) noexcept {
    vcenter = _vcenter;
  }

  constexpr void SetControl([[maybe_unused]] bool _control=true) noexcept {
#ifndef USE_GDI
    control = _control;
#endif
  }

  void InvalidateLayout() noexcept {}

  [[gnu::pure]]
  unsigned GetHeight(Canvas &canvas, PixelRect rc,
                     std::string_view text) const noexcept;

  [[gnu::pure]]
  unsigned GetHeight(Canvas &canvas, unsigned width,
                     std::string_view text) const noexcept;

  [[gnu::pure]]
  unsigned GetHeight(const Font &font, unsigned width,
                     std::string_view text) const noexcept;

  void Draw(Canvas &canvas, PixelRect rc, std::string_view text) const noexcept;
};
