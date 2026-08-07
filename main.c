#include <assert.h>
#include <stdint.h>

#include <nob.h>

#include "config.h"
#include "src/png.h"
#include "src/win.h"
#include "src/x11.h"

int main(int argc, char** argv)
{
    nob_log(NOB_INFO, "Building '%s' cursor pack", DEFAULT_NAME);
    nob_mkdir_if_not_exists(config.output);
    load_folders(SRC_DIR);
    build_win_cursor(DEFAULT_NAME);
    build_x11_cursor(DEFAULT_NAME);
    return 0;
}
