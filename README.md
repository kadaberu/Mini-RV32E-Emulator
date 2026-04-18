# minirvEUM

A lightweight RISC-V RV32E emulator written in C, originally built to verify machine code
for a custom Logisim CPU implementation. Now with AM-based VGA display support — it can
actually draw things on screen.

---

## Architecture

- **Core**: RV32E (16 general-purpose registers, x0–x15)
- **ISA**: 8 instructions only — `lui`, `add`, `addi`, `lw`, `sw`, `lbu`, `sb`, `jalr`
  - No branch instructions. All control flow goes through `jalr`. Yes, really.
- **Memory**: 24-bit address space; 4MB ROM + 4MB RAM

---

## How It Works

Standard fetch-decode-execute loop:

1. **Fetch** — Read 32-bit instruction from ROM at current PC
2. **Decode** — Extract opcode, funct3, immediates, assert control signals
3. **Execute** — ALU ops or memory access; sign extension handled via C bit shifts
4. **Halt** — Stops on `ebreak` (0x00100073), checks `x10` for exit code:
   - `x10 == 0` → `HIT GOOD TRAP`
   - `x10 != 0` → `HIT BAD TRAP`

---

## VGA Display

Memory-mapped display over the range `0x20000000–0x2003FFFF`.

- Resolution: **256×256**

- Format: Each `sw` to this range writes one RGB pixel
  - Address bits [9:2] → X coordinate
  - Address bits [17:10] → Y coordinate
  - Value bits [23:0] → RGB color
  
- Rendering via AM's `AM_GPU_FBDRAW`

  

To use the display, you need to provide your own RV32E binary that writes to the memory-mapped display range. By default the emulator loads `mem.bin`; to load a different binary, change the filename in the two `fopen` calls near the top of `main()` in
`minirvEUM.c`:

```c
FILE *fp_rom = fopen("your_binary.bin", "rb");
FILE *fp_ram = fopen("your_binary.bin", "rb");
```

---

## Build & Run

> ⚠️ Tested on Linux only. Windows is not supported and has not been tested.

**Prerequisites**: gcc (C99+), make, AbstractMachine environment

```bash
# Build
make

# Run (requires mem.bin in project root by default)
make run

# Debug with GDB
make debug
gdb ./minirvemu
```

---

## Supported Instructions

| Instruction | Opcode    | Notes                        |
| ----------- | --------- | ---------------------------- |
| `addi`      | 0x13      | Immediate add, sign-extended |
| `add`       | 0x33      | Register add                 |
| `lui`       | 0x37      | Load upper immediate         |
| `lw`        | 0x03/f3=2 | Load word                    |
| `lbu`       | 0x03/f3=4 | Load byte unsigned           |
| `sw`        | 0x23/f3=2 | Store word                   |
| `sb`        | 0x23/f3=0 | Store byte                   |
| `jalr`      | 0x67      | Jump and link register       |

---

## Limitations

- Only 16 registers (RV32E constraint)
- No branches — implement loops with `jalr` gymnastics
- No interrupts, no floating point, no M-extension
- Display is write-only; no framebuffer readback

---

Built as a hardware verification tool. Grew into something that can draw pixels. Close enough.