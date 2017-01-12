#pragma once

#include <cstdint>
#include <cstdbool>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

#include "libplatform/libplatform.h"
#include "v8.h"

using namespace std;
using namespace v8;

enum rose_screenmode { ROSE_GAMEMODE, ROSE_EDITORMODE };

#include "enums.h"
#include "common.h"

#include "file.h"
#include "fs.h"

#include "js.h"
#include "js_common.h"
#include "js_graphics.h"
#include "js_input.h"
#include "js_memory.h"

#include "rt.h"

#include "game.h"

#include "editor.h"

#include "api_graphics.h"
#include "api_input.h"
#include "api_memory.h"



