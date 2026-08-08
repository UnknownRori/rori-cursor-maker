#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <nob.h>
#include <rcursor_win.h>
#include <stdio.h>

#include "config.h"
#include "globals.h"
#include "install_inf.h"
#include "src/cursor.h"
#include "src/png.h"
#include "src/util.h"
#include "win.h"

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
            xhotspot * config.scale, 
            yhotspot * config.scale, 
            &o
        );
        nob_da_append(out, cur);
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

void build_win_cursor(const char* name)
{
    char buffer[1024] = {0};

    snprintf(buffer, TEMP_BUFFER, "%s/windows", config.output);
    nob_mkdir_if_not_exists(buffer);

    for (size_t i = 0; i < WINDOWS_MAPPING_SIZE; i++) {
        cursor_mapping_t map = WINDOWS_MAPPING[i];
        const char* name = map.name;
        const char* folder = map.folder;

        int loaded_id = get_index_mapping(folder);
        if (loaded_id < 0) assert(0 && "Folder not found!");
        PNG_files* pngs = &loaded[loaded_id];
        cursors cursors = {0};
        create_cursors(pngs, &cursors, map.xhotspot, map.yhotspot);
        ANI_cursor cursor = create_ani_cursor(&cursors);
        snprintf(buffer, sizeof(buffer), "%s/windows/%s.ani", config.output, name);
        if (!save_buffer(buffer, cursor.buffer, cursor.size))
            nob_log(NOB_INFO, "Failed to save: '%s'", buffer);
        unload_cursor(&cursors);
        unload_ani_cursor(&cursor);
    }

    snprintf(buffer, TEMP_BUFFER, "%s/windows/" INSTALL_FILE, config.output);
    Nob_String_Builder sb = create_windows_metadata(name);
    if (!save_buffer(buffer, (uint8_t*) sb.items, sb.count))
        nob_log(NOB_INFO, "Failed to save: '%s'", buffer);
}
