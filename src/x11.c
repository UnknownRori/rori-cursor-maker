#include <assert.h>
#include <nob.h>
#include <rcursor_x11.h>

#include "config.h"
#include "globals.h"
#include "src/cursor.h"
#include "src/png.h"
#include "src/util.h"
#include "theme_x11.h"
#include "x11.h"

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

void build_x11_cursor(const char* name)
{
    char buffer[2024] = {0};

    snprintf(buffer, TEMP_BUFFER, "%s/x11", config.output);
    nob_mkdir_if_not_exists(buffer);
    snprintf(buffer, TEMP_BUFFER, "%s/x11/cursors", config.output);
    nob_mkdir_if_not_exists(buffer);

    for (size_t i = 0; i < X11_MAPPING_SIZE; i++) {
        cursor_mapping_t map = X11_MAPPING[i];
        const char* name = map.name;
        const char* folder = map.folder;

        int loaded_id = get_index_mapping(folder);
        if (loaded_id < 0) assert(0 && "Folder not found!");
        PNG_files* pngs = &loaded[loaded_id];
        cursors cursors = {0};
        create_xcursors(pngs, &cursors, map.xhotspot, map.yhotspot);
        ANI_cursor cursor = create_xani_cursor(&cursors);
        snprintf(buffer, sizeof(buffer), "%s/x11/cursors/%s", config.output, name);
        if (!save_buffer(buffer, cursor.buffer, cursor.size))
            nob_log(NOB_INFO, "Failed to save the file");
        unload_cursor(&cursors);
        unload_ani_cursor(&cursor);
    }

    snprintf(buffer, TEMP_BUFFER, "%s/x11/" THEME_FILE, config.output);
    Nob_String_Builder sb = create_x11_metadata(name);
    if (!save_buffer(buffer, (uint8_t*) sb.items, sb.count))
        nob_log(NOB_INFO, "Failed to save: '%s'", buffer);
}
