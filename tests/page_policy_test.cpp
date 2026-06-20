#include "core/page_policy.hpp"
#include "core/page_type.hpp"
#include <cassert>
int main() {

  const auto bootstrap_type = clawvm::core::PageType::Bootstrap;
  const auto &policy = clawvm::core::policy_for(bootstrap_type);
  assert(policy.default_min_fidelity == clawvm::core::Fidelity::Structured);
  assert(policy.default_pin_class == clawvm::core::PinClass::Hard);
  assert(policy.degradation_path.size() == 2);
  assert(policy.degradation_path[0] == clawvm::core::Fidelity::Full);
  assert(policy.degradation_path[1] == clawvm::core::Fidelity::Structured);
  return 0;
}
