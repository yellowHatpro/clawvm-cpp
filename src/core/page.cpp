#include "core/page.hpp"
#include "core/page_policy.hpp"
#include "core/page_type.hpp"

#include <utility>

namespace clawvm::core {
Page::Page(std::string id_, PageType type_, Scope scope_)
    : id(std::move(id_)), type(type_), scope(scope_),
      min_fidelity(policy_for(type_).default_min_fidelity),
      pin_class(policy_for(type_).default_pin_class) {}

} // namespace clawvm::core
