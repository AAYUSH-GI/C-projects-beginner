# STYLE_GUIDE

## Commenting
- Every `.c` and `.h` file starts with a tutorial block: responsibility, algorithm overview, invariants, complexity.
- Precede each struct with a paragraph on memory layout and invariants.
- Precede each function with: purpose, parameters, ownership/lifetimes, return value, and complexity.
- Inline comments should explain **why** decisions are made (branching factors, rebuild triggers), not just what a line does.

## Naming
- Use lowercase_with_underscores for functions and variables.
- Prefix internal helpers with a short module identifier to avoid collisions (e.g., `wt_`, `wtco_`).
- Keep header APIs minimal and stable; expose opaque handles when possible.

## Error handling
- Check every allocation. On failure, clean up partial allocations and leave the data structure in a safe state.
- Validate user input with `assert` for programmer errors and runtime checks for recoverable issues.

## Build flags
- Default to `-std=c11 -O2 -Wall -Wextra -Wpedantic`.
- Avoid compiler-specific extensions unless guarded by feature macros.

## Adding new projects
- Follow the directory pattern `projects/NNN_short_name/{include,src,tests,examples}`.
- Provide a `README.md`, `Makefile`, at least one test, and a minimal example program.
- Keep APIs header-first: design the interface before writing the implementation.
