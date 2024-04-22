// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Profile/Keys.hpp"
#include "Profile/Profile.hpp"
#include "Form/DataField/Enum.hpp"
#include "Interface.hpp"
#include "GlideComputerConfigPanel.hpp"
#include "Language/Language.hpp"
#include "Widget/RowFormWidget.hpp"
#include "UIGlobals.hpp"
#include "UtilsSettings.hpp"

using namespace std::chrono;

enum ControlIndex {
  AutoMcMode,
  BlockSTF,
  EnableNavBaroAltitude,
  EnableExternalTriggerCruise,
  AverEffTime,
  PredictWindDrift,
  WaveAssistant,
  CruiseToCirclingModeSwitchThreshold,
  CirclingToCruiseModeSwitchThreshold,
};

class GlideComputerConfigPanel final : public RowFormWidget {
public:
  GlideComputerConfigPanel()
    :RowFormWidget(UIGlobals::GetDialogLook()) {}

  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  bool Save(bool &changed) noexcept override;
};

void
GlideComputerConfigPanel::Prepare(ContainerWindow &parent,
                                  const PixelRect &rc) noexcept
{
  const ComputerSettings &settings_computer = CommonInterface::GetComputerSettings();
  const TaskBehaviour &task_behaviour = settings_computer.task;

  RowFormWidget::Prepare(parent, rc);

  static constexpr StaticEnumChoice auto_mc_list[] = {
    { TaskBehaviour::AutoMCMode::FINALGLIDE, N_("Final glide"),
      N_("Adjusts MC for fastest arrival.  For contest sprint tasks, the MacCready is adjusted in "
          "order to cover the greatest distance in the remaining time and reach the finish height.") },
    { TaskBehaviour::AutoMCMode::CLIMBAVERAGE, N_("Trending average climb"),
      N_("Sets MC to the trending average climb rate based on all climbs.") },
    { TaskBehaviour::AutoMCMode::BOTH, N_("Both"),
      N_("Uses trending average during task, then fastest arrival when in final glide mode.") },
    nullptr
  };

  AddEnum(_("Auto MC mode"),
          _("This option defines which auto MacCready algorithm is used."),
          auto_mc_list, (unsigned)settings_computer.task.auto_mc_mode);

  AddBoolean(_("Block speed to fly"),
             _("If enabled, the command speed in cruise is set to the MacCready speed to fly in "
                 "no vertical air-mass movement. If disabled, the command speed in cruise is set "
                 "to the dolphin speed to fly, equivalent to the MacCready speed with vertical "
                 "air-mass movement."),
             settings_computer.features.block_stf_enabled);
  SetExpertRow(BlockSTF);

  AddBoolean(_("Nav. by baro altitude"),
             _("When enabled and if connected to a barometric altimeter, barometric altitude is "
                 "used for all navigation functions. Otherwise GPS altitude is used."),
             settings_computer.features.nav_baro_altitude_enabled);
  SetExpertRow(EnableNavBaroAltitude);

  AddBoolean(_("Flap forces cruise"),
             _("When Vega variometer is connected and this option is true, the positive flap "
                 "setting switches the flight mode between circling and cruise."),
             settings_computer.circling.external_trigger_cruise_enabled);
  SetExpertRow(EnableExternalTriggerCruise);

  static constexpr StaticEnumChoice aver_eff_list[] = {
    { AverageEffTime::ae15seconds, "15 s", N_("Preferred period for paragliders.") },
    { AverageEffTime::ae30seconds, "30 s" },
    { AverageEffTime::ae60seconds, "60 s" },
    { AverageEffTime::ae90seconds, "90 s", N_("Preferred period for gliders.") },
    { AverageEffTime::ae2minutes, "2 min" },
    { AverageEffTime::ae3minutes, "3 min" },
    nullptr
  };

  AddEnum(_("GR average period"),
          _("Here you can decide on how many seconds of flight this calculation must be done. "
              "Normally for gliders a good value is 90-120 seconds, and for paragliders 15 seconds."),
          aver_eff_list, (unsigned)settings_computer.average_eff_time);
  SetExpertRow(AverEffTime);

  AddBoolean(_("Predict wind drift"),
             _("Account for wind drift for the predicted circling duration. This reduces the arrival height for legs with head wind."),
             task_behaviour.glide.predict_wind_drift);
  SetExpertRow(PredictWindDrift);

  AddBoolean(_("Wave assistant"), nullptr,
             settings_computer.wave.enabled);

  AddDuration(_("Cruise/Circling period"),
              _("How many seconds of turning before changing from cruise to circling mode."),
              seconds{2}, seconds{30}, seconds{1},
              settings_computer.circling.cruise_to_circling_mode_switch_threshold);
  SetExpertRow(CruiseToCirclingModeSwitchThreshold);

  AddDuration(_("Circling/Cruise period"),
              _("How many seconds of flying straight before changing from circling to cruise mode."),
              seconds{2}, seconds{30}, seconds{1},
              settings_computer.circling.circling_to_cruise_mode_switch_threshold);
  SetExpertRow(CirclingToCruiseModeSwitchThreshold);
}

bool
GlideComputerConfigPanel::Save(bool &_changed) noexcept
{
  bool changed = false;

  ComputerSettings &settings_computer = CommonInterface::SetComputerSettings();
  TaskBehaviour &task_behaviour = settings_computer.task;

  changed |= SaveValueEnum(AutoMcMode, ProfileKeys::AutoMcMode, settings_computer.task.auto_mc_mode);

  changed |= SaveValue(BlockSTF, ProfileKeys::BlockSTF,
                       settings_computer.features.block_stf_enabled);

  changed |= SaveValue(EnableNavBaroAltitude, ProfileKeys::EnableNavBaroAltitude,
                       settings_computer.features.nav_baro_altitude_enabled);

  changed |= SaveValue(EnableExternalTriggerCruise, ProfileKeys::EnableExternalTriggerCruise,
                       settings_computer.circling.external_trigger_cruise_enabled);

  if (SaveValueEnum(AverEffTime, ProfileKeys::AverEffTime,
                    settings_computer.average_eff_time))
    require_restart = changed = true;

  changed |= SaveValue(PredictWindDrift, ProfileKeys::PredictWindDrift,
                       task_behaviour.glide.predict_wind_drift);

  changed |= SaveValue(WaveAssistant, ProfileKeys::WaveAssistant,
                       settings_computer.wave.enabled);

  changed |= SaveValue(CruiseToCirclingModeSwitchThreshold, ProfileKeys::CruiseToCirclingModeSwitchThreshold,
                       settings_computer.circling.cruise_to_circling_mode_switch_threshold);

  changed |= SaveValue(CirclingToCruiseModeSwitchThreshold, ProfileKeys::CirclingToCruiseModeSwitchThreshold,
                       settings_computer.circling.circling_to_cruise_mode_switch_threshold);

  _changed |= changed;

  return true;
}

std::unique_ptr<Widget>
CreateGlideComputerConfigPanel()
{
  return std::make_unique<GlideComputerConfigPanel>();
}
