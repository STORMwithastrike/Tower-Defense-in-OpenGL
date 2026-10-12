// Single translation unit that compiles the header-only libraries' implementations.
// Include stb_image.h / stb_truetype.h / miniaudio.h elsewhere WITHOUT these defines.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
