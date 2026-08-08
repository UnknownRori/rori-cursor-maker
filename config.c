#include "config.h"
#include <string.h>

config_t config = {
    .width   = 32,
    .height  = 32,
    .rate    = 3 ,
    .scale   = SCALE,
    .output  = OUTPUT_DIR,
};

const char* FOLDER_MAPPING[TOTAL_FOLDER_MAPPING] = {
    "normal",
    "help",
    "link",
    "busy",
    "working",
    "text",
    "pen",
    "precision",
    "unavailable",
    "pin",
    "person",
    "move",
    "diagonal1",
    "diagonal2",
    "vertical",
    "horizontal",
    "alternate"
};

int get_index_mapping(const char* name)
{
    for (size_t i = 0; i < TOTAL_FOLDER_MAPPING; i++) {
        if (strcmp(name, FOLDER_MAPPING[i]) == 0) return i;
    }
    return -1;
}

const cursor_mapping_t WINDOWS_MAPPING[] = {
    {"Normal"     , "normal"        , 0 , 0  },
    {"Help"       , "help"          , 0 , 0  },
    {"Link"       , "link"          , 0 , 0  },
    {"Busy"       , "busy"          , 0 , 0  },
    {"Working"    , "working"       , 0 , 0  },

    {"Text"       , "text"          , 4 , 4  },
    {"Handwriting", "pen"           , 0 , 0  },
    {"Precision"  , "precision"     , 4 , 4  },
    {"Unavailable", "unavailable"   , 0 , 0  },
    {"Pin"        , "pin"           , 0 , 0  },
    {"Person"    , "person"        , 0 , 0  },

    {"Move"      , "move"          , 18, 16 },
    {"Diagonal1" , "diagonal1"     , 18, 16 },
    {"Diagonal2" , "diagonal2"     , 18, 16 },
    {"Vertical"  , "vertical"      , 18, 16 },
    {"Horizontal", "horizontal"    , 18, 16 },

    {"Alternate" , "alternate"     , 0 , 0  },
};
const size_t WINDOWS_MAPPING_SIZE = sizeof(WINDOWS_MAPPING) / sizeof(WINDOWS_MAPPING[0]);

const cursor_mapping_t X11_MAPPING[] = {
    {"left_ptr",            "normal",       0,  0  },
    {"arrow",               "normal",       0,  0  },
    {"default",             "normal",       0,  0  },

    {"question_arrow",      "help",         0,  0  },
    {"help",                "help",         0,  0  },
    {"whats_this",          "help",         0,  0  },

    {"hand2",               "link",         0,  0  },
    {"pointer",             "link",         0,  0  },
    {"pointing_hand",       "link",         0,  0  },

    {"watch",               "busy",          0,  0  },
    {"wait",                "busy",         0,  0  },

    {"left_ptr_watch",      "working",      0,  0  },
    {"halfbusy",            "working",      0,  0  },
    {"progress",            "working",      0,  0  },

    {"xterm",               "text",         0,  0  },
    {"text",                "text",         0,  0  },
    {"ibeam",               "text",         0,  0  },

    {"pencil",              "pen",          0,  0  },

    {"crosshair",           "precision",    0,  0  },
    {"cross",               "precision",    0,  0  },

    {"crossed_circle",      "unavailable",  0,  0  },
    {"not-allowed",         "unavailable",  0,  0  },
    {"forbidden",           "unavailable",  0,  0  },

    {"pin",                 "pin",          0,  0  },
    {"dnd-copy",            "person",       0,  0  },
    {"copy",                "person",       0,  0  },

    {"fleur",               "move",         18, 16 },
    {"move",                "move",         18, 16 },
    {"size_all",            "move",         18, 16 },

    {"nwse-resize",         "diagonal1",    18, 16 },
    {"size_nw_se",          "diagonal1",    18, 16 },
    {"top_left_corner",     "diagonal1",    18, 16 },
    {"bottom_right_corner", "diagonal1",    18, 16 },

    {"nesw-resize",         "diagonal2",    18, 16 },
    {"size_ne_sw",          "diagonal2",    18, 16 },
    {"top_right_corner",    "diagonal2",    18, 16 },
    {"bottom_left_corner",  "diagonal2",    18, 16 },

    {"ns-resize",           "vertical",     18, 16 },
    {"size_ns",             "vertical",     18, 16 },
    {"sb_v_double_arrow",   "vertical",     18, 16 },
    {"v_double_arrow",      "vertical",     18, 16 },
    {"top_side",            "vertical",     18, 16 },
    {"bottom_side",         "vertical",     18, 16 },

    {"ew-resize",           "horizontal",   18, 16 },
    {"size_we",             "horizontal",   18, 16 },
    {"sb_h_double_arrow",   "horizontal",   18, 16 },
    {"h_double_arrow",      "horizontal",   18, 16 },
    {"left_side",           "horizontal",   18, 16 },
    {"right_side",          "horizontal",   18, 16 },

    {"up_arrow",            "alternate",    0,  0  },
    {"center_ptr",          "alternate",    0,  0  },
};
const size_t X11_MAPPING_SIZE = sizeof(X11_MAPPING) / sizeof(X11_MAPPING[0]);
