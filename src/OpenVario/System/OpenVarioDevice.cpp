// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "OpenVario/System/OpenVarioDevice.hpp"
#ifdef DBUS_FUNCTIONS
# include "lib/dbus/Connection.hxx"
# include "lib/dbus/ScopeMatch.hxx"
# include "lib/dbus/Systemd.hxx"
#endif

#include "system/Process.hpp"
#include "system/FileUtil.hpp"
#include "io/KeyValueFileReader.hpp"
#include "io/FileOutputStream.hxx"
#include "io/BufferedOutputStream.hxx"
#include "io/FileLineReader.hpp"
#include "Dialogs/Error.hpp"
#include "Hardware/RotateDisplay.hpp"

#include "Profile/File.hpp"
#include "Profile/Map.hpp"

#include "util/StaticString.hxx"
#include "util/ConvertString.hpp"

#include "LogFile.hpp"
#include "LocalPath.hpp"

#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <fmt/format.h>

#include <stdarg.h>

#include <map>
#include <string>
#include <iostream>

#ifndef __MSVC__
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <map>

using std::string_view_literals::operator""sv;

OpenVario_Device ovdevice;


void ReadBool(std::map<std::string, std::string, std::less<>> &map,
              std::string_view name, bool &value) noexcept;
void ReadInteger(std::map<std::string, std::string, std::less<>> &map,
                 std::string_view name, unsigned &value) noexcept;
void ReadString(std::map<std::string, std::string, std::less<>> &map,
                std::string_view name, std::string_view &value) noexcept;

  void
OpenVario_Device::Initialise() noexcept {
  if (!initialised) {
    InitialiseDataPath();
    StaticString<0x100> home;
    home.SetUTF8(getenv("HOME"));
    home_path = Path(home);
#ifdef _WIN32
    data_path = Path(_T("D:/Data/OpenSoarData"));
#else
    data_path = Path(_T("data"));

    system_config =
        AllocatedPath::Build(Path(_T("/boot")), Path(_T("config.uEnv")));
    is_real = File::Exists(system_config);
#endif
    if (Directory::Exists(data_path)) {
      // auto config = AllocatedPath::Build(data_path,
      // Path(_T("openvario.cfg")));
      settings_config =
          AllocatedPath::Build(data_path, Path(_T("openvario.cfg")));
      upgrade_config = AllocatedPath::Build(data_path, Path(_T("upgrade.cfg")));
    } else {
      settings_config =
          AllocatedPath::Build(home_path, Path(_T("openvario.cfg")));
      upgrade_config = AllocatedPath::Build(home_path, Path(_T("upgrade.cfg")));
    }
    if (!File::Exists(settings_config))
      File::CreateExclusive(settings_config);

    assert(File::Exists(settings_config));

#ifndef DBUS_FUNCTIONS
    // This path is only for Debug purposes on Non-OpenVario systems
    internal_config =
        AllocatedPath::Build(home_path, Path(_T("ov-internal.cfg")));
#endif

    SetPrimaryDataPath(data_path);
    //----------------------------
    LogFormat("data_path = %s", data_path.ToUTF8().c_str());
#ifdef DEBUG_OPENVARIO
    LogFormat("home_path = %s", home_path.ToUTF8().c_str());
    LogFormat("settings_config = %s", settings_config.ToUTF8().c_str());
    LogFormat("system_config = %s", system_config.ToUTF8().c_str());
    if (!is_real)
      system_config = AllocatedPath::Build(home_path, Path(_T("config.uEnv")));

    LogFormat("system_config = %s", system_config.ToUTF8().c_str());
    LogFormat("upgrade_config = %s", upgrade_config.ToUTF8().c_str());
    // the same...: LogFormat(_T("upgrade_config = %s"), upgrade_config.c_str());
    LogFormat("is_real = %s", is_real ? "True" : "False");

    LogFormat("exe_path = %s", exe_path.c_str());
    LogFormat("bin_path = %s", bin_path.c_str());
#endif
    //----------------------------
    // StaticString<0x200> str;
    // str.Format(_T("%s/%s"), home_path, _T("process.txt"));
    run_output_file = AllocatedPath::Build(home_path, Path(_T("tmp.txt")));
    initialised = true;
  } 
}
void OpenVario_Device::Deinitialise() noexcept {}
//----------------------------------------------------------
void ReadBool(std::map<std::string, std::string, std::less<>> &map,
              std::string_view name, bool &value) noexcept {
  if (map.find(name) != map.end())
    value = map.find(name)->second != "False";
}
//----------------------------------------------------------
void ReadInteger(std::map<std::string, std::string, std::less<>> &map,
                 std::string_view name, unsigned &value) noexcept {
  if (map.find(name) != map.end())
    value = std::stoul(map.find(name)->second);
}
//----------------------------------------------------------
void ReadString(std::map<std::string, std::string, std::less<>> &map,
                std::string_view name, std::string_view &value) noexcept {
  if (map.find(name) != map.end())
    value = map.find(name)->second;
}
//----------------------------------------------------------
void 
OpenVario_Device::LoadSettings() noexcept
{
  LoadConfigFile(system_map, GetSystemConfig());
  LoadConfigFile(settings, GetSettingsConfig());
  LoadConfigFile(upgrade_map, GetUpgradeConfig());
#ifndef DBUS_FUNCTIONS
  LoadConfigFile(internal_map, GetInternalConfig());
#endif
  ReadInteger(system_map, "rotation", rotation);
#ifdef _DEBUG
  std::string_view fdtfile;
  ReadString(system_map, "fdtfile", fdtfile);
#endif

  ReadBool(settings, "Enabled", enabled);
  ReadInteger(settings, "iTest", iTest);
  ReadInteger(settings, "Timeout", timeout);
  ReadInteger(settings, "Brightness", brightness);

}
//----------------------------------------------------------
void
LoadConfigFile(std::map<std::string, std::string, std::less<>> &map, Path path)
{
  if (File::Exists(path)) {
    FileLineReaderA reader(path);
    KeyValueFileReader kvreader(reader);
    KeyValuePair pair;
    while (kvreader.Read(pair))
      map.emplace(pair.key, pair.value);
  }
}

//----------------------------------------------------------
void
WriteConfigFile(std::map<std::string, std::string, std::less<>> &map, Path path)
{
  FileOutputStream file(path);
  BufferedOutputStream buffered(file);

  for (const auto &i : map)
    buffered.Fmt("{}={}\n", i.first, i.second);

  buffered.Flush();
  file.Commit();
}

//----------------------------------------------------------
uint_least8_t 
OpenVario_Device::GetBrightness() noexcept
{
  char line[4];
  int result = 10;

  if (File::ReadString(Path(_T("/sys/class/backlight/lcd/brightness")), line, sizeof(line))) {
    result = atoi(line);
  }

  return result;
}

void 
OpenVario_Device::SetBrightness(uint_least8_t value) noexcept
{
  if (value < 1) { value = 1; }
  if (value > 10) { value = 10; }

  File::WriteExisting(Path(_T("/sys/class/backlight/lcd/brightness")), fmt::format_int{value}.c_str());
}

DisplayOrientation
OpenVario_Device::GetRotation()
{
  std::map<std::string, std::string, std::less<>> map;
  LoadConfigFile(map, Path(_T("/boot/config.uEnv")));

  uint_least8_t result;
  result = map.contains("rotation") ? std::stoi(map.find("rotation")->second) : 0;

  switch (result) {
  case 0: return DisplayOrientation::LANDSCAPE;
  case 1: return DisplayOrientation::REVERSE_PORTRAIT;
  case 2: return DisplayOrientation::REVERSE_LANDSCAPE;
  case 3: return DisplayOrientation::PORTRAIT;
  default: return DisplayOrientation::DEFAULT;
  }
}

void
OpenVario_Device::SetRotation(DisplayOrientation orientation)
{
  std::map<std::string, std::string, std::less<>> map;

  Display::Rotate(orientation);

  int rotation = 0; 
  switch (orientation) {
  case DisplayOrientation::DEFAULT:
  case DisplayOrientation::LANDSCAPE:
    break;
  case DisplayOrientation::REVERSE_PORTRAIT:
    rotation = 1;
    break;
  case DisplayOrientation::REVERSE_LANDSCAPE:
    rotation = 2;
    break;
  case DisplayOrientation::PORTRAIT:
    rotation = 3;
    break;
  };

  File::WriteExisting(Path(_T("/sys/class/graphics/fbcon/rotate")), fmt::format_int{rotation}.c_str());

  LoadConfigFile(map, Path(_T("/boot/config.uEnv")));
  map.insert_or_assign("rotation", fmt::format_int{rotation}.c_str());
  WriteConfigFile(map, Path(_T("/boot/config.uEnv")));
}

#ifdef DBUS_FUNCTIONS
SSHStatus
OpenVario_Device::GetSSHStatus()  noexcept
{
  auto connection = ODBus::Connection::GetSystem();

  if (Systemd::IsUnitEnabled(connection, "dropbear.socket")) {
    return SSHStatus::ENABLED;
  } else if (Systemd::IsUnitActive(connection, "dropbear.socket")) {
    return SSHStatus::TEMPORARY;
  } else {
    return SSHStatus::DISABLED;
  }
}

void 
OpenVario_Device::SetSSHStatus(SSHStatus state) noexcept
{
  auto connection = ODBus::Connection::GetSystem();
  const ODBus::ScopeMatch job_removed_match{connection,
                                            Systemd::job_removed_match};
  switch (state) {
  case SSHStatus::ENABLED:
    Systemd::EnableUnitFile(connection, "dropbear.socket");
    Systemd::StartUnit(connection, "dropbear.socket");
    break;
  case SSHStatus::TEMPORARY:
    Systemd::DisableUnitFile(connection, "dropbear.socket");
    Systemd::StartUnit(connection, "dropbear.socket");
    break;
  case SSHStatus::DISABLED:
    Systemd::DisableUnitFile(connection, "dropbear.socket");
    Systemd::StopUnit(connection, "dropbear.socket");
    break;
  }
}

bool
OpenVario_Device::GetSystemStatus(std::string_view system) noexcept
{
#ifdef WITH_NEW_DBUS
  auto connection = ODBus::Connection::GetSystem();
  return Systemd::IsUnitEnabled(connection, system.data());
#else
  // StaticString
 
  std::cout << "  0: " << system << std::endl;
  StaticString<0x20> file;

  std::string_view _dirname("/home/august2111");
  //Path run_tmp_file("/home/august2111/test.txt");
  Path run_tmp_file(_dirname.data());

  if (system == "sensord")
    file = _T("SensorD.txt");
  else if (system == "variod")
    file = _T("VarioD.txt");
  else if (system == "dropbear.socket")
    file = _T("SSH.txt");
  AllocatedPath _tmp_file = AllocatedPath::Build(home_path, Path(file));

  Path tmp_file = _tmp_file;

  std::cout << "  1: " << tmp_file.ToUTF8() << ", " << system << std::endl;
  if (File::Exists(tmp_file))
    File::Delete(tmp_file);  // remove, if exists
  auto run_value = Run(tmp_file, "/bin/systemctl", "is-enabled", system.data());
  std::cout << "  2: " << tmp_file.ToUTF8() << ", " << std::endl;
  char buffer[0x20]; 
  File::ReadString(tmp_file, buffer, sizeof(buffer));
  std::cout << "  3: " << buffer << ", " << std::endl;
  switch (run_value) {
  case 0:
    if (std::string_view(buffer).starts_with("enabled"))
      std::strncpy(buffer, "enabled -> ok!", sizeof(buffer));
    else
      std::strncpy(buffer, "enabled -> not ok???", sizeof(buffer));
    std::cout << "  4: " << system << ": " << buffer << std::endl;
    return true;
  case 1:
    if (std::string_view(buffer).starts_with("enabled"))
      std::strncpy(buffer, "disabled -> ok!", sizeof(buffer));
    else
      std::strncpy(buffer, "disabled -> not ok???", sizeof(buffer));
    std::cout << "  4: " << system << ": " << buffer << std::endl;
    return false;
  default:
    std::cout << "  4: " << system << " = Wrong" << std::endl;
    return false;
  }
#endif
}
void
OpenVario_Device::SetSystemStatus(std::string_view system, bool value) noexcept
{
#ifdef WITH_NEW_DBUS
  auto connection = ODBus::Connection::GetSystem();
  if (value)
    Systemd::EnableUnitFile(connection, system.data());
  else
    Systemd::DisableUnitFile(connection, system.data());
#else
  Run("/bin/systemctl", value ? "enable" : "disable", system.data());
#endif
}

#else   // DBUS_FUNCTIONS
bool 
OpenVario_Device::GetSystemStatus(std::string_view system) noexcept {
  bool value;
  ReadBool(internal_map, system.data(), value);
  return value != 0;
}
void 
OpenVario_Device::SetSystemStatus(std::string_view system, bool value) noexcept {
  internal_map.insert_or_assign(system.data(),
                                         value ? "True" : "False");
  WriteConfigFile(internal_map, GetInternalConfig());
}

SSHStatus
OpenVario_Device::GetSSHStatus() noexcept
{  
  unsigned ssh;
  ReadInteger(ovdevice.internal_map, "SSH", ssh);

  if (ssh == 0) {
    return SSHStatus::ENABLED;
  } else if (ssh == 1) {
    return SSHStatus::DISABLED;
  } else {
    return SSHStatus::TEMPORARY;
  }
}

void 
OpenVario_Device::SetSSHStatus(SSHStatus state) noexcept
{
  switch (state) {
  case SSHStatus::DISABLED:
  case SSHStatus::ENABLED:
  case SSHStatus::TEMPORARY:
    ovdevice.internal_map.insert_or_assign("SSH",
                                           std::to_string((unsigned)state));
    WriteConfigFile(ovdevice.internal_map, ovdevice.GetInternalConfig());
    break;
  }
}
#endif  // DBUS_FUNCTIONS
//----------------------------------------------------------