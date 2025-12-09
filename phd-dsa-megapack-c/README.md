# phd-dsa-megapack-c

A mega-pack of **500 advanced data structures and algorithms projects in C11**. Each project focuses on a high-end topic—succinct data structures, dynamic graphs, probabilistic sketches, cache-oblivious design—and is accompanied by heavily commented, tutorial-grade code. Many entries are already implemented; the remainder are fully designed in `PROJECT_INDEX.md` for future contributors.

## Repository layout
```
phd-dsa-megapack-c/
  README.md
  PROJECT_INDEX.md          # Catalog of 500 curated projects
  common/                   # Shared utilities (reserved for future reuse)
  projects/NNN_short_name/  # Individual project implementations
  docs/                     # Style guide and contribution notes
  ci/                       # CI configuration stubs
```

## Getting started
1. Pick a project from `PROJECT_INDEX.md`.
2. Build and run a demo:
   ```bash
   cd projects/001_wavelet_tree_rrr
   make
   ./bin/demo
   ```
3. Run the accompanying tests:
   ```bash
   make test
   ```

## Highlighted projects
- **001 – Wavelet Tree Range Analytics with Succinct RRR Backing:** teaches rank/select-enabled wavelet trees.
- **002 – Wavelet Tree Range Analytics with Cache-Oblivious Layout:** shows how layout alone influences performance.
- Future entries include dynamic connectivity with Euler tour trees, compressed suffix arrays, external-memory queues, probabilistic sketches, and more.

## Philosophy
- **Education-first:** Every file begins with a tutorial block explaining the data structure, invariants, and complexity.
- **Systems-grade C11:** Clean headers, predictable ownership, and `-Wall -Wextra -Wpedantic`-clean builds.
- **Extensibility:** Designed as a monorepo so new projects can reuse patterns from earlier ones.
