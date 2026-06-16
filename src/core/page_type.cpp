#include "core/page_type.hpp"

namespace clawvm::core {

Fidelity default_min_fidelity(PageType type) {
  switch (type) {
  case PageType::Bootstrap:
  case PageType::Constraint:
    return Fidelity::Structured;
  case PageType::Plan:
  case PageType::Preference:
  case PageType::Evidence:
  case PageType::Conversation:
    return Fidelity::Pointer;
  }
  return Fidelity::Pointer;
}
} // namespace clawvm::core
