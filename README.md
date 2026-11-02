# Educational Operating System Kernel — C & RISC-V

An educational operating system kernel developed in **C**, targeting the **RISC-V 64-bit architecture**, as part of the Embedded Systems and Connected Devices (SEOC) engineering program at Grenoble INP – Phelma.

The project consisted of extending a provided kernel skeleton by implementing fundamental operating system mechanisms, including hardware timer interrupts, process scheduling, context switching, and process lifecycle management.

## Project Overview

The objective was to understand how an operating system manages execution at a low level, from handling hardware interrupts to coordinating multiple processes.

The kernel combines C programming with assembly routines for hardware-specific operations and runs in a RISC-V environment using QEMU.

The main areas of development were:

- Console output and cursor management.
- System clock and timer interrupts.
- Round-robin process scheduling.
- Process creation and lifecycle management.
- Process sleeping and waking.
- Context switching between processes.

## Main Features

### 1. System Clock & Timer Interrupts

Implemented timer management in `horloge.c`, including:

- Configuration of RISC-V timer interrupts.
- Interrupt handling through a dedicated trap handler.
- Periodic timer rearming using the CLINT timer registers.
- Internal time tracking.
- Console clock display in `HH:MM:SS` format.
- Integration of timer interrupts with the process scheduler.

The timer handler updates the system clock, manages sleeping processes, and invokes the scheduler.

### 2. Round-Robin Process Scheduler

Implemented a process scheduler in `ordonnanceur.c`.

The scheduler:

- Maintains a table of processes.
- Selects the next runnable process in circular order.
- Skips sleeping and terminated processes.
- Updates process states during scheduling.
- Switches execution contexts using the assembly routine `ctx_sw()`.
- Falls back to the idle process when no other process is runnable.

Scheduling is triggered by timer interrupts, enabling time-based switching between processes.

### 3. Process Lifecycle Management

Processes are represented by a structure containing:

- Process identifier (PID).
- Process name.
- Execution state.
- Saved register context.
- Dedicated stack.
- Remaining sleep duration.

Four execution states are defined:

| State | Description |
|---|---|
| `ELU` | Currently running |
| `ACTIVABLE` | Ready to execute |
| `ENDORMI` | Sleeping |
| `MORT` | Terminated |

The project includes mechanisms for process creation, termination, sleeping, and waking.

### 4. Process Sleeping & Waking

The `dors()` function allows a running process to sleep for a specified duration.

When a process goes to sleep:

1. Its state changes to `ENDORMI`.
2. A wake-up countdown is initialized.
3. The scheduler selects another runnable process.
4. The timer handler periodically decreases the remaining sleep duration.
5. Once the countdown reaches zero, the process becomes runnable again.

The idle process is protected against being put to sleep.

### 5. Console Display

The project also covers basic console management:

- Character rendering.
- Cursor positioning.
- Newline, tab, and backspace handling.
- Screen scrolling.
- Displaying the system clock.
- Displaying process states for debugging.

## Architecture

| File | Responsibility |
|---|---|
| `horloge.c` | System clock and timer interrupt management |
| `ordonnanceur.c` | Process scheduling and sleep management |
| `processus.c` | Process creation, state management, and termination |
| `processus.h` | Process structures and state definitions |
| `affichage.c` | Console output |
| `curseur.c` | Cursor management |
| `traitant.S` | Low-level interrupt handling |
| `ctxt.S` | Assembly context switching |
| `start.c` | Kernel initialization |
| `kernel.lds` | Linker script |
| `Makefile` | Build, execution, and debugging |

## Technologies

- **C11** — Kernel implementation
- **RISC-V 64-bit** — Target architecture
- **Assembly** — Interrupt and context-switching routines
- **QEMU** — System emulation
- **GNU Make** — Build automation
- **GDB** — Low-level debugging
- **Git / GitLab CI** — Version control and automated evaluation

## Build & Run

### Prerequisites

The original project uses an academic RISC-V development environment containing:

- `riscv64-unknown-elf-gcc`
- `riscv64-unknown-elf-objcopy`
- `qemu-system-riscv64`

The Makefile expects these tools under a configurable `RVDIR` directory.

The QEMU configuration uses the `cep` machine, which may require the original academic environment or a compatible QEMU build.

### Build

From the `src/` directory:

```bash
make
```

This compiles the C and assembly sources and produces `kernel.bin`.

### Run

```bash
make run
```

To run with graphical output:

```bash
make rung
```

### Debug

```bash
make debug
```

This starts QEMU with a GDB server, allowing inspection of kernel execution.

### Tests

```bash
make test
```

The project also includes automated evaluation tests covering display operations, timer interrupts, process scheduling, sleeping, waking, and termination.

## Technical Challenges

### Scheduling and Context Switching

Implementing the scheduler required coordinating process states, selecting the next runnable process, and preserving execution contexts.

The implementation uses a circular scheduling strategy and an assembly context-switching routine.

### Timer-Driven Execution

The timer handler connects hardware-level interrupts with higher-level process management.

Each timer interrupt can trigger a scheduling decision, while periodic updates maintain the system clock and wake sleeping processes.

### Process State Management

Managing process transitions required accounting for multiple execution states and edge cases, including situations where all user processes are sleeping or terminated.

## Learning Outcomes

This project provided hands-on experience in:

- Low-level systems programming in C.
- Operating system kernel architecture.
- Hardware interrupts and timer configuration.
- Preemptive process scheduling.
- Context switching and register preservation.
- Process lifecycle management.
- Debugging embedded software.

## Academic Context

**Institution:** Grenoble INP – Phelma  
**Program:** Embedded Systems and Connected Devices (SEOC)  
**Project:** Operating System Kernel Development

---

*Educational kernel project based on a provided skeleton, extended through the implementation of core operating system mechanisms.*

![lastupdate](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/lastupdate.svg)
[Log du dernier test](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/pcsef.log)

# Affichage

![Affichage_simple](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/Affichage_simple.svg)
![LF_new_line](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/LF_new_line.svg)
![HT_horizontal_tab](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/HT_horizontal_tab.svg)
![BS_backspace](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/BS_backspace.svg)
![CR_carriage_ret](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/CR_carriage_ret.svg)
![Curseur_simple](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/Curseur_simple.svg)
![scroll](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/scroll.svg)
![Curseur_après_scroll](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/Curseur_après_scroll.svg)
![FF_formfeed](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/FF_formfeed.svg)
![Curseur_après_FF](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/Curseur_après_FF.svg)

# Timer

![init_traitant_timer](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/init_traitant_timer.svg)
![enable_timer_mie](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/enable_timer_mie.svg)
![enable_timer_re-active](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/enable_timer_re-active.svg)
![trap_handler_re-active](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/trap_handler_re-active.svg)
![trap_handler_affichage_du_temps](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/trap_handler_affichage_du_temps.svg)
![nbr_secondes](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/nbr_secondes.svg)

# Processus gestion basique

![init_processus_mon_nom_mon_pid](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/init_processus_mon_nom_mon_pid.svg)
![cree_processus_ordonnance_simple](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/cree_processus_ordonnance_simple.svg)
![cree_processus_ordonnance_double](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/cree_processus_ordonnance_double.svg)
![cree_processus_ordonnance_apres_reset](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/cree_processus_ordonnance_apres_reset.svg)
![Processus_par_interruption](https://pcsef.pages.ensimag.fr/riscv/seoc/evals/ProjetPCSEF_hurstelm/Processus_par_interruption.svg)
