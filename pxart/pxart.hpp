#pragma once

#include <string>
#include <ostream>
#include <stdexcept>

namespace pxart
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  inline void
  say_hello (std::ostream& o, const std::string& name)
  {
    using namespace std;

    if (name.empty ())
      throw invalid_argument ("empty name");

    o << "Hello, " << name << '!' << endl;
  }
}
