/**
 * example_small.c
 * ----------------
 * Minimal example showing how to build the wavelet tree and perform rank/k-th queries.
 */
#include "wavelet_tree_rrr.h"
#include <stdio.h>

int main(void) {
    uint8_t seq[] = {7, 3, 7, 8, 2, 3, 1};
    wt_rrr tree = wt_rrr_build(seq, 7, 0, 8);

    printf("rank(7, 5) => %zu\n", wt_rrr_rank(&tree, 7, 5));
    printf("kth([1,6), k=2) => %u\n", wt_rrr_kth(&tree, 1, 6, 2));
    wt_rrr_free(&tree);
    return 0;
}
