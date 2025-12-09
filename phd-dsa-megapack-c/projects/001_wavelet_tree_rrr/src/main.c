/**
 * main.c
 * ------
 * Small demonstration driver that builds a wavelet tree over a sample sequence and
 * issues a few queries. This doubles as a sanity check outside the unit tests.
 */
#include "wavelet_tree_rrr.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    uint8_t data[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    size_t n = sizeof(data) / sizeof(data[0]);
    wt_rrr tree = wt_rrr_build(data, n, 0, 9);

    printf("Built wavelet tree over %zu symbols.\n", n);
    printf("rank(5, n) = %zu\n", wt_rrr_rank(&tree, 5, n));
    printf("kth(l=0, r=%zu, k=3) = %u\n", n, wt_rrr_kth(&tree, 0, n, 3));
    printf("range_count(0,%zu, 2..5) = %zu\n", n, wt_rrr_range_count(&tree, 0, n, 2, 5));

    wt_rrr_free(&tree);
    return 0;
}
