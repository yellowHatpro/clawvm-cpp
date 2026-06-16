#include "core/page.hpp"
#include "core/page_type.hpp"
#include <string>

namespace clawvm::core {

Page::Page(std::string id_, PageType type_, Scope scope_)
    : id(std::move(id_)), type(type_), scope(scope_),
      min_fidelity(default_min_fidelity(type_)) {};
}
