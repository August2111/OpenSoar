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

#include "PrepareFlightUploadDialog.hpp"
#include "LogFile.hpp"
#include "UIGlobals.hpp"
// #include "Dialogs/HelpDialog.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Language/Language.hpp"
#include "Operation/Cancelled.hpp"
#include "Operation/PopupOperationEnvironment.hpp"
#include "system/Path.hpp"
// #include "system/Sleep.h"
#include "Widget/RowFormWidget.hpp"

class UploadPrepareWidget final : public RowFormWidget {

public:
  UploadPrepareWidget(const DialogLook &look, const Path &igc_path, const WeGlide::User &user_,
                      const uint_least32_t glider_id)
      : RowFormWidget(look), igcpath(igc_path), user(user),
        aircraft_id(glider_id) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
  bool Save(bool &changed) noexcept override;

private:
  Path igcpath;
  const WeGlide::User user;
  uint_least32_t aircraft_id;
};


void UploadPrepareWidget::Prepare(ContainerWindow &parent,
                                const PixelRect &rc) noexcept {

  TCHAR buffer[0x100];
  AddSpacer();
  AddReadOnly(_("IGC File"), NULL, igcpath.c_str());
  AddSpacer();
  _stprintf(buffer, _T("%s (%u)"), _("August"), user.id);
  AddReadOnly(_("Pilot"), NULL, buffer);
  _stprintf(buffer, _T("%s (%u)"), _("Twin"), aircraft_id);
  AddReadOnly(_("Aircraft"), NULL, buffer);
  //      _("cid"), flightdata.competition_id.c_str());
  //  AddReadOnly(_("Glider"), NULL, buffer);

  AddSpacer();
  AddMultiLine(_("Do you want to upload this flight to WeGlide?"));
}

bool UploadPrepareWidget::Save(bool &_changed) noexcept try {
  PopupOperationEnvironment env;
  // bool changed = false;
  // NarrowString<32> buffer;

  // _changed |= changed;
  return true;
} catch (OperationCancelled) {
  return false;
} catch (...) {
  ShowError(std::current_exception(), _T("WeGlide Upload"));
  return false;
}

namespace WeGlide {

int 
PrepareFlightUploadDialog(const Path &igc_path, const User &user,
                          const uint_least32_t glider_id) noexcept
{
  // LogFormat(_T("%s: %s"), _("WeGlide Upload"), msg);
  UploadPrepareWidget widget(UIGlobals::GetDialogLook(), igc_path,
                             user, glider_id);
  return DefaultWidgetDialog(UIGlobals::GetMainWindow(),
    UIGlobals::GetDialogLook(), _("Upload Flight"), widget);
}

} // namespace WeGlide
