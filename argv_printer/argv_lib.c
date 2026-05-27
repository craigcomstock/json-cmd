#include "argv_lib.h"
#include <stdio.h>

void print_argv(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: '%s'\n", i, argv[i]);
    }
}
