/**
 * wavelet_tree_rrr.h
 * -------------------
 * Public interface for a static wavelet tree using RRR-inspired blocks for rank/select support.
 * This file exposes an educational API for building a wavelet tree over small integer alphabets
 * (0..255 by default) and answering rank, k-th order statistic, and range counting queries.
 *
 * Responsibilities:
 *   - Define the `wt_rrr` handle and supporting node/bitvector types.
 *   - Provide construction, query, and teardown functions.
 *   - Document invariants and memory ownership rules.
 *
 * Algorithm overview:
 *   A wavelet tree recursively partitions the alphabet into halves. Each node stores a bitvector
 *   marking whether each symbol in its subsequence belongs to the left or right half. Rank and
 *   select on the bitvectors allow navigation down the tree to answer queries in O(log σ) time.
 *   We store rank-supporting prefix counts every fixed block (block_size = 64) to emulate RRR's
 *   fast rank without full compression, keeping implementation approachable while exposing the
 *   same interface a compressed bitvector would provide.
 *
 * Complexity:
 *   - Build: O(n log σ) time, O(n log σ) space for the node bitvectors.
 *   - rank/query/k-th: O(log σ) time, O(1) extra space.
 *   - Free: O(n log σ) time to release buffers.
 *
 * Invariants:
 *   - The tree is complete across the alphabet range [alphabet_low, alphabet_high].
 *   - Each node's bitvector length equals the number of symbols routed through that node.
 *   - Prefix table `ranks` has length ceil(len / block_size) + 1 and stores counts of 1-bits up to each block.
 *
 * Usage and ownership:
 *   - Caller allocates `wt_rrr` via `wt_rrr_build` and must later call `wt_rrr_free`.
 *   - Input data array is borrowed during construction; the tree stores its own copies.
 */
#ifndef WAVELET_TREE_RRR_H
#define WAVELET_TREE_RRR_H

#include <stddef.h>
#include <stdint.h>

/** Represents a single rank-supported bitvector.
 * Layout:
 *   - `bits` holds the raw bits (packed in uint64_t words).
 *   - `ranks` holds cumulative popcount up to each block to answer rank queries quickly.
 * Invariants:
 *   - block_size is fixed at 64.
 */
typedef struct {
    uint64_t *bits;     // packed bits, length in words = (len + 63) / 64
    uint32_t *ranks;    // prefix popcounts per block (length = blocks + 1)
    size_t len;         // number of logical bits
} wt_rrr_bitvector;

/** Represents a wavelet tree node.
 * Memory layout:
 *   - `bitv` stores routing bits for this level.
 *   - `left` and `right` point to child nodes; NULL for leaves.
 *   - `low`/`high` describe the inclusive alphabet interval represented by the node.
 * Invariants:
 *   - If low == high, node is a leaf and bitv.len equals sequence length routed here.
 */
typedef struct wt_rrr_node {
    wt_rrr_bitvector bitv;
    struct wt_rrr_node *left;
    struct wt_rrr_node *right;
    uint16_t low;
    uint16_t high;
} wt_rrr_node;

/** Public handle for the wavelet tree. */
typedef struct {
    wt_rrr_node *root;
    uint16_t alphabet_low;
    uint16_t alphabet_high;
} wt_rrr;

// Construction / Destruction -------------------------------------------------

/**
 * Builds a wavelet tree over the provided symbols.
 * Params:
 *   data           - pointer to input array of uint8_t symbols.
 *   n              - number of elements in data.
 *   alphabet_low   - lowest symbol value present (inclusive).
 *   alphabet_high  - highest symbol value present (inclusive). Must be >= alphabet_low.
 * Returns:
 *   Fully constructed wavelet tree handle. `root` is NULL if n == 0.
 * Complexity:
 *   O(n log σ) time, where σ = alphabet_high - alphabet_low + 1.
 */
wt_rrr wt_rrr_build(const uint8_t *data, size_t n, uint16_t alphabet_low, uint16_t alphabet_high);

/**
 * Releases all memory owned by the wavelet tree.
 * Params:
 *   tree - pointer to tree handle returned by build.
 * Notes:
 *   Safe to call with tree->root == NULL.
 */
void wt_rrr_free(wt_rrr *tree);

// Query operations -----------------------------------------------------------

/**
 * Counts occurrences of symbol `c` in data[0..pos).
 * Params:
 *   tree - pointer to built wavelet tree.
 *   c    - symbol to count.
 *   pos  - exclusive upper bound on position (0-based). pos may equal length.
 * Returns:
 *   Number of occurrences of c before position pos.
 * Complexity:
 *   O(log σ).
 */
size_t wt_rrr_rank(const wt_rrr *tree, uint8_t c, size_t pos);

/**
 * Finds the k-th smallest symbol in data[l..r) (0-based k).
 * Params:
 *   tree - pointer to built wavelet tree.
 *   l    - inclusive range start.
 *   r    - exclusive range end.
 *   k    - 0-based order statistic within [l, r). Must satisfy k < r - l.
 * Returns:
 *   Symbol value of the k-th smallest element.
 * Complexity:
 *   O(log σ).
 */
uint8_t wt_rrr_kth(const wt_rrr *tree, size_t l, size_t r, size_t k);

/**
 * Counts how many symbols in data[l..r) fall within [a, b].
 * Params:
 *   tree - pointer to built wavelet tree.
 *   l,r  - query range [l, r) with l <= r.
 *   a,b  - value interval inclusive.
 * Returns:
 *   Count of symbols in the value interval.
 * Complexity:
 *   O(log σ).
 */
size_t wt_rrr_range_count(const wt_rrr *tree, size_t l, size_t r, uint8_t a, uint8_t b);

#endif // WAVELET_TREE_RRR_H
