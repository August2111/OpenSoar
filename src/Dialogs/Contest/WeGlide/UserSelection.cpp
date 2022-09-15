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

#include "UserSelection.hpp"
#include "LogFile.hpp"
#include "UIGlobals.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "Widget/RowFormWidget.hpp"

class SelectionResponseDialog final : public WidgetDialog {

public:
  SelectionResponseDialog(Auto style, UI::SingleWindow &parent,
                       const DialogLook &look, const TCHAR *caption,
                       Widget *widget) noexcept
      : WidgetDialog(style, parent, look, caption, widget) {}

private:
  bool OnAnyKeyDown(unsigned key_code) noexcept override;
};


class UserSelectionWidget final : public RowFormWidget {

public:
  UserSelectionWidget(const DialogLook &look, const WeGlide::User &_user,
                      const TCHAR *msg)
      : RowFormWidget(look), user(_user), update_message(msg) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent,
               const PixelRect &rc) noexcept override;
  bool ShowSuccessDialog();
  
private:
  WeGlide::User user;
  const TCHAR *update_message;
};


void
UserSelectionWidget::Prepare([[maybe_unused]] ContainerWindow &parent,  // TODO(August2111): parent is unused?
                              [[maybe_unused]] const PixelRect &rc) noexcept  // TODO(August2111): rc is unused?
{
  TCHAR buffer[0x100];
  AddSpacer();
  _stprintf(buffer, _T("%s (%u)"), user.name.c_str(), user.id);
  AddReadOnly(_("User"), NULL, buffer);
  AddSpacer();
  AddLabel(update_message);
}

bool
UserSelectionWidget::ShowSuccessDialog()
{
  SelectionResponseDialog dialog(WidgetDialog::Auto{},
                        UIGlobals::GetMainWindow(),
                        UIGlobals::GetDialogLook(),
                        _("Get User Info"), this);
  // only one 'Close' button
  dialog.AddButton(_("Close"), mrOK);
  dialog.ShowModal();

  /* the caller manages the Widget */
  dialog.StealWidget();

  return true;
}

bool
SelectionResponseDialog::OnAnyKeyDown([[maybe_unused]] unsigned key_code) noexcept  // TODO(August2111): key_code is unused?
{
  // any key is closing the dialog! 
  WndForm::SetModalResult(mrOK);
  return true;
}

namespace WeGlide {

int 
UserSelection(User &user, const TCHAR *msg)
{
  LogFormat(_T("%s: %s"), _("WeGlide Upload"), msg);
  UserSelectionWidget widget(UIGlobals::GetDialogLook(), user, msg);

  return widget.ShowSuccessDialog();
}

} // namespace WeGlide
