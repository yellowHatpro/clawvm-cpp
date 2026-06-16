#pragma once

#include "core/page_type.hpp"
#include <string>

namespace clawvm::core {
struct Representation {
  Fidelity fidelity;
  std::string content;
  int token_cost = 0;
};
} // namespace clawvm::core
