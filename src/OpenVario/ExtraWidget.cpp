// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#ifdef IS_OPENVARIO

#include "OpenVario/ExtraWidget.hpp"

#include "Dialogs/ProcessDialog.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Form/Form.hpp"
#include "Hardware/DisplayGlue.hpp"
#include "Hardware/RotateDisplay.hpp"
#include "Language/Language.hpp"
#include "Widget/RowFormWidget.hpp"

#include "UIGlobals.hpp"

#include "OpenVario/System/OpenVarioDevice.hpp"
#include "OpenVario/System/OpenVarioTools.hpp"

#ifndef OPENVARIO_BASEMENU
# include "UIActions.hpp"
# include "ui/window/ContainerWindow.hpp"
#endif

#include <tchar.h>
#include <string>

constexpr const TCHAR *opensoar = _T("OpenSoar");
constexpr const TCHAR *xcsoar = _T("XCSoar");
constexpr const TCHAR *main_app = opensoar;
constexpr const char *_main_app = "OpenSoar";  // only temporarily

class ExtraWidget final : public RowFormWidget {
public:
  ExtraWidget() noexcept : RowFormWidget(UIGlobals::GetDialogLook()) {}

private:
  /* virtual methods from class Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
};

void ExtraWidget::Prepare([[maybe_unused]] ContainerWindow &parent,
                        [[maybe_unused]] const PixelRect &rc) noexcept
{
  AddButton(_("Upgrade Firmware"), [this]() {
#ifdef OPENVARIO_BASEMENU
    exit(START_UPGRADE);
#else // OPENVARIO_BASEMENU
    ContainerWindow::SetExitValue(START_UPGRADE);
    UIActions::SignalShutdown(false);
    return mrOK;  //    START_UPGRADE;
#endif
  });

  AddButton(_("Calibrate Sensors"), CalibrateSensors);

  AddButton(_("Calibrate Touch"), [this]() {
// the programm exit in OpenSoar looks complete different fro OpenVarioBaseMenu
#ifdef OPENVARIO_BASEMENU
    exit(LAUNCH_TOUCH_CALIBRATE);
#else // OPENVARIO_BASEMENU
    ContainerWindow::SetExitValue(LAUNCH_TOUCH_CALIBRATE);
    UIActions::SignalShutdown(true);
    return mrOK;
#endif // OPENVARIO_BASEMENU
  });
}

bool
ShowExtraWidget(ContainerWindow &parent,
  const DialogLook &look) noexcept
{
  TWidgetDialog<ExtraWidget> sub_dialog(
      WidgetDialog::Full{}, (UI::SingleWindow &)parent, look,
      _("OpenVario File Transfers"));
  sub_dialog.SetWidget();
  sub_dialog.AddButton(_("Close"), mrOK);
  return sub_dialog.ShowModal();
}

std::unique_ptr<Widget>
CreateExtraWidget() noexcept {
  return std::make_unique<ExtraWidget>();
}
#endif
