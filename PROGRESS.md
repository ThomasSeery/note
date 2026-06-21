# note — progress / resume point

A tiny command-line notes app. **v1 "done" =** `note add "buy milk"` appends a
note to a file, and `note ls` prints all notes numbered. C++23, plain `std::`
(no external libs except Catch2 for tests). See repo `CLAUDE.md` for *how to work
with me* (I write the code myself; teach in chat, not via code comments; keep
files clean; don't pre-write my logic) and `LIBRARY_CONVENTIONS.md` for deps.

## Architecture (decided)

- `libs/core/` — the logic library. Knows *how*: `add` and `load` operate on the notes file.
- `libs/core/src/paths.cpp` + `include/core/paths.hpp` — path resolution logic in `note::paths` namespace. Shared by core and app.
- `app/` — the executable. Knows *what/where*: parses argv, calls `create_directories`, prints output. Depends on core; core knows nothing of app.
- `tests/` — Catch2, links core. (Logic lives in a lib so it's testable.)
- A note in v1 is just a `std::string` (no struct/OOP yet — add one only if a real second field appears).

## Done ✓

- Scaffolded via `newcpp`.
- `libs/core/include/core/core.hpp` — `add` and `load` signatures.
- `libs/core/include/core/paths.hpp` / `libs/core/src/paths.cpp` — `note::paths::notes_file()`: cross-platform path resolution (Win32/APPDATA, macOS/HOME+Library, Linux/XDG_DATA_HOME with HOME fallback). Local static IIFE — computed once at runtime.
- `libs/core/src/core.cpp` — `add` (append mode, checks stream) and `load` (getline loop, empty vector if file missing).
- `app/src/main.cpp` — argv parsing via `std::span`, `create_directories` before writing, `add`/`ls` branches, numbered output via `std::views::enumerate`.
- **v1 working**: `note add "..."` and `note ls` both work. Binary installed to `/usr/local/bin/note`.

## Tooling / aliases (in ~/.bashrc)

- `cconf` — `cmake --preset dev`
- `cbuild` — `cmake --build --preset dev`
- `cinstall` — `sudo cmake --install build/dev --prefix /usr/local`
- `crun` — configure + build + install (full dev cycle)
- `cdebug` — configure + build gdb preset (no sanitizers) + launch gdb

## Next steps

- Write Catch2 tests for `add` and `load` in `tests/core/core_test.cpp`.
- Consider `note rm <n>` to delete a note by number.
- Consider `note edit <n>` to edit a note in `$EDITOR`.
