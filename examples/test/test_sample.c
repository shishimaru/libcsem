#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

#define FOR(i,a,b) for(i=(a);i<(b);++i)
#define REP(i,n) FOR(i,0,n)
#define CLR(a) memset((a), 0 ,sizeof(a))

void main_small_test_tree() {
    int i = 0;
    REP(i, 10) {
        printf("%d\n", i);
    }
}
