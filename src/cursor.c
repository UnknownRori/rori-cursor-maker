#define RCURSOR_IMPLEMENTATION
#include <rcursor.h>
#define RCURSOR_WIN_IMPLEMENTATION
#include <rcursor_win.h>
#define RCURSOR_X11_IMPLEMENTATION
#include <rcursor_x11.h>

#include "cursor.h"

void unload_cursor(cursors* cursors)
{
    for (size_t i = 0; i < cursors->count; i++) {
        free(cursors->items[i]);
    }
}

void unload_ani_cursor(ANI_cursor* cursor)
{
    free(cursor->buffer);
}
