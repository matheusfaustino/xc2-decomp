# Xenoblade Chronicles 2 decompilation

A work-in-progress matching decompilation of *Xenoblade Chronicles 2* (Nintendo Switch, version 1.0.0).

The goal is to write C++ source code that, compiled with the same compiler and flags as the original, produces exactly the same machine code as the game, function by function.

**This repository contains reconstructed C++ code, but must not include original game executables, extracted assets, console keys or proprietary SDK files.** You need your own legally obtained copy of the game to work on it.

## Target

| | |
|---|---|
| Title ID | `0100E95004038000` |
| Version | 1.0.0 (base game, without update) |
| Build ID | `5087AF2917D85858A0FD49CB4D6EED67` |
| `main` (uncompressed NSO) SHA-256 | `956348ce46e0db8ed3f378847495f2869aa59027811351733eb17c6e5f1149d9` |

Version 1.0.0 is used because its executable still ships its dynamic symbol table: about 51,000 functions keep their original C++ names. Later updates are stripped.

The original source paths (`C:/Monolithsoft/BF2/prog/application/BF2/...`) appear in assertion strings, and `src/application/BF2/` mirrors that layout.

## Progress

<!-- progress -->
36,639 / 48,739 functions (75.174%), 4,327,964 / 10,024,304 bytes of code (43.175%)
<!-- /progress -->

Counted over the named functions of `main`, as reported by `check`. Run `check --readme` to update this section.

## Requirements

- [Nix](https://nixos.org/) with flakes enabled. The dev shell provides everything else: the original compiler, LLVM tools, hactool, Ghidra, Python.
- Optionally [direnv](https://direnv.net/), to enter the shell automatically (`direnv allow`).
- An XCI dump of the game, and the matching `prod.keys` from your own console.

## Setup

Set `XC2_ROM` to the path of your own XCI dump; there is no machine-specific
default. Paths containing spaces must be quoted. `SWITCH_KEYS` is optional and
defaults to `$HOME/.switch/prod.keys`. The development shell preserves values
you have already exported.

```sh
nix develop
export XC2_ROM="/path/to/Xenoblade.xci"
export SWITCH_KEYS=~/.switch/prod.keys   # default
setup
```

`setup` extracts the 1.0.0 executable from the XCI, checks its hash, and converts it into `work/main_100.elf`, an ELF with relocations applied and the full symbol table. Everything under `work/` is ignored by git and must never be committed.

## Checking functions

```sh
check                                       # every function defined in src/, and the overall progress
check -v                                    # same, listing the matching functions too
check --readme                              # same, and update the progress in this README
check gf::GfDataBladeCreate                 # one class or namespace, and the functions left to do
check gf::GfDataBlade::setHikariCurrent     # side by side diff of one function
check 0x710028c7ec src/application/BF2/gf/data/gf_data_obj.cpp unknown001
                                            # diff a function the game has no symbol for
```

`check` compiles `src/`, places each of your functions at the address of the original with every relocation resolved, and compares the bytes: it prints `MATCH`, or a colored side by side diff and the first instruction that differs. Only source locations may differ: the directory of a `__FILE__` path, the `__LINE__` passed to an assertion or to `operator new`, and the spaces of an assertion text. Data local to your file has no symbol in the game: read-only data (strings, literals, tables) must hold the game's bytes, and writable data must be at one game address wherever the function uses it. A full `check` lists only the functions that do not match; `-v` lists them all. It also compiles every header on its own, so a header must include or forward-declare everything it uses. It exits with an error when something does not match or fails to compile.

`check` keeps its work in `work/check_cache`: each object and its results are reused until its source or one of the headers it includes changes, and the game's symbols are read once. A change to `tools/check.py`, the compiler or its flags discards the whole cache; delete the directory to start over.

A function you define that the game has no symbol for is listed as `NOT IN GAME`, with the game functions of the same name: it usually means a wrong namespace or parameter type in its signature.

Functions local to their source file (`static`, anonymous namespace) have no symbol in the game. A full `check` finds the game address of each local function where the callers that match call it, and checks it too, listed as `(local, address)`; a local function only ever inlined has no call to follow. To diff one by hand, pass its game address and its name in your file. The game side of that diff ends at the next named function or at the next address called by a `bl`; your function is compared over its own size.

## Includes

```sh
includes                                    # list the includes to add, remove or replace by a forward declaration
includes --fix                              # apply them
```

Each file includes the header of every name it uses, and a header forward-declares what it only needs by pointer or reference instead of including it. `includes` runs include-what-you-use over `src/` and `include/` to enforce it; class templates are always included, never forward-declared. Run `check` afterwards: adding a line above an `operator new` or an assertion shifts its `__LINE__`, which can change the instructions (pin it with `#line` when it does).

## Compiler

The game was built with **Clang 3.9.1**. The flags that matter:

```
--target=aarch64-none-elf -O3 -std=c++14 -fno-exceptions -fno-rtti -fno-strict-aliasing -fno-slp-vectorize -mno-implicit-float -fPIC -g
```

`-g` is not optional: with Clang 3.9, debug info changes instruction scheduling, and nothing matches without it. `-fno-slp-vectorize` is needed too: the game was built without SLP vectorization (two vtable pointers stay an `stp`, a run of float to u16 conversions stays scalar). So is `-mno-implicit-float`: the game never copies structs through q registers (a 24-byte copy is three `ldr`/`str` pairs of x registers). Clang 4.0.1 is also available in the shell (`clang++-4.0.1`) for comparison.

To try another compiler: `XC2_CXX=clang++-4.0.1 check ...`.

## Standard library

The game uses the Nintendo SDK's libc++ (`std::__1`) on top of its musl based C library (`__nnmusl_init_dso`). The dev shell provides the same era of headers: the libc++ headers of LLVM 3.9.1, configured for musl and the pthread API, and the musl 1.1.16 headers for AArch64. The shell points `XC2_SYSROOT` at them (and links `work/sysroot` for editors). Include them as system headers: `<string.h>`, `<new>`, `<functional>`, `<unordered_map>`...

With exceptions disabled, libc++ 3.9 reports an allocation overflow with `fprintf(stderr, ...)` and `abort()` instead of throwing, as the game does. `std::sort`, `std::function`, `std::unordered_map<std::string, ...>` and the other `std::__1` templates the game instantiates compile to the same code as the game's.

## Ghidra

The dev shell includes Ghidra. Import `work/main_100.elf` (AArch64, little endian, base `0x7100000000`) and run the auto analysis.

`tools/ghidra/ExportDecomp.java` exports the pseudo-C of every function to one file per class, which makes the decompiler output searchable with grep:

```sh
_JAVA_OPTIONS=-Xmx24g ghidra-analyzeHeadless <project dir> <project name> -process main_100.elf \
  -noanalysis -readOnly -scriptPath tools/ghidra -postScript ExportDecomp.java work/decomp
```

Ghidra cannot open a project that is already open in the GUI: run it on a copy.

## Glossary

Abbreviations and names imposed by the game's symbols:

| Term | Meaning |
|---|---|
| `Dr`, `Bl` | Driver, Blade |
| `Chr` | Character (`GfBdatChrDr`: the driver table) |
| `Btl` | Battle |
| `Rsc` | Resource: models, motions, effects |
| `Pc`, `Wpn` | Player character, weapon (`getPcWpnDamage`) |
| `Cri` | Critical (`getPcWpnCriRate`) |
| `Wp` | Weapon points, spent to level up driver arts |
| `MS` | Message: `Bdat::getMSText` reads a text from a message table |
| `FP` | Pointer to a loaded BDAT table (`GfDataBdat::getFP`) |
| BDAT | The game's data tables (characters, items, quests, texts...), browsable at [xenoblade.github.io/xb2/bdat](https://xenoblade.github.io/xb2/bdat) |
| `Homura`, `Hikari` | Pyra, Mythra (Japanese names) |
| `Homuri` | Pyra and Mythra together |
| `Byakko` | Dromarch (Japanese name) |
| `Saika` | Pandoria (Japanese name) |
| `Hana` | Poppi (Japanese name) |
| `Hana JS`, `Hana JK`, `Hana JD` | Poppi α, Poppi QT, Poppi QTπ |
| `Metsu` | Malos (Japanese name) |
| `Nanakoori` | Ursula (Japanese name) |
| `Jiku` | Zeke (Japanese name) |
| `Meleph` | Mòrag (Japanese name) |
| `Shin` | Jin (Japanese name) |
| `Shin 2`, `Jiku hood` | Variants of a character, numbered or described as in the `CHR_Dr` BDAT table |
| `Avaritia`, `Gula`, `Invidia`, `Superbia`, `Luxuria`, `Liberalitas` | Argentum, Gormott, Uraya, Mor Ardain, Tantal, Leftheria (Japanese names) |
| `Arcadia` | Indol (Japanese name) |
| `Elpis` | Spirit Crucible Elpys |
| `Vandamme` | Vandham |
| `mm`, `mtl`, `ml` | Monolith Soft's in-house libraries (`mtl` is their template library, a replacement for the STL) |

## Repository layout

| Path | Content |
|---|---|
| `src/application/BF2/` | Decompiled game code, following the original source tree |
| `include/` | Shared headers (basic types, libraries); the C and C++ standard headers come from the dev shell |
| `tools/check.py` | Compile and compare functions |
| `tools/quality_gate.py` | Flag match-forcing patterns and duplicated code added since a revision |
| `tools/includes.py`, `tools/iwyu.imp` | Include-what-you-use pass and its mappings |
| `tools/setup.sh` | Extract the executable from the XCI |
| `tools/nso2elf.py`, `tools/nso_syms.py` | NSO to ELF conversion, symbol dump |
| `tools/ghidra/` | Ghidra scripts |

## Tips

- Functions appear in the binary in the order they were written in their source file. The neighbors of a function tell you which file it belongs to and what comes next.
- Ghidra guesses types and return values. When in doubt, the assembly is the reference.
- A destructor that must store the vtable pointer as the game does gets a body holding only `;`: with an empty body `{}`, the compiler drops the store.
- Different C++ can produce the same machine code. Prefer the version a programmer would naturally have written.
