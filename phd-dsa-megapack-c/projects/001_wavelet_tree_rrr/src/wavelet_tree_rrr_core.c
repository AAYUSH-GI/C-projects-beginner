/**
 * wavelet_tree_rrr_core.c
 * -----------------------
 * Implements a static wavelet tree with rank-supported bitvectors. The design mimics
 * RRR blocks by storing prefix popcounts per 64-bit word so that rank queries are O(1)
 * per level without performing a full popcount scan. Compression is omitted to keep
 * the educational focus on the navigation logic rather than bit-packing tricks.
 *
 * Major components:
 *   - Bitvector builder that packs bits and precomputes prefix ranks.
 *   - Recursive wavelet tree construction splitting the alphabet midpoint.
 *   - Rank, k-th, and range-count queries built atop the bitvector interface.
 *
 * Complexity and performance notes:
 *   - Build cost: O(n log σ) time, O(n log σ) space.
 *   - Query cost: O(log σ) time; each level does O(1) work.
 *   - Space overhead: Each symbol contributes one bit per level it traverses.
 *
 * Safety:
 *   - All allocations checked; functions bail out cleanly on allocation failure by
 *     freeing partial subtrees. This keeps the tree in a consistent state.
 */
#include "wavelet_tree_rrr.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility prototypes ---------------------------------------------------------
static void bitv_build(wt_rrr_bitvector *bv, const uint8_t *bits, size_t len);
static size_t bitv_rank1(const wt_rrr_bitvector *bv, size_t pos);
static void bitv_free(wt_rrr_bitvector *bv);
static wt_rrr_node *wt_build_rec(const uint8_t *data, size_t n, uint16_t low, uint16_t high);
static void wt_free_rec(wt_rrr_node *node);
static size_t wt_rank_rec(const wt_rrr_node *node, uint8_t c, size_t pos, uint16_t low, uint16_t high);
static uint8_t wt_kth_rec(const wt_rrr_node *node, size_t l, size_t r, size_t k, uint16_t low, uint16_t high);
static size_t wt_range_count_rec(const wt_rrr_node *node, size_t l, size_t r, uint8_t a, uint8_t b, uint16_t low, uint16_t high);

// Public API -----------------------------------------------------------------
wt_rrr wt_rrr_build(const uint8_t *data, size_t n, uint16_t alphabet_low, uint16_t alphabet_high) {
    wt_rrr tree = {.root = NULL, .alphabet_low = alphabet_low, .alphabet_high = alphabet_high};
    if (n == 0 || alphabet_low > alphabet_high) {
        return tree;
    }
    tree.root = wt_build_rec(data, n, alphabet_low, alphabet_high);
    return tree;
}

void wt_rrr_free(wt_rrr *tree) {
    if (!tree || !tree->root) return;
    wt_free_rec(tree->root);
    tree->root = NULL;
}

size_t wt_rrr_rank(const wt_rrr *tree, uint8_t c, size_t pos) {
    if (!tree || !tree->root || c < tree->alphabet_low || c > tree->alphabet_high) return 0;
    if (pos == 0) return 0;
    return wt_rank_rec(tree->root, c, pos, tree->alphabet_low, tree->alphabet_high);
}

uint8_t wt_rrr_kth(const wt_rrr *tree, size_t l, size_t r, size_t k) {
    assert(tree && tree->root);
    assert(l <= r);
    assert(k < r - l);
    return wt_kth_rec(tree->root, l, r, k, tree->alphabet_low, tree->alphabet_high);
}

size_t wt_rrr_range_count(const wt_rrr *tree, size_t l, size_t r, uint8_t a, uint8_t b) {
    if (!tree || !tree->root || l >= r || b < a) return 0;
    if (b < tree->alphabet_low || a > tree->alphabet_high) return 0;
    if (a < tree->alphabet_low) a = (uint8_t)tree->alphabet_low;
    if (b > tree->alphabet_high) b = (uint8_t)tree->alphabet_high;
    return wt_range_count_rec(tree->root, l, r, a, b, tree->alphabet_low, tree->alphabet_high);
}

// Bitvector operations -------------------------------------------------------
static uint64_t mask_until(size_t pos) {
    // Return mask with bits [0, pos) set to 1.
    if (pos == 64) return UINT64_MAX;
    return (pos == 0) ? 0 : ((1ULL << pos) - 1ULL);
}

static void bitv_build(wt_rrr_bitvector *bv, const uint8_t *bits, size_t len) {
    bv->len = len;
    size_t words = (len + 63) / 64;
    size_t blocks = words; // one rank entry per 64-bit word
    bv->bits = calloc(words, sizeof(uint64_t));
    bv->ranks = calloc(blocks + 1, sizeof(uint32_t));
    if (!bv->bits || !bv->ranks) {
        free(bv->bits);
        free(bv->ranks);
        bv->bits = NULL;
        bv->ranks = NULL;
        bv->len = 0;
        return;
    }

    // Pack bits into 64-bit words.
    for (size_t i = 0; i < len; ++i) {
        size_t w = i / 64;
        size_t offset = i % 64;
        if (bits[i]) {
            bv->bits[w] |= (1ULL << offset);
        }
    }

    // Precompute prefix ranks per block.
    uint32_t accum = 0;
    bv->ranks[0] = 0;
    for (size_t w = 0; w < words; ++w) {
        accum += __builtin_popcountll(bv->bits[w]);
        bv->ranks[w + 1] = accum;
    }
}

static size_t bitv_rank1(const wt_rrr_bitvector *bv, size_t pos) {
    // Returns number of 1-bits in bv[0..pos)
    if (!bv || pos == 0) return 0;
    if (pos > bv->len) pos = bv->len;
    size_t w = pos / 64;
    size_t offset = pos % 64;
    size_t base = bv->ranks[w];
    if (offset == 0) return base;
    uint64_t masked = bv->bits[w] & mask_until(offset);
    return base + (size_t)__builtin_popcountll(masked);
}

static void bitv_free(wt_rrr_bitvector *bv) {
    if (!bv) return;
    free(bv->bits);
    free(bv->ranks);
    bv->bits = NULL;
    bv->ranks = NULL;
    bv->len = 0;
}

// Recursive wavelet tree construction ---------------------------------------
static wt_rrr_node *wt_build_rec(const uint8_t *data, size_t n, uint16_t low, uint16_t high) {
    wt_rrr_node *node = calloc(1, sizeof(wt_rrr_node));
    if (!node) return NULL;
    node->low = low;
    node->high = high;

    if (low == high || n == 0) {
        // Leaf: store a zero-filled bitvector so rank helpers never dereference NULL.
        uint8_t *zeros = NULL;
        if (n > 0) {
            zeros = calloc(n, sizeof(uint8_t));
            if (!zeros) { free(node); return NULL; }
        }
        bitv_build(&node->bitv, zeros, n);
        free(zeros);
        return node;
    }

    uint16_t mid = (uint16_t)((low + high) / 2);

    // Temporary buffers to stable-partition data into left/right subsequences.
    uint8_t *bits = malloc(n);
    uint8_t *left_buf = malloc(n);
    uint8_t *right_buf = malloc(n);
    if (!bits || !left_buf || !right_buf) {
        free(bits); free(left_buf); free(right_buf); free(node);
        return NULL;
    }

    size_t left_sz = 0, right_sz = 0;
    for (size_t i = 0; i < n; ++i) {
        uint8_t sym = data[i];
        if (sym <= mid) {
            bits[i] = 0;
            left_buf[left_sz++] = sym;
        } else {
            bits[i] = 1;
            right_buf[right_sz++] = sym;
        }
    }

    bitv_build(&node->bitv, bits, n);
    free(bits);

    // Recurse to children using the partitioned buffers.
    node->left = wt_build_rec(left_buf, left_sz, low, mid);
    node->right = wt_build_rec(right_buf, right_sz, mid + 1, high);
    free(left_buf);
    free(right_buf);
    return node;
}

static void wt_free_rec(wt_rrr_node *node) {
    if (!node) return;
    wt_free_rec(node->left);
    wt_free_rec(node->right);
    bitv_free(&node->bitv);
    free(node);
}

// Query helpers --------------------------------------------------------------
static size_t wt_rank_rec(const wt_rrr_node *node, uint8_t c, size_t pos, uint16_t low, uint16_t high) {
    if (!node || pos == 0) return 0;
    if (low == high) {
        // At a leaf, every symbol is identical to low==high.
        return pos;
    }
    uint16_t mid = (uint16_t)((low + high) / 2);
    size_t zeros = pos - bitv_rank1(&node->bitv, pos); // count of left-directed symbols
    if (c <= mid) {
        return wt_rank_rec(node->left, c, zeros, low, mid);
    } else {
        size_t ones = bitv_rank1(&node->bitv, pos);
        return wt_rank_rec(node->right, c, ones, mid + 1, high);
    }
}

static uint8_t wt_kth_rec(const wt_rrr_node *node, size_t l, size_t r, size_t k, uint16_t low, uint16_t high) {
    if (low == high) {
        return (uint8_t)low;
    }
    uint16_t mid = (uint16_t)((low + high) / 2);
    size_t l_ones = bitv_rank1(&node->bitv, l);
    size_t r_ones = bitv_rank1(&node->bitv, r);
    size_t ones = r_ones - l_ones; // number routed to right child
    size_t zeros = (r - l) - ones;

    if (k < zeros) {
        size_t l_zero = l - l_ones;
        size_t r_zero = r - r_ones;
        return wt_kth_rec(node->left, l_zero, r_zero, k, low, mid);
    } else {
        size_t k_in_right = k - zeros;
        return wt_kth_rec(node->right, l_ones, r_ones, k_in_right, mid + 1, high);
    }
}

static size_t wt_range_count_rec(const wt_rrr_node *node, size_t l, size_t r, uint8_t a, uint8_t b, uint16_t low, uint16_t high) {
    if (!node || l >= r || b < low || a > high) return 0;
    if (a <= low && high <= b) {
        // Entire node interval fits; all symbols contribute.
        return r - l;
    }
    uint16_t mid = (uint16_t)((low + high) / 2);
    size_t l_ones = bitv_rank1(&node->bitv, l);
    size_t r_ones = bitv_rank1(&node->bitv, r);
    size_t left_count = wt_range_count_rec(node->left, l - l_ones, r - r_ones, a, b, low, mid);
    size_t right_count = wt_range_count_rec(node->right, l_ones, r_ones, a, b, mid + 1, high);
    return left_count + right_count;
}
