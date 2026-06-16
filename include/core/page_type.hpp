#pragma once

namespace clawvm::core {

enum class PageType {
  Bootstrap,
  Constraint,
  Plan,
  Preference,
  Evidence,
  Conversation
};

enum class PageScope { Session, Project };

enum class Fidelity { Pointer = 0, Structured = 1, Compressed = 2, Full = 3 };

enum class Scope { Session, Project };

Fidelity default_min_fidelity(PageType type);
} // namespace clawvm::core
