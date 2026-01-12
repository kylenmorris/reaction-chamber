# Pathoscan Blink Firmware - AI Coding Agent Instructions

## Project Overview

**Pathoscan** is a diagnostic instrument firmware for the Raspberry Pi Pico 2W that automates DNA/RNA reaction testing. The system heats, runs reactions, monitors optical sensors, and displays results via terminal UI.

- **Target**: Raspberry Pi Pico 2W (ARM Cortex-M33)
- **Language**: C11 with Pico SDK
- **Build**: CMake + Ninja
- **Key Feature**: Dual-core architecture (core0: main logic, core1: UI/buttons)

## Architecture

### Core Design Pattern: Multi-State Machine with Global State

The system uses a global **SystemState enum** (`IDLE` → `HEATING` → `REACTING` → `RESULTS` → `HISTORY`) defined in [model/data_structs.h](model/data_structs.h). State transitions occur in [system_state_loop.c](system_state_loop.c#L13), which runs on core0 every 10ms.

```
Core0 (main):     State machine logic → Sensor/heater control → 100Hz loop
Core1:            Button polling + display rendering (overlaps core0 writes carefully)
```

**Critical Pattern**: Each controller module provides `_init()` and `_step()` functions. Initialization happens in `main()`, stepping happens in [system_state_loop.c](system_state_loop.c).

### Module Organization

| Module | Purpose | Key Files |
|--------|---------|-----------|
| **Model** | Global state, shared structs, constants | [data_structs.h](model/data_structs.h), [constants.h](model/constants.h), [imodel_structs.h](model/imodel_structs.h) |
| **Controller** | Hardware abstraction (buttons, temp sensor, heater, optics, tests) | `controller/*.c` files with `_init()` and `_step()` |
| **Display** | UI rendering and screen state | [display_ctrl.c](display/display_ctrl.c), [screens.c](display/screens.c) |
| **Helper** | LED utilities | `helper/pico_led.c` |

### Data Flow

1. **Input**: `button_ctrl_step()` → updates `gButtonInput` struct (core1)
2. **Logic**: `run_system_state_loop()` → reads inputs, updates `gSystemState` and `gTestStatus` (core0)
3. **Control**: `temp_sens_ctrl_step()`, `heater_ctrl_step()`, `tube_optical_ctrl_step()` → hardware writes (core0)
4. **Output**: `display_ctrl_draw()` → reads global state, renders to terminal (core1)

## Key Patterns & Conventions

### 1. Global Extern Variables (Single Writers Pattern)
- `gSystemState`: written only in [system_state_loop.c](system_state_loop.c), read everywhere
- `gTestStatus`: written by test controllers, read by display/state loop
- `gButtonInput`: written only in `button_ctrl.c`, read in system_state_loop.c
- **Never create local static copies**; always read fresh from globals

### 2. Timing & Constants
- All timing is in milliseconds: `SYSTEM_DELAY_MS = 10` (100 Hz loop)
- Pin assignments in [constants.h](model/constants.h) (buttons: GPIO 15-19, debug pin 15)
- Reaction duration: 30 minutes; temp target 61-64°C; optical threshold: 5000
- **Debounce**: 5-cycle window to filter button noise

### 3. State Transitions
Example from [system_state_loop.c](system_state_loop.c#L20-L45):
```c
switch (gSystemState) {
    case IDLE:
        if (gButtonInput.wasPressed && gButtonInput.lastPressed == SELECT) {
            gSystemState = HEATING;  // Only place state changes
            gHeatingMenuIM.needs_redraw = true;
        }
```
**Pattern**: Set `needs_redraw = true` when entering a state to force immediate UI refresh.

### 4. Display Rendering (Terminal Only, No Graphics Library)
- Core1 calls `display_ctrl_draw()` every 10ms
- Checks `needs_redraw` flag OR elapsed time (500ms threshold)
- Calls screen-specific functions in [screens.c](display/screens.c)
- **No buffering/double-buffering**; direct printf to serial

### 5. Hardware Controller Pattern
Each controller (`temp_sens_ctrl.c`, `heater_ctrl.c`, etc.):
- `_init()`: configure GPIO pins, set defaults
- `_step()`: called every 10ms, reads sensors → updates global state
- **Never block**; all I/O is non-blocking or pre-initialized

## Build & Deployment

### Build Commands
```bash
ninja -C build              # Compile (uses build.ninja)
picotool load <binary> -fx  # USB flash (if in bootloader mode)
openocd -c "program ..."    # CMSIS-DAP flash (with debugger)
```

### File Structure
- [CMakeLists.txt](CMakeLists.txt): Links all `.c` files, includes Pico SDK
- [main.c](main.c): Initialization + core1 launch
- Build artifacts: `build/main.uf2` (USB firmware) or `build/main.dis` (disassembly)

## Common Development Tasks

### Adding a New Button Input
1. Add pin in [constants.h](model/constants.h) (e.g., `#define NEW_BUTTON_GPIO_PIN 20`)
2. Initialize in `button_ctrl_init()` with `gpio_init()`, `gpio_set_dir()`
3. Poll in `button_ctrl_step()`, update `gButtonInput` struct
4. Handle in [system_state_loop.c](system_state_loop.c) state cases

### Modifying Display Output
1. Edit screen function in [screens.c](display/screens.c) (e.g., `terminal_draw_idle_menu()`)
2. Set `needs_redraw = true` in [system_state_loop.c](system_state_loop.c) when state enters
3. Display calls `display_ctrl_draw()` → checks flag → renders

### Adding a New Hardware Controller
1. Create `controller/new_ctrl.c` + `include/new_ctrl.h`
2. Implement `new_ctrl_init()` and `new_ctrl_step()`
3. Add to [CMakeLists.txt](CMakeLists.txt) in `add_executable()` sources
4. Call `new_ctrl_init()` in [main.c](main.c), `new_ctrl_step()` in [system_state_loop.c](system_state_loop.c)

## Embedded-Specific Gotchas

- **Clangd Warning Suppression**: `pico/stdlib.h` is required despite linting warnings (see [main.c](main.c#L2))
- **Memory Constraints**: Pico 2W has 520KB SRAM; terminal UI is lightweight by design
- **Multicore Safety**: Core0 writes state, core1 reads—no mutexes (single writer pattern)
- **Timing**: Loop runs every 10ms; anything taking >10ms delays the entire system
- **No Printf in ISRs**: If adding interrupts, avoid I/O from interrupt context

## Testing Strategy

- **Manual Hardware Testing**: Use LED indicator (`pico_set_led()`) for debug signals
- **State Verification**: Print `gSystemState` and `gTestStatus` values to serial
- **No Unit Tests**: Firmware is hardware-dependent; test on actual Pico board

---

**Last Updated**: 2026-01-08  
**Hardware**: Raspberry Pi Pico 2W  
**SDK Version**: 2.2.0
