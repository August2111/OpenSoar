// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Printing.hpp"
#include "harness_airspace.hpp"
#include "Route/AirspaceRoute.hpp"
#include "Engine/Airspace/AirspaceAircraftPerformance.hpp"
#include "Engine/Airspace/Predicate/AirspacePredicate.hpp"
#include "Geo/SpeedVector.hpp"
#include "Geo/GeoVector.hpp"
#include "GlideSolvers/GlideSettings.hpp"
#include "GlideSolvers/GlidePolar.hpp"
#include "Terrain/RasterMap.hpp"
#include "Terrain/Loader.hpp"
#include "system/FileUtil.hpp"
#include "Compatibility/path.h"
#include "Operation/Operation.hpp"
#include "util/PrintException.hxx"
#include "test_debug.hpp"

#include <zzip/zzip.h>

#include <fstream>

#include <string.h>

extern "C" {
#include "tap.h"
}

static constexpr unsigned NUM_SOL = 15;

static bool
test_route(const unsigned n_airspaces, const RasterMap& map)
{
  Airspaces airspaces;
  setup_airspaces(airspaces, map.GetMapCenter(), n_airspaces);

  {
    Directory::Create(Path("output/results"));
    std::ofstream fout("output/results/terrain.txt");

    unsigned nx = 100;
    unsigned ny = 100;
    GeoPoint origin(map.GetMapCenter());

    for (unsigned i = 0; i < nx; ++i) {
      for (unsigned j = 0; j < ny; ++j) {
        auto fx = (double)i / (nx - 1) * 2 - 1;
        auto fy = (double)j / (ny - 1) * 2 - 1;
        GeoPoint x(origin.longitude + Angle::Degrees(0.2 + 0.7 * fx),
                   origin.latitude + Angle::Degrees(0.9 * fy));
        auto h = map.GetInterpolatedHeight(x);
        fout << x.longitude.Degrees() << " " << x.latitude.Degrees()
             << " " << h.GetValue() << "\n";
      }

      fout << "\n";
    }

    fout << "\n";
  }

  {
    // local scope, see what happens when we go out of scope
    GeoPoint p_start(Angle::Degrees(-0.3), Angle::Degrees(0.0));
    p_start += map.GetMapCenter();

    GeoPoint p_dest(Angle::Degrees(0.8), Angle::Degrees(-0.7));
    p_dest += map.GetMapCenter();

    AGeoPoint loc_start(p_start, map.GetHeight(p_start).GetValueOr0() + 100);
    AGeoPoint loc_end(p_dest, map.GetHeight(p_dest).GetValueOr0() + 100);

    AircraftState state;
    GlidePolar glide_polar(0.1);
    const AirspaceAircraftPerformance perf(glide_polar);

    GeoVector vec(loc_start, loc_end);
    auto range = 10000 + vec.distance / 2;

    state.location = loc_start;
    state.altitude = loc_start.altitude;

    {
      Airspaces as_route;
      // dummy

      // real one, see if items changed
      as_route.SynchroniseInRange(airspaces, vec.MidPoint(loc_start), range,
                                  AirspacePredicateTrue);
      int size_1 = as_route.GetSize();
      if (verbose)
        printf("# route airspace size %d\n", size_1);

      as_route.SynchroniseInRange(airspaces, vec.MidPoint(loc_start), 1,
                                  AirspacePredicateTrue);
      int size_2 = as_route.GetSize();
      if (verbose)
        printf("# route airspace size %d\n", size_2);

      ok(size_2 < size_1, "shrink as", 0);

      // go back
      as_route.SynchroniseInRange(airspaces, vec.MidPoint(loc_end), range,
                                  AirspacePredicateTrue);
      int size_3 = as_route.GetSize();
      if (verbose)
        printf("# route airspace size %d\n", size_3);

      ok(size_3 >= size_2, "grow as", 0);

      // and again
      as_route.SynchroniseInRange(airspaces, vec.MidPoint(loc_start), range,
                                  AirspacePredicateTrue);
      int size_4 = as_route.GetSize();
      if (verbose)
        printf("# route airspace size %d\n", size_4);

      ok(size_4 >= size_3, "grow as", 0);

      scan_airspaces(state, as_route, perf, true, loc_end);
    }

    // try the solver
    SpeedVector wind(Angle::Degrees(0), 0);
    GlidePolar polar(1);

    GlideSettings settings;
    settings.SetDefaults();
    RoutePlannerConfig config;
    config.mode = RoutePlannerConfig::Mode::BOTH;

    AirspaceRoute route;
    route.UpdatePolar(settings, config, polar, polar, wind);
    route.SetTerrain(&map);

    auto predicate = AirspacePredicateTrue;

    bool sol = false;
    for (unsigned i = 0; i < NUM_SOL; i++) {
      loc_end.latitude += Angle::Degrees(0.1);
      loc_end.altitude = map.GetHeight(loc_end).GetValueOr0() + 100;
      route.Synchronise(airspaces, predicate, loc_start, loc_end);
      if (route.Solve(loc_start, loc_end, config)) {
        sol = true;
        if (verbose) {
          PrintHelper::print_route(route);
        }
      } else {
        if (verbose) {
          printf("# fail\n");
        }
        sol = false;
      }
      char buffer[80];
      sprintf(buffer, "route %d solution", i);
      ok(sol, buffer, 0);
    }
  }

  return true;
}

int main()
try {
  static const char map_path[] = "tmp/map.xcm";

  ZZIP_DIR *dir = zzip_dir_open(map_path, nullptr);
  if (dir == nullptr) {
    fprintf(stderr, "Failed to open %s\n", map_path);
    return EXIT_FAILURE;
  }

  RasterMap map;

  {
    NullOperationEnvironment operation;
    LoadTerrainOverview(dir, map.GetTileCache(), operation);
  }

  map.UpdateProjection();

  SharedMutex mutex;
  do {
    UpdateTerrainTiles(dir, map.GetTileCache(), mutex,
                       map.GetProjection(),
                       map.GetMapCenter(), 100000);
  } while (map.IsDirty());
  zzip_dir_close(dir);

  plan_tests(4 + NUM_SOL);
  ok(test_route(28, map), "route 28", 0);
  return exit_status();
} catch (const std::runtime_error &e) {
  PrintException(e);
  return EXIT_FAILURE;
}
