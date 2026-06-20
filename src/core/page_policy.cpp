#include "core/page_policy.hpp"
#include "core/page_type.hpp"

#include <algorithm>
#include <stdexcept>

namespace clawvm::core {
namespace {
const PageTypePolicy bootstrap_policy{
    .default_min_fidelity = Fidelity::Structured,
    .default_pin_class = PinClass::Hard,
    .degradation_path = {Fidelity::Full, Fidelity::Structured}};

const PageTypePolicy constraint_policy{
    .default_min_fidelity = Fidelity::Structured,
    .default_pin_class = PinClass::Hard,
    .degradation_path = {Fidelity::Full, Fidelity::Structured}};

const PageTypePolicy plan_policy{.default_min_fidelity = Fidelity::Pointer,
                                 .default_pin_class = PinClass::None,
                                 .degradation_path = {Fidelity::Full,
                                                      Fidelity::Structured,
                                                      Fidelity::Pointer}};

const PageTypePolicy preference_policy{
    .default_min_fidelity = Fidelity::Pointer,
    .default_pin_class = PinClass::None,
    .degradation_path = {Fidelity::Full, Fidelity::Compressed,
                         Fidelity::Structured, Fidelity::Pointer}};

const PageTypePolicy evidence_policy{
    .default_min_fidelity = Fidelity::Pointer,
    .default_pin_class = PinClass::None,
    .degradation_path = {Fidelity::Full, Fidelity::Compressed,
                         Fidelity::Structured, Fidelity::Pointer}};

const PageTypePolicy conversation_policy{
    .default_min_fidelity = Fidelity::Pointer,
    .default_pin_class = PinClass::None,
    .degradation_path = {Fidelity::Full, Fidelity::Compressed,
                         Fidelity::Structured, Fidelity::Pointer}};
} // namespace

const PageTypePolicy &policy_for(PageType type) {
  switch (type) {
  case PageType::Bootstrap:
    return bootstrap_policy;
  case PageType::Constraint:
    return constraint_policy;
  case PageType::Plan:
    return plan_policy;
  case PageType::Preference:
    return preference_policy;
  case PageType::Evidence:
    return evidence_policy;
  case PageType::Conversation:
    return conversation_policy;
  }
  throw std::logic_error("Invalid PageType");
}

bool PageTypePolicy::supports_fidelity(Fidelity fidelity) const {
  return std::find(degradation_path.begin(), degradation_path.end(),
                   fidelity) != degradation_path.end();
}

} // namespace clawvm::core
