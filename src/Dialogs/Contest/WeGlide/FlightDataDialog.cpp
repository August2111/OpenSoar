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

#include "FlightDataDialog.hpp"
#include "LogFile.hpp"
#include "UIGlobals.hpp"
#include "Dialogs/ListPicker.hpp"
#include "Language/Language.hpp"
#include "Look/DialogLook.hpp"
#include "Widget/ListWidget.hpp"
#include "Renderer/TwoTextRowsRenderer.hpp"

int FlightResponse(const TCHAR *caption, unsigned num_items,
                   unsigned initial_value,
                   unsigned item_height, ListItemRenderer &item_renderer,
                   bool update = false, const TCHAR *help_text = nullptr,
                   ItemHelpCallback_t itemhelp_callback = nullptr,
                   const TCHAR *extra_caption = nullptr);

int FlightResponse(const TCHAR *caption, unsigned num_items,
                   unsigned initial_value, unsigned item_height,
                   ListItemRenderer &item_renderer, bool update,
                   const TCHAR *help_text,
                   ItemHelpCallback_t itemhelp_callback,
                   const TCHAR *extra_caption)
{
  assert(num_items <= 0x7fffffff);
  assert((num_items == 0 && initial_value == 0) || initial_value < num_items);
  assert(item_height > 0);

  WidgetDialog dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
                      UIGlobals::GetDialogLook(), caption);

  ListPickerWidget *const list_widget =
      new ListPickerWidget(num_items, initial_value, item_height, item_renderer,
                           dialog, caption, help_text);

  std::unique_ptr<Widget> widget(list_widget);

  if (_itemhelp_callback != nullptr) {
    widget = std::make_unique<TwoWidgets>(std::move(widget),
                                          std::make_unique<TextWidget>());
    auto &two_widgets = (TwoWidgets &)*widget;
    list_widget->EnableItemHelp(
        _itemhelp_callback, (TextWidget &)two_widgets.GetSecond(), two_widgets);
  }

  if (num_items > 0)
    dialog.AddButton(_("Select"), mrOK);

  if (extra_caption != nullptr)
    dialog.AddButton(extra_caption, -2);

  if (help_text != nullptr)
    dialog.AddButton(_("Help"), [list_widget]() { list_widget->ShowHelp(); });

  dialog.AddButton(_("Cancel"), mrCancel);

  dialog.EnableCursorSelection();

  UI::PeriodicTimer update_timer(
      [list_widget]() { list_widget->GetList().Invalidate(); });
  if (update)
    update_timer.Schedule(std::chrono::seconds(1));

  dialog.FinishPreliminary(widget.release());

  int result = dialog.ShowModal();
  if (result == mrOK)
    result = (int)list_widget->GetList().GetCursorIndex();
  else if (result != -2)
    result = -1;

  return result;
}


struct FlightDataEntry {
  const TCHAR *name;
  const TCHAR *data = nullptr;
};

const FlightDataEntry flightdata_entries[] = {
  {_("Flight Info")},
  {_("User")},          
  {_("Aircraft")},
  {_("Upload Message")},
};

class FlightDataRenderer : public ListItemRenderer {
  TwoTextRowsRenderer row_renderer;
  public:
  unsigned CalculateLayout( // const DialogLook &look,
                           const WeGlide::Flight &flight_data,
                           const TCHAR *msg) noexcept
  {
    const DialogLook &look = UIGlobals::GetDialogLook();
    flightdata = flight_data;
    message = msg;
    // row_renderer.
    return row_renderer.CalculateLayout(look.small_font,
     *look.list.font_bold);
   }

  void OnPaintItem(Canvas &canvas, const PixelRect rc,
                   unsigned i) noexcept override
  {
    row_renderer.DrawFirstRow(canvas, rc, flightdata_entries[i].name);
    StaticString<0x100> info; 
    switch (i) { 
      case 0: // "Flight Info"
        info.Format(_T("%s = %d, %s: %s"), _("id"), flightdata.flight_id,
          _("date = "), flightdata.scoring_date.c_str());
        break;
      case 1:  // "User"
        info.Format(_T("%s (%u)"), flightdata.user.name.c_str(),
                    flightdata.user.id);
        break;
      case 2:  // "Aircraft"
        info.Format(_T("%s (%u)"), flightdata.aircraft.name.c_str(),
          flightdata.aircraft.id);
        break;
      case 3:  // "Message"
        info.Format(_T("%s"), message);
        break;

      default:
        info = _T("info");
        break;
    }
    row_renderer.DrawSecondRow(canvas, rc, info.c_str());
  }
  WeGlide::Flight flightdata;
  const TCHAR *message;
};

namespace WeGlide {

int FlightDataDialog(const WeGlide::Flight &flightdata, const TCHAR *msg) {
  FlightDataRenderer item_renderer;
  LogFormat(_T("%s: %s"), _("WeGlide Upload"), msg);
  unsigned n = sizeof(flightdata_entries) / sizeof(FlightDataEntry);

  assert(n > 0);

  return FlightResponse(
      _("Flight Data"), n, 1,
      item_renderer.CalculateLayout(flightdata, msg),
      item_renderer);
}

} // namespace WeGlide