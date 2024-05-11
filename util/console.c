#include "console.h"
#include <stdio.h>

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF ) { }
}