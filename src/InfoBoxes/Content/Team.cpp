// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "InfoBoxes/Content/Team.hpp"
#include "InfoBoxes/Panel/Panel.hpp"
#include "InfoBoxes/Data.hpp"
#include "Interface.hpp"
#include "TeamActions.hpp"
#include "Dialogs/Traffic/TrafficDialogs.hpp"
#include "Widget/CallbackWidget.hpp"
#include "Language/Language.hpp"
#include "util/StringCompare.hxx"

#include <stdio.h>

static void
ShowTeamCodeDialog() noexcept
{
  dlgTeamCodeShowModal();
}

static std::unique_ptr<Widget>
LoadTeamCodeDialog([[maybe_unused]] unsigned id) noexcept
{
  return std::make_unique<CallbackWidget>(ShowTeamCodeDialog);
}

static constexpr InfoBoxPanel team_code_infobox_panels[] = {
  { N_("Team Code"), LoadTeamCodeDialog },
  { nullptr, nullptr }
};

const InfoBoxPanel *
InfoBoxContentTeamCode::GetDialogContent() noexcept
{
  return team_code_infobox_panels;
}

void
InfoBoxContentTeamCode::Update(InfoBoxData &data) noexcept
{
  const TeamCodeSettings &settings =
    CommonInterface::GetComputerSettings().team_code;
  const TeamInfo &teamcode_info = CommonInterface::Calculated();

  if (!settings.team_code_reference_waypoint) {
    data.SetInvalid();
    return;
  }

  // Set Value
  data.SetValue(CommonInterface::Calculated().own_teammate_code.GetCode());

  // Set Comment
  if (teamcode_info.flarm_teammate_code.IsDefined()) {
    data.SetComment(teamcode_info.flarm_teammate_code.GetCode());
    data.SetCommentColor(teamcode_info.flarm_teammate_code_current ? 2 : 1);
  } else if (settings.team_code.IsDefined()) {
    data.SetComment(settings.team_code.GetCode());
    data.SetCommentColor(0);
  }
  else
    data.SetCommentInvalid();
}

bool
InfoBoxContentTeamCode::HandleKey(const InfoBoxKeyCodes keycode) noexcept
{
  TeamCodeSettings &settings =
    CommonInterface::SetComputerSettings().team_code;
  const TrafficList &flarm = CommonInterface::Basic().flarm.traffic;
  const FlarmTraffic *traffic =
    settings.team_flarm_id.IsDefined()
    ? flarm.FindTraffic(settings.team_flarm_id)
    : NULL;

  if (keycode == ibkUp)
    traffic = (traffic == NULL ?
               flarm.FirstTraffic() : flarm.NextTraffic(traffic));
  else if (keycode == ibkDown)
    traffic = (traffic == NULL ?
               flarm.LastTraffic() : flarm.PreviousTraffic(traffic));
  else
    return false;

  if (traffic != NULL) {
    TeamActions::TrackFlarm(traffic->id, traffic->name);
  } else {
    // no flarm traffic to select!
    settings.team_flarm_id.Clear();
    settings.team_flarm_callsign.clear();
  }
  return true;
}

void
UpdateInfoBoxTeamBearing(InfoBoxData &data) noexcept
{
  const TeamCodeSettings &settings =
    CommonInterface::GetComputerSettings().team_code;
  const TrafficList &flarm = CommonInterface::Basic().flarm.traffic;
  const TeamInfo &teamcode_info = CommonInterface::Calculated();

  if (teamcode_info.teammate_available) {
    // Set Value
    data.SetValue(teamcode_info.teammate_vector.bearing);
  }
  else
    data.SetValueInvalid();

  // Set Comment
  if (!settings.team_flarm_id.IsDefined())
    data.SetCommentInvalid();
  else if (!settings.team_flarm_callsign.empty())
    data.SetComment(settings.team_flarm_callsign.c_str());
  else
    data.SetComment("???");

  if (flarm.FindTraffic(settings.team_flarm_id) != NULL)
    data.SetCommentColor(2);
  else
    data.SetCommentColor(1);
}

void
UpdateInfoBoxTeamBearingDiff(InfoBoxData &data) noexcept
{
  const TeamCodeSettings &settings =
    CommonInterface::GetComputerSettings().team_code;
  const NMEAInfo &basic = CommonInterface::Basic();
  const TrafficList &flarm = basic.flarm.traffic;
  const TeamInfo &teamcode_info = CommonInterface::Calculated();

  if (teamcode_info.teammate_available && basic.track_available) {
    // Set Value
    Angle Value = teamcode_info.teammate_vector.bearing - basic.track;
    data.SetValueFromBearingDifference(Value);
  } else
    data.SetValueInvalid();

  // Set Comment
  if (!settings.team_flarm_id.IsDefined())
    data.SetCommentInvalid();
  else if (!StringIsEmpty(settings.team_flarm_callsign))
    data.SetComment(settings.team_flarm_callsign);
  else
    data.SetComment("???");

  if (flarm.FindTraffic(settings.team_flarm_id) != NULL)
    data.SetCommentColor(2);
  else
    data.SetCommentColor(1);
}

void
UpdateInfoBoxTeamDistance(InfoBoxData &data) noexcept
{
  const TeamCodeSettings &settings =
    CommonInterface::GetComputerSettings().team_code;
  const TeamInfo &teamcode_info = CommonInterface::Calculated();

  // Set Value
  if (teamcode_info.teammate_available)
    data.SetValueFromDistance(teamcode_info.teammate_vector.distance);
  else
    data.SetValueInvalid();

  // Set Comment
  if (!settings.team_flarm_id.IsDefined())
    data.SetCommentInvalid();
  else if (!StringIsEmpty(settings.team_flarm_callsign))
    data.SetComment(settings.team_flarm_callsign);
  else
    data.SetComment("???");

  data.SetCommentColor(teamcode_info.flarm_teammate_code_current ? 2 : 1);
}
