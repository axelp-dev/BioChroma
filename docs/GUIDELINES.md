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

- [ ] **Step 2.1 — 2D Finite-Difference Diffusion**
  - Implement continuous-to-discrete spatial mapping functions (`Vector2` to grid coordinates).
  - Implement `PetriDish::diffuse(float dt)` using an explicit 5-point Laplacian stencil.
  - Enforce boundary reflections (Neumann conditions) or closed dish constraints.
  - Add unit tests verifying global nutrient mass conservation across successive diffusion steps.

- [ ] **Step 2.2 — Polymorphic Bacterium Hierarchy**
  - Define the abstract base class `Bacterium` with pure virtual methods (`update`, `clone`) and a virtual destructor.
  - Implement canonical form (copy constructor, copy assignment operator) for the abstract base class.
  - Implement `StandardBacterium`: 2D isotropic random walk, baseline metabolic consumption, and transition to `SPORE` upon local starvation.
  - Implement `ChemotacticBacterium`: local gradient sensing, biased directional walk, and distinct velocity/metabolic parameters.
  - Write unit tests in `tests/test_bacterium.cpp` validating dynamic dispatch via `Bacterium*` and virtual cloning.

- [ ] **Step 2.3 — Headless Simulation Orchestration**
  - Implement `Simulation` managing the polymorphic collection (`std::vector<Bacterium*>`).
  - Implement the core temporal execution step:
    1. Diffuse environmental nutrients via `PetriDish::diffuse(dt)`.
    2. Update agents via polymorphic calls (`b->update(dt, local_nutrient)`).
    3. Process divisions via `b->clone()` and append new agents to the population vector.
    4. Safely clean up dead agents (`delete` raw pointers followed by `std::erase_if`).
  - Implement the canonical class form for `Simulation` ensuring deep copies of all heap-allocated agents.

---

## Phase 3: Telemetry, I/O & Verification (v0.1 -> v0.2)

- [ ] **Step 3.1 — ASCII Configuration Parser**
  - Implement the `PetriDish(const std::string& filepath)` constructor.
  - Parse ASCII maps: decode nutrient levels (`0`–`9`), obstacles (`#`), and initial colonies (`S`, `C`).
  - Instantiate and inject initial polymorphic `Bacterium` pointers based on the map characters.

- [ ] **Step 3.2 — Binary Logging & CSV Conversion**
  - Implement `Logger` to stream binary snapshots (`BacteriumRecord`, `NutrientSnapshot`) per timestep.
  - Implement the post-simulation conversion step generating clean, normalized CSV files (`trajectories.csv`, `nutrients.csv`).
  - Declare `friend class Logger;` inside `Simulation` to allow zero-copy serialization.

- [ ] **Step 3.3 — Python Validation Scripts**
  - Write `scripts/plot_population.py` using Pandas and Matplotlib to plot population curves over time.
  - Write `scripts/plot_heatmap.py` to animate nutrient field consumption alongside agent trajectories.

---

## Phase 4: Interactive Viewport (v0.2)

- [ ] **Step 4.1 — Raylib Build Integration**
  - Link Raylib in `CMakeLists.txt` via `find_package(raylib REQUIRED)` with fallback support.
- [ ] **Step 4.2 — 2D Rendering Engine**
  - Implement real-time nutrient grid rendering as a background color heatmap.
  - Render bacteria as colored discs (e.g. green for `Standard`, cyan for `Chemotactic`, grey for `SPORE`).
- [ ] **Step 4.3 — Interactive Controls**
  - Add keyboard toggles (spacebar pause, stepped mode, speed control).
  - Add mouse interaction to seed nutrients or spawn new bacterial colonies dynamically.