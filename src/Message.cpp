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

#include "Message.hpp"
#include "PopupMessage.hpp"
#include "MainWindow.hpp"
#include "Interface.hpp"

void
Message::AddMessage(const TCHAR *text, const TCHAR *data) noexcept
{
  if (CommonInterface::main_window->popup != nullptr)
    CommonInterface::main_window->popup->AddMessage(text, data);
}

#if 0 // TODO(August2111): welches ist das richtige???
// #else
void
Message::AddMessage(const TCHAR *data, long long ms_delay)
{
  if (CommonInterface::main_window->popup != nullptr)
    CommonInterface::main_window->popup->AddMessage(
      data, std::chrono::milliseconds(ms_delay));
}

#endif