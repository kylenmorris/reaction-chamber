# Pathoscan Blink Firmware - AI Coding Agent Instructions

## Project Overview

**Pathoscan** is a diagnostic instrument firmware for the Raspberry Pi Pico 2W that automates DNA/RNA reaction testing. The system heats samples in tubes, runs timed reaction cycles, monitors optical sensors for results, and displays test status/history via terminal UI.

- **Target**: Raspberry Pi Pico 2W (ARM Cortex-M33, dual-core)
- **Language**: C11 with Pico SDK 2.2.0
- **Build**: CMake + Ninja
- **Architecture**: Core0 runs system logic; Core1 handles UI/buttons
- **Test Throughput**: Multiple tubes processed in parallel during reaction phases

## System Architecture

### Core Design: Dual-Core State Machine

```
Core0 (main):       State machine → sensor reads → heater/test control → 100Hz loop
Core1 (ui):         Button polling → display rendering → overlaps core0 reads safely
```

**State Machine**: `BOOT` → `IDLE` → `HEATING` → `REACTING` → `RESULTS` → `HISTORY` (from [system_state_loop.c](system_state_loop.c))

**Key Insight**: 
- Core0 writes `gSystemState`, `gTestStatus` (test per-tube tracking)
- Core1 reads these globals to display + polls buttons
- Single-writer pattern avoids mutexes entirely

### Module Organization

| Module | Purpose | Key Files |
|--------|---------|-----------|
| **Model** | Global state, data structures, pin/timing constants | [data_structs.h](model/data_structs.h), [constants.h](model/constants.h), [imodel_structs.h](model/imodel_structs.h) |
| **Logic** | State machine, controllers, test manager | [system_state_loop.c](system_state_loop.c), [button_ctrl.c](logic/button_ctrl.c), [heater_ctrl.c](logic/heater_ctrl.c), [temp_sens_ctrl.c](logic/temp_sens_ctrl.c), [tube_optical_ctrl.c](logic/tube_optical_ctrl.c), [tube_sens_ctrl.c](logic/tube_sens_ctrl.c), [display_ctrl.c](logic/display_ctrl.c), [test_manager.c](test_manager.c) |
| **HAL** | Hardware abstraction, drivers, board setup | [hal/drivers.c](hal/drivers.c), [hal/board_setup.c](hal/board_setup.c), [hal/screens_hw.c](hal/screens_hw.c) |
| **Simulation** | HAL replacements for testing without hardware | [hal_sim/drivers_sim.c](hal_sim/drivers_sim.c), [hal_sim/screens_sim.c](hal_sim/screens_sim.c) |

### Data Flow & Global State

1. **Input Phase (Core1)**: 
   - `button_ctrl_step()` → polls GPIO, debounces, writes `gButtonInput` struct
   - `display_ctrl_step()` → renders UI to terminal, reads globals

2. **Logic Phase (Core0)**:
   - `run_system_state_loop_core0()` processes state machine
   - Calls `temp_sens_ctrl_step()` → reads temp sensor, updates `gTestStatus`
   - Calls `heater_ctrl_step()` → PWM heater control based on temp target
   - Calls `tube_optical_ctrl_step()` → reads optical sensors, updates per-tube results
   - Calls `tube_sens_ctrl_step()` → monitors which tubes are inserted
   - `test_manager_step()` → handles test timing, state transitions, result classification

3. **Control Output (Core0)**:
   - Heater GPIO/PWM writes
   - Optical sensor polling
   - Test timing (reaction duration: configurable, typically 30 min)

## Key Patterns & Conventions

### 1. Global Extern Variables (Single Writer Pattern)
- `gSystemState` (SystemState enum): Written **only** in [system_state_loop.c](system_state_loop.c) — read everywhere
- `gTestStatus` (TestStatus array): Written **only** by [test_manager.c](test_manager.c) — read by display/state loop
- `gButtonInput` (ButtonInput struct): Written **only** in [button_ctrl.c](logic/button_ctrl.c) — read in system_state_loop.c
- `gSysControl` (SystemControl struct): Written **only** in [tube_sens_ctrl.c](logic/tube_sens_ctrl.c) — tracks inserted tubes
- **Rule**: Never create local static copies; always read fresh globals from their canonical sources

### 2. Timing & Constants
- **Loop Rate**: `SYSTEM_DELAY_MS = 10` (100 Hz, both cores)
- **Pin Assignments**: [constants.h](model/constants.h)
  - Buttons: GPIO 15-19 (UP, DOWN, SELECT, BACK, DEBUG)
  - Temperature sensor: I2C interface
  - Heater: GPIO with PWM control
  - Optical sensors: ADC channels for each tube
  - Tube detection: GPIO per tube
- **Test Parameters** (configurable in constants):
  - Reaction duration: 30+ minutes
  - Temperature setpoint: 61-64°C
  - Optical positive threshold: ~5000 (ADC units)
  - Debounce: 5-cycle window (50ms) for button noise filtering

### 3. State Transitions & Menu Structure
State transitions occur in [system_state_loop.c](system_state_loop.c#L20-L80). Each state manages an associated menu item struct from [imodel_structs.h](model/imodel_structs.h):

```c
switch (gSystemState) {
    case IDLE:
        if (button_is_pressed(SELECT)) {
            gSystemState = HEATING;
            gHeatingMenuIM.needs_redraw = true;  // Force immediate UI refresh
        }
        break;
    
    case HEATING:
        if (current_temp >= target_temp) {
            gSystemState = REACTING;
            gReactingMenuIM.needs_redraw = true;
            test_manager_start_reaction();
        }
        break;
    
    case REACTING:
        if (test_manager_is_complete()) {
            gSystemState = RESULTS;
            gResultsIM.needs_redraw = true;
        }
        break;
```

**Pattern**: Always set `needs_redraw = true` when entering a state to trigger immediate display refresh.

### 4. Display Rendering (Terminal-Based UI)
- Core1 calls `display_ctrl_step()` every 10ms in [main.c](main.c) via `run_system_state_loop_core1()`
- Checks `needs_redraw` flag **OR** elapsed time (500ms threshold)
- Reads current `gSystemState`, `gTestStatus`, `gButtonInput` to determine what to draw
- Direct `printf()` to serial—no buffering, no graphics library
- Screen-specific rendering logic in [logic/display_ctrl.c](logic/display_ctrl.c)

### 5. Hardware Controller Pattern
All hardware controllers follow the same init+step pattern (e.g., [button_ctrl.c](logic/button_ctrl.c), [heater_ctrl.c](logic/heater_ctrl.c)):

```c
// Initialization (called once in main or early startup)
void button_ctrl_init() {
    gpio_init(BUTTON_UP_GPIO);
    gpio_set_dir(BUTTON_UP_GPIO, GPIO_IN);
    // ... set pull-up, etc.
}

// Stepping (called every 10ms from core0 or core1)
void button_ctrl_step() {
    // Read GPIO, debounce, write to gButtonInput
    // Non-blocking; never sleep or I/O-wait
}
```

**Rules**:
- `_init()`: Configure GPIO, I2C, ADC, PWM—done once at startup
- `_step()`: Execute fast, non-blocking reads/writes every 10ms
- Write results to global state (`gTestStatus`, `gButtonInput`, etc.)
- Never call `sleep()` or blocking I/O in `_step()`

### 6. Test Manager (Test Sequencing)
[test_manager.c](test_manager.c) orchestrates multi-step reactions:
- `test_manager_init()`: Set up test state arrays
- `test_manager_step()`: Called from [system_state_loop.c](system_state_loop.c), updates `gTestStatus`
- Tracks per-tube state (EMPTY, RUNNING, COMPLETED, ERROR) and results (POSITIVE, NEGATIVE, INVALID)
- Manages reaction timer, classification thresholds, and result logging
- Writes results to persistent storage if configured

## Build & Deployment

### Build System
- **CMake**: [CMakeLists.txt](CMakeLists.txt) includes Pico SDK, configures compiler, links all source files
- **Compilation**: `ninja -C build` (incremental rebuild)
- **Clean Build**: `rm -rf build && mkdir build && cd build && cmake .. && ninja`

### Build Targets
```bash
ninja -C build              # Standard compile
```

### Flashing to Hardware
```bash
# Option 1: USB bootloader mode (hold BOOT button, plug in)
picotool load ./build/main.uf2 -fx

# Option 2: CMSIS-DAP debugger (openocd)
openocd -s ~/.pico-sdk/openocd/0.12.0+dev/scripts \
  -f interface/cmsis-dap.cfg \
  -f target/rp2350.cfg \
  -c "adapter speed 5000; program ./build/main.elf verify reset exit"
```

### Build Artifacts
- `build/main.elf`: Executable (for debugger)
- `build/main.uf2`: USB firmware (for bootloader)
- `build/main.dis`: Disassembly (for analysis)
- `compile_commands.json`: For clangd/IDE integration

## Configuration & Compilation Variables

[model/compilation_variables.h](model/compilation_variables.h) controls build behavior:
- `PRINT_DEBUG`: Enable debug logging
- `MASTER_SIMULATION_MODE`: Use simulated HAL instead of real hardware
- `USE_HW_PICO`, `USE_HW_BUTTONS`, `USE_HW_HEATER`, `USE_HW_TEMP_SENS`, `USE_HW_OPTICAL`, `USE_HW_TUBE_SENS`: Toggle individual hardware drivers
- `IS_SIMULATING`: Read-only flag set based on above

Modify before build to change behavior (no recompile needed for feature flags if they're set at compile time).

## Common Development Tasks

### Adding a New Button or GPIO Input
1. Add pin definition in [model/constants.h](model/constants.h):
   ```c
   #define NEW_BUTTON_GPIO_PIN 20
   ```
2. Add button enum to [model/data_structs.h](model/data_structs.h) ButtonType if new type
3. Update [logic/button_ctrl.c](logic/button_ctrl.c):
   - Initialize in `button_ctrl_init()` with `gpio_init()`, `gpio_set_dir()`, pull-ups
   - Poll in `button_ctrl_step()`, debounce, write to `gButtonInput`
4. Handle in [system_state_loop.c](system_state_loop.c) state cases using `button_is_pressed()` helper

### Modifying Display Output
1. Check current state rendering in [logic/display_ctrl.c](logic/display_ctrl.c)
2. Update text, layout, or conditionals based on `gSystemState`, `gTestStatus`, menu flags
3. Set `needs_redraw = true` in [system_state_loop.c](system_state_loop.c) when state transitions to force immediate UI
4. Rebuild and test on Pico

### Adding a New Hardware Controller (Sensor/Actuator)
1. Create `logic/new_ctrl.c` with:
   ```c
   void new_ctrl_init() { /* GPIO setup */ }
   void new_ctrl_step() { /* Read/write, update globals */ }
   ```
2. Create `include_ctrl/new_ctrl.h` with function declarations
3. Include header in [system_state_loop.c](system_state_loop.c)
4. Call `new_ctrl_init()` in [main.c](main.c) startup
5. Call `new_ctrl_step()` in appropriate place in [system_state_loop.c](system_state_loop.c)
6. Update [CMakeLists.txt](CMakeLists.txt) to link new source file
7. If hardware-specific, add board setup to [hal/board_setup.c](hal/board_setup.c) or [hal/drivers.c](hal/drivers.c)

### Changing Test Timing or Optical Thresholds
1. Edit constants in [model/constants.h](model/constants.h):
   - Reaction duration, temperature setpoint, optical threshold
2. Rebuild: `ninja -C build`
3. Flash and test

### Simulating Without Hardware
1. Set `MASTER_SIMULATION_MODE` in [model/compilation_variables.h](model/compilation_variables.h)
2. Rebuild: `ninja -C build`
3. Simulated HAL in [hal_sim/](hal_sim/) provides mock sensor/heater outputs
4. Useful for UI/logic testing without real tubes

## Embedded-Specific Gotchas

- **Include Order**: `pico/stdlib.h` required despite clangd warnings (needed for sleep_ms, etc.)
- **Memory Constraints**: Pico 2W has 520KB SRAM; avoid large buffers; terminal UI minimizes footprint
- **Multicore Safety**: Core0 owns state writes, Core1 reads—no mutexes needed (single-writer pattern)
- **Timing Criticality**: 10ms loop is tight; any >10ms operation blocks entire system
- **No Interrupts in ISRs**: Avoid printf/I/O from interrupt handlers; use flags instead
- **GPIO Pin Conflicts**: Always check [model/constants.h](model/constants.h) for already-assigned pins before adding new ones
- **PWM/ADC Setup**: Ensure GPIO config in HAL matches controller expectations; duplicate init = hard errors
- **I2C/SPI Bus Sharing**: Temperature sensor and optional display controllers may share buses; verify [hal/board_setup.c](hal/board_setup.c)

## Testing & Debugging

### On-Device Testing
1. Connect Pico 2W via USB (provides serial console)
2. Build and flash: `ninja -C build && picotool load ./build/main.elf -fx`
3. Monitor serial output: Use PuTTY, minicom, or Arduino Serial Monitor at 115200 baud
4. Verify state transitions, temperature readings, optical sensor values

### Debug Output
- `DEBUG_LOG()` macro in [model/constants.h](model/constants.h) (enabled via `PRINT_DEBUG` flag)
- State/status values printed to serial for inspection
- Use GPIO toggle for timing analysis (scope-friendly)

### Simulation Mode
- `MASTER_SIMULATION_MODE` in [model/compilation_variables.h](model/compilation_variables.h)
- Mocked HAL in [hal_sim/](hal_sim/) simulates sensor readings, heater, optical results
- Run on PC or Pico without real hardware present

### No Unit Tests
Firmware is hardware-dependent by design. Primary validation:
- Manual testing on actual Pico 2W board
- Functional verification of state machine + sensor/heater control
- Result classification accuracy against known samples

## File Structure at a Glance

```
.github/copilot-instructions.md     (This file)
CMakeLists.txt                       (Build config)
main.c                               (Core0/Core1 startup)
system_state_loop.c/.h               (State machine, core0 logic loop)
test_manager.c/.h                    (Test sequencing & result classification)

model/
  data_structs.h                     (Global state enums/structs)
  constants.h                        (Pin assignments, timing, thresholds)
  imodel_structs.h                   (Menu/UI state per state)
  compilation_variables.h            (Feature flags)

logic/
  button_ctrl.c/.h                   (Button input, debouncing)
  heater_ctrl.c/.h                   (Temperature control, PWM)
  temp_sens_ctrl.c/.h                (Temperature sensor reading)
  tube_optical_ctrl.c/.h             (Optical result detection)
  tube_sens_ctrl.c/.h                (Tube presence detection)
  display_ctrl.c/.h                  (Terminal UI rendering)

hal/
  board_setup.c                      (GPIO/I2C/SPI initialization)
  drivers.c/.h                       (Hardware-specific implementations)
  screens_hw.c/.h                    (Display/terminal setup)

hal_sim/
  drivers_sim.c                      (Mock HAL for simulation)
  screens_sim.c                      (Mock display)

include/
  drivers.h                          (HAL interface declarations)
  screens_interface.h

include_ctrl/
  *.h                                (Controller headers)

lib/
  glcd-master/                       (Graphics library, optional)

build/
  (Compiled artifacts: main.elf, main.uf2, compile_commands.json, etc.)
```

## Recent Project Status

- **Active Development**: State machine, test manager, multi-tube orchestration
- **Core Features**: Heating control, optical sensing, result classification, terminal UI
- **Known Areas**: Sensor calibration, result thresholds, error handling for edge cases

---

**Last Updated**: 2026-01-23  
**Hardware Target**: Raspberry Pi Pico 2W  
**SDK Version**: Pico SDK 2.2.0  
**C Standard**: C11  
**Status**: Active Development
