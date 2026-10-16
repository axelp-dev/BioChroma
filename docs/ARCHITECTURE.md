# BioChroma Pipeline and Architecture Description

> This document describes the system architecture, class relationships, and data pipeline of the project.

## Table of Contents

- [BioChroma Pipeline and Architecture Description](#biochroma-pipeline-and-architecture-description)
  - [Table of Contents](#table-of-contents)
  - [1. Project Overview](#1-project-overview)
  - [2. Architecture \& Class Diagrams](#2-architecture--class-diagrams)
    - [2.1 Data Structures](#21-data-structures)
    - [2.2 Class Diagram](#22-class-diagram)
  - [3. Data Pipeline](#3-data-pipeline)

---

## 1. Project Overview

This project is developed as an exploratory C++ portfolio project focused on scientific computing and software design. The primary goal is to implement complex Markov chain processes for multiple bacteria simulation in a virtual Petri dish.

Bacterial agents move, consume resources, undergo cell division, and interact locally. Their individual and collective evolution depends on:
- Nutrient availability across the continuous domain.
- Local population density (competition for resources and physical exclusion).
- Stochastic parameter mutations occurring during cell division.

Each bacterium is governed by an independent Markov process controlling state transitions (active, dividing, spore, or dead) and behavioral parameters (velocity, metabolic rate, chemotactic bias).

---

## 2. Architecture & Class Diagrams

Here are presented all the structures and classes used in the project.

### 2.1 Data Structures

**Cell States.** First, we need to define some basic structures. `BacteriumState` defines the current state of a `Bacterium` object: `DIVISION` means that the cell is dividing itself into two parts to produce two clones, and `SPORE` is an available state for `StandardBacterium` objects reached when nearby resources are too low.

**The Grid.** The Petri dish is modeled by an $N \times N$ discrete grid stored in a dynamically allocated `float` array (`nutrients`). This grid represents the local concentration of nutrients (normalized values between `0.0` and `9.0`), where `-1.0` encodes impenetrable obstacles (`#`). It is initialized directly by the `Simulation` parser from an ASCII configuration file.

> **NOTE**: Bacterial agents are not stored inside the grid cells. The `Simulation` orchestrator manages them on the heap through a dynamic pointer array (`Bacterium** bacteria`) using a Swap & Pop pattern for $\mathcal{O}(1)$ deletions. Continuous spatial coordinates ($\mathbb{R}^2$) are tracked via `Vector2`.

```cpp
// Internal states of a bacterium
enum class BacteriumState : uint8_t {
    ALIVE,      // Active agent: moves, feeds, metabolizes
    DEAD,       // Inactive agent scheduled for cleanup
    DIVISION,   // Undergoing binary fission
    SPORE       // Dormant state under severe nutrient scarcity
};
```

**Modular Subsystems.** The code base is organized into decoupled domain modules:
- `core/`: Fundamental math structures (`Vector2`), data containers (`Queue<T>`), and common types (`Types.h`).
- `entities/`: Autonomous biological agents (`Bacterium`, `StandardBacterium`, `ChemotacticBacterium`).
- `simulation/`: Environment simulation (`Grid`), run configurations (`SimulationConfig`), and engine orchestrator (`Simulation`).
- `io/`: CLI argument parsing (`CmdParser`) and telemetry logging (`Logger`).
- `stats/`: Telemetry collector (`Stats`) and lineage data structures (`BacteriumRecord`).

**Generic FIFO Queue.** A standalone dynamic singly linked list `Queue<T>` (defined in `include/core/Queue.h`) handles sequential event pipelines, generation tracking, and telemetry buffers with strict $\mathcal{O}(1)$ push/pop guarantees.

### 2.2 Class Diagram

**Simulation Objects.** Below are represented all the simulation classes and their inheritance and usage relationships. The main class is `Simulation`, which orchestrates the simulation steps, holding the environment `PetriDish` and managing all polymorphic `Bacterium` instances.

**Bacterium Types.** Two concrete `Bacterium` child classes are implemented:
- `StandardBacterium`: A basic bacterium walking randomly on the continuous domain, capable of entering a `SPORE` dormant state when local nutrient concentrations drop below a critical threshold.
- `ChemotacticBacterium`: Cannot enter spore state, but moves along the local nutrient gradient (chemotaxis) with higher motility.

```mermaid 
classDiagram
    namespace core {
        class Vector2 {
            +float x
            +float y
            +operator+(const Vector2& other) const Vector2
            +operator*(float scalar) const Vector2
        }
        class Queue~T~ {
            -Node* head
            -Node* tail
            -size_t size_
            +push(const T& value) void
            +pop() void
            +front() T&
            +empty() const bool
            +clear() void
        }
    }

    namespace stats {
        class BacteriumRecord {
            +uint32_t id
            +uint32_t parent_id
            +uint32_t birth_step
            +uint32_t death_step
            +float max_energy_reached
            +BacteriumRecord(uint32_t, uint32_t, uint32_t)
        }
        class Stats {
            -Queue~BacteriumRecord~* queue
            -string output_folder
            -SimulationConfig config
            +Stats(const SimulationConfig&, string)
            +insert(const BacteriumRecord& record) void
            +save() void
        }
    }

    namespace entities {
        class Bacterium {
            <<abstract>>
            #Vector2 position
            #BacteriumState state
            #float energy
            #float velocity
            #float mutation_prob
            #BacteriumRecord* record
            +virtual update(float dt, float local_nutrient)* void
            +virtual clone()* Bacterium*
            +get_record() BacteriumRecord*
        }

        class StandardBacterium {
            +update(float dt, float local_nutrient) override void
            +clone() override Bacterium*
        }

        class ChemotacticBacterium {
            -float sensing_radius
            +update(float dt, float local_nutrient) override void
            +clone() override Bacterium*
        }
    }

    namespace simulation {
        class SimulationConfig {
            +string grid_filepath
            +int total_steps
            +float dt
            +bool headless
            +bool real_time
            +float display_delay
            +uint32_t seed
            +string export_csv
            +bool save_simulation
            +SimulationConfig(..., bool save_simulation)
        }

        class Grid {
            -float* nutrients
            +diffuse(float dt, float diffusion_coeff) void
            +get_nutrient_at(Vector2 pos) const float
            +consume_nutrient_at(Vector2 pos, float amount) void
        }

        class Simulation {
            -Bacterium** bacteria
            -int n_bacteria
            -Grid* grid
            -SimulationConfig config
            -Stats* stats
            +step() void
            +launch() void
            +delete_dead_bacteria() void
        }
    }

    Bacterium <|-- StandardBacterium
    Bacterium <|-- ChemotacticBacterium
    Bacterium *-- Vector2
    Bacterium *-- BacteriumRecord : tracks
    Stats *-- BacteriumRecord : buffers
    Stats *-- Queue~BacteriumRecord~ : uses
    Simulation *-- Grid : owns
    Simulation *-- SimulationConfig : holds
    Simulation *-- Stats : owns
    Simulation o-- Bacterium : manages (Bacterium**)
```

---

## 3. Data Pipeline

The simulation runs through a discrete-time execution pipeline:
1. **Initialization**: The configuration file is parsed by `Simulation(filepath)`, allocating the internal `Grid*` buffer, the `Stats*` module, and initial `Bacterium*` heap pointers.
2. **Temporal Step** (`Simulation::step()`):
   - **2.1 Diffusion**: Nutrient diffusion runs across the grid via explicit finite differences (`grid->diffuse(dt, diffusion_coeff)`), preserving obstacle boundaries.
   - **2.2 Nutrient Intake & Metabolic Update**: Each active bacterium consumes available local nutrients (`grid->consume_nutrient_at`) and executes `b->update(dt, nut)`. The bacterium updates its internal `BacteriumRecord` with its new energy peak.
   - **2.3 Fission**: Cells flagged with `BacteriumState::DIVISION` spawn child instances via `b->clone()`, receiving a unique `next_bacterium_id` and passing their current ID as `parent_id`.
   - **2.4 Population Pruning**: `delete_dead_bacteria()` isolates inactive agents. Before `delete` is called, the simulation extracts the `BacteriumRecord`, assigns the exact `death_step`, and pushes it to the `Stats` queue. The pointer is then removed in $\mathcal{O}(1)$ using Swap & Pop.
3. **Visualization & Telemetry**: `Simulation::display()` prints ANSI-colored terminal views. Upon termination, `Stats::save()` pops all accumulated records to serialize the complete lineage into a CSV format.

```mermaid 
flowchart TD
    Config[(CLI Flags / ASCII .txt)] -->|Init Params| Init[Simulation Initialization]

    subgraph EngineLoop [Temporal Simulation Loop: step dt]
        Init --> Diffuse[1. Nutrient Diffusion: 5-point stencil]
        Diffuse --> Markov[2. Random Walk & Markov Transitions]
        Markov --> Consume[3. Nutrient Uptake & Energy Consumption]
        Consume --> Events{Division or Death?}
        Events -->|Yes, Division| Clone[Clone agent & Init new BacteriumRecord]
        Events -->|Yes, Death| FinalizeRecord[Add death_step & Push to Stats Queue]
        FinalizeRecord --> DeletePop[Delete agent & Swap/Pop array]
        Clone --> CheckEnd
        DeletePop --> CheckEnd
        Events -->|No| CheckEnd
    end

    CheckEnd{t < total_steps?}
    CheckEnd -->|Yes| NextStep[t = t + dt]
    NextStep --> Diffuse

    CheckEnd -->|No| ExportCSV[("Stats::save() CSV Export")]
    ExportCSV -.->|Python Analysis| PyPlots[Lineage Plots & Metrics]
```

