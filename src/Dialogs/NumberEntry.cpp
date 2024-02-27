// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/NumberEntry.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Widget/FixedWindowWidget.hpp"
#include "Form/DigitEntry.hpp"
#include "Language/Language.hpp"
#include "Math/Angle.hpp"
#include "UIGlobals.hpp"

// bool LeftOverflowCallback() noexcept {
//   last_button->SetFocus();
//   return true;
// }
// 
// bool RightOverflowCallback() noexcept {
//   first_button->SetFocus();
//   return true;
// }

// ----------------------------------------------------------------------------
/** NumberEntryDialog for big signed numbers  -> SIGNED with +/-! */
bool
NumberEntryDialog(const TCHAR *caption,
                  int &value, unsigned length)
{
  /* create the dialog */

  const DialogLook &look = UIGlobals::GetDialogLook();

  TWidgetDialog<FixedWindowWidget>
    dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(), look, caption);

  ContainerWindow &client_area = dialog.GetClientAreaWindow();

  /* create the input control */

  WindowStyle control_style;
  control_style.Hide();
  control_style.TabStop();

  auto entry = std::make_unique<DigitEntry>(look);
  entry->CreateSigned(client_area, client_area.GetClientRect(), control_style,
                      length, 0);
  entry->Resize(entry->GetRecommendedSize());
  entry->SetValue(value);
  entry->SetCallback(dialog.MakeModalResultCallback(mrOK));
  entry->SetLeftOverflow(dialog.SetFocusButtonCallback(dialog.last_button));
  entry->SetRightOverflow(dialog.SetFocusButtonCallback(dialog.first_button));

  /* create buttons */

  dialog.first_button = dialog.AddButton(_("OK"), mrOK);
  dialog.last_button = dialog.AddButton(_("Cancel"), mrCancel);

  /* run it */

  dialog.SetWidget(std::move(entry));

  bool result = dialog.ShowModal() == mrOK;
  if (!result)
    return false;

  value = ((DigitEntry &)dialog.GetWidget().GetWindow()).GetIntegerValue();
  return true;
}

// ----------------------------------------------------------------------------
/** NumberEntryDialog for big unsigned numbers -> UNSIGNED! */
bool
NumberEntryDialog(const TCHAR *caption,
                  unsigned &value, unsigned length)
{
  /* create the dialog */

  const DialogLook &look = UIGlobals::GetDialogLook();

  TWidgetDialog<FixedWindowWidget>
    dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(), look, caption);

  ContainerWindow &client_area = dialog.GetClientAreaWindow();

  /* create the input control */

  WindowStyle control_style;
  control_style.Hide();
  control_style.TabStop();

  auto entry = std::make_unique<DigitEntry>(look);
  entry->CreateUnsigned(client_area, client_area.GetClientRect(), control_style,
                        length, 0);
  entry->Resize(entry->GetRecommendedSize());
  entry->SetValue(value);
  entry->SetCallback(dialog.MakeModalResultCallback(mrOK));
  entry->SetLeftOverflow(dialog.SetFocusButtonCallback(dialog.last_button));
  entry->SetRightOverflow(dialog.SetFocusButtonCallback(dialog.first_button));

  /* create buttons */

  dialog.first_button = dialog.AddButton(_("OK"), mrOK);
  dialog.last_button = dialog.AddButton(_("Cancel"), mrCancel);

  /* run it */

  dialog.SetWidget(std::move(entry));

  bool result = dialog.ShowModal() == mrOK;
  if (!result)
    return false;

  value = ((DigitEntry &)dialog.GetWidget().GetWindow()).GetUnsignedValue();
  return true;
}

// ----------------------------------------------------------------------------
/** NumberEntryDialog for big angle values */
bool
AngleEntryDialog(const TCHAR *caption, Angle &value)
{
  /* create the dialog */

  const DialogLook &look = UIGlobals::GetDialogLook();

  TWidgetDialog<FixedWindowWidget>
    dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(), look, caption);

  ContainerWindow &client_area = dialog.GetClientAreaWindow();

  /* create the input control */

  WindowStyle control_style;
  control_style.Hide();
  control_style.TabStop();

  auto entry = std::make_unique<DigitEntry>(look);
  entry->CreateAngle(client_area, client_area.GetClientRect(), control_style);
  entry->Resize(entry->GetRecommendedSize());
  entry->SetValue(value);
  entry->SetCallback(dialog.MakeModalResultCallback(mrOK));
  entry->SetLeftOverflow(dialog.SetFocusButtonCallback(dialog.last_button));
  entry->SetRightOverflow(dialog.SetFocusButtonCallback(dialog.first_button));

  /* create buttons */

  dialog.first_button = dialog.AddButton(_("OK"), mrOK);
  dialog.last_button = dialog.AddButton(_("Cancel"), mrCancel);

  /* run it */

  dialog.SetWidget(std::move(entry));

  bool result = dialog.ShowModal() == mrOK;
  if (!result)
    return false;

  value = ((DigitEntry &)dialog.GetWidget().GetWindow()).GetAngleValue();
  return true;
}
