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
    bool build_windows;
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
        "    -n name: name of the cursor pack, default is 'Lilith Itou'\n" \
        "    -i init: create directory for the cursor pack\n" \
        "    -b build: build the cursor pack based on template\n" \
        "    -W windows installer: build the cursor pack specific windows\n" \
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

void windows_assets_to_c()
{
    nob_log(NOB_INFO, "Creating windows installer file");
    nob_mkdir_if_not_exists("temp");

    char windows_output_dir[1024];
    snprintf(
        windows_output_dir, 
        sizeof(windows_output_dir), 
        "./%s/windows", config.output
    );
    // Nob_String_Builder src = {0};
    Nob_String_Builder out = {0};

    Nob_Cmd cmd = {0};
    // for (size_t i = 0; i < WINDOWS_MAPPING_SIZE; i++) {
    //     nob_sb_appendf(&src, "'%s/%s.ani'", windows_output_dir, WINDOWS_MAPPING[i].name);
    //     nob_sb_appendf(&out, "temp/%s.c", WINDOWS_MAPPING[i].name);
    //     nob_cmd_append(&cmd, "xxd", "-i", src.items);
    //     if (!nob_cmd_run(&cmd, .stdout_path = out.items)) {
    //         nob_log(NOB_ERROR, "Failed to generate: '%s' file", out.items);
    //     }
    //     src.count = 0;
    //     out.count = 0;
    // }
    //
    // nob_sb_appendf(&src, "'%s/install.inf'", windows_output_dir);
    // nob_cmd_append(&cmd, "xxd", "-i", src.items);
    // if (!nob_cmd_run(&cmd, .stdout_path = "temp/install_inf.c")) {
    //     nob_log(NOB_ERROR, "Failed to generate: 'temp/install_inf.c' file");
    // }


    FILE *temp = fopen("./temp/config.c", "wb");
    if (temp == NULL) {
        nob_log(NOB_ERROR, "failed to generate metadata");
        return;
    }
    nob_sb_appendf(&out, "#define CURSOR_NAME \"%s\"", args_cli.name);
    fwrite(out.items, 1, out.count, temp);
    fclose(temp);

    if (file_exists("./temp/icon.ico")) {
        nob_delete_file("./temp/icon.ico");
    }
    nob_cmd_append(&cmd, "ffmpeg", "-i", "./tests/normal/frame0000.png", "./temp/icon.ico");
    if (!nob_cmd_run(&cmd)) {
        nob_log(NOB_ERROR, "Failed to generate ico file");
    }

    nob_cmd_append(&cmd, "windres", "installer.rc", "-o", "./temp/installer_res.o");
    if (!nob_cmd_run(&cmd)) {
        nob_log(NOB_ERROR, "Failed to generate ico file");
    }

    out.count = 0;
    nob_cmd_append(&cmd, "gcc", "-mwindows", "windows_installer.c", "./temp/installer_res.o", "-o", args_cli.name);
    if (!nob_cmd_run(&cmd)) {
        nob_log(NOB_ERROR, "Failed to compile installer file");
    }
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
        if (strcmp(arg, "-n") == 0) {
            args_cli.name = nob_shift(argv, argc);
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
        if (strcmp(arg, "-W") == 0) {
            args_cli.build_windows = true;
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

    if (args_cli.build_windows) {
        windows_assets_to_c();
        // nob_mkdir_if_not_exists(config.output);
        // load_folders(args_cli.src_dir);
        // build_win_cursor(args_cli.name);
        return 0;
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

    if (!args_cli.build && !args_cli.build_windows) {
        print_help(program_name);
    }

    return 0;
}
