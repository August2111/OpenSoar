// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Dialogs/DialogSettings.hpp"
#include "Dialogs/Message.hpp"
#include "Dialogs/ProcessDialog.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "DisplayOrientation.hpp"
#include "Hardware/DisplayDPI.hpp"
#include "Hardware/DisplayGlue.hpp"
#include "Hardware/RotateDisplay.hpp"
#include "Look/DialogLook.hpp"
#include "Profile/File.hpp"
#include "Profile/Map.hpp"
#include "Screen/Layout.hpp"
#include "UIGlobals.hpp"
// #include "Widget/RowFormWidget.hpp"
#include "system/FileUtil.hpp"
#include "system/Process.hpp"
#include "ui/event/KeyCode.hpp"
// #include "ui/event/Queue.hpp"
#include "ui/event/Timer.hpp"
#include "ui/window/Init.hpp"
// #include "ui/window/SingleWindow.hpp"

#include "Language/Language.hpp"

#include "io/KeyValueFileReader.hpp"
#include "io/FileOutputStream.hxx"
#include "io/BufferedOutputStream.hxx"
#include "io/FileLineReader.hpp"

#include "OpenVario/System/System.hpp"
#include "OpenVario/System/Setting/SensordWidget.hpp"

#ifndef __MSVC__
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <fmt/format.h>

#include <map>
#include <string>

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
void
SettingSensordWidget::Prepare([[maybe_unused]] ContainerWindow &parent,
                             [[maybe_unused]] const PixelRect &rc) noexcept
{
  AddButton(_T("Enable"), [](){
    static constexpr const char *argv[] = {
      "/bin/sh", "-c", 
      "systemctl enable sensord && printf '\nsensord has been enabled'", 
      nullptr
    };

    RunProcessDialog(UIGlobals::GetMainWindow(),
                     UIGlobals::GetDialogLook(),
                     _T("Enable"), argv);
  });

  AddButton(_T("Disable"), [](){
    static constexpr const char *argv[] = {
      "/bin/sh", "-c", 
      "systemctl disable sensord && printf '\nsensord has been disabled'", 
      nullptr
    };

    RunProcessDialog(UIGlobals::GetMainWindow(),
                     UIGlobals::GetDialogLook(),
                     _T("Disable"), argv);
  });
}
