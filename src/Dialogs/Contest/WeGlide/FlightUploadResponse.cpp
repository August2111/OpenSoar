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

#include "FlightUploadResponse.hpp"
#include "LogFile.hpp"
#include "UIGlobals.hpp"
#include "net/client/WeGlide/PatchIGCFile.hpp"
#include "net/client/WeGlide/DeleteIGCFile.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "Widget/RowFormWidget.hpp"

#include "json/ParserOutputStream.hxx"

#include <sstream>

class UploadResponseDialog final : public WidgetDialog {

public:
  UploadResponseDialog(Auto style, UI::SingleWindow &parent,
                       const DialogLook &look, const TCHAR *caption,
                       Widget *widget) noexcept
      : WidgetDialog(style, parent, look, caption, widget) {}

private:
  bool OnAnyKeyDown(unsigned key_code) noexcept override;
};


class UploadResponseWidget final : public RowFormWidget {

public:
  UploadResponseWidget(const DialogLook &look,
                       const WeGlide::Flight &_flightdata, const TCHAR *msg)
      : RowFormWidget(look), flightdata(_flightdata), update_message(msg) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent,
               const PixelRect &rc) noexcept override;
  int ShowSuccessDialog();
  
private:
  WeGlide::Flight flightdata;
  const TCHAR *update_message;
};


void
UploadResponseWidget::Prepare([[maybe_unused]] ContainerWindow &parent,  // TODO(August2111): parent is unused?
                              [[maybe_unused]] const PixelRect &rc) noexcept  // TODO(August2111): rc is unused?
{
  TCHAR buffer[0x100];
  AddSpacer();
  AddReadOnly(_("IGC File"), NULL, flightdata.igc_name.c_str());
  AddReadOnly(_("Flight ID"), NULL, _T("%.0f"), flightdata.flight_id);
  AddSpacer();
  AddReadOnly(_("Date"), NULL, flightdata.scoring_date.c_str());
  AddReadOnly(_("Pilot"), NULL, flightdata.user.name.c_str());
  AddReadOnly(_("Aircraft"), NULL, flightdata.aircraft.name.c_str());
  _stprintf(buffer, _T("%s, %s =  %s"), flightdata.registration.c_str(),
      _("cid"), flightdata.competition_id.c_str());
  AddReadOnly(_("Glider"), NULL, buffer);
  // TODO(August2111): Editfield for personale messages:
  // AddMultiLine(_T("Das ist ein Test"));
  AddText(_T("Comment"), nullptr, _T("Direct Upload via XCSoar!"));
  AddSpacer();
  AddLabel(update_message);
}

int
UploadResponseWidget::ShowSuccessDialog()
{
  UploadResponseDialog dialog(WidgetDialog::Auto{},
                              UIGlobals::GetMainWindow(),
                              UIGlobals::GetDialogLook(),
                              _("Upload Flight"), this);
  // only one "Close" button
  dialog.AddButton(_("Close"), mrOK);
  dialog.AddButton(_("Patch"), 98);
  dialog.AddButton(_("Delete"), 99);
  auto result = dialog.ShowModal();

  /* the caller manages the Widget */
  dialog.StealWidget();

  return result;
}

bool
UploadResponseDialog::OnAnyKeyDown([[maybe_unused]] unsigned key_code) noexcept  // TODO(August2111): key_code is unused?
{
  // any key is closing the dialog! 
  WndForm::SetModalResult(mrOK);
  return true;
}

namespace WeGlide {

int
FlightUploadResponse(const WeGlide::Flight &flightdata, const TCHAR *msg)
{
  LogFormat(_T("%s: %s"), _("WeGlide Upload"), msg);
  UploadResponseWidget widget(UIGlobals::GetDialogLook(), flightdata, msg);

  auto button = widget.ShowSuccessDialog();
  switch (button) {
  case 98: try {
    DeleteIGCFile(flightdata.user, flightdata.flight_id-1);
    Json::ParserOutputStream parser;
    std::stringstream ss;
    ss << "{";
    ss << "\"comment\": \"Uploaded via XCSoar\", ";
    ss << "\"competition_id\": \"T81\", ";
//    ss << "\"co_user_name\": \"Lou Reed\", ";
    ss << "\"rescore\": false ";
    ss << "}";
    parser.Write(ss.str().c_str(), ss.str().length());
    boost::json::value json = parser.Finish();
    PatchIGCFile(flightdata.user, flightdata.flight_id, json);
    } catch (...) {
      DeleteIGCFile(flightdata.user, flightdata.flight_id);
    }
    break;
  case 99:
    DeleteIGCFile(flightdata.user, flightdata.flight_id);
#ifdef _DEBUG
# ifdef TEST_DOUBLE_DELETE
    // To test a non valid flight no (flight is removed!)
    DeleteIGCFile(flightdata.user, flightdata.flight_id - 1);
# endif
#endif
    break;
  default:
    break;
  }
  return mrOK;
}

} // namespace WeGlide
