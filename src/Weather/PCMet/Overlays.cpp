// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Overlays.hpp"
#include "Settings.hpp"
#include "ui/canvas/Bitmap.hpp"
#include "net/http/CoDownload.hpp"
#include "Job/Runner.hpp"
#include "co/Task.hxx"
#include "system/FileUtil.hpp"
#include "util/StaticString.hxx"
#include "util/Macros.hpp"
#include "LocalPath.hpp"

#include <stdexcept>

#include <string.h>
#include <stdio.h>

#define PCMET_FTP "ftp://ftp.pcmet.de"

static constexpr const char *type_names[] = {
  "nb_cosde_ome",
};

static constexpr const char *type_labels[] = {
  "Vertikal",
};

static_assert(ARRAY_SIZE(type_names) == unsigned(PCMet::OverlayInfo::Type::COUNT),
              "");

static_assert(ARRAY_SIZE(type_labels) == unsigned(PCMet::OverlayInfo::Type::COUNT),
              "");

static constexpr const char *area_names[] = {
  "nord",
  "sued",
};

static constexpr const char *area_labels[] = {
  "Nord",
  "Süd",
};

static_assert(ARRAY_SIZE(area_names) == unsigned(PCMet::OverlayInfo::Area::COUNT),
              "");

static_assert(ARRAY_SIZE(area_labels) == unsigned(PCMet::OverlayInfo::Area::COUNT),
              "");

static void
MakeOverlayLabel(PCMet::OverlayInfo &info)
{
  StaticString<64> label;
  label.Format("%s %s %um +%uh",
               type_labels[unsigned(info.type)],
               area_labels[unsigned(info.area)],
               info.level,
               info.step);
  info.label = label;
}

static void
FindLatestOverlay(PCMet::OverlayInfo &info)
{
  struct Visitor : public File::Visitor {
    PCMet::OverlayInfo &info;
    std::chrono::system_clock::time_point latest_modification = std::chrono::system_clock::time_point::min();
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    explicit Visitor(PCMet::OverlayInfo &_info)
      :info(_info) {}

    void Visit(Path path, Path) override {
      const auto last_modification = File::GetLastModification(path);
      if (last_modification > latest_modification &&
          last_modification <= now) {
        latest_modification = last_modification;
        info.path = path;
      }
    }
  } visitor(info);

  const auto cache_path = MakeCacheDirectory("pc_met");
  StaticString<256> pattern;
  pattern.Format("%s_%s_lv_%06u_p_%03u_*.tiff",
                 type_names[unsigned(info.type)],
                 area_names[unsigned(info.area)],
                 info.level, info.step);
  Directory::VisitSpecificFiles(cache_path, pattern, visitor);
}

std::list<PCMet::OverlayInfo>
PCMet::CollectOverlays()
{
  std::list<OverlayInfo> list;

  for (unsigned area = 0; area < ARRAY_SIZE(area_names); ++area) {
    for (unsigned step = 2; step <= 6; ++step) {
      OverlayInfo info;
      info.type = OverlayInfo::Type::VERTICAL;
      info.area = OverlayInfo::Area(area);
      info.level = 3000;
      info.step = step;
      MakeOverlayLabel(info);
      FindLatestOverlay(info);
      list.emplace_back(std::move(info));
    }
  }

  return list;
}

Co::Task<PCMet::Overlay>
PCMet::DownloadOverlay(const OverlayInfo &info, BrokenDateTime now_utc,
                       const PCMetSettings &settings,
                       CurlGlobal &curl, ProgressListener &progress)
{
  const unsigned run_hour = (now_utc.hour / 3) * 3;
  unsigned run = (now_utc.hour / 3) * 300;

  StaticString<256> url;
  url.Format(PCMET_FTP "/%s_%s_lv_%06u_p_%03u_%04u.tiff",
             type_names[unsigned(info.type)],
             area_names[unsigned(info.area)],
             info.level, info.step, run);

  const auto cache_path = MakeCacheDirectory("pc_met");
  auto path = AllocatedPath::Build(cache_path,
                                   url.c_str() + sizeof(PCMET_FTP));

  {
    auto data = new Net::CurlData;
    data->username = settings.ftp_credentials.username;
    data->password = settings.ftp_credentials.password;
    data->type = Net::FILE;

    const auto ignored_response = co_await Net::CoDownloadToFile(
      curl, url, path, data, progress);

//    const auto ignored_response = co_await Net::CoDownloadToFile(
//        curl, url, settings.ftp_credentials.username,
//        settings.ftp_credentials.password, path, nullptr, progress);
  }

  BrokenDateTime run_time(now_utc.GetDate(), BrokenTime(run_hour, 0));
  if (run_hour < now_utc.hour)
    run_time.DecrementDay();

  BrokenDateTime valid_time = run_time;
  valid_time.hour += info.step;
  if (valid_time.hour >= 24) {
    valid_time.hour -= 24;
    valid_time.IncrementDay();
  }

  co_return Overlay{run_time, valid_time, std::move(path)};
}
