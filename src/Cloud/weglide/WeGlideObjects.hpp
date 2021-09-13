#pragma once

#include "util/StaticString.hxx"
#include "time/BrokenDate.hpp"

namespace WeGlide {

  struct User {
    uint32_t id;
    BrokenDate birthdate;
    StaticString<0x80> name;
  };

  struct Aircraft {
    uint32_t id;
    StaticString<0x40> name;
    StaticString<4> kind;  // 'MG' - motor aircraft,...
    StaticString<10> sc_class;
  };
  
struct Flight {
  uint64_t flight_id = 0;
  User user;
  Aircraft aircraft;
  StaticString<0x40> scoring_date;
  StaticString<0x40> registration;
  StaticString<0x40> competition_id;
};

};