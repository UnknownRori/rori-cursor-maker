#pragma once

#include <stdint.h>

#define FRAME_PNG_FORMAT    "frame%04d.png"

typedef struct {
    uint8_t* buffer;
    size_t   size;
} PNG;

typedef struct {
    PNG* items;
    size_t capacity;
    size_t count;
} PNG_files;

void get_png_frames_from_folder(PNG_files* files, const char* name);
void unload_png_files(PNG_files* files);

void load_folders(const char* name);
