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

#include "TaskDownloadDialog.hpp"

#include "LogFile.hpp"
#include "UIGlobals.hpp"
#include "net/client/WeGlide/DownloadTask.hpp"
#include "net/client/WeGlide/GetObjectInfo.hpp"
#include "net/client/WeGlide/GetJsonString.hpp"
#include "net/client/WeGlide/Settings.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Dialogs/Message.hpp"
#include "Form/DataField/Listener.hpp"
#include "Form/DataField/Integer.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "Widget/RowFormWidget.hpp"
#include "system/FileUtil.hpp"
#include "system/Path.hpp"

#include "Task/Ordered/OrderedTask.hpp"

class TaskResponseDialog final : public WidgetDialog {

public:
  TaskResponseDialog(Auto style, UI::SingleWindow &parent,
                  const DialogLook &look, const TCHAR *caption,
                  Widget *widget) noexcept
      : WidgetDialog(style, parent, look, caption, widget) {}

private:
  bool OnAnyKeyDown(unsigned key_code) noexcept override;
};


class TaskDownloadWidget final : public RowFormWidget, DataFieldListener {

public:
  TaskDownloadWidget(const DialogLook &look, const WeGlide::User &_user,
     const TCHAR *msg)
      : RowFormWidget(look), user(_user), update_message(msg) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent,
               const PixelRect &rc) noexcept override;
  int ShowDialog();

  /* methods from DataFieldListener */
  void OnModified(DataField &df) noexcept override;

private:
  WeGlide::User user;
  const TCHAR *update_message;
  boost::json::object json_task;
};

/*/
boost::json::value parse_file(Path file) {
  NarrowString<0x100> filename;
  filename.SetASCII(file.c_str());
  size_t size = File::GetSize(file);
  char *file_buffer = new char[size + 1];
  File::ReadString(file, file_buffer, size + 1);
  file_buffer[size] = 0;
  boost::json::stream_parser parser;
  boost::json::error_code ec;
  parser.write(file_buffer, size, ec);
  if (ec)
    return nullptr;
  parser.finish(ec);
  if (ec)
    return nullptr;
  return parser.release();
}
*/

  /* methods from DataFieldListener */
void 
TaskDownloadWidget::OnModified(DataField &df) noexcept
{
  TCHAR buffer[0x100];
  user = WeGlide::GetUserInfo(static_cast<DataFieldInteger &>(df).GetValue());

  _stprintf(buffer, _T("%s (id: %u, club: %s)"), user.name.c_str(), user.id,
            user.club.c_str());
  SetText(1, buffer);
  SetText(4, user.club);
  SetText(5, _T("Label!!!"));
}


void
TaskDownloadWidget::Prepare(ContainerWindow &parent,
                            const PixelRect &rc) noexcept
try {
  TCHAR buffer[0x100];
  AddSpacer();  // Field 0
  AddReadOnly(_("User Name"), nullptr, user.name.c_str()); // Field 1
  AddInteger(_("User ID"), nullptr, _T("User: %u"), _T("%u"), 1, 10000, 1,
             user.id, this); // Field 2
  AddSpacer();               // Field 3
#if 1
  // TODO(August2111): Die Nummer ('62') muss aus den Settings genommen werden!?!
  WeGlide::Aircraft aircraft = WeGlide::GetAircraftInfo(62);
  AddReadOnly(_("Aircraft"), nullptr, aircraft.name);        // Field 4
#endif
  _stprintf(buffer, _T("%s (id = %u)\n%s (%6.2f km)"),
            update_message ? update_message : _T(""),
            json_task.at("id").to_number<uint32_t>(),
            GetJsonString(json_task, "name").c_str(),
            json_task.contains("distance")
              ? json_task.at("distance").to_number<double>()
                : 0);
#ifdef JSON_TEST
    auto idx =
        json_task.contains("idx") ? json.at("idx").to_number<uint32_t>() : 0;
    if (idx == 0)
      _stprintf(buffer, _T("%s\nerror: 'idx' n.a.!"), update_message);
#endif
    AddLabel(buffer); // Field 5
} catch (std::exception & /* e */) {
  ShowError(std::current_exception(), _T("WeGlide Load Task"));
}

int
TaskDownloadWidget::ShowDialog()
{
  int result = 0;
  if (user.id > 0)
    user = WeGlide::GetUserInfo(user.id);

  json_task = WeGlide::GetJsonObject("task/declaration", user.id);

  if (json_task.empty()) {
    // without Internet connection no access to dialog!
    ShowMessageBox(_("Internet not available"), _("Load WeGlide Task"), MB_OK);
  } else {
    TaskResponseDialog dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(),
                           UIGlobals::GetDialogLook(), _("Load WeGlide Task"),
                           this);
    // only one 'Close' button
    dialog.AddButton(_("Save"), mrOK);
    dialog.AddButton(_("Activate"), 15);
    dialog.AddButton(_("Close"), mrCancel);
    result = dialog.ShowModal();

    /* the caller manages the Widget */
    dialog.StealWidget();
  }
  return result;
}

bool
TaskResponseDialog::OnAnyKeyDown(unsigned key_code) noexcept
{
  switch (tolower(key_code)) {
  case 'j':
  case 'y':
  case 13:
    WndForm::SetModalResult(mrOK);
    return true;

  case 'n':
  case 27:
    WndForm::SetModalResult(mrCancel);
    return true;

  default:
    return false;
  }
}

namespace WeGlide {

int 
// std::unique_ptr<OrderedTask>
TaskDownloadDialog(const User &user, const TCHAR *msg) {
  // std::unique_ptr<OrderedTask> task;
  int task = 0;
  LogFormat(_T("%s: %s"), _("WeGlide Upload"),
                                         msg);
#if 1
  TaskDownloadWidget widget(UIGlobals::GetDialogLook(), user, msg);

  int result = widget.ShowDialog();
  switch (result) {
    case mrOK:
      // Save
      ShowMessageBox(_("Save Task!"), _T("Load Task"),
                     MB_OK | MB_ICONINFORMATION);
      break;

    case mrCancel:
      // do nothing! ShowMessageBox(_("Cancel Task!"), _T("Load Task"), MB_ICONWARNING);
    break;

    case 15:
      // Save & Activate
      ShowMessageBox(_("Save and Activate Task!"), _T("Load Task"),
                     MB_OK | MB_ICONINFORMATION);
    break;

    default:
      break;
  }
    return task;
     // result;
#else
  const auto task_file // = DownloadTaskFile(user);
   = TaskToFile(user);

  return 0;
#endif

}

} // namespace WeGlide
