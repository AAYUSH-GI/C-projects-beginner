/**
 * test_wavelet_tree_rrr.c
 * -----------------------
 * Unit tests stressing correctness of rank, k-th, and range queries. Each test names
 * the property being validated and includes comments explaining the adversarial case.
 */
#include "wavelet_tree_rrr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_rank_basic(void) {
    uint8_t data[] = {0,1,2,3,4,5,6,7,8,9};
    wt_rrr tree = wt_rrr_build(data, 10, 0, 9);
    // Expect rank(5,10) == 1 because 5 appears once.
    assert(wt_rrr_rank(&tree, 5, 10) == 1);
    // Prefix query: before index 5 (value 4) there is no 7.
    assert(wt_rrr_rank(&tree, 7, 5) == 0);
    wt_rrr_free(&tree);
}

static void test_kth_multiset(void) {
    uint8_t data[] = {5,5,5,4,4,3,3,2,1,1,0};
    wt_rrr tree = wt_rrr_build(data, 11, 0, 5);
    // Sorted order would be 0,1,1,2,3,3,4,4,5,5,5.
    assert(wt_rrr_kth(&tree, 0, 11, 0) == 0);
    assert(wt_rrr_kth(&tree, 0, 11, 5) == 3);
    assert(wt_rrr_kth(&tree, 0, 11, 10) == 5);
    wt_rrr_free(&tree);
}

static void test_range_count_edges(void) {
    uint8_t data[] = {9,9,9,9,9};
    wt_rrr tree = wt_rrr_build(data, 5, 0, 9);
    // Query outside the alphabet should return zero.
    assert(wt_rrr_range_count(&tree, 0, 5, 0, 8) == 0);
    // Full coverage on the only symbol.
    assert(wt_rrr_range_count(&tree, 0, 5, 9, 9) == 5);
    wt_rrr_free(&tree);
}

int main(void) {
    test_rank_basic();
    test_kth_multiset();
    test_range_count_edges();
    printf("All wavelet tree RRR tests passed.\n");
    return 0;
}
