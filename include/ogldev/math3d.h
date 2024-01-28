#pragma once

#ifdef _WIN64
#ifndef _USE_MATH_DEFINES
// For some reason this works on in Debug build of Visual Studio but not in
// Release build. The solution people suggested was to define this as a
// preprocessor macro in the project.
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#else
#include <math.h>
#endif
#include <cfloat>
#include <stdio.h>

#include <ogldev/utility.h>

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

// #define USE_GLM

#ifdef USE_GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#endif
#include <iostream>
#include <stdlib.h>
