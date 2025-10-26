// SPDX-License-Identifier: BSD-2-Clause
// Copyright OpenSoar
// author: Uwe Augustin <Info@OpenSoar.de>

#pragma once

#include <boost/json/fwd.hpp>
#include <string>

class Path;

enum enumConfigs {
  SYSTEM_CONFIG,
  PORT_CONFIG,
  PROFILE,
  LAST
};

namespace Json {


boost::json::value &Load(enumConfigs config, Path path);

bool Save(enumConfigs config); //  , const boost::json::value &v);

void PrettyPrint(std::ostream &os, boost::json::value const &jv,
  const size_t indent_size = 4, std::string *indent = nullptr);
} // namespace Json
