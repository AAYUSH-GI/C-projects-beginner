/**
 * Demonstration driver for cache-oblivious wavelet tree.
 */
#include "wt_cache_oblivious.h"
#include <stdio.h>

int main(void) {
    uint8_t data[] = {1,4,2,8,5,7,5,2,6,0,9,3};
    size_t n = sizeof(data)/sizeof(data[0]);
    wtco_tree tree = wtco_build(data, n, 0, 9);
    printf("rank(5, n)=%zu\n", wtco_rank(&tree, 5, n));
    printf("kth([0,%zu), k=4)=%u\n", n, wtco_kth(&tree, 0, n, 4));
    printf("range_count([2,10), 2..7)=%zu\n", wtco_range_count(&tree, 2, 10, 2, 7));
    wtco_free(&tree);
    return 0;
}
