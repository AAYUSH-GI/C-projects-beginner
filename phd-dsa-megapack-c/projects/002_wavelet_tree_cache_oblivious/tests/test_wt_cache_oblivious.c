#include "wt_cache_oblivious.h"
#include <assert.h>
#include <stdio.h>

static void test_rank_ordering(void) {
    uint8_t data[] = {2,2,2,3,3,4,4,4};
    wtco_tree tree = wtco_build(data, 8, 0, 4);
    assert(wtco_rank(&tree, 2, 8) == 3);
    assert(wtco_rank(&tree, 3, 5) == 2);
    wtco_free(&tree);
}

static void test_kth_range(void) {
    uint8_t data[] = {9,1,8,2,7,3,6,4,5};
    wtco_tree tree = wtco_build(data, 9, 0, 9);
    // Sorted subrange [1,8) is {1,2,3,4,5,6,7}
    assert(wtco_kth(&tree, 1, 8, 3) == 4);
    wtco_free(&tree);
}

static void test_range_count(void) {
    uint8_t data[] = {1,2,3,4,5,6,7,8,9};
    wtco_tree tree = wtco_build(data, 9, 0, 9);
    assert(wtco_range_count(&tree, 0, 9, 3, 6) == 4);
    assert(wtco_range_count(&tree, 3, 6, 0, 10) == 3);
    wtco_free(&tree);
}

int main(void) {
    test_rank_ordering();
    test_kth_range();
    test_range_count();
    printf("cache-oblivious WT tests passed\n");
    return 0;
}
