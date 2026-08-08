#include <assert.h>
#include <stdint.h>

#include <nob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "src/png.h"
#include "src/win.h"
#include "src/x11.h"

struct {
    bool help;
    bool init;
    bool build;
    bool verbose; // TODO : Implement this later

    const char* src_dir;
    const char* name;
} args_cli = {
    0
};

void print_help(const char* name)
{
    const char* help_template = 
        "rori-cursor-maker\n" \
        "UnknownRori's Cursor Maker, the streamlined cross-platform cursor maker in CLI without GUI\n" \
        "With automatic scaling\n\n" \
        "USAGE: %s -b \"Lilith Itou\"\n\n" \
        "available command listing:\n" \
        "    -h help: show this help message\n" \
        "    -i init: create directory for the cursor pack\n" \
        "    -b build: build the cursor pack based on template\n" \
        "    -s scale: set the scale of the cursor (example '3' for high-dpi)\n" \
        "    -S source: set the source\n" \
        "    -v verbose: output verbosity\n";
    fprintf(stderr, help_template, name);
}

void build(const char* name, const char* src)
{
    nob_log(NOB_INFO, "=== Building '%s' cursor pack ===", name);
    nob_mkdir_if_not_exists(config.output);
    load_folders(src);
    build_win_cursor(name);
    build_x11_cursor(name);
    nob_log(NOB_INFO, "=== Done Building '%s' cursor pack ===", name);
}

int main(int argc, char** argv)
{
    const char* program_name = nob_shift(argv, argc);
    while (argc > 0) {
        char* arg = nob_shift(argv, argc);
        if (strcmp(arg, "-h") == 0) {
            args_cli.help = true;
            continue;
        }
        if (strcmp(arg, "-i") == 0) {
            args_cli.init = true;
            continue;
        }
        if (strcmp(arg, "-S") == 0) {
            args_cli.src_dir = nob_shift(argv, argc);
            continue;
        }
        if (strcmp(arg, "-b") == 0) {
            args_cli.name = nob_shift(argv, argc);
            args_cli.build = true;
            continue;
        }
        if (strcmp(arg, "-v") == 0) {
            args_cli.verbose = true;
            continue;
        }
        if (strcmp(arg, "-s") == 0) {
            const char* str_num = nob_shift(argv, argc);
            int number = atoi(str_num);
            if (number <= 0) number = 3;
            config.scale = number;
            continue;
        }
    }

    if (args_cli.build) {
        if (args_cli.src_dir == NULL) args_cli.src_dir = ".";
        if (args_cli.name == NULL) args_cli.name = DEFAULT_NAME;
        build(args_cli.name, args_cli.src_dir);
        return 0;
    }

    if (args_cli.help) {
        print_help(program_name);
        return 0;
    }

    if (args_cli.init) {
        for (size_t i = 0; i < TOTAL_FOLDER_MAPPING; i++) {
            nob_mkdir_if_not_exists(FOLDER_MAPPING[i]);
        }
        return 0;
    }

    print_help(program_name);

    return 0;
}
