# Mini-RV32E: 8-Instruction CPU & Emulator

- ## Overview

  minirvEUM is a lightweight, C-based software emulator built to verify and execute machine code for a custom Logisim implementation of the RISC-V RV32E architecture. It is designed to model a highly constrained hardware environment.

  ## Architecture Specifications

  - **Core:** RV32E subset (16 General-Purpose Registers).

  - **Instruction Set Architecture (ISA):** Strictly limited to 8 instructions: `lui`, `add`, `addi`, `lw`, `sw`, `lbu`, `sb`, and `jalr`.

    Note: The architecture lacks conditional branch instructions (e.g.`beq`). All control flow and looping mechanisms must be implemented via unconditional jumps (`jalr`).

  - **Memory System:**

    - 24-bit address width.
    - Simulated 4MB ROM and 4MB RAM arrays.
    - Loads a pre-compiled `mem.bin` binary file directly into the memory space upon initialization.

  ## Implementation Details

  The emulator executes via a standard fetch-decode-execute cycle:

  1. **Instruction Fetch:** Reads 32-bit instructions from the `ROM` array based on the Program Counter (PC).
  2. **Decode:** Extracts the opcode, `funct3`, and immediate fields to assert control signals.
  3. **Execute:** Performs ALU operations or memory accesses. Memory load/store operations execute sign-extension natively via C bitwise shifting.
  4. **Halt Condition:** The simulation continuously runs until an `ebreak` (0x00100073) instruction is fetched, subsequently evaluating `x10` (GPR[10]) to report the exit status (`HIT GOOD TRAP` / `HIT BAD TRAP`).

  ## Build and Usage

  ### Prerequisites

  - A C99-compatible compiler (e.g. `gcc`).
  - `make` utility.

  ### Compilation

  Build the executable using the provided Makefile:

  Bash

  ```
  make
  ```

  ### Execution

  The repository includes two pre-compiled test binaries: `mem.bin` and `sum.bin`.

  **Important Note:** The emulator source code is currently hardcoded to specifically load the `mem.bin` file upon execution. To run the `sum.bin` test, you must rename it to `mem.bin` in your directory before running the emulator.

  Bash

  ```
  # Ensure the file you want to test is named 'mem.bin'
  make run
  ```

  To run the emulator through GDB for register-level debugging:

  Bash

  ```
  make debug
  gdb ./minirvemu
  ```

  ## Planned Features

  - [ ] **Video Memory Integration:** Support for a 256x256 RGB memory-mapped display buffer to simulate hardware graphics rendering.