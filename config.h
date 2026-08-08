#pragma once

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_NAME        "Eau Rouge"

#define TEMP_BUFFER         4096 * 2
#define OUTPUT_DIR          "dist"
#define SCALE               3

#define TOTAL_FOLDER_MAPPING 17

typedef struct cursor_mapping_t {
    const char* name;
    const char* folder;
    uint16_t    xhotspot;
    uint16_t    yhotspot;
} cursor_mapping_t;

typedef struct {
    const char* output;
    uint16_t    width;
    uint16_t    height;
    uint16_t    rate;
    uint8_t     scale;
} config_t;

int get_index_mapping(const char* name);

extern config_t config;

extern const char* FOLDER_MAPPING[];

extern const cursor_mapping_t WINDOWS_MAPPING[];
extern const size_t WINDOWS_MAPPING_SIZE;

extern const cursor_mapping_t X11_MAPPING[];
extern const size_t X11_MAPPING_SIZE;
