// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "RaspStore.hpp"
#include "Language/Language.hpp"
#include "Units/Units.hpp"
#include "system/Path.hpp"
#include "io/ZipArchive.hpp"
#include "util/StringCompare.hxx"
#include "util/Macros.hpp"
#include <string>
#include "zzip/zzip.h"
#include "LogFile.hpp"

#include <set>

#include <cassert>
#include <stdio.h>
#include <windef.h> // for MAX_PATH

#define RASP_FORMAT "%s.curr.%02u%02ulst.d2.jp2"

static constexpr RaspStore::MapInfo WeatherDescriptors[] = {
  {
    "wstar",
    N_("W*"),
    N_("Average dry thermal updraft strength near mid-BL height.  Subtract glider descent rate to get average vario reading for cloudless thermals.  Updraft strengths will be stronger than this forecast if convective clouds are present, since cloud condensation adds buoyancy aloft (i.e. this neglects \"cloudsuck\").  W* depends upon both the surface heating and the BL depth."),
  },
  {
    "wstar_bsratio",
    N_("W*"),
    N_("Average dry thermal updraft strength near mid-BL height.  Subtract glider descent rate to get average vario reading for cloudless thermals.  Updraft strengths will be stronger than this forecast if convective clouds are present, since cloud condensation adds buoyancy aloft (i.e. this neglects \"cloudsuck\").  W* depends upon both the surface heating and the BL depth."),
  },
  {
    "blwindspd",
    N_("BL Wind spd"),
    N_("The speed and direction of the vector-averaged wind in the BL.  This prediction can be misleading if there is a large change in wind direction through the BL."),
  },
  {
    "hbl",
    N_("H bl"),
    N_("Height of the top of the mixing layer, which for thermal convection is the average top of a dry thermal.  Over flat terrain, maximum thermalling heights will be lower due to the glider descent rate and other factors.  In the presence of clouds (which release additional buoyancy aloft, creating \"cloudsuck\") the updraft top will be above this forecast, but the maximum thermalling height will then be limited by the cloud base.  Further, when the mixing results from shear turbulence rather than thermal mixing this parameter is not useful for glider flying. "),
  },
  {
    "dwcrit",
    N_("dwcrit"),
    N_("This parameter estimates the height above ground at which the average dry updraft strength drops below 225 fpm and is expected to give better quantitative numbers for the maximum cloudless thermalling height than the BL Top height, especially when mixing results from vertical wind shear rather than thermals.  (Note: the present assumptions tend to underpredict the max. thermalling height for dry consitions.) In the presence of clouds the maximum thermalling height may instead be limited by the cloud base.  Being for \"dry\" thermals, this parameter omits the effect of \"cloudsuck\"."),
  },
  {
    "blcloudpct",
    N_("bl cloud"),
    N_("This parameter provides an additional means of evaluating the formation of clouds within the BL and might be used either in conjunction with or instead of the other cloud prediction parameters.  It assumes a very simple relationship between cloud cover percentage and the maximum relative humidity within the BL.  The cloud base height is not predicted, but is expected to be below the BL Top height."),
  },
  {
    "sfctemp",
    N_("Sfc temp"),
    N_("The temperature at a height of 2m above ground level.  This can be compared to observed surface temperatures as an indication of model simulation accuracy; e.g. if observed surface temperatures are significantly below those forecast, then soaring conditions will be poorer than forecast."),
  },
  {
    "hwcrit",
    N_("hwcrit"),
    N_("This parameter estimates the height at which the average dry updraft strength drops below 225 fpm and is expected to give better quantitative numbers for the maximum cloudless thermalling height than the BL Top height, especially when mixing results from vertical wind shear rather than thermals.  (Note: the present assumptions tend to underpredict the max. thermalling height for dry consitions.) In the presence of clouds the maximum thermalling height may instead be limited by the cloud base.  Being for \"dry\" thermals, this parameter omits the effect of \"cloudsuck\"."),
  },
  {
    "wblmaxmin",
    N_("wblmaxmin"),
    N_("Maximum grid-area-averaged extensive upward or downward motion within the BL as created by horizontal wind convergence. Positive convergence is associated with local small-scale convergence lines.  Negative convergence (divergence) produces subsiding vertical motion, creating low-level inversions which limit thermalling heights."),
  },
  {
    "blcwbase",
    N_("blcwbase"),
    nullptr,
  },
};

RaspStore::MapItem::MapItem(const char *_name)
  :name(_name)
{
  std::fill_n(times, ARRAY_SIZE(times), false);
}

BrokenTime
RaspStore::IndexToTime(unsigned index)
{
  return BrokenTime(index / 4, (index % 4) * 15);
}

unsigned
RaspStore::GetNearestTime(unsigned item_index, unsigned time_index) const
{
  assert(item_index < maps.size());
  assert(time_index < MAX_WEATHER_TIMES);

  // scan forward to next valid time
  for (unsigned t = time_index; t < MAX_WEATHER_TIMES; ++t)
    if (IsTimeAvailable(item_index, t))
      return t;

  for (int t = time_index; t >= 0; --t)
    if (IsTimeAvailable(item_index, t))
      return t;

  return MAX_WEATHER_TIMES;
}

bool
RaspStore::WeatherFilename(char *filename, Path path, unsigned time_index)
{
//  if (path != nullptr)
  if (path.empty())
    return false;
  const BrokenTime t = IndexToTime(time_index);
  // MAX_PATH - max length of filename
  snprintf(filename, MAX_PATH, RASP_FORMAT,
          path.c_str(), t.hour, t.minute);
  return true;
}

std::unique_ptr<ZipArchive>
RaspStore::OpenArchive() const
{
  return std::make_unique<ZipArchive>(path);
}

bool
RaspStore::ExistsItem(const ZipArchive &archive, Path name, unsigned time_index)
{
  char filename[MAX_PATH];
  if (!WeatherFilename(filename, name, time_index))
    return false;

  return archive.Exists(filename);
}

bool
RaspStore::ScanMapItem(const ZipArchive &archive, MapItem &item)
{
  bool found = false;
  for (unsigned i = 0; i < MAX_WEATHER_TIMES; i++)
    if (ExistsItem(archive, Path(item.name), i))
      found = item.times[i] = true;

  return found;
}

void
RaspStore::ScanAll()
try {
  /* not holding the lock here, because this method is only called
     during startup, when the other threads aren't running yet */

  auto archive = OpenArchive();
  if (!archive)
    return;

  maps.clear();

  std::set<std::string> names;

  for (const auto &i : WeatherDescriptors) {
    if (maps.full())
      break;

    MapItem item(i.name);
    item.label = i.label;
    item.help = i.help;
    if (ScanMapItem(*archive, item))
      maps.push_back(item);

    names.insert(i.name);
  }

  std::string name;
  while (!maps.full() && !(name = archive->NextName()).empty()) {
    if (!StringEndsWith(name.c_str(), ".jp2"))
      continue;

    MapItem item("");

    auto dot = name.find('.');
    if (dot == name.npos || dot == 0 ||
        dot >= item.name.capacity())
      continue;

    item.name.SetASCII(std::string_view{name}.substr(0, dot));
    item.label = nullptr;
    item.help = nullptr;

    if (!names.insert(item.name.c_str()).second)
      continue;

    if (ScanMapItem(*archive, item))
      maps.push_back(item);
  }

  // TODO: scan the rest
} catch (...) {
  LogError(std::current_exception(), "No rasp data file");
}
