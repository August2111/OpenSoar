// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "LibTiff.hpp"
#include "UncompressedImage.hpp"
#include "system/Path.hpp"
#include "util/ScopeExit.hxx"

#include <stdexcept>

#include <tiffio.h>

#ifdef USE_GEOTIFF
#include "Geo/Quadrilateral.hpp"

#include <geotiff.h>
#include <geo_normalize.h>
#include <geovalues.h>
#include <xtiffio.h>
#endif

static TIFF *
TiffOpen(Path path, const char *mode)
{
#ifdef USE_GEOTIFF
  XTIFFInitialize();
#endif

  return TIFFOpen(path.c_str(), mode);
}

#if defined(_WIN32)
// bool TiffLoader::blur_tiff = true;
bool blur_tiff = true;
bool ToggleBlur() {
  blur_tiff = !blur_tiff;
  return blur_tiff;
}
bool GetBlur() {
    return blur_tiff;
}
#endif  // defined(_WIN32)

class TiffLoader {
  TIFF *const tiff;

public:
  explicit TiffLoader(Path path)
    :tiff(TiffOpen(path, "r")) {
    if (tiff == nullptr)
      throw std::runtime_error("Failed to open TIFF file");
  }

  ~TiffLoader() {
    TIFFClose(tiff);
  }

  TIFF *Get() {
    return tiff;
  }

  void GetField(uint32_t tag, int &value_r) {
    TIFFGetField(tiff, tag, &value_r);
  }

  void RGBAImageBegin(TIFFRGBAImage &img) {
    char emsg[1024];
    if (!TIFFRGBAImageBegin(&img, tiff, 0, emsg))
      throw std::runtime_error(emsg);
  }
};

#if defined(_WIN32)
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
#if  0 //  defined(_WIN32)
        auto d = (c & 1) ? c : (c + 2) % 4;  // swap R and B for RGBA format
        dst[d] = (uint8_t)(val + 0.5f);
#else
        dst[c] = (uint8_t)(val + 0.5f);
#endif
      }
    }
  }

  return UncompressedImage(format, dst_pitch, dst_w, dst_h,
    std::move(dst_data), src.IsFlipped());
}
#endif  // defined(_WIN32)



static UncompressedImage
LoadTiff(TIFFRGBAImage &img)
{
  if (img.width > 8192 || img.height > 8192)
    throw std::runtime_error("TIFF file is too large");

  std::unique_ptr<uint8_t[]> data(new uint8_t[img.width * img.height * 4]);
  uint32_t *data32 = (uint32_t *)(void *)data.get();

  if (!TIFFRGBAImageGet(&img, data32, img.width, img.height))
    throw std::runtime_error("Failed to copy TIFF data");

  auto uncompressed = UncompressedImage(UncompressedImage::Format::RGBA, img.width * 4,
    img.width, img.height, std::move(data), true);
#if defined(_WIN32)
  if (blur_tiff)
    uncompressed = BilinearUpscale(std::move(uncompressed), 4);
#endif  // _WIN32
  return uncompressed;
}

static UncompressedImage
LoadTiff(TiffLoader &tiff)
{
  TIFFRGBAImage img;
  tiff.RGBAImageBegin(img);

  AtScopeExit(&img) { TIFFRGBAImageEnd(&img); };

  return LoadTiff(img);
}

UncompressedImage
LoadTiff(Path path)
{
  TiffLoader tiff(path);
  return LoadTiff(tiff);
}

#ifdef USE_GEOTIFF

static GeoPoint
TiffPixelToGeoPoint(GTIF &gtif, GTIFDefn &defn, double x, double y)
{
  if (!GTIFImageToPCS(&gtif, &x, &y))
    return GeoPoint::Invalid();

  if (defn.Model != ModelTypeGeographic &&
      !GTIFProj4ToLatLong(&defn, 1, &x, &y))
    return GeoPoint::Invalid();

  return GeoPoint(Angle::Degrees(x), Angle::Degrees(y));
}

std::pair<UncompressedImage, GeoQuadrilateral>
LoadGeoTiff(Path path)
{
  TiffLoader tiff(path);

  GeoQuadrilateral bounds;

  {
    auto gtif = GTIFNew(tiff.Get());
    if (gtif == nullptr)
      throw std::runtime_error("Not a GeoTIFF file");

    AtScopeExit(gtif) { GTIFFree(gtif); };

    GTIFDefn defn;
    if (!GTIFGetDefn(gtif, &defn))
      throw std::runtime_error("Failed to parse GeoTIFF metadata");

    int width, height;
    tiff.GetField(TIFFTAG_IMAGEWIDTH, width);
    tiff.GetField(TIFFTAG_IMAGELENGTH, height);

    bounds.top_left = TiffPixelToGeoPoint(*gtif, defn, 0, 0);
    bounds.top_right = TiffPixelToGeoPoint(*gtif, defn, width, 0);
    bounds.bottom_left = TiffPixelToGeoPoint(*gtif, defn, 0, height);
    bounds.bottom_right = TiffPixelToGeoPoint(*gtif, defn, width, height);

    if (!bounds.Check())
      throw std::runtime_error("Invalid GeoTIFF bounds");
  }

  return std::make_pair(LoadTiff(tiff), bounds);
}

#endif
