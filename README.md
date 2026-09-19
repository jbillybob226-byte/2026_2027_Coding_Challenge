# 2026_2027 Coding Challenge

Implement a stateful discrete low-pass filter in `src/low_pass_filter.c`.
The standalone C11 testbench prints correctness results in the terminal.
The filter interface is `fp32 low_pass_filter(fp32 x, fp32 alpha, int init)`.
`fp32` is a 32-bit `float`, enforced by a compile-time check in
`include/low_pass_filter.h`. Use `f` suffixes for filter arithmetic constants, such as
`1.0f`. This replaces the original PDF's `double`/`real` type requirement.
The recommended workflow is the included Makefile, which builds with GCC and runs the tests with one command. Direct GCC commands and CMake are available as alternatives.

## Repository layout

```text
src/low_pass_filter.c             Challenge implementation to edit
include/low_pass_filter.h         Public fp32 interface
testbench/low_pass_filter_testbench.c Correctness tests and terminal entry point
docs/                            Original challenge requirements PDF
examples/                        Prebuilt correct solutions and expected output
build/                           Generated files (ignored by Git)
Makefile                         Recommended build entry point
CMakeLists.txt                   Alternative CMake build
.github/workflows/               Cross-platform CI
```

Run commands from the repository root. Make outputs go to `build/make`, direct
GCC outputs to `build/gcc`, and CMake outputs to `build/cmake`. Previous local
builds and temporary reference tooling are preserved under `build/legacy`;
their cached paths are obsolete, so use a fresh build instead of reusing them.

The original specification is in
[docs/Coding_Challenge_Requirements.pdf](docs/Coding_Challenge_Requirements.pdf).

## Run a correct implementation example

Prebuilt reference executables for Windows x64, Linux x64, and macOS (Intel and
Apple Silicon) are available in [examples](examples/README.md). They show passing
test results and the expected filter response without requiring a compiler.
They contain a fixed solution; use the Makefile below to test your own code.

## Environment requirements

- A C11-capable GCC toolchain, including the C standard library headers and linker.
  A current GCC release is recommended; the testbench has also been verified with
  GCC 6.3.0 on Windows.
- Windows with MinGW-w64 GCC, or Linux with its distribution's GCC package.
  The MSYS2 installation instructions below target x86-64 Windows 10 (1809+) or
  Windows 11, matching [MSYS2's installation requirements](https://www.msys2.org/).
- PowerShell or the MSYS2 UCRT64 terminal on Windows; a shell such as Bash on Linux.
- A local copy of this repository and write access to its folder for the executable.
  Run build commands from the repository root, where `src/low_pass_filter.c` lives.
- Internet access for installing the compiler. Once installed, building and running
  the testbench works offline. Linux package installation requires `sudo` access.
- GNU Make for the recommended Makefile workflow. Alternatively, use GCC directly
  without a build tool, or CMake 3.16+ and a build
  backend such as GNU Make for the CMake option. All options compile the same
  C11 sources and require no external test libraries.


## Recommended: build and test with the Makefile (You will need this for future code development)

The Makefile automates the GCC commands and rebuilds when source files or the
header change. It uses GCC by default. Run it from the repository root (the folder containing `Makefile` and `src/low_pass_filter.c`).

### Install GNU Make

On Windows, after following the [Windows GCC setup](#install-gcc-on-windows) below, run this in **MSYS2 UCRT64**:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

The [MSYS2 native Make package](https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-make)
provides `mingw32-make`. In PowerShell, ensure `C:\msys64\ucrt64\bin` is on
PATH as described in the Windows GCC setup below, then verify with `mingw32-make --version`.

On Ubuntu/Debian, `sudo apt install build-essential` installs GCC and Make.
On Fedora, use `sudo dnf install gcc make`. Verify with `make --version`.

### Build and run

Windows PowerShell:

```powershell
mingw32-make
mingw32-make test
```

Linux/macOS:

```sh
make
make test
```

Run the same test command after editing the code; it rebuilds as needed. Running `make` (or `mingw32-make`) without a target builds `build/make/lpf_testbench` (with `.exe` on Windows).
`test` builds and runs the correctness testbench, and `clean` removes its generated executable. A failed check
makes the `test` target fail. Use `mingw32-make` in place of `make` on Windows.

Compiler and flags can be overridden, for example `make CC=clang` on macOS or
`make CFLAGS="-O0 -g -Wall -Wextra"`. Run `make clean` before switching compilers
or flags because Make does not track command-line variable changes.

If `make` on Windows prints a Windows command-prompt banner and stops, type
`exit` to leave that prompt. Older MSYS Make versions can do this when a Makefile
forces `cmd.exe` as their shell. This Makefile preserves the MSYS shell and also
supports native `mingw32-make`. Use `where.exe make` to see which installation
your terminal is finding. `make` builds the executables; `make test` runs tests.

## Install GCC on Windows

If `gcc --version` already works, you can proceed to the build instructions.
Otherwise, install MinGW-w64 GCC using [MSYS2](https://www.msys2.org/):

1. Download and run the x86-64 MSYS2 installer. The examples assume the default
   installation folder, `C:\msys64`.
2. Open **MSYS2 UCRT64** from the Start menu.
3. Update its packages using the command below. If prompted to close the terminal,
   let it close, reopen **MSYS2 UCRT64**, and run the command again to finish the
   update. See the [MSYS2 update guide](https://www.msys2.org/docs/updating/).

```sh
pacman -Syu
```

4. In the same UCRT64 terminal, install GCC and verify it:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
gcc --version
```

To use this GCC from **PowerShell**, add its executable directory to the current
session's PATH (adjust the directory if you installed MSYS2 elsewhere):

```powershell
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
gcc --version
Get-Command gcc
```

For a persistent setup, search Windows for **Edit environment variables for your
account**, edit your user **Path**, and add `C:\msys64\ucrt64\bin`. Reopen your
terminal afterward. If another GCC is installed, `Get-Command gcc` shows which
one PowerShell selects; the session command above puts the MSYS2 compiler first.

## Install GCC on Linux

On **Ubuntu or Debian**, install the compiler, standard headers, and build tools:

```sh
sudo apt update
sudo apt install build-essential
```

The package includes GCC and the required development tools. See the
[Ubuntu compiler installation guide](https://help.ubuntu.com/community/InstallingCompilers).

On **Fedora**, install GCC:

```sh
sudo dnf install gcc
```

See the [Fedora C installation guide](https://developer.fedoraproject.org/tech/languages/c/c_installation.html).
For other distributions, use their package manager to install GCC and the C library
development headers. Verify the installation in your terminal:

```sh
gcc --version
command -v gcc
```

## Alternative: compile directly with GCC

First open a terminal in the repository folder, or use `cd` to enter it. Replace
example paths with the location of your copy.

### Windows: PowerShell

```powershell
cd "C:\path\to\2026_2027_Coding_Challenge"
New-Item -ItemType Directory -Force build/gcc | Out-Null
gcc -std=c11 -Wall -Wextra -Iinclude src/low_pass_filter.c testbench/low_pass_filter_testbench.c -lm -o build/gcc/lpf_testbench.exe
if ($LASTEXITCODE -eq 0) { .\build\gcc\lpf_testbench.exe }
```

The last command runs the testbench only if compilation succeeded.

### Windows: MSYS2 UCRT64 terminal

MSYS2 uses paths such as `/c/Users/your-name/...` for Windows files:

```sh
cd /c/path/to/2026_2027_Coding_Challenge
mkdir -p build/gcc
gcc -std=c11 -Wall -Wextra -Iinclude src/low_pass_filter.c testbench/low_pass_filter_testbench.c -lm -o build/gcc/lpf_testbench.exe && ./build/gcc/lpf_testbench.exe
```

### Linux

```sh
cd /path/to/2026_2027_Coding_Challenge
mkdir -p build/gcc
gcc -std=c11 -Wall -Wextra -Iinclude src/low_pass_filter.c testbench/low_pass_filter_testbench.c -lm -o build/gcc/lpf_testbench && ./build/gcc/lpf_testbench
```

Re-run the compile command after editing the code. The testbench supplies the
`main` entry point; `-I.` locates the shared header. `-std=c11` selects the required C language standard.

On macOS, the same source can be compiled with a C11-capable compiler using the
Linux command above with the appropriate compiler name (for example, `clang`).
Apple's `gcc` command may invoke Clang rather than GNU GCC.

## Alternative: build with CMake and GCC

CMake configures the build; GCC still compiles the code. Install both the compiler
and a build backend along with CMake. This option uses the existing `CMakeLists.txt`
and keeps generated files in `build/cmake` separately from the Makefile outputs.

### Install CMake and GCC

On Windows, run this in **MSYS2 UCRT64** after the MSYS2 setup above:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-cmake
```

Alternatively, install the Windows x64 installer from the
[official CMake download page](https://cmake.org/download/), add its `bin` directory
to PATH, and install GCC and Make through MSYS2 as above. Reopen PowerShell after
changing PATH. CMake alone does not include GCC.

On Ubuntu/Debian:

```sh
sudo apt update
sudo apt install build-essential cmake
```

On Fedora:

```sh
sudo dnf install gcc make cmake
```

Distribution package references: [Ubuntu CMake](https://packages.ubuntu.com/noble/cmake)
and [Fedora CMake](https://packages.fedoraproject.org/pkgs/cmake/cmake/).
Verify `gcc --version`, `cmake --version`, and `mingw32-make --version` (Windows)
or `make --version` (Linux) in the terminal you will use to build.

### Configure, build, and test

Windows PowerShell (with the GCC and Make directories on PATH):

```powershell
cmake -S . -B build/cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --parallel
cmake --build build/cmake --target run_tests
```

Linux:

```sh
cmake -S . -B build/cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --parallel
cmake --build build/cmake --target run_tests
```

Stop if a command fails and resolve its error before running the next command.
After editing source files, run the final command again to rebuild and test.
CTest can also print the results: `ctest --test-dir build/cmake --verbose`.
Use a new build directory when changing the generator or compiler; CMake caches
those choices. The Makefile is the recommended workflow; direct GCC and CMake are alternatives.

## Test results and coverage

Each case prints `[PASS]` or `[FAIL]`. Failed checks print the expression and source
line; numerical failures also show expected/actual values and the tolerance.
The final summary reports cases passed, total checks, and failures. When all
checks pass, the testbench reseeds the filter and prints a 64-sample step response
with alpha 0.10 and input changing from 0 to 5 at sample 10. These values come
from your implementation; the step response is omitted if any check fails.
The process
returns zero on success and nonzero on failure, including in Release builds.

The 11 cases cover automatic first-call initialization, explicit/repeated resets,
runtime alpha changes, exact `init == 1` semantics, pass-through state, eight
invalid alpha values, tiny positive alpha, constant input, rising/falling steps,
impulse decay, and a deterministic mixed signal compared to a reference.
The first-call case always runs first; subsequent cases reseed their own state.
Numerical checks reject unexpected NaN/infinity results.
The tests use tolerances appropriate for single-precision rounding, with
higher-precision reference calculations for signal responses.

NaN alpha and non-finite input samples are unspecified by the challenge, so the
testbench does not impose behavior for them. Infinite alpha is checked because it
satisfies the specified out-of-range comparisons. The filter is not thread-safe.
This is a correctness testbench; it does not report performance timings.

## Challenge implementation

Implement `low_pass_filter` in `src/low_pass_filter.c` and preserve its declaration
in `include/low_pass_filter.h`. The challenge source contains only the filter function;
`testbench/low_pass_filter_testbench.c` provides the program's `main` entry point.
Before posting an unsolved exercise, replace only the filter function body with
the exercise stub. A temporary `return 0.0;` gives deterministic failing tests;
an empty non-void function has undefined behavior.



