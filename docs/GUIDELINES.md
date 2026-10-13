# BioChroma Implementation Guidelines & Step-by-Step Roadmap

> Practical developer roadmap and modular task breakdown for implementing `BioChroma`.

---

## Phase 1: Build Infrastructure & Core Algebra (v0.1)

- [x] **Step 1.1 — Build System & Directory Setup**
  - Create the root `Makefile` wrapper (`all`, `build`, `test`, `clean`).
  - Add standard `.gitignore` rules for CMake build directories, generated binaries, and data outputs.
  - Verify that a minimal `src/main.cpp` compiles and executes correctly.

- [x] **Step 1.2 — Basic Types & Vector Arithmetic**
  - Implement `include/Types.hpp` containing `BacteriumState` enum class and `Vector2` struct.
  - Implement continuous vector operator overloads: `operator+`, `operator*`, and equality checks.
  - Set up `tests/test_types.cpp` to validate vector algebra and boundary conditions.

- [x] **Step 1.3 — Dynamic Memory Grid Management**
  - Implement the `Grid` class managing a dynamically allocated continuous buffer (`float* nutrients` via `new[]` and `delete[]`).
  - Strictly enforce canonical class form: deep-copy copy constructor, copy assignment `operator=`, and destructor.
  - Overload `operator()(int x, int y)` (both mutable reference and `const` value accessors).
  - Write `tests/test_grid.cpp` to verify deep copying and memory safety using AddressSanitizer.

---

## Phase 2: Physical Modeling & Agent Hierarchy (v0.1)

- [x] **Step 2.1 — 2D Finite-Difference Diffusion**
  - Implement continuous-to-discrete spatial mapping functions (`Vector2` to grid coordinates).
  - Implement diffusion using an explicit 5-point Laplacian stencil.
  - Enforce boundary reflections (Neumann conditions) or closed dish constraints.
  - Add unit tests verifying global nutrient mass conservation across successive diffusion steps.
  
- [x] **Step 2.2 — Core Bacterium Dynamics**
  - Implement base `Bacterium` agent with randomized step motility and metabolic intake.
  - Implement Markov state transitions (`ALIVE`, `DIVISION`, `DEAD`, `SPORE`).
  - Implement cloning mechanism (`clone()`) with energy splitting upon binary fission.
  - Add unit tests validating motility, cloning, and state stability.

- [x] **Step 2.3 — Headless Simulation Orchestration**
  - Implement `Simulation` managing the agent collection via dynamic heap array (`Bacterium**`).
  - Implement swap-and-pop memory pruning (`delete_dead_bacteria()`) in $\mathcal{O}(1)$.
  - Implement the core temporal step (`step()`): nutrient diffusion, agent updates, intake, division, and dead cleanup.
  - Implement ANSI terminal renderer (`display()`) supporting 256-color palettes for nutrient levels and agent states.

---

## Phase 3: Telemetry, I/O & Verification (v0.1 -> v0.2)

- [x] **Step 3.1 — ASCII Configuration Parser**
  - Implement the `Simulation(const std::string& filepath)` constructor.
  - Parse metadata headers (`N`, `CELL_SIZE`, `DIFFUSION`).
  - Parse multi-grid matrix sections (`--- NUTRIENTS ---` and `--- BACTERIA ---`).
  - Ensure robust bounds validation and heap initialization before population injection.
- [ ] **Step 3.2 — Binary Logging & CSV Conversion**
  - Implement `Logger` to stream binary snapshots (`BacteriumRecord`, `NutrientSnapshot`) per timestep.
  - Implement the post-simulation conversion step generating clean, normalized CSV files (`trajectories.csv`, `nutrients.csv`).
  - Declare `friend class Logger;` inside `Simulation` to allow zero-copy serialization.

- [ ] **Step 3.3 — Python Validation Scripts**
  - Write `scripts/plot_population.py` using Pandas and Matplotlib to plot population curves over time.
  - Write `scripts/plot_heatmap.py` to animate nutrient field consumption alongside agent trajectories.
