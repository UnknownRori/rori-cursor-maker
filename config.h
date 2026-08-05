#pragma once

#include <stddef.h>
#include <stdint.h>

#define FRAME_PNG_FORMAT    "frame%04d.png"
#define OUTPUT_DIR          "dist"

typedef struct cursor_mapping_t {
    const char* name;
    const char* folder;
    uint16_t    xhotspot;
    uint16_t    yhotspot;
} cursor_mapping_t;

struct {
    const char* output;
    uint16_t    width;
    uint16_t    height;
    uint16_t    rate;
} config = {
    .width   = 32,
    .height  = 32,
    .rate    = 3 ,
    .output  = OUTPUT_DIR,
};

const cursor_mapping_t WINDOWS_MAPPING[] = {
    {"Normal"     , "normal"        , 0 , 0  },
    {"Help"       , "help"          , 0 , 0  },
    {"Link"       , "link"          , 0 , 0  },
    {"Busy"       , "busy"          , 0 , 0  },
    {"Working"    , "working"       , 0 , 0  },

    {"Text"       , "text"          , 0 , 0  },
    {"Handwriting", "pen"           , 0 , 0  },
    {"Precision"  , "precision"     , 0 , 0  },
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
