#include "core/page.hpp"
#include "core/page_policy.hpp"
#include "core/page_type.hpp"
#include <stdexcept>
#include <utility>

namespace clawvm::core {
Page::Page(std::string id_, PageType type_, Scope scope_)
    : id(std::move(id_)), type(type_), scope(scope_),
      min_fidelity(policy_for(type_).default_min_fidelity),
      pin_class(policy_for(type_).default_pin_class) {}

void Page::add_representation(Representation representation) {
  if (representation.token_cost < 0) {
    throw std::invalid_argument("token cost is less than 0");
  }
  if (representation.content.empty()) {
    throw std::invalid_argument("empty content, retard!");
  }
  const auto &policy = policy_for(type);
  if (!policy.supports_fidelity(representation.fidelity)) {
    throw std::invalid_argument(
        "representation fidelity is not supported for this page type");
  }
  const Fidelity fidelity = representation.fidelity;
  representations.insert_or_assign(fidelity, std::move(representation));
}

const Representation *Page::representation_at(Fidelity fidelity) const {
  const auto it = representations.find(fidelity);
  if (it == representations.end()) {
    return nullptr;
  }
  return &it->second;
}

bool Page::has_representation(Fidelity fidelity) const {
  return representations.contains(fidelity);
}

} // namespace clawvm::core
