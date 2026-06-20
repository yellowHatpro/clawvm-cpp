#pragma once

#include "core/page_type.hpp"

#include <vector>

namespace clawvm::core {

struct PageTypePolicy {
  Fidelity default_min_fidelity;
  PinClass default_pin_class;

  // ordered from highest fidelity to the lowest legal representation
  std::vector<Fidelity> degradation_path;
};

const PageTypePolicy &policy_for(PageType type);

} // namespace clawvm::core
