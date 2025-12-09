#include "wt_cache_oblivious.h"
#include <stdio.h>

int main(void) {
    uint8_t seq[] = {4,1,4,2,4,3,4};
    wtco_tree tree = wtco_build(seq, 7, 0, 4);
    printf("rank(4,7)=%zu\n", wtco_rank(&tree, 4, 7));
    printf("kth([0,7),k=2)=%u\n", wtco_kth(&tree,0,7,2));
    wtco_free(&tree);
    return 0;
}
