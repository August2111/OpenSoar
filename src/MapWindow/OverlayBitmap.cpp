// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "OverlayBitmap.hpp"
#include "ui/canvas/Canvas.hpp"
#if 1  // only temporary for Debug purposes
# include "LogFile.hpp"
#endif
#ifdef ENABLE_OPENGL
# include "ui/canvas/opengl/Texture.hpp"
# include "ui/canvas/opengl/Scope.hpp"
# include "ui/canvas/opengl/ConstantAlpha.hpp"
# include "ui/canvas/opengl/VertexPointer.hpp"
#elif defined (USE_GDI)
# include "ui/canvas/gdi/BufferCanvas.hpp"
#endif
#include "Projection/WindowProjection.hpp"
#include "Math/Point2D.hpp"
#include "Math/Quadrilateral.hpp"
#include "Math/Boost/Point.hpp"
#include "system/Path.hpp"
#include "util/StaticArray.hxx"

#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/strategies/strategies.hpp>

using ArrayQuadrilateral = StaticArray<DoublePoint2D, 5>;
BOOST_GEOMETRY_REGISTER_RING(ArrayQuadrilateral);

using ClippedPolygon = boost::geometry::model::polygon<DoublePoint2D>;

using ClippedMultiPolygon =
  boost::geometry::model::multi_polygon<ClippedPolygon>;

MapOverlayBitmap::MapOverlayBitmap(Path path)
  :label((path.GetBase() != nullptr ? path.GetBase() : path).c_str())
{
  bounds = bitmap.LoadGeoFile(path);
  simple_bounds = bounds.GetBounds();
}

/**
 * Convert a GeoPoint to a "fake" flat DoublePoint2D.  This conversion
 * is flawed in many ways, but good enough for clipping polygons.
 */
static constexpr DoublePoint2D
GeoTo2D(GeoPoint p) noexcept
{
  return {p.longitude.Native(), p.latitude.Native()};
}

#ifdef ENABLE_OPENGL
/**
 * Inverse of GeoTo2D().
 * - Only used with OpenGL yet
 */
static constexpr GeoPoint
GeoFrom2D(DoublePoint2D p) noexcept
{
  return {Angle::Native(p.x), Angle::Native(p.y)};
}
#endif  // ENABLE_OPENGL

/**
 * Convert a #GeoBounds instance to a boost::geometry box.
 */
[[gnu::const]]
static boost::geometry::model::box<DoublePoint2D>
ToBox(const GeoBounds b) noexcept
{
  return {GeoTo2D(b.GetSouthWest()), GeoTo2D(b.GetNorthEast())};
}

/**
 * Convert a #GeoQuadrilateral instance to a boost::geometry ring.
 */
[[gnu::const]]
static ArrayQuadrilateral
ToArrayQuadrilateral(const GeoQuadrilateral q) noexcept
{
  return {GeoTo2D(q.top_left), GeoTo2D(q.top_right),
      GeoTo2D(q.bottom_right), GeoTo2D(q.bottom_left),
      /* close the ring: */
      GeoTo2D(q.top_left) };
}

/**
 * Clip the quadrilateral inside the screen bounds.
 */
[[gnu::pure]]
static ClippedMultiPolygon
Clip(const GeoQuadrilateral &_geo, const GeoBounds &_bounds) noexcept
{
  const auto geo = ToArrayQuadrilateral(_geo);
  const auto bounds = ToBox(_bounds);

  ClippedMultiPolygon clipped;

  try {
    boost::geometry::intersection(geo, bounds, clipped);
  } catch (const boost::geometry::exception &) {
    /* this can (theoretically) occur with self-intersecting
       geometries; in that case, return an empty polygon */
  }

  return clipped;
}

#ifdef ENABLE_OPENGL
// only used in OpenGL case
[[gnu::pure]]
static DoublePoint2D
MapInQuadrilateral(const GeoQuadrilateral &q, const GeoPoint p) noexcept
{
  return MapInQuadrilateral(GeoTo2D(q.top_left), GeoTo2D(q.top_right),
                            GeoTo2D(q.bottom_right), GeoTo2D(q.bottom_left),
                            GeoTo2D(p));
}
#endif

bool
MapOverlayBitmap::IsInside(GeoPoint p) const noexcept
{
  return simple_bounds.IsInside(p) &&
    boost::geometry::covered_by(GeoTo2D(p), ToArrayQuadrilateral(bounds));
}

void
MapOverlayBitmap::Draw([[maybe_unused]] Canvas &canvas,
                       [[maybe_unused]] const WindowProjection &projection)
                        noexcept
{
  if (!simple_bounds.Overlaps(projection.GetScreenBounds()))
    /* not visible, outside of screen area */
    return;

  auto clipped = Clip(bounds, projection.GetScreenBounds());
  if (clipped.empty())
    return;


#ifdef _DEBUG
  // projection.AngleToPixels();
  auto center_point = projection.GetScreenOrigin();
  auto center_chord = projection.GetGeoLocation();
#endif


#if defined(ENABLE_OPENGL)  || 0
  GLTexture &texture = *bitmap.GetNative();
  const PixelSize allocated = texture.GetAllocatedSize();
  const double x_factor = double(texture.GetWidth()) / allocated.width;
  const double y_factor = double(texture.GetHeight()) / allocated.height;

  Point2D<GLfloat> coord[16];
  BulkPixelPoint vertices[16];

  const ScopeVertexPointer vp(vertices);

  texture.Bind();

  const ScopeTextureConstantAlpha blend(use_bitmap_alpha, alpha);

  glEnableVertexAttribArray(OpenGL::Attribute::TEXCOORD);
  glVertexAttribPointer(OpenGL::Attribute::TEXCOORD, 2, GL_FLOAT, GL_FALSE,
                        0, coord);

  for (const auto &polygon : clipped) {
    const auto &ring = polygon.outer();

    size_t n = ring.size();
    if (ring.front() == ring.back())
      --n;

    for (size_t i = 0; i < n; ++i) {
      const auto v = GeoFrom2D(ring[i]);

      auto p = MapInQuadrilateral(bounds, v);
#ifdef ANDROID
      // TODO(August2111): In Android Bitmap does not set the flip value!
      // if (!bitmap.IsFlipped())
#if 1  // only temporary for Debug purposes
      static uint64_t counter = 0;
      if (counter++ < 3)  LogFmt("Bitmap is {}",
        bitmap.IsFlipped() ? "FLIPPED" : "not flipped!");
#endif
#else
#endif
      if (bitmap.IsFlipped())
        p.y = 1 - p.y;
      coord[i].x = p.x * x_factor;
      coord[i].y = p.y * y_factor;

      vertices[i] = projection.GeoToScreen(v);
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, n);
  }

  glDisableVertexAttribArray(OpenGL::Attribute::TEXCOORD);

#else  // ENABLE_OPENGL
//  bitmap.bounds
 // GeoTo2D(simple_bounds)
    // GeoBounds b1 = ((GeoBitmap)bitmap).GetBounds(const TileData & data);
    //GeoQuadrilateral g = bitmap.GetGeoQuadrilateral(const TileData & data);
  // boost::geometry::model::box<DoublePoint2D> box = ToBox(simple_bounds);

  auto ChartWest = simple_bounds.GetWest().Native();
  auto ChartNorth = simple_bounds.GetNorth().Native();
  auto ChartWidth = simple_bounds.GetWidth().Native();
  auto ChartHeight = simple_bounds.GetHeight().Native();

  auto x = projection.GetScreenBounds();
  auto MapWidth = x.GetWidth().Native();
  auto MapHeight = x.GetHeight().Native();
  auto MapWest = x.GetWest().Native();
  auto MapNorth = x.GetNorth().Native();

  // Zusatzoffset:
  PixelPoint null_point = { 0, 0 };

 if (bitmap.GetWidth() == bitmap.GetHeight() && bitmap.GetWidth() <= 1024) {
   PixelPoint top_left(
     (long)round(((ChartWest - center_chord.longitude.Native()) / ChartWidth) * bitmap.GetWidth()),
    -(long)round(((ChartNorth - center_chord.latitude.Native()) / ChartHeight) * bitmap.GetHeight())
   );
   auto center = projection.GetScreenCenter();
   PixelPoint ref_point = {
     (long)(center_point.x % bitmap.GetWidth()),
     (long)(center_point.y % bitmap.GetHeight()) };
   auto src_point = top_left + center_point;
#if 1 && defined(_DEBUG)
 if (src_point.x > 0)
   src_point.x += (1 + src_point.x / 256) * 5;
 if (src_point.y > 0)
   src_point.y += (1 + src_point.y / 256) * 5;
#endif

   canvas.Copy(src_point, bitmap.GetSize(), bitmap, null_point);  // = Copy..
 } else {
   PixelPoint src_point(
    -(long)(((ChartWest - MapWest) / ChartWidth) * bitmap.GetWidth()),
    +(long)(((ChartNorth - MapNorth) / ChartHeight) * bitmap.GetHeight())
   );
   PixelSize src_size(
     1+(long)((MapWidth * bitmap.GetWidth()) / ChartWidth),
     1+(long)((MapHeight * bitmap.GetHeight()) / ChartHeight)
   );
   // src_point = -src_point;
    canvas.Stretch({ 0, 0 }, canvas.GetSize(), bitmap, src_point, src_size);
 }

#endif  // ENABLE_OPENGL
}
