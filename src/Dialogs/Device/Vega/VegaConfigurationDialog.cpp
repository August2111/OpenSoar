// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "VegaDialogs.hpp"
#include "Schemes.hpp"
#include "HardwareParameters.hpp"
#include "CalibrationParameters.hpp"
#include "AudioModeParameters.hpp"
#include "AudioDeadbandParameters.hpp"
#include "AudioParameters.hpp"
#include "LoggerParameters.hpp"
#include "MixerParameters.hpp"
#include "FlarmAlertParameters.hpp"
#include "FlarmIdentificationParameters.hpp"
#include "FlarmRepeatParameters.hpp"
#include "AlertParameters.hpp"
#include "LimitParameters.hpp"
#include "DisplayParameters.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Message.hpp"
#include "Widget/ArrowPagerWidget.hpp"
#include "Screen/Layout.hpp"
#include "Device/Driver/Vega/Internal.hpp"
#include "UIGlobals.hpp"
#include "Look/DialogLook.hpp"
#include "Operation/MessageOperationEnvironment.hpp"

static const char *const captions[] = {
  " 1 Hardware",
  " 2 Calibration",
  " 3 Audio Modes",
  " 4 Deadband",
  " 5 Tones: Cruise Faster",
  " 6 Tones: Cruise Slower",
  " 7 Tones: Cruise in Lift",
  " 8 Tones: Circling, climbing fast",
  " 9 Tones: Circling, climbing slow",
  "10 Tones: Circling, descending",
  "11 Vario flight logger",
  "12 Audio mixer",
  "13 FLARM Alerts",
  "14 FLARM Identification",
  "15 FLARM Repeats",
  "16 Alerts",
  "17 Airframe Limits",
  "18 Audio Schemes",
  "19 Display",
};

static const char *const audio_pages[] = {
  "CruiseFaster",
  "CruiseSlower",
  "CruiseLift",
  "CirclingClimbingHi",
  "CirclingClimbingLow",
  "CirclingDescending",
  NULL
};

static VegaDevice *device;
static bool changed, dirty;

class VegaConfigurationExtraButtons final
  : public NullWidget {
  struct Layout {
    PixelRect demo, save;

    Layout(const PixelRect &rc):demo(rc), save(rc) {
      const unsigned height = rc.GetHeight();
      const unsigned max_v_height = 2 * ::Layout::GetMaximumControlHeight();

      if (height >= max_v_height) {
        demo.top = rc.bottom - max_v_height;
        demo.bottom = save.top = unsigned(demo.top + rc.bottom) / 2;
      } else
        demo.right = save.left = unsigned(rc.left + rc.right) / 2;
    }
  };

  WidgetDialog &dialog;

  Button demo_button, save_button;

public:
  VegaConfigurationExtraButtons(WidgetDialog &_dialog)
    :dialog(_dialog) {}

protected:
  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent,
               const PixelRect &rc) noexcept override {
    Layout layout(rc);

    WindowStyle style;
    style.Hide();
    style.TabStop();

    const auto &button_look = dialog.GetLook().button;
    demo_button.Create(parent, button_look, _("Demo"),
                       layout.demo, style,
                       [this](){ OnDemo(); });
    save_button.Create(parent, button_look, _("Save"),
                       layout.save, style,
                       [this](){ OnSave(); });
  }

  void Show(const PixelRect &rc) noexcept override {
    Layout layout(rc);
    demo_button.MoveAndShow(layout.demo);
    save_button.MoveAndShow(layout.save);
  }

  void Hide() noexcept override {
    demo_button.FastHide();
    save_button.FastHide();
  }

  void Move(const PixelRect &rc) noexcept override {
    Layout layout(rc);
    demo_button.Move(layout.demo);
    save_button.Move(layout.save);
  }

private:
  void OnDemo();
  void OnSave();
};

static void
SetParametersScheme(PagerWidget &pager, int schemetype)
{
  if(ShowMessageBox(_("Set new audio scheme?  Old values will be lost."),
                 "Vega",
                 MB_YESNO | MB_ICONQUESTION) != IDYES)
    return;

  const VEGA_SCHEME &scheme = VegaSchemes[schemetype];

  pager.PrepareWidget(2);
  LoadAudioModeScheme((VegaParametersWidget &)pager.GetWidget(2), scheme);

  for (unsigned i = 0; audio_pages[i] != NULL; ++i) {
    pager.PrepareWidget(4 + i);
    ((VegaAudioParametersWidget &)pager.GetWidget(4 + i)).LoadScheme(scheme.audio[i]);
  }
}

static auto
MakeSetParametersScheme(PagerWidget &pager, int schemetype) noexcept
{
  return [&pager, schemetype](){ SetParametersScheme(pager, schemetype); };
}

static void
UpdateCaption(WndForm &form, unsigned page)
{
  form.SetCaption(captions[page]);
}

inline void
VegaConfigurationExtraButtons::OnSave()
{
  bool _changed = false;
  if (!dialog.GetWidget().Save(_changed))
    return;

  changed |= _changed;
  dirty |= changed;

  // make sure changes are sent to device
  MessageOperationEnvironment env;
  if (dirty) {
    try {
      device->SendSetting("StoreToEeprom", 2, env);
      dirty = false;
    } catch (OperationCancelled) {
    } catch (...) {
      env.SetError(std::current_exception());
    }
  }
}

inline void
VegaConfigurationExtraButtons::OnDemo()
{
  // retrieve changes from form
  if (!dialog.GetWidget().Save(changed))
    return;

  dlgVegaDemoShowModal();
}

class VegaSchemeButtonsPage : public RowFormWidget {
  PagerWidget &pager;

public:
  VegaSchemeButtonsPage(PagerWidget &_pager, const DialogLook &look)
    :RowFormWidget(look), pager(_pager) {}

  /* methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override {
    RowFormWidget::Prepare(parent, rc);

    AddButton("Vega", MakeSetParametersScheme(pager, 0));
    AddButton("Borgelt", MakeSetParametersScheme(pager, 1));
    AddButton("Cambridge", MakeSetParametersScheme(pager, 2));
    AddButton("Zander", MakeSetParametersScheme(pager, 3));
  }
};

static void
FillPager(PagerWidget &pager)
{
  const DialogLook &look = UIGlobals::GetDialogLook();

  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, hardware_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, calibration_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, audio_mode_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device,
                                                   audio_deadband_parameters));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device, "CruiseFaster"));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device, "CruiseSlower"));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device, "CruiseLift"));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device,
                                                        "CirclingClimbingHi"));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device,
                                                        "CirclingClimbingLow"));
  pager.Add(std::make_unique<VegaAudioParametersWidget>(look, *device,
                                                        "CirclingDescending"));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, logger_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, mixer_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, flarm_alert_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, flarm_id_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, flarm_repeat_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, alert_parameters));
  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, limit_parameters));

  pager.Add(std::make_unique<VegaSchemeButtonsPage>(pager, look));

  pager.Add(std::make_unique<VegaParametersWidget>(look, *device, display_parameters));
}

bool
dlgConfigurationVarioShowModal(Device &_device)
{
  device = (VegaDevice *)&_device;
  changed = dirty = false;

  const DialogLook &look = UIGlobals::GetDialogLook();

  TWidgetDialog<ArrowPagerWidget>
    dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
           look, _("Vario Configuration"));
  dialog.SetWidget(look.button,
                   dialog.MakeModalResultCallback(mrOK),
                   std::make_unique<VegaConfigurationExtraButtons>(dialog));
  FillPager(dialog.GetWidget());

  dialog.GetWidget().SetPageFlippedCallback([&dialog](){
    UpdateCaption(dialog, dialog.GetWidget().GetCurrentIndex());
  });
  UpdateCaption(dialog, dialog.GetWidget().GetCurrentIndex());

  dialog.ShowModal();

  return changed || dialog.GetChanged();
}
