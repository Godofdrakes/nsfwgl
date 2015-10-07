#pragma once

#include <iostream>
#include <cstring>
#include <cassert>

#define GLM_FORCE_PURE
#define GLM_SWIZZLE

//this garbage needs to come before the includes below
// http://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// stringised version of line number (must be done in two steps)
#define STRINGISE(N) #N
#define EXPAND_THEN_STRINGISE(N) STRINGISE(N)
#define __LINE_STR__ EXPAND_THEN_STRINGISE(__LINE__)

// MSVC-suitable routines for formatting <#pragma message>
#define __LOC__ __FILE__ "(" __LINE_STR__ ")"
#define __OUTPUT_FORMAT__(type) __LOC__ " : " type " : "

// specific message types for <#pragma message>
#define __WARN__ __OUTPUT_FORMAT__("warning")
#define __ERR__ __OUTPUT_FORMAT__("error")
#define __MSG__ __OUTPUT_FORMAT__("programmer's message")

#include "Window.h"
#include "Assets.h"
#include "render.h"
#include "Application.h"
#include "Vertex.h"
