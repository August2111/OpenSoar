// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "WindArrowRenderer.hpp"
#include "TextInBox.hpp"
#include "Look/WindArrowLook.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Screen/Layout.hpp"
#include "Math/Angle.hpp"
#include "Math/Constants.hpp"
#include "Math/Util.hpp"
#include "Math/Screen.hpp"
#include "NMEA/Derived.hpp"
#include "NMEA/MoreData.hpp"
#include "Units/Units.hpp"
#include "util/Macros.hpp"
#include "MapSettings.hpp"


#ifdef ENABLE_OPENGL
#include "ui/canvas/opengl/Scope.hpp"
#endif

void
WindArrowRenderer::DrawArrow(Canvas &canvas, PixelPoint pos, Angle angle,
                             unsigned width, unsigned length,
                             unsigned tail_length,
                             WindArrowStyle arrow_style,
                             int offset,
                             unsigned scale,
                             const Brush &brush) noexcept
{
  // Draw arrow

  BulkPixelPoint arrow[] = {
    { 0, -offset + int(tail_length) },
    { -int(width), -offset - int(tail_length + length) },
    { 0, -offset - int(length - tail_length) },
    { int(width), -offset - int(tail_length + length) },
  };

  // Rotate the arrow
  PolygonRotateShift({arrow, ARRAY_SIZE(arrow)}, pos, angle, scale);

  canvas.Select(look.arrow_pen);
  canvas.Select(brush);
  {
#ifdef ENABLE_OPENGL
    const ScopeAlphaBlend alpha_blend;
#endif
    canvas.DrawPolygon(arrow, ARRAY_SIZE(arrow));
  }

  // Draw arrow shaft

  if (arrow_style == WindArrowStyle::FULL_ARROW) {
    BulkPixelPoint shaft[] = {
      { 0, -offset + int(tail_length) },
      { 0, -offset - int(tail_length + std::min(20u, length) * 3u) },
    };

    PolygonRotateShift(shaft, pos, angle, scale);

    canvas.Select(look.shaft_pen);
    canvas.DrawLine(shaft[0], shaft[1]);
  }
}

void
WindArrowRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                        const SpeedVector wind, const PixelPoint pos,
                        const PixelRect &rc, WindArrowStyle arrow_style,
                        const Brush &brush) noexcept
{
  constexpr unsigned arrow_width = 6;
  constexpr unsigned arrow_tail_length = 3;
  constexpr unsigned arrow_offset = 23;

  const unsigned scale = Layout::Scale(100U);
  const Angle angle = wind.bearing - screen_angle;

  // Draw arrow (and shaft)

  const unsigned length = uround(4 * wind.norm);
  DrawArrow(canvas, pos, angle,
            arrow_width, length, arrow_tail_length,
            arrow_style,
            arrow_offset,
            scale, brush);

  // Draw wind speed label
  StaticString<12> buffer;
  buffer.Format("%i", iround(Units::ToUserWindSpeed(wind.norm)));

  canvas.SetTextColor(COLOR_BLACK);
  canvas.Select(*look.font);

  BulkPixelPoint label[] = {
    { 0, -int(arrow_offset + length + arrow_tail_length) },
  };
  PolygonRotateShift(label, pos, angle, scale);

  TextInBoxMode style;
  style.align = TextInBoxMode::Alignment::CENTER;
  style.vertical_position = TextInBoxMode::VerticalPosition::CENTERED;
  style.shape = LabelShape::OUTLINED;

  TextInBox(canvas, buffer, label[0], style, rc);
}

void
WindArrowRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                        const PixelPoint pos, const PixelRect &rc,
                        const DerivedInfo &calculated, const MoreData &basic,
                        const MapSettings &settings) noexcept
{
  if (!calculated.wind_available ||
      settings.wind_arrow_style == WindArrowStyle::NO_ARROW)
    return;

  // don't bother drawing it if not significant
  if (calculated.wind.norm < 1)
    return;

  WindArrowRenderer::Draw(canvas, screen_angle, calculated.wind, pos, rc,
                          settings.wind_arrow_style, 
                          (calculated.wind_source == 
                          DerivedInfo::WindSource::EXTERNAL) ?
                          look.arrow_brush_extern : look.arrow_brush);

  if (!basic.external_instantaneous_wind_available) {
    return;
  }

  WindArrowRenderer::Draw(canvas, screen_angle,
                          basic.external_instantaneous_wind, pos, rc,
                          settings.wind_arrow_style,
                          look.arrow_brush_instantaneous);
}
