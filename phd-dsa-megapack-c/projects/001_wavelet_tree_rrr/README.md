# 001 – Wavelet Tree Range Analytics with Succinct RRR Backing

This project implements a tutorial-grade static wavelet tree that mimics RRR rank-support blocks. The goal is to explain how rank/select-enabled bitvectors power wavelet-tree navigation without getting lost in compression minutiae.

## Motivation
Wavelet trees form the backbone of many succinct indexes (FM-index, compressed suffix arrays). Being able to answer rank, select, order statistics, and value-range counts in `O(log σ)` while storing only bitvectors makes them attractive for memory-bound workloads.

## Data structures and algorithms
- **Wavelet tree:** recursively partitions the alphabet; each node holds a routing bitvector.
- **RRR-inspired bitvector:** packed bits with prefix popcounts every 64 bits (real RRR compresses blocks; here we keep raw bits for clarity).
- **Queries:** rank, k-th order statistic, and value range counting implemented via recursive descent.

## Complexity
- Build: `O(n log σ)` time and space.
- Query (rank/k-th/range): `O(log σ)` time.

## Building
```bash
make
```

## Running the demo
```bash
./bin/demo
```

## Running tests
```bash
make test
```

## Sample output
```
Built wavelet tree over 11 symbols.
rank(5, n) = 3
kth(l=0, r=11, k=3) = 1
range_count(0,11, 2..5) = 6
```
