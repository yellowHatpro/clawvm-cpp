#pragma once

#include "core/representation.hpp"
#include "page_type.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace clawvm::core {

struct Page {
  std::string id;
  PageType type;
  Scope scope;

  Fidelity min_fidelity;
  std::string provenance;

  PinClass pin_class; // Runtime pin state; independent from the page type.
  bool dirty = false;

  std::int64_t last_access_turn = 0;
  double recompute_cost = 1.0;

  std::unordered_map<Fidelity, Representation> representations;

  Page(std::string id, PageType type, Scope scope);
};
} // namespace clawvm::core
