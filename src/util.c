#include "util.h"
#include <stdio.h>

int save_buffer(const char* path, uint8_t* contents, size_t size)
{
    FILE* f = fopen(path, "wb");
    if (f == NULL) return 0;
    fwrite(contents, 1, size, f);
    fclose(f);
    return 1;
}
