#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#include <complex>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include <algorithm>
#include <cmath>
#include <functional>

#include <chrono>
#include <cctype>
#include <clocale>
#include <fcntl.h>
#include <mutex>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#undef max
#undef min
#include <codecvt>
#else
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#ifndef __EMSCRIPTEN__
#undef main
#endif

#define dict std::unordered_map


namespace fs = std::experimental::filesystem;
