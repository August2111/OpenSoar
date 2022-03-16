/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
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

#include "UIActions.hpp"
#include "UIGlobals.hpp"
#include "Interface.hpp"
#include "Input/InputEvents.hpp"
#include "MainWindow.hpp"
#include "Language/Language.hpp"
#include "Dialogs/Message.hpp"
#include "FLARM/Glue.hpp"
#include "Gauge/BigTrafficWidget.hpp"
#include "Gauge/BigThermalAssistantWidget.hpp"
#include "Look/Look.hpp"
#include "Widget/HorizonWidget.hpp"
#include "Widget/WeatherWidget.hpp"

#include "Widget/ViewImageWidget.hpp"

static bool force_shutdown = false;

void
UIActions::SignalShutdown(bool force)
{
  force_shutdown = force;
  CommonInterface::main_window->Close();
}

bool
UIActions::CheckShutdown()
{
  if (force_shutdown)
    return true;

  return ShowMessageBox(_("Quit program?"), _T("XCSoar"),
                     MB_YESNO | MB_ICONQUESTION) == IDYES;

}

void
UIActions::ShowTrafficRadar()
{
  if (InputEvents::IsFlavour(_T("Traffic")))
    return;

  LoadFlarmDatabases();

  CommonInterface::main_window->SetWidget(new TrafficWidget());
  InputEvents::SetFlavour(_T("Traffic"));
}

void
UIActions::ShowThermalAssistant()
{
  if (InputEvents::IsFlavour(_T("TA")))
    return;

  auto ta_widget =
    new BigThermalAssistantWidget(CommonInterface::GetLiveBlackboard(),
                                  UIGlobals::GetLook().thermal_assistant_dialog);
  CommonInterface::main_window->SetWidget(ta_widget);
  InputEvents::SetFlavour(_T("TA"));
}

void
UIActions::ShowHorizon()
{
  if (InputEvents::IsFlavour(_T("Horizon")))
    return;

  auto widget = new HorizonWidget();
  CommonInterface::main_window->SetWidget(widget);
  InputEvents::SetFlavour(_T("Horizon"));
}


#include "system/Path.hpp"
// #include "Dialogs/Weather/PCMetDialog.hpp"

void
UIActions::ShowWeather()
{
//  if (InputEvents::IsFlavour(_T("Weather")))
//    return;

 // auto widget = new WeatherWidget();
  Bitmap *bitmap = new Bitmap();
  bitmap->LoadFile(AllocatedPath( _T("D:/XCSoarData/cache/pc_met/nb_ir_rgb_mdl_2109071600_sat.jpg")));
//  bitmap.LoadFile(AllocatedPath( _T("cache/pc_met/nb_ir_rgb_mdl_2109071600_sat.jpg")));
  //  BitmapDialog(bitmap);
//  BitmapDialog(bitmap);
//  TWidgetDialog<ViewImageWidget> dialog(
//      WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
//      UIGlobals::GetDialogLook(), _T("pc_met"), new ViewImageWidget(bitmap));
//  dialog.AddButton(_("Close"), mrOK);
//  //  dialog.SetWidget();
//  dialog.ShowModal();

  auto widget = new ViewImageWidget(*bitmap);
  CommonInterface::main_window->SetWidget(widget);
  // PixelRect rc(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
  // widget->Prepare(nullptr, rc);
  CommonInterface::main_window->Show();
  // bitmap.
  // widget->Initialise();
  // widget->Show(rc);
  // InputEvents::SetFlavour(_T("Weather"));
  InputEvents::SetFlavour(nullptr);
  // delete bitmap;
}

