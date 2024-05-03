// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Config.hpp"
#include "Asset.hpp"
#include "Language/Language.hpp"
#include "util/Compiler.h"
#include "util/StringCompare.hxx"

#ifdef ANDROID
#include "Android/Main.hpp"
#include "Android/BluetoothHelper.hpp"
#include "java/Global.hxx"
#endif

bool
DeviceConfig::IsAvailable() const noexcept
{
  if (!enabled)
    return false;

  switch (port_type) {
  case PortType::DISABLED:
    return false;

  case PortType::SERIAL:
    return true;

  case PortType::RFCOMM:
  case PortType::BLE_HM10:
  case PortType::BLE_SENSOR:
  case PortType::USB_SERIAL:
    return IsAndroid() || true; // IsWindows()
  case PortType::RFCOMM_SERVER:
  case PortType::GLIDER_LINK:
    return IsAndroid();

  case PortType::IOIOUART:
  case PortType::DROIDSOAR_V2:
  case PortType::NUNCHUCK:
  case PortType::I2CPRESSURESENSOR:
  case PortType::IOIOVOLTAGE:
    return HasIOIOLib();

  case PortType::AUTO:
    return false;

  case PortType::INTERNAL:
    return IsAndroid() || IsApple();

  case PortType::TCP_CLIENT:
    return true;

  case PortType::TCP_LISTENER:
  case PortType::UDP_LISTENER:
    return true;

  case PortType::PTY:
#if defined(HAVE_POSIX) && !defined(ANDROID)
    return true;
#else
    return false;
#endif
  }

  /* unreachable */
  return false;
}

bool
DeviceConfig::ShouldReopenOnTimeout() const noexcept
{
  switch (port_type) {
  case PortType::DISABLED:
    return false;

  case PortType::SERIAL:
  case PortType::AUTO:
    /* TODO: old branch for Windows CE due to its quirks */
    return false;

  case PortType::RFCOMM:
  case PortType::BLE_SENSOR:
  case PortType::BLE_HM10:
  case PortType::RFCOMM_SERVER:
  case PortType::USB_SERIAL:
  case PortType::IOIOUART:
  case PortType::DROIDSOAR_V2:
  case PortType::NUNCHUCK:
  case PortType::I2CPRESSURESENSOR:
  case PortType::IOIOVOLTAGE:
  case PortType::TCP_CLIENT:
    /* errors on these are detected automatically by the driver */
    return false;

  case PortType::INTERNAL:
    /* reopening the Android / Apple internal GPS doesn't help */
    return false;

  case PortType::TCP_LISTENER:
  case PortType::UDP_LISTENER:
    /* this is a server, and if no data gets received, this can just
       mean that nobody connected to it, but reopening it periodically
       doesn't help */
    return false;

  case PortType::PTY:
  case PortType::GLIDER_LINK:
    return false;
  }

  gcc_unreachable();
}

bool
DeviceConfig::MaybeBluetooth(PortType port_type, [[maybe_unused]] const char *path) noexcept
{
  /* note: RFCOMM_SERVER is not considered here because this
     function is used to check for the K6-Bt protocol, but the K6-Bt
     will never connect to XCSoar  */

  if (port_type == PortType::RFCOMM)
    return true;

#ifdef HAVE_POSIX
  if (port_type == PortType::SERIAL && strstr(path, "/rfcomm") != nullptr)
    return true;
#endif

  return false;
}

bool
DeviceConfig::MaybeBluetooth() const noexcept
{
  /* note: RFCOMM_SERVER is not considered here because this
     function is used to check for the K6-Bt protocol, but the K6-Bt
     will never connect to XCSoar  */

  if (port_type == PortType::RFCOMM)
    return true;

#ifdef HAVE_POSIX
  if (port_type == PortType::SERIAL && path.Contains("/rfcomm"))
    return true;
#endif

  return false;
}

bool
DeviceConfig::BluetoothNameStartsWith([[maybe_unused]] const char *prefix) const noexcept
{
#ifdef ANDROID
  if (port_type != PortType::RFCOMM)
    return false;

  if (bluetooth_helper == nullptr)
    return false;

  const char *name =
    bluetooth_helper->GetNameFromAddress(Java::GetEnv(),
                                         bluetooth_mac.c_str());
  return name != nullptr && StringStartsWith(name, prefix);
#else
  return false;
#endif
}

void
DeviceConfig::Clear() noexcept
{
  port_type = PortType::DISABLED;
  baud_rate = 4800u;
  bulk_baud_rate = 0u;
  tcp_port = 4353u;
  i2c_bus = 2u;
  i2c_addr = 0;
  press_use = PressureUse::STATIC_ONLY;
  path.clear();
  port_name.clear();
  bluetooth_mac.clear();
  driver_name.clear();
  enabled = true;
  sync_from_device = true;
  sync_to_device = true;
  k6bt = false;
  engine_type = EngineType::NONE;
#ifndef NDEBUG
  dump_port = false;
#endif
//  dump_port = true;
}

const char *
DeviceConfig::GetPortName(char *buffer, size_t max_size) const noexcept
{
  switch (port_type) {
  case PortType::DISABLED:
    return _("Disabled");

  case PortType::SERIAL:
    return path.c_str();

  case PortType::BLE_SENSOR: {
    const char *name = bluetooth_mac.c_str();
#ifdef ANDROID
    if (bluetooth_helper != nullptr) {
      const char *name2 =
        bluetooth_helper->GetNameFromAddress(Java::GetEnv(), name);
      if (name2 != nullptr)
        name = name2;
    }
#elif defined(_WIN32)
    name = port_name;
#endif

    StringFormat(buffer, max_size, "%s: %s",
                 _("BLE sensor"), name);
    return buffer;
    }

  case PortType::BLE_HM10: {
    const char *name = bluetooth_mac.c_str();
#ifdef ANDROID
    if (bluetooth_helper != nullptr) {
      const char *name2 =
        bluetooth_helper->GetNameFromAddress(Java::GetEnv(), name);
      if (name2 != nullptr)
        name = name2;
    }
#elif defined(_WIN32)
    name = port_name;
#endif

    StringFormat(buffer, max_size, "%s: %s",
                 _("BLE port"), name);
    return buffer;
    }

  case PortType::RFCOMM: {
    const char *name = bluetooth_mac.c_str();
#ifdef ANDROID
    if (bluetooth_helper != nullptr) {
      const char *name2 =
        bluetooth_helper->GetNameFromAddress(Java::GetEnv(), name);
      if (name2 != nullptr)
        name = name2;
    }
#elif defined(_WIN32)
    name = port_name;
#endif

    StringFormat(buffer, max_size, "Bluetooth %s", name);
    return buffer;
    }

  case PortType::RFCOMM_SERVER:
    return _("Bluetooth server");

  case PortType::IOIOUART:
    StringFormat(buffer, max_size, "IOIO UART %d", ioio_uart_id);
    return buffer;

  case PortType::DROIDSOAR_V2:
    return "DroidSoar V2";

  case PortType::NUNCHUCK:
    return "Nunchuck";

  case PortType::I2CPRESSURESENSOR:
    return "IOIO i2c pressure sensor";

  case PortType::IOIOVOLTAGE:
    return "IOIO voltage sensor";

  case PortType::AUTO:
    return _("GPS Intermediate Driver");

  case PortType::INTERNAL:
    return _("Built-in GPS & sensors");

  case PortType::GLIDER_LINK:
    return _("GliderLink traffic receiver");

  case PortType::TCP_CLIENT:
    StringFormat(buffer, max_size, "TCP client %s:%u",
                 ip_address.c_str(), tcp_port);
    return buffer;

  case PortType::TCP_LISTENER:
    StringFormat(buffer, max_size, "TCP port %d", tcp_port);
    return buffer;

  case PortType::UDP_LISTENER:
    StringFormat(buffer, max_size, "UDP port %d", tcp_port);
    return buffer;

  case PortType::PTY:
    StringFormat(buffer, max_size, "Pseudo-terminal %s", path.c_str());
    return buffer;

  case PortType::USB_SERIAL:
    StringFormat(buffer, max_size, "%s: %s",
                 _("USB serial"), port_name.c_str());
    return buffer;
  }

  gcc_unreachable();
}
