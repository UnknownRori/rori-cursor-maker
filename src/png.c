#include <stdio.h>
#include <stdlib.h>
#include <nob.h>

#include <assert.h>
#include <stb_image.h>
#include <stb_image_resize2.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "png.h"
#include "config.h"
#include "globals.h"

void get_png_frames_from_folder(PNG_files* out, const char* name)
{
    assert(out != NULL && "Skill issue");

    char buffer[1024] = {0};
    int i = 0;

    while (true) {
        int w, h, c;
        snprintf(buffer, sizeof(buffer), "%s/" FRAME_PNG_FORMAT, name, i);
        
        uint8_t* png = stbi_load(buffer, &w, &h, &c, 4);
        if (png == NULL) return;
        
        int new_width  = w * config.scale;
        int new_height = h * config.scale;

        uint8_t* upscaled = malloc(new_width * new_height * 4);
        assert(upscaled != NULL && "Buy more RAM lol.");
        
        stbir_resize(
            png, w, h, 0, 
            upscaled, new_width, new_height, 0, 
            STBIR_RGBA, STBIR_TYPE_UINT8_SRGB, STBIR_EDGE_CLAMP, STBIR_FILTER_BOX
        );

        int encoded_png_size = 0;
        uint8_t* encoded_png = stbi_write_png_to_mem(upscaled, new_width * 4, new_width, new_height, 4, &encoded_png_size);
        assert(encoded_png != NULL && "Failed to encode PNG to memory");

        PNG p = (PNG) {
            .buffer = encoded_png,
            .size   = encoded_png_size,
        };

        free(upscaled);
        stbi_image_free(png);

        nob_da_append(out, p);
        i++;
    }
}

void unload_png_files(PNG_files* files)
{
    assert(files != NULL && "Skill issue");
    for (size_t i = 0; i < files->count; i++) {
        free(files->items[i].buffer);
    }
}

void load_folders(const char* path)
{
    char buffer[1024] = {0};
    for (size_t i = 0 ; i < TOTAL_FOLDER_MAPPING; i++) {
        snprintf(buffer, sizeof(buffer), "%s/%s", path, FOLDER_MAPPING[i]);
        if (!nob_file_exists(buffer)) {
            nob_log(NOB_ERROR, "folder '%s' is missing, stopping the cursor creation...", buffer);
            exit(69);
        }
        get_png_frames_from_folder(&loaded[i], buffer);
        nob_log(NOB_INFO, "loading: '%s' (animation frames: %zu)", FOLDER_MAPPING[i], loaded[i].count);
    }
}
