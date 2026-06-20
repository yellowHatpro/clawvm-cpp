#pragma once

namespace clawvm::core {

enum class PageType {
  Bootstrap,  // Bootstrap/Policy : things like System Prompts, Agent
              // Instructions, Constraints, Policies. Defines the agent's
              // behavior.
  Constraint, // Describes hard requirements. Ex: "NEVER expose secrets", "Only
              // use approved tools"
  Plan, // This is the current active plan. This is the goal + current step.
  Preference, // Stores preferences. Ex: "User likes markdown", "Project uses
              // bazel".
  Evidence, // This contains tool outputs, retrieved documents, search results,
            // database results etc.
  Conversation // This represents conversation history, not the whole
               // conversation, a segment.
};

// Controls whether the representation selector may omit a page under token
// budget pressure.
enum class PinClass {
  None, // Selected only when its utility justifies its token cost.
  Soft, // Strongly preferred, but may be omitted when the budget is tight.
  Hard, // Must be selected at or above its minimum fidelity.
};

enum class Fidelity { Pointer = 0, Structured = 1, Compressed = 2, Full = 3 };

enum class Scope { Session, Project };

} // namespace clawvm::core
