#pragma once

#include <stdint.h>

typedef struct {
    uint8_t** items;
    size_t capacity;
    size_t count;
} cursors;

typedef struct {
    uint8_t* buffer;
    size_t   size;
} ANI_cursor;

void unload_cursor(cursors* cursors);
void unload_ani_cursor(ANI_cursor* cursor);
