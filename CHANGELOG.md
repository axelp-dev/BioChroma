# BioChroma Changelogs 

Here are listed all the project changes from the initial commit to the last Unreleased version. 


## [Unreleased]

### Added
- `Simulation::wrap_position` for `Bacterium` random walks and bouncing for walls. 
- Integrated `CmdParser` header-only library to provide a structured CLI interface for simulation execution.
- Added CLI arguments with full validation and formatted `--help` output:
  - `-g, --grid_filepath` (required): path to the grid scenario file.
  - `-d, --duration`: total simulation steps (default: 50).
  - `-t, --dt`: time step duration in seconds (default: 1.0).
  - `-v, --verbose`: toggle terminal ANSI rendering (default: true).
  - `-s, --seed`: pseudo-random number generator seed for reproducibility.
- Wired CLI options directly to `SimulationConfig` instantiation in `main.cpp`.
- Introduce `SimulationConfig` structure encapsulating simulation execution parameters (`grid_filepath`, `total_steps`, `dt`, `headless`, `seed`, `export_csv`).
- Add unit test suite for configuration handling (`tests/test_simulation_config.cpp`) covering default state, custom constructors, copy semantics, and engine injection.
- Add `get_config()` accessor to `Simulation` for state inspection and testing.

### Changed
- Update `Simulation` constructors to accept and propagate `SimulationConfig` instances instead of unbundled parameter lists.
- Refactor `Simulation(const std::string& filepath)` as a delegating constructor forwarding to `SimulationConfig`.
- Default `total_steps` argument to 50 in `Simulation(Grid&, float, int)`.

### Fixed
- Fix stack-allocated bacterium pointer injection in `tests/test_simulation.cpp` by heap-allocating test instances.
- Fix compiler warning `-Wdeprecated-copy` on `SimulationConfig` by explicitly defaulting copy assignment operator `operator=`.
- Fix constructor overload resolution errors during `Simulation` test initialization.

### Deprecated 

### Removed 


---

## [v0.1.0] - 2026-10-13

### Added 
- Grid config files examples in `data/grid-examples/`. 
- Comprehensive test suite for `Simulation` (`tests/test_simulation.cpp`) covering invalid files, ASCII parsing, and dynamic capacity growth.
- ANSI terminal visualizer with 256-color palette for nutrient density gradients and bacterial agent states.
- ASCII map scenario parser supporting multi-grid section headers (`--- NUTRIENTS ---`, `--- BACTERIA ---`).
- `Grid.h` and `Grid.cpp` implementation of the `N` times `N` nutrients grid. Also build unit tests for this class (in `tests/` and update `Makefile` with new modules). 
- `Vector2.h` and `Vector2.cpp` implementation of $\mathbb{R}^2$ basic vectors calculus. 
- `TestRunner` definition and units tests for `Vector2` class. 
- `.gitignore` file containing `obj/` and `bin/` folders. 
- `Makefile` with differents rules (`all`, `run` and `clean`) for quick project management en tests. 
- Comprehensive implementation roadmap and engineering guidelines (`docs/GUIDELINES.md`).
- Clarified polymorphic bacterial hierarchy (`StandardBacterium`, `ChemotacticBacterium`) in design specs.
- Canonical class form specifications and raw memory management design for `Grid`.
- Decoupled telemetry pipeline architecture (binary snapshots and twin CSV exports).
- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) : Write a minimal architecture class diagram and pipeline for first code implementations. 
- [`README.md`](README.md) : Write a first version. 
- Add basic project structure folders and compilation tools (`Makefile` and `CMakeLists.txt`). 

### Changed 
- `Simulation`: Change `Bacterium* bacteria` array in the class to a pointers one `Bacterium** bacteria` to store polymorphic objects (`ChemotacticBacterium` or `StandardBacterium`). 

### Fixed 
- Fixed stack-use-after-scope memory corruption in `Simulation(filepath)` by heap-allocating initial `Bacterium` pointers.
- Fixed `bad-free` runtime failure in `Simulation::~Simulation()` by taking deep ownership of input grid references.
- Fixed out-of-bounds nutrient indexing caused by shared line counters during configuration parsing.
- Fixed grid underflow exceptions during consumption by evaluating coordinates post-movement and enforcing safe clamp checks.
- Fixed const-correctness on `Grid::is_obstacle()` and `Grid::get_n()`.