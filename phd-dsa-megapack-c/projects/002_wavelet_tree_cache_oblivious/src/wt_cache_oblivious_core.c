/**
 * wt_cache_oblivious_core.c
 * --------------------------
 * Implements a cache-oblivious wavelet tree by storing every node in a contiguous array
 * according to recursive pre-order. This vEB-inspired layout makes it likely that a full
 * root-to-leaf traversal touches only a couple of cache lines, especially for tall trees.
 *
 * Differences from the RRR variant:
 *   - Nodes are indexed, not pointer-linked, avoiding pointer chasing and improving prefetching.
 *   - Bitvectors and rank logic mirror the RRR project but are written to operate in-place.
 *
 * Complexity: identical asymptotics to a standard wavelet tree but improved constants.
 */
#include "wt_cache_oblivious.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    wtco_node *buf;
    size_t size;
    size_t cap;
} node_vec;

static void node_vec_init(node_vec *v) { v->buf = NULL; v->size = v->cap = 0; }
static void node_vec_free(node_vec *v) { free(v->buf); v->buf = NULL; v->size = v->cap = 0; }
static int node_vec_push(node_vec *v, wtco_node node) {
    if (v->size == v->cap) {
        size_t new_cap = v->cap ? v->cap * 2 : 8;
        wtco_node *tmp = realloc(v->buf, new_cap * sizeof(wtco_node));
        if (!tmp) return -1;
        v->buf = tmp;
        v->cap = new_cap;
    }
    v->buf[v->size] = node;
    return (int)v->size++;
}

static uint64_t mask_until(size_t pos) {
    return pos == 64 ? UINT64_MAX : (pos == 0 ? 0 : ((1ULL << pos) - 1ULL));
}

static void bitv_build(wtco_bitvector *bv, const uint8_t *bits, size_t len) {
    bv->len = len;
    size_t words = (len + 63) / 64;
    bv->bits = calloc(words, sizeof(uint64_t));
    bv->ranks = calloc(words + 1, sizeof(uint32_t));
    if (!bv->bits || !bv->ranks) {
        free(bv->bits); free(bv->ranks); bv->bits = NULL; bv->ranks = NULL; bv->len = 0; return;
    }
    for (size_t i = 0; i < len; ++i) {
        if (bits[i]) bv->bits[i / 64] |= (1ULL << (i % 64));
    }
    uint32_t acc = 0; bv->ranks[0] = 0;
    for (size_t w = 0; w < words; ++w) { acc += __builtin_popcountll(bv->bits[w]); bv->ranks[w+1] = acc; }
}

static size_t bitv_rank1(const wtco_bitvector *bv, size_t pos) {
    if (!bv || pos == 0) return 0;
    if (pos > bv->len) pos = bv->len;
    size_t w = pos / 64; size_t off = pos % 64; size_t base = bv->ranks[w];
    if (off == 0) return base;
    return base + (size_t)__builtin_popcountll(bv->bits[w] & mask_until(off));
}

static void bitv_free(wtco_bitvector *bv) { if (!bv) return; free(bv->bits); free(bv->ranks); bv->bits=NULL; bv->ranks=NULL; bv->len=0; }

// Recursive build that writes into node vector and returns index of created node.
static int build_rec(node_vec *vec, const uint8_t *data, size_t n, uint16_t low, uint16_t high) {
    wtco_node node = {0};
    node.low = low; node.high = high; node.left_idx = node.right_idx = -1;

    // Leaf node: no routing needed. Still allocate a zero-filled bitvector so rank helpers
    // never dereference NULL pointers.
    if (low == high || n == 0) {
        uint8_t *zeros = NULL;
        if (n > 0) {
            zeros = calloc(n, sizeof(uint8_t));
            if (!zeros) return -1;
        }
        bitv_build(&node.bitv, zeros, n);
        free(zeros);
        return node_vec_push(vec, node);
    }

    uint16_t mid = (uint16_t)((low + high) / 2);
    uint8_t *bits = malloc(n);
    uint8_t *left = malloc(n);
    uint8_t *right = malloc(n);
    if (!bits || !left || !right) { free(bits); free(left); free(right); return -1; }

    size_t lsz = 0, rsz = 0;
    for (size_t i = 0; i < n; ++i) {
        uint8_t sym = data[i];
        if (sym <= mid) { bits[i] = 0; left[lsz++] = sym; }
        else { bits[i] = 1; right[rsz++] = sym; }
    }
    bitv_build(&node.bitv, bits, n);
    free(bits);

    int idx = node_vec_push(vec, node);
    if (idx < 0) { free(left); free(right); return -1; }

    // Recursively build children; thanks to push order they end up adjacent.
    int lidx = build_rec(vec, left, lsz, low, mid);
    int ridx = build_rec(vec, right, rsz, mid+1, high);
    free(left); free(right);
    vec->buf[idx].left_idx = lidx;
    vec->buf[idx].right_idx = ridx;
    return idx;
}

wtco_tree wtco_build(const uint8_t *data, size_t n, uint16_t alphabet_low, uint16_t alphabet_high) {
    wtco_tree tree = {.nodes = NULL, .count = 0, .alphabet_low = alphabet_low, .alphabet_high = alphabet_high};
    if (n == 0 || alphabet_low > alphabet_high) return tree;
    node_vec vec; node_vec_init(&vec);
    int root = build_rec(&vec, data, n, alphabet_low, alphabet_high);
    if (root < 0) { node_vec_free(&vec); return tree; }
    tree.nodes = vec.buf; tree.count = vec.size;
    return tree;
}

void wtco_free(wtco_tree *tree) {
    if (!tree || !tree->nodes) return;
    for (size_t i = 0; i < tree->count; ++i) {
        bitv_free(&tree->nodes[i].bitv);
    }
    free(tree->nodes);
    tree->nodes = NULL; tree->count = 0;
}

static size_t rank_rec(const wtco_tree *tree, int idx, uint8_t c, size_t pos) {
    if (idx < 0 || pos == 0) return 0;
    const wtco_node *node = &tree->nodes[idx];
    if (node->low == node->high) return pos;
    uint16_t mid = (uint16_t)((node->low + node->high) / 2);
    size_t ones = bitv_rank1(&node->bitv, pos);
    size_t zeros = pos - ones;
    if (c <= mid) return rank_rec(tree, node->left_idx, c, zeros);
    return rank_rec(tree, node->right_idx, c, ones);
}

size_t wtco_rank(const wtco_tree *tree, uint8_t c, size_t pos) {
    if (!tree || !tree->nodes || c < tree->alphabet_low || c > tree->alphabet_high) return 0;
    return rank_rec(tree, 0, c, pos);
}

static uint8_t kth_rec(const wtco_tree *tree, int idx, size_t l, size_t r, size_t k) {
    const wtco_node *node = &tree->nodes[idx];
    if (node->low == node->high) return (uint8_t)node->low;
    size_t l1 = bitv_rank1(&node->bitv, l);
    size_t r1 = bitv_rank1(&node->bitv, r);
    size_t ones = r1 - l1; size_t zeros = (r - l) - ones;
    if (k < zeros) return kth_rec(tree, node->left_idx, l - l1, r - r1, k);
    return kth_rec(tree, node->right_idx, l1, r1, k - zeros);
}

uint8_t wtco_kth(const wtco_tree *tree, size_t l, size_t r, size_t k) {
    assert(tree && tree->nodes && l <= r && k < r - l);
    return kth_rec(tree, 0, l, r, k);
}

static size_t range_rec(const wtco_tree *tree, int idx, size_t l, size_t r, uint8_t a, uint8_t b) {
    if (idx < 0 || l >= r) return 0;
    const wtco_node *node = &tree->nodes[idx];
    if (b < node->low || a > node->high) return 0;
    if (a <= node->low && node->high <= b) return r - l;
    size_t l1 = bitv_rank1(&node->bitv, l);
    size_t r1 = bitv_rank1(&node->bitv, r);
    size_t left = range_rec(tree, node->left_idx, l - l1, r - r1, a, b);
    size_t right = range_rec(tree, node->right_idx, l1, r1, a, b);
    return left + right;
}

size_t wtco_range_count(const wtco_tree *tree, size_t l, size_t r, uint8_t a, uint8_t b) {
    if (!tree || !tree->nodes || l >= r || b < a) return 0;
    return range_rec(tree, 0, l, r, a, b);
}
