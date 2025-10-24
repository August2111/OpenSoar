// SPDX-License-Identifier: BSD-2-Clause
// Copyright OpenSoar
// author: Uwe Augustin <Info@OpenSoar.de>

#pragma once

#include <boost/json/fwd.hpp>
#include <string>

class Path;

namespace Json {

boost::json::value &
Load(Path path);

bool Save(Path path, const boost::json::value &v);

void PrettyPrint(std::ostream &os, boost::json::value const &jv,
  const size_t indent_size = 4, std::string *indent = nullptr);
} // namespace Json
