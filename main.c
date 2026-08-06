#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NOB_IMPLEMENTATION
#include <nob.h>

#define RCURSOR_IMPLEMENTATION
#include <rcursor.h>
#define RCURSOR_WIN_IMPLEMENTATION
#include <rcursor_win.h>
#define RCURSOR_X11_IMPLEMENTATION
#include <rcursor_x11.h>

#include "config.h"
#include "install_inf.h"
#include "theme_x11.h"

typedef struct {
    uint8_t* buffer;
    size_t   size;
} PNG;

typedef struct {
    uint8_t* buffer;
    size_t   size;
} ANI_cursor;

typedef struct {
    PNG* items;
    size_t capacity;
    size_t count;
} PNG_files;

typedef struct {
    uint8_t** items;
    size_t capacity;
    size_t count;
} cursors;

void get_frames_from_folder(PNG_files* files, const char* name)
{
    assert(files != NULL && "Skill issue");

    char buffer[1024] = {0};
    int i = 0;

    while (true) {
        snprintf(buffer, sizeof(buffer), "%s/" FRAME_PNG_FORMAT, name, i);
        FILE* f = fopen(buffer, "rb");
        if (f == NULL) return;

        fseek(f, 0, SEEK_END);
        long png_size = ftell(f);
        fseek(f, 0, SEEK_SET);

        uint8_t* png = malloc(png_size);
        if (png == NULL) assert(0 && "Buy more RAM lol");
        fread(png, 1, png_size, f);
        fclose(f);

        PNG p = (PNG) {
            .buffer = png,
            .size   = png_size,
        };
        nob_da_append(files, p);

        // nob_log(NOB_INFO, "File: '%s' loaded (%lu bytes)", buffer, png_size);
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

void create_cursors(
    PNG_files* files, 
    cursors* out, 
    uint16_t xhotspot, 
    uint16_t yhotspot
)
{
    assert(files != NULL && "Skill issue");
    for (size_t i = 0; i < files->count; i++) {
        PNG* f = &files->items[i];
        size_t o = 0;
        uint8_t* cur = create_cur_buffer_from_memory(
            f->buffer, 
            f->size, 
            config.width, 
            config.height, 
            xhotspot, 
            yhotspot, 
            &o
        );
        nob_da_append(out, cur);
    }
}

void create_xcursors(
    PNG_files* files, 
    cursors* out, 
    uint16_t xhotspot, 
    uint16_t yhotspot
)
{
    assert(files != NULL && "Skill issue");
    for (size_t i = 0; i < files->count; i++) {
        PNG* f = &files->items[i];
        size_t o = 0;
        uint8_t* cur = create_xcur_buffer_from_memory(
            f->buffer, 
            f->size, 
            config.width, 
            config.height, 
            xhotspot, 
            yhotspot, 
            &o
        );
        nob_da_append(out, cur);
    }
}

void unload_cursors(cursors* files)
{
    assert(files != NULL && "Skill issue");
    for (size_t i = 0; i < files->count; i++) {
        free(files->items[i]);
    }
}

ANI_cursor create_ani_cursor(cursors* files)
{
    assert(files != NULL && "Skill issue");
    size_t out = 0;
    uint8_t* ani =  create_ani_buffer_from_cur_buffer(
        (const uint8_t**)files->items, 
        files->count, 
        config.rate, 
        &out
    );
    // nob_log(NOB_INFO, "ANI Cursor: created (%zu bytes)", out);
    return (ANI_cursor) {
        .buffer = ani,
        .size = out,
    };
}

ANI_cursor create_xani_cursor(cursors* files)
{
    assert(files != NULL && "Skill issue");
    size_t out = 0;
    uint8_t* ani =  create_xani_buffer_from_cur_buffer(
        (const uint8_t**)files->items, 
        files->count, 
        config.rate, 
        &out
    );
    // nob_log(NOB_INFO, "ANI Cursor: created (%zu bytes)", out);
    return (ANI_cursor) {
        .buffer = ani,
        .size = out,
    };
}

bool save_ani_cursor(ANI_cursor* ani, const char* filename)
{
    assert(ani != NULL && "Skill issue");
    FILE* f = fopen(filename, "wb");
    if (f == NULL) return false;
    fwrite(ani->buffer, 1, ani->size, f);
    fclose(f);
    return true;
}

void unload_ani_cursor(ANI_cursor* ani)
{
    free(ani->buffer);
    ani->buffer = NULL;
    ani->size   = 0;
}

void build_win_cursor(const char* name, const char* folder)
{
    char* buffer = malloc(TEMP_BUFFER);

    snprintf(buffer, TEMP_BUFFER, "%s/windows", config.output);
    nob_mkdir_if_not_exists(buffer);

    for (int i = 0; i < NOB_ARRAY_LEN(WINDOWS_MAPPING); i++) {
        cursor_mapping_t schema = WINDOWS_MAPPING[i];
        PNG_files files = {0};
        snprintf(buffer, TEMP_BUFFER, "./%s/%s", folder, schema.folder);
        get_frames_from_folder(&files, buffer);
        cursors cur = {0};
        create_cursors(&files, &cur, schema.xhotspot, schema.yhotspot);

        ANI_cursor ani = create_ani_cursor(&cur);
        snprintf(buffer, TEMP_BUFFER, "%s/windows/%s.ani", config.output, schema.name);
        save_ani_cursor(&ani, buffer);

        nob_log(NOB_INFO, "schema: %s - animation %zu", schema.name, files.count);
        unload_ani_cursor(&ani);
        unload_cursors(&cur);
        unload_png_files(&files);
    }

    snprintf(buffer, TEMP_BUFFER, "%s/windows/" INSTALL_FILE, config.output);
    Nob_String_Builder sb = create_windows_metadata(name);
    FILE* f = fopen(buffer, "wb+");
    fwrite(sb.items, 1, sb.count, f);
    fclose(f);

    free(buffer);
}

void build_x11_cursor(const char* name, const char* folder)
{
    char* buffer = malloc(TEMP_BUFFER);

    snprintf(buffer, TEMP_BUFFER, "%s/x11", config.output);
    nob_mkdir_if_not_exists(buffer);
    snprintf(buffer, TEMP_BUFFER, "%s/x11/cursors", config.output);
    nob_mkdir_if_not_exists(buffer);

    for (int i = 0; i < NOB_ARRAY_LEN(X11_MAPPING); i++) {
        cursor_mapping_t schema = X11_MAPPING[i];
        PNG_files files = {0};
        snprintf(buffer, TEMP_BUFFER, "./%s/%s", folder, schema.folder);
        get_frames_from_folder(&files, buffer);
        cursors cur = {0};
        create_xcursors(&files, &cur, schema.xhotspot, schema.yhotspot);

        ANI_cursor ani = create_xani_cursor(&cur);
        snprintf(buffer, TEMP_BUFFER, "%s/x11/cursors/%s", config.output, schema.name);
        save_ani_cursor(&ani, buffer);

        nob_log(NOB_INFO, "schema: %s - animation %zu", schema.name, files.count);
        unload_ani_cursor(&ani);
        unload_cursors(&cur);
        unload_png_files(&files);
    }

    snprintf(buffer, TEMP_BUFFER, "%s/x11/" THEME_FILE, config.output);
    Nob_String_Builder sb = create_x11_metadata(name);
    FILE* f = fopen(buffer, "wb+");
    fwrite(sb.items, 1, sb.count, f);
    fclose(f);

    free(buffer);
}

int main(int argc, char** argv)
{
    char* buffer = malloc(TEMP_BUFFER);

    nob_log(NOB_INFO, "Building cursor pack: %s", DEFAULT_NAME);
    nob_mkdir_if_not_exists(config.output);

    free(buffer);
    build_win_cursor(DEFAULT_NAME, "tests");
    build_x11_cursor(DEFAULT_NAME, "tests");


    return 0;
}
