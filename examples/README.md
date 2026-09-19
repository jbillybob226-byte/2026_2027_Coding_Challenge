# Correct implementation examples

These prebuilt command-line programs show the behavior of a correct solution.
They run the same correctness testbench as the challenge, then print the 64-sample
step response with alpha 0.10. No compiler, Make, or CMake is needed to run them.
The reference filter uses `fp32` (32-bit `float`) inputs, state, and output.

Each program contains a fixed reference implementation. It does **not** load or
test your edited `src/low_pass_filter.c`. Use `mingw32-make test` on Windows or
`make test` on Linux/macOS to test your own implementation.

| System | Executable |
| --- | --- |
| Windows, Intel/AMD 64-bit | [Windows x64](bin/lpf-reference-windows-x64.exe) |
| Linux, Intel/AMD 64-bit | [Linux x64](bin/lpf-reference-linux-x64) |
| macOS 13+, Intel | [macOS x64](bin/lpf-reference-macos-x64) |
| macOS 13+, Apple Silicon (M-series) | [macOS ARM64](bin/lpf-reference-macos-arm64) |

Run the commands below from the repository root. These are terminal programs;
opening them by double-click may close the output window immediately.

## Windows (PowerShell)

```powershell
.\examples\bin\lpf-reference-windows-x64.exe
```

## Linux (x64)

```sh
chmod +x examples/bin/lpf-reference-linux-x64
./examples/bin/lpf-reference-linux-x64
```

## macOS

For Apple Silicon:

```sh
chmod +x examples/bin/lpf-reference-macos-arm64
./examples/bin/lpf-reference-macos-arm64
```

For Intel:

```sh
chmod +x examples/bin/lpf-reference-macos-x64
./examples/bin/lpf-reference-macos-x64
```

The Mac binaries are not Developer ID signed or notarized. If macOS blocks a
downloaded copy, follow its **System Settings > Privacy & Security** approval
prompt only if you trust this repository. Do not disable Gatekeeper globally.

## Expected output

The complete output is saved in [expected-output.txt](expected-output.txt).
The test summary should be:

```text
Result: 11/11 cases passed; 13452 checks, 0 failures.
```

At sample 10, the input changes from 0 to 5 and output is 0.50000000.
At sample 11, output is approximately 0.95; it approaches 5 over subsequent
samples. The printed last digits reflect single-precision rounding.
The process exits with zero when all correctness checks pass.

## Build provenance

The examples were cross-compiled from a temporary correct implementation and
`testbench/low_pass_filter_testbench.c` using Zig 0.15.2's C compiler, with C11 and
`-O2`. The temporary solution is not included in the exercise sources.
The targets are `x86_64-windows-gnu`, `x86_64-linux-musl` (statically linked),
`x86_64-macos`, and `aarch64-macos`. Mac executables use system libraries.
SHA-256 checksums are listed in [SHA256SUMS.txt](SHA256SUMS.txt).

These are reference snapshots: source or test changes do not rebuild them.

Validation: the Windows executable was run on Windows and the Linux executable
on Ubuntu through WSL; both passed all 13,452 checks and printed the step response.
Both macOS binaries were successfully cross-compiled and their Mach-O headers
checked, but have not been run on a Mac. Their minimum target is macOS 13.0.


