# 002 – Wavelet Tree Range Analytics with Cache-Oblivious Layout

This project mirrors the first wavelet tree but flattens every node into a single array laid out in recursive pre-order, which mirrors a van Emde Boas cache-oblivious arrangement. The goal is to show how data placement alone can reduce cache misses without tuning to a specific hardware cache size.

## Key ideas
- **Cache-oblivious layout:** no explicit block size; recursion clusters subtrees tightly.
- **Flattened nodes:** children referenced by indices rather than pointers, enabling compact arrays and easier prefetch.
- **Queries:** rank, k-th, and value-range queries identical to the RRR variant but benefiting from the layout.

## Complexity
- Build: `O(n log σ)` time, `O(n log σ)` space.
- Queries: `O(log σ)` time; locality improvements reduce constants.

## Build & run
```bash
make
./bin/demo
```

## Tests
```bash
make test
```
