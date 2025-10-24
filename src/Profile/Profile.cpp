// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Profile.hpp"
#include "Map.hpp"
#include "File.hpp"
#include "Current.hpp"
#include "LogFile.hpp"
#include "Asset.hpp"
#include "LocalPath.hpp"
#include "io/FileOutputStream.hxx"
#include "util/StringUtil.hpp"
#include "util/StringCompare.hxx"
#include "util/StringAPI.hxx"
#include "system/FileUtil.hpp"
// #include "system/Path.hpp"

#include "Device/Config.hpp"
#include "Interface.hpp"
#include "Profile/DeviceConfig.hpp"
#include <json/Get.hpp>
#include <json/File.hpp>

#include <windef.h> /* for MAX_PATH */
#include <cassert>
#include <string>

#define XCSPROFILE "default.prf"
#define DEVICE_PORTS "device_ports.xcd"
#define OLDXCSPROFILE "xcsoar-registry.prf"


#define TEMP_FILE_RENAME_ACTION
#ifdef TEMP_FILE_RENAME_ACTION
# define DEVICE_MAP "device_map.map"
#endif

static AllocatedPath startProfileFile = nullptr;
static AllocatedPath portSettingFile = nullptr;
// static const auto sysConfigPath = GetCachePath("test_config.xcc");
// static const auto sysConfigPath = GetCachePath("system_config.xcc");
static AllocatedPath sysConfigPath = nullptr;
static boost::json::value &sys_config = Json::GetNull();

#ifdef TEMP_FILE_RENAME_ACTION
static AllocatedPath old_dev_file;
#endif
Path
Profile::GetPath() noexcept
{
  return startProfileFile;
}

void
Profile::Load() noexcept
{
#ifdef _DEBUG
  LogFmt("LoadProfile {} ", Profile::GetPath().c_str());
#endif
  if (startProfileFile == nullptr)
    SetFiles(nullptr);
  assert(startProfileFile != nullptr);

#ifdef TEMP_FILE_RENAME_ACTION
  auto old_dev_file = LocalPath(DEVICE_MAP);
  if (File::Exists(old_dev_file)) {
    if (!File::Exists(portSettingFile))
      File::Rename(old_dev_file, portSettingFile);
    else
      File::Delete(old_dev_file);
  }
#endif

  if (File::Exists(portSettingFile)) {
    // if portSettingFile exist load port information from this
#ifdef _DEBUG
    LogString("Loading device port information");
#endif
    LoadFile(portSettingFile);
  }

#ifdef _DEBUG
  LogString("Loading profile information");
#endif
  LoadFile(startProfileFile);

  SetModified(false);
}

namespace Profile {
static void
MovePortSettings() noexcept
{
  /* if no device_ports exist, load port information from normal
   * profile file - and move it to the device_ports */
  for (auto setting : map) { // don't use this with 'Remove'!
    if (setting.first.starts_with("Port")) {
      if (std::isdigit(setting.first[4]))
         device_ports.Set(setting.first, setting.second.c_str());
      else {
        std::string first = "Port1" + setting.first.substr(4);
        device_ports.Set(first, setting.second.c_str());
      }
    } if (setting.first.starts_with("Device")) {
      std::string first = "PortXDriver";
      first[4] = setting.first[6] - 'A' + '1';  // set the correct port
      device_ports.Set(first, setting.second.c_str());
    }
  }
  device_ports.SetModified(true);
}
};

void
Profile::LoadFile(Path path) noexcept
{
  try {
    if (path == portSettingFile) {
      LoadFile(device_ports, path);
    } else if (path == startProfileFile) {
      // normal Profile file
      LoadFile(map, path);
      if (device_ports.empty()) 
        MovePortSettings();
    } else if (path.str().starts_with("test/data")) {
      LoadFile(map, path);  // test-data
    } else {
      LogFmt("LoadFile with wrong file: {}!", path.c_str());
    }
  } catch (...) {
    LogError(std::current_exception(), "Failed to load profile");
  }
}

void
Profile::Save(ProfileMap &_map) noexcept
{
  if (!map.IsModified())
    return;

  Path path = (&_map == &device_ports) ? portSettingFile : startProfileFile;
#ifdef _DEBUG
  LogString("Saving profiles");
#endif
  if (path == nullptr)
    SetFiles(nullptr);

  assert(startProfileFile != nullptr);

  try {
    SaveFile(_map, path);
    _map.SetModified(false);
  } catch (...) {
    LogError(std::current_exception(), "Failed to save profile");
  }
}

void
Profile::Save() noexcept
{ 
  if (!IsModified())
    return;

#ifdef _DEBUG
  LogString("Saving profiles");
#endif
  if (startProfileFile == nullptr)
    SetFiles(nullptr);

  assert(startProfileFile != nullptr);

  try {
    if (map.IsModified())
      SaveFile(map, startProfileFile);
    if (device_ports.IsModified())
      SaveFile(device_ports, portSettingFile);
    SetModified(false);  // set both to 'saved'
  } catch (...) {
    LogError(std::current_exception(), "Failed to save profile");
  }
}

void
Profile::SaveFile(Path path)
{
  // save Profile only (not the port setting!)
  // caller could be InputEventsSettings
#ifdef _DEBUG
  LogFmt("Saving profile to {}", path.c_str());
#endif
  SaveFile(map, path);
}

void
Profile::SetFiles(Path override_path) noexcept
{
  // Set the port profile file
  portSettingFile = LocalPath(DEVICE_PORTS);

  if (override_path != nullptr) {
    if (override_path.IsBase()) {
      if (StringFind(override_path.c_str(), '.') != nullptr)
        startProfileFile = LocalPath(override_path);
      else {
        std::string t(override_path.c_str());
        t += ".prf";
        startProfileFile = LocalPath(t.c_str());
      }
    } else
      startProfileFile = Path(override_path);
    return;
  }

  // Set the default profile file
  startProfileFile = LocalPath(XCSPROFILE);
}

AllocatedPath
Profile::GetPath(std::string_view key) noexcept
{
  return map.GetPath(key);
}

bool
Profile::GetPathIsEqual(std::string_view key, Path value) noexcept
{
  return map.GetPathIsEqual(key, value);
}

void
Profile::SetPath(std::string_view key, Path value) noexcept
{
  map.SetPath(key, value);
}

#if 0
std::string_view 
GetConfigString(std::string_view arg1, std::string_view arg2, std::string_view arg3) noexcept
{
  try {
    return sys_config.at(arg1).at(arg2).at(arg3).as_string().c_str();
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception GetConfigString(3): {}", e.what());
  }
  return "exception";
}

std::string_view 
GetConfigString(std::string_view arg1, std::string_view arg2) noexcept
{
  try {
    return sys_config.at(arg1).at(arg2).as_string().c_str();
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception GetConfigString(2): {}", e.what());
  }
  return "exception";
}


bool
GetConfigBool(std::string_view arg1, std::string_view arg2) noexcept
{
  try {
    return sys_config.at(arg1).at(arg2).as_bool();
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception GetConfigBool(2): {}", e.what());
  }
  return false;
}

void
SetConfigBool(std::string_view arg1, std::string_view arg2, bool b) noexcept
{
  try {
    sys_config.at(arg1).at(arg2).as_bool() = b;
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception SetConfigBool(2): {}", e.what());
  }
}

namespace Json {
  boost::json::value &
    GetValue(boost::json::value &root, std::vector<std::string_view> args) noexcept
  {
    boost::json::value *value = &root;
    std::string name = "sys_config";
    size_t i = 0;
    try {
      for (auto str : args) {
        i++;
        if (value->is_object() && value->as_object().if_contains(str)) {
          value = &value->at(str);
          name += "->";
          name += str;
        } else {
          LogFmt("Json::GetValue: Param {}: {}->{} not exists!",
            i, name, str);
          return json_null;
        }
      }
      return *value;
    }
    catch (const std::exception &e) {
      LogFmt("Json-Exception GetConfigBool({}): {}", args.size(), e.what());
    }
    return json_null;
  }

  boost::json::value &
    GetValue(boost::json::value &root, std::string_view str) noexcept
  {
    std::vector<std::string_view> args;
    for (auto x = str.find('.'); x < 1000; x = str.find('.')) {
      args.push_back(str.substr(0, x));
      str = str.substr(x + 1);

    }
    args.push_back(str);
    return GetValue(root, args);
  }


  bool
  GetBool(boost::json::value &root, std::vector<std::string_view> args) noexcept
  {
    auto value = GetValue(root, args);
    if (value.is_bool())
      return value.as_bool();
    else {
      LogFmt("No Bool");
      return false;
    }
  }

  bool
  GetBool(boost::json::value &root, std::string_view str) noexcept
  {
    auto value = GetValue(root, str);
    if (value.is_bool())
      return value.as_bool();
    else {
      LogFmt("No Bool");
      return false;
    }
  }

  boost::json::object &
  GetObject(boost::json::value &root, std::string_view str) noexcept
  {
    auto value = GetValue(root, str);
    if (value.is_object())
      return value.as_object();
    else {
      LogFmt("No Object");
      return value.as_object();   // last valid value
    }
  }

}

bool
GetConfigBool(std::string_view arg1, std::string_view arg2, std::string_view arg3) noexcept
{
  try {
    return sys_config.at(arg1).at(arg2).at(arg3).as_bool();
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception GetConfigBool(3): {}", e.what());
  }
  return false;
}

void
SetConfigBool(std::vector<std::string_view> args, bool b) noexcept
{
  try {
    boost::json::value *value;
    // auto value = sys_config;
    for (auto str : args)
      value = &value->at(str);
    value->as_bool() = b;
  }
  catch (const std::exception &e) {
    LogFmt("Json-Exception SetConfigBool(3): Param {} not exists", e.what());
  }
}
#endif  // 0

void
Profile::LoadConfiguration() noexcept
{
  auto path = GetCachePath("__system_config.xcc");
  sysConfigPath = GetCachePath("system_config.xcc");
  
  if (File::Exists(path)) {
    try {
      sys_config = Json::Load(path);

      if (sys_config.is_object()) {
        LogFmt("JSON: {}", to_string(sys_config.kind())); //.as_string().c_str());
        auto &config = Json::GetValue(sys_config, "Config");
        if (config.is_null()) {
          LogFmt("JSON 'config is null' {} ", __LINE__);
          // config.add???
          bool &b = config.emplace_bool();
          LogFmt("JSON (bool): {}", config.as_bool());
          b = true;
          LogFmt("JSON (bool): {}", config.as_bool());
        }
        else {
          LogFmt("JSON 'config is NOT null' {} ", __LINE__);
        }
        auto &config2 = Json::GetValue(sys_config, "Config2");
        //auto &config2 = sys_config.at("Config2").get_object();
#if 0
        LogFmt("JSON: {}", Json::GetValue(config, "Club.Profile").as_string().c_str());
        LogFmt("JSON: {}", Json::GetValue(config, "Club.Enabled").as_bool());
        LogFmt("JSON: {}", Json::GetValue(sys_config, "Config.Club.Profile").as_string().c_str());
#endif
        LogFmt("JSON: {}", Json::GetValue(sys_config, "Config2.ClubProfile").as_string().c_str());
        boost::json::value val1 = Json::GetValue(sys_config, "Config2.ClubEnabled.Test");
        boost::json::value &val = Json::GetValue(sys_config, "Config2.ClubEnabled");
        // boost::json::value val = Json::GetValue(sys_config,{ "Config2", "ClubEnabled" });
        LogFmt("JSON: {}", val.as_bool());
        val.as_bool() = false;
        LogFmt("JSON: {}", val.as_bool());
        // SetConfigBool({ "Config2", "ClubEnabled" }, false);
        // LogFmt("JSON: {}", Json::GetValue(sys_config, { "Config2", "ClubEnabled" }).as_bool());
        LogFmt("JSON: {}", Json::GetBool(sys_config, "Config2.ClubEnabled"));
        // LogFmt("JSON: {}", Json::GetValue(sys_config, "Config2.Irgendwas").as_bool());
#if 0
        bool &b = config.at("Club").at("Test").emplace_bool();
        b = true;
#else  //0
        if (!config.is_null()) {
          boost::json::value &obj = Json::GetValue(sys_config, { "Config", "Club", "Test" });
          // LogFmt("JSON: {}", GetConfigString("Config", "Club", "Test"));
          LogFmt("JSON (str): {}", obj.as_string().c_str());
          // bool &b = config.at("Club").at("Test").emplace_bool();
          bool &b = obj.emplace_bool();
          LogFmt("JSON (bool): {}", obj.as_bool());
          b = true;
          // LogFmt("JSON: {}", obj.get_bool());
          LogFmt("JSON (bool): {}", obj.as_bool());
          // LogFmt("JSON: {}", GetConfigBool({ "Config", "Club", "Test" }));
        }
#endif // 0
        LogFmt("JSON 'Part 1 ");
        if (config.is_null()) {
          LogFmt("JSON 'config is null' {} ", __LINE__);
        }
        else {
          auto val = config.at("value");
          if (val.is_null()) {
            LogFmt("JSON 'value in config is null' {} ", __LINE__);
          }
          else {
            auto &array = config.at("value").as_array();
            int x = 0;
            for (auto &a : array) {
              LogFmt("JSON ({}: {})", ++x, a.as_int64());
              if (x == 3) a = 10;
            }
          }
        }
        LogFmt("JSON: {}", config2.at("ClubProfile").as_string().c_str());
        auto &config2obj = Json::GetObject(sys_config, "Config2");
        // config2obj.emplace("NewItem", "new_item");
        // config2obj.emplace("BoolItem", false);
        // config2obj.emplace("IntItem", 12345);
      }
      if (!Json::Save(sysConfigPath, sys_config)) {
        LogFmt("Error saving configuration file: {} ", sysConfigPath.c_str());
      }
    }
    catch (std::exception &e) {
      LogFmt("Json-Exception: {}", e.what());
    }
  } else {  // ============================================ File not exist
    LogFmt("Configuration file not exists: {} ", path.c_str());
    boost::json::value &sys_config = Json::Load(path);
    if (sys_config.is_null()) {
      sys_config = boost::json::object{};
    }

    path = GetCachePath("test2_system_config.json");
    auto x = sys_config.as_object().insert_or_assign("Device A", boost::json::object{});
    x.first->value().as_object().insert_or_assign("Baudrate", 36400);
    x.first->value().as_object().insert_or_assign("Driver", "Larus");
    x.first->value().as_object().insert_or_assign("Port", "ttyUSB0");

    if (!Json::Save(sysConfigPath, sys_config)) {
      LogFmt("Error saving configuration file: {} ", sysConfigPath.c_str());
    }

    // std::fstream os(GetCachePath("system_config.xcj").c_str(), std::fstream::out | std::fstream::binary); // = fos(GetCachePath("system_config.xcj"));
    // Json::PrettyPrint(os, sys_config, 2);
    // os.close();

    // FileOutputStream fos(GetCachePath("system_config.xcj"));
    // Json::Serialize(fos, sys_config);
    // fos.Commit();  // make it permanent!
  }
}

void
Profile::SaveConfiguration() noexcept
{
  auto path = GetCachePath("device_config.xcc");
}

AllocatedPath
Profile::GetConfigPath(std::string_view key) noexcept
{
  return system_config.GetPath(key);
}

