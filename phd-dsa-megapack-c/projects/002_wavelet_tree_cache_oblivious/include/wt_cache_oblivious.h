/**
 * wt_cache_oblivious.h
 * ---------------------
 * Cache-oblivious wavelet tree interface. Nodes are laid out in a van-Emde-Boas-like
 * preorder so that queries traversing root-to-leaf paths touch contiguous memory,
 * reducing cache and TLB misses without relying on hardware-specific parameters.
 *
 * Responsibilities:
 *   - Expose a flattened wavelet tree handle built from an input symbol array.
 *   - Provide rank, k-th, and value-range counting queries.
 *   - Document invariants and layout strategies for cache-obliviousness.
 *
 * Layout and invariants:
 *   - Nodes live in a single array `nodes` where children of a node are stored in the
 *     next slots determined by recursion order (depth-first). This mirrors a vEB layout
 *     that clusters subtrees to improve spatial locality.
 *   - Each node stores its alphabet interval [low, high] and a bitvector that routes
 *     symbols to left(0)/right(1). A leaf has low == high and children indices of -1.
 *
 * Complexity:
 *   - Build: O(n log σ)
 *   - Queries: O(log σ) time with improved constant factors due to layout.
 */
#ifndef WT_CACHE_OBLIVIOUS_H
#define WT_CACHE_OBLIVIOUS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t *bits;
    uint32_t *ranks;
    size_t len;
} wtco_bitvector;

typedef struct {
    wtco_bitvector bitv;
    int left_idx;   // index in nodes array, -1 if null
    int right_idx;  // index in nodes array, -1 if null
    uint16_t low;
    uint16_t high;
} wtco_node;

typedef struct {
    wtco_node *nodes;
    size_t count;
    uint16_t alphabet_low;
    uint16_t alphabet_high;
} wtco_tree;

wtco_tree wtco_build(const uint8_t *data, size_t n, uint16_t alphabet_low, uint16_t alphabet_high);
void wtco_free(wtco_tree *tree);

size_t wtco_rank(const wtco_tree *tree, uint8_t c, size_t pos);
uint8_t wtco_kth(const wtco_tree *tree, size_t l, size_t r, size_t k);
size_t wtco_range_count(const wtco_tree *tree, size_t l, size_t r, uint8_t a, uint8_t b);

#endif // WT_CACHE_OBLIVIOUS_H
