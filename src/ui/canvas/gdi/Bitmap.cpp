// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "ui/canvas/Bitmap.hpp"
#include "ui/canvas/gdi/GdiPlusBitmap.hpp"
#include "ui/canvas/custom/UncompressedImage.hpp"
#include "ui/canvas/custom/LibTiff.hpp"

#include "Screen/Debug.hpp"
#include "system/Path.hpp"

#include <wingdi.h>
#include <winuser.h>

#include <cassert>
#include <utility>

Bitmap::Bitmap(Bitmap &&src) noexcept
  :bitmap(std::exchange(src.bitmap, nullptr)),
   has_colors(src.has_colors)
{
}

Bitmap &Bitmap::operator=(Bitmap &&src) noexcept
{
  using std::swap;
  swap(bitmap, src.bitmap);
  swap(has_colors, src.has_colors);
  return *this;
}

/**
 * Upscale an UncompressedImage using bilinear interpolation.
 * This produces smoother output than nearest-neighbor scaling,
 * important for coarse forecast grid data that would otherwise
 * appear as large blocky pixels on the map.
 */
static UncompressedImage
BilinearUpscale(UncompressedImage &&src, unsigned scale)
{
  if (scale <= 1)
    return std::move(src);

  const unsigned src_w = src.GetWidth();
  const unsigned src_h = src.GetHeight();
  const unsigned dst_w = src_w * scale;
  const unsigned dst_h = src_h * scale;

  const auto format = src.GetFormat();
  unsigned bpp;
  switch (format) {
    case UncompressedImage::Format::RGBA: bpp = 4; break;
    case UncompressedImage::Format::RGB:  bpp = 3; break;
    case UncompressedImage::Format::GRAY: bpp = 1; break;
    default: return std::move(src);
  }

  const size_t src_pitch = src.GetPitch();
  const auto *src_data = (const uint8_t *)src.GetData();

  const size_t dst_pitch = (size_t)dst_w * bpp;
  auto dst_data = std::make_unique<uint8_t[]>(dst_pitch * dst_h);

  for (unsigned dy = 0; dy < dst_h; dy++) {
    /* map destination pixel center to source coordinate */
    const float sy = ((float)dy + 0.5f) / (float)scale - 0.5f;
    const int isy = (int)floorf(sy);
    const float fy = sy - (float)isy;

    const unsigned csy0 = (unsigned)std::max(isy, 0);
    const unsigned csy1 = std::min((unsigned)(isy + 1), src_h - 1);

    for (unsigned dx = 0; dx < dst_w; dx++) {
      const float sx = ((float)dx + 0.5f) / (float)scale - 0.5f;
      const int isx = (int)floorf(sx);
      const float fx = sx - (float)isx;

      const unsigned csx0 = (unsigned)std::max(isx, 0);
      const unsigned csx1 = std::min((unsigned)(isx + 1), src_w - 1);

      const uint8_t *p00 = src_data + csy0 * src_pitch + csx0 * bpp;
      const uint8_t *p10 = src_data + csy0 * src_pitch + csx1 * bpp;
      const uint8_t *p01 = src_data + csy1 * src_pitch + csx0 * bpp;
      const uint8_t *p11 = src_data + csy1 * src_pitch + csx1 * bpp;

      uint8_t *dst = dst_data.get() + dy * dst_pitch + dx * bpp;

      for (unsigned c = 0; c < bpp; c++) {
        float val = (float)p00[c] * (1.0f - fx) * (1.0f - fy)
          + (float)p10[c] * fx * (1.0f - fy)
          + (float)p01[c] * (1.0f - fx) * fy
          + (float)p11[c] * fx * fy;
        dst[c] = (uint8_t)(val + 0.5f);
      }
    }
  }

  return UncompressedImage(format, dst_pitch, dst_w, dst_h,
    std::move(dst_data), src.IsFlipped());
}

bool
Bitmap::LoadFile(Path path)
{
#if 0
  UncompressedImage image = LoadTiff(path);
  bitmap = GdiLoadImage(std::move(image));
  return IsDefined();
#else
  bitmap = GdiLoadImage(path.c_str());
  return IsDefined();
#endif
}

bool 
Bitmap::Load(UncompressedImage &&uncompressed, [[maybe_unused]] Type type)
{
  Reset();
  uncompressed = BilinearUpscale(std::move(uncompressed), 4);
  bitmap = GdiLoadImage(std::move(uncompressed));
  return IsDefined();
}


void
Bitmap::Reset() noexcept
{
  if (bitmap != nullptr) {
    assert(IsScreenInitialized());

#ifndef NDEBUG
    bool success =
#endif
      ::DeleteObject(bitmap);
    assert(success);

    bitmap = nullptr;
  }
}

PixelSize
Bitmap::GetSize() const noexcept
{
#if defined(_DEBUG)
  if (!IsDefined())
    return { 0, 0 };
#endif
  assert(IsDefined());

  BITMAP bm;
  ::GetObject(bitmap, sizeof(bm), &bm);
  const PixelSize size = { bm.bmWidth, bm.bmHeight };
  return size;
}
