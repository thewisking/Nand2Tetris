# Nand2Tetris

This repository contains my implementations/solutions for the **Nand2Tetris** course projects (chips, programs, and toolchain pieces).

**Important:** none of the hardware logic here is “runnable” by itself. To run tests or simulate chips/programs, you **must** use the official **Nand2Tetris Software Suite** (the GUI tools like **Hardware Simulator**, **CPU Emulator**, **Assembler**, and **VM Emulator**).

## Requirements

- **Java** installed (the Nand2Tetris tools are Java apps)
- The official **Nand2Tetris Software Suite**
  - Download from: `https://www.nand2tetris.org/software`
  - You’ll use these tools to load `.tst` scripts and verify output against `.cmp` files.

## How to run the projects

### Hardware chips (HDL)

Folders like `Gates/`, `Adders/`, `ALU/`, `RAM/`, `Register/`, `Mult/`, `Decode/`, etc. contain chip implementations and tests.

1. Open **HardwareSimulator** (from the Nand2Tetris tools).
2. Use **File → Load Script…**
3. Select the relevant `.tst` file inside the folder you want.
4. Click **Run** (or **Run All**) and confirm it matches the expected `.cmp`.

**Common file types**
- `.hdl` = the chip implementation
- `.tst` = the test script you load in the simulator
- `.cmp` = expected output the simulator compares against
- `.out` = output produced by running the test

### Assembly programs (.asm → .hack)

If a folder contains `.asm` programs:

1. Open **Assembler**
2. Assemble the `.asm` into a `.hack`
3. Optionally open **CPUEmulator** to run/step the `.hack` program (or load a provided `.tst` script if included)

### VM / Jack programs

If you’re working with `.vm` or `.jack` programs:

- Use the **VM Emulator** to run VM test scripts (when provided), or
- If this repo includes my own `VMTranslator/` and `Compiler/` implementations, build/run them to produce `.asm`/`.vm` outputs, then use the standard Nand2Tetris tools to execute and verify.

> Note: exact build steps depend on the language used inside `VMTranslator/` and `Compiler/`. The intended workflow is still the same: generate outputs, then verify them using the official Nand2Tetris test scripts/tools.

## Repo structure (high level)

- `Gates/` — basic logic gates
- `Adders/`, `AddSub/` — arithmetic building blocks
- `ALU/` — arithmetic logic unit work
- `RAM/`, `Register/` — storage components
- `Mult/`, `Abs/`, `ArrMax/`, `ArrMin/`, `ArrSort/`, `Fib/` — extra chips/programs/exercises
- `VMTranslator/` — VM → ASM translator (if implemented here)
- `Compiler/` — Jack → VM compiler (if implemented here)
- `Decode/` — decoding-related logic (depending on project scope)

## Troubleshooting

- **Tests fail immediately:** confirm you loaded the `.tst` file in the correct tool (Hardware Simulator vs CPU/VM tools).
- **“File not found” errors:** the Nand2Tetris tools resolve paths relative to the folder where you loaded the `.tst` script from—keep the project folder structure intact.
- **Weird output formatting:** make sure the `.cmp` file is present and unchanged, and you’re running the correct test script for that chip/program.
