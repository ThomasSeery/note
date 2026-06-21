# PROBLEM — build config is not compiler-agnostic

**Symptom:** the project builds on Linux (GCC/Clang) but not with MSVC on
Windows. The notes app itself is portable C++; the **build configuration** is
not. Compiler-specific flags are hardcoded as if GCC/Clang is the only toolchain.

## Where

1. **`CMakeLists.txt`** — warning flags are GNU-style and applied unconditionally:
   ```cmake
   target_compile_options(note_warnings INTERFACE
       -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
   ...
   if(NOTE_WARNINGS_AS_ERRORS)
       target_compile_options(note_warnings INTERFACE -Werror)
   endif()
   ```
   MSVC doesn't understand `-Wextra`, `-Wpedantic`, `-Werror`, etc. → `D9002
   ignoring unknown option` noise, and `-Wall` means something completely
   different (and far noisier) to `cl`.

2. **`CMakePresets.json`** (`dev` preset) — sanitizer/debug flags baked into
   `CMAKE_CXX_FLAGS`:
   ```json
   "CMAKE_CXX_FLAGS": "-fsanitize=address,undefined -fno-omit-frame-pointer -g"
   ```
   `cl` doesn't take `-fsanitize=address,undefined` (MSVC ASan is
   `/fsanitize=address`, and there's no UBSan), nor `-g` / `-fno-omit-frame-pointer`.
   So the `dev` preset fails to configure under MSVC entirely.

## Why it matters

The whole reason for the cross-platform `paths::resolve_notes_file` work was to
be OS-agnostic. The build should match: it must configure and build on Linux,
macOS, **and** Windows so the per-OS test cases (and CI matrix) can run on a
real runner of each platform.

## Direction for the fix (next session)

- **Gate warning flags by compiler** with a generator expression instead of
  hardcoding GNU style, e.g.:
  ```cmake
  target_compile_options(note_warnings INTERFACE
      $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion>
      $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)
  ```
  Same idea for warnings-as-errors: `-Werror` (GNU/Clang) vs `/WX` (MSVC).
- **Make sanitizers an opt-in, compiler-aware option**, not a flag baked into
  the preset's `CMAKE_CXX_FLAGS`. e.g. a `NOTE_SANITIZE` option applying
  `-fsanitize=address,undefined` on GNU/Clang and `/fsanitize=address` on MSVC.
- **Drop `-g` / `-fno-omit-frame-pointer` from `CMAKE_CXX_FLAGS`** — let
  `CMAKE_BUILD_TYPE=Debug` supply debug info portably (`-g` vs `/Zi`).
- **Presets:** Ninja generator is fine cross-platform (VS bundles Ninja). Keep
  presets free of compiler-specific `CMAKE_CXX_FLAGS`; rely on build type plus
  the gated targets above. Optionally add per-toolchain presets later.

## Note

Until this is fixed, on Windows you can sidestep the presets with a plain MSVC
configure from a *Developer PowerShell for VS*:
```powershell
cmake -S . -B build/msvc -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/msvc
ctest --test-dir build/msvc --output-on-failure
```
(warning noise expected, but it builds and runs).
