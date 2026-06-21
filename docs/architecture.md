# ClawVM Architecture

This document summarizes the end-to-end flow described in the
[ClawVM paper](https://arxiv.org/abs/2604.10352). Solid component names are
paper concepts; the C++ implementation is currently focused on `Page`,
`PageTypePolicy`, and representation storage.

```mermaid
flowchart TD
    Event["Harness event<br/>user message, tool result, retrieval result,<br/>compaction, reset, or model turn"]

    subgraph Ingestion["1. Page ingestion or update"]
        Adapter["Source adapter / page ingestor"]
        Classify["Assign stable ID, PageType,<br/>Scope, Provenance, PinClass,<br/>and minimum-fidelity invariant"]
        Generate["Generate representations at ingestion time"]
        Full["Full<br/>verbatim content"]
        Compressed["Compressed<br/>token-reduced text"]
        Structured["Structured<br/>typed invariant-preserving fields"]
        Pointer["Pointer<br/>resolvable handle + metadata"]

        Adapter --> Classify --> Generate
        Generate --> Full
        Generate --> Compressed
        Generate --> Structured
        Generate --> Pointer
    end

    subgraph State["2. SessionPageTable"]
        PageTable["Store typed pages and runtime state<br/>dirty flag, recency, recompute cost,<br/>available representations"]
    end

    subgraph Assembly["3. Prompt assembly before every model call"]
        Candidates["Determine candidate and demanded pages"]
        Budget["Calculate page token budget<br/>after fixed prompt regions and output reserve"]

        Phase1["Phase 1: install required baseline<br/>hard-pinned and demanded pages at the<br/>cheapest available representation that<br/>satisfies minimum fidelity"]
        Pressure{"Does the required<br/>baseline fit?"}
        InvariantFault["Record invariant pressure /<br/>pinned-invariant miss"]

        Upgrades["Generate legal upgrades<br/>Pointer → Structured → Compressed → Full"]
        Score["Score each upgrade using marginal<br/>utility / additional tokens<br/>pin, demand, recency, scope,<br/>membership, recompute cost"]
        Phase2["Phase 2: apply deterministic<br/>highest-value upgrades that fit"]
        ResidentSet["Selected resident set<br/>one representation per selected page"]

        Candidates --> Phase1
        Budget --> Phase1
        Phase1 --> Pressure
        Pressure -- "No" --> InvariantFault
        Pressure -- "Yes" --> Upgrades --> Score --> Phase2 --> ResidentSet
    end

    subgraph Execution["4. Model and tool execution"]
        Prompt["Assemble fixed prompt + resident pages"]
        Model["Invoke model"]
        Tools["Harness mediates tool calls"]
        Updates["Capture new evidence, plans,<br/>preferences, and conversation"]

        Prompt --> Model --> Tools --> Updates
    end

    subgraph Observation["5. Observable fault and decision tracing"]
        FaultObserver["FaultObserver<br/>refetch, duplicate-tool, pinned miss,<br/>post-compaction bootstrap,<br/>silent recall, flush miss"]
        DecisionTrace["DecisionTrace<br/>append-only deterministic audit log"]
    end

    subgraph Lifecycle["6. Lifecycle-complete writeback"]
        Boundary{"Destructive lifecycle boundary?<br/>compaction, reset, pruning, shutdown"}
        Dirty["Collect dirty pages"]
        Stage["Stage typed operations<br/>append, merge, set-with-version"]
        Validate["Validate schema, provenance, scope,<br/>policy, and non-destructive semantics"]
        Valid{"Valid?"}
        Commit["Deterministic scoped commit<br/>to durable backing store"]
        Reject["Retain rejected journal entry<br/>with reason code"]
        Restore["Restore required Bootstrap and<br/>Constraint state after lifecycle event"]

        Boundary -- "Yes" --> Dirty --> Stage --> Validate --> Valid
        Valid -- "Yes" --> Commit --> Restore
        Valid -- "No" --> Reject
    end

    Event --> Adapter
    Full --> PageTable
    Compressed --> PageTable
    Structured --> PageTable
    Pointer --> PageTable

    Event --> Candidates
    PageTable --> Candidates
    ResidentSet --> Prompt
    Updates --> Adapter

    Event --> Boundary
    PageTable --> Dirty
    Commit --> PageTable
    Restore --> Candidates

    InvariantFault --> FaultObserver
    PageTable --> FaultObserver
    Model --> FaultObserver
    Tools --> FaultObserver
    Reject --> FaultObserver

    Phase1 --> DecisionTrace
    Phase2 --> DecisionTrace
    FaultObserver --> DecisionTrace
    Commit --> DecisionTrace
    Reject --> DecisionTrace
```

## Key distinction

- **Ingestion** creates or updates the available representations. It may parse,
  compress, count tokens, and store durable artifacts.
- **Selection** does not generate representations under budget pressure. It only
  chooses among variants already stored in the page table.
- **Phase 1** establishes the cheapest safe resident set.
- **Phase 2** spends the remaining token budget on higher-value fidelity.
- **Writeback** protects dirty state before a lifecycle operation can destroy
  the only current copy.

## Current C++ implementation boundary

Implemented or in progress:

- `PageType`, `Fidelity`, `PinClass`, and `Scope`
- `PageTypePolicy` and legal degradation paths
- `Page` representation validation and storage

Still to implement:

- minimum-viable representation lookup
- page ingestion and representation generation
- `SessionPageTable`
- two-phase `RepresentationSelector`
- `FaultObserver` and `DecisionTrace`
- `WritebackJournal`
- lifecycle orchestration in `ClawVMEngine`
