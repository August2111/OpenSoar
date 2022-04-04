/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "FlightPatchDialog.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"
#include "net/client/WeGlide/PatchIGCFile.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Form/DataField/Listener.hpp"
#include "Form/DataField/File.hpp"
#include "Form/DataField/Date.hpp"
#include "Form/DataField/Integer.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "system/FileUtil.hpp"
#include "ui/event/KeyCode.hpp"
#include "Widget/RowFormWidget.hpp"

class PatchDialog final : public WidgetDialog {

public:
  PatchDialog(Auto style, UI::SingleWindow &parent, const DialogLook &look,
               const TCHAR *caption, Widget *widget) noexcept
      : WidgetDialog(style, parent, look, caption, widget) {}

  // take focus setting from here, not the default!
  void SetDefaultFocus() noexcept override {}

private:
  bool OnAnyKeyDown(unsigned key_code) noexcept override;
};


class PatchWidget final : public RowFormWidget, DataFieldListener {

public:
  PatchWidget(const DialogLook &look, const WeGlide::User &user_,
              const uint_least32_t flight_id_)
      : RowFormWidget(look), user(user_),
        flight_id(flight_id_) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  bool SetFocus() noexcept override;

  /* Getter for Widget data */
  auto GetUser() { return user; }
  auto GetFlight() { return flight_id; }

  bool ShowPatchDialog();
private:
  // DataFieldIndex is the order of AddField in Prepare()!
  enum DataFieldIndex {
    SPACER1,
    USER_ID,
    USER_TOKEN,
    FLIGHT_ID,
    SPACER3
  };

  WeGlide::User user;
  uint_least32_t flight_id;

  Button *ok_button;
  Button *cancel_button;
  WndProperty *data_fields[5];

  /* virtual methods from DataFieldListener */
  void OnModified(DataField &df) noexcept override;
};

bool 
PatchWidget::SetFocus() noexcept
{
  bool data_ok;
  RowFormWidget::SetFocus();

  // Parse data and find 1st failure:
  if (user.id == 0) {
    data_ok = false;
    data_fields[1]->SetFocus();
  } else if (!user.birthdate.IsPlausible()) {
    data_ok = false;
    data_fields[2]->SetFocus();
  } else if (flight_id == 0) {
    data_ok = false;
    data_fields[3]->SetFocus();
  } else {
    data_ok = true;
    ok_button->SetFocus();  // darkblue
  }

  ok_button->SetEnabled(data_ok);  // normal or gray
  ok_button->SetSelected(data_ok);   // lightblue
  cancel_button->SetSelected(!data_ok);

  return data_ok;
}

void 
PatchWidget::OnModified(DataField &df) noexcept
{
  if (IsDataField(USER_ID, df)) {
    user.id = static_cast<DataFieldInteger &>(df).GetValue();
  } else if (IsDataField(USER_TOKEN, df)) {
    user.token = df.GetAsString();
  } else if (IsDataField(FLIGHT_ID, df)) {
    flight_id = static_cast<DataFieldInteger &>(df).GetValue();
  }
  SetFocus(); // with check state of fields!
}

void
PatchWidget::Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept
{
  RowFormWidget::Prepare(parent, rc);

//  const auto settings = CommonInterface::GetComputerSettings(); 
//  if (user.id == 0 || user.id = settings.weglide.pilot.id)
//    user = settings.weglide.pilot;

  // if (flight_id == 0)
  //   aircraft_id = settings.plane.weglide_glider_type;
  
  AddSpacer();
  data_fields[0] =AddInteger(_("Pilot"),
             _("Take this from your WeGlide Profile. Or set to 0 if not used."),
             _T("%u"), _T("%u"), 0, 99999, 1, user.id, this);

  data_fields[1] = AddText(_("User token"), nullptr, user.token, this);

  data_fields[2] =  AddInteger(_("Flight"),
      _("Take this from your WeGlide Profile. Or set to 0 if not used."),
      _T("%u"), _T("%u"), 1, 9999, 1, flight_id, this);

  AddSpacer();

  AddLabel(_("Do you want to upload this flight to WeGlide?"));

  SetFocus();
}

bool
PatchWidget::ShowPatchDialog()
{
  PatchDialog dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(),
                      UIGlobals::GetDialogLook(), _("Patch Flight"), this);
  ok_button = dialog.AddButton(_("OK"), mrOK);
  cancel_button = dialog.AddButton(_("Cancel"), mrCancel);
 
  bool do_upload = dialog.ShowModal() == mrOK;

  /* the caller manages the Widget */
  dialog.StealWidget();

  if (do_upload) {
    dialog.Hide();  // dialog is objectionable in the Background
    boost::json::value patch;
    return PatchIGCFile(GetUser(), GetFlight(), patch);
  } else {
    return false;
  }
}

bool
PatchDialog::OnAnyKeyDown(unsigned key_code) noexcept
{
  switch (toupper(key_code)) {
  case 'Q':  // 'Quit
  case 'X':  // 'eXit
    WndForm::SetModalResult(mrCancel);
    return true;

  default:
    return WidgetDialog::OnAnyKeyDown(key_code);
  }
}


namespace WeGlide {

int 
FlightPatchDialog(const WeGlide::User &user,
                  const uint_least32_t flight_id) noexcept
{
  PatchWidget widget(UIGlobals::GetDialogLook(), user, flight_id);

  return widget.ShowPatchDialog();
}

} // namespace WeGlide
