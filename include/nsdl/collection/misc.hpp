#pragma once

#include <nsdl/collection/includes.hpp>


typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef std::complex<double> cmplx;

typedef glm::i32vec2 Int2;
typedef glm::u32vec2 Uint2;
typedef glm::vec2 Vec2;

typedef glm::i32vec3 Int3;
typedef glm::u32vec3 Uint3;
typedef glm::vec3 Vec3;

typedef glm::u8vec4 Color;
typedef glm::vec4 Vec4;

typedef glm::mat2x2 Mat2x2;
typedef glm::mat3x3 Mat3x3;
typedef glm::mat4x4 Mat4x4;


const double PI = 3.14159265358979323846;
const double E = 2.718281828459045;
const double INF = std::numeric_limits<double>::infinity();

namespace nk {
	bool NK_HAS_BEEN_SETUP = false;

	// Sets up NSDL
	inline unsigned int setup() {
		if (nk::NK_HAS_BEEN_SETUP)
			return 0;
		else
			nk::NK_HAS_BEEN_SETUP = true;

		// Sets the pseudo-random seed to the current time point
		srand((unsigned int)time(NULL));

		// For Windows, std::wcout needs to be configured in order to work
		#ifdef _WIN32
		std::ios_base::sync_with_stdio(false);
		std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
		std::wcout.imbue(utf8);
		#endif

		std::setlocale(LC_ALL, "en_US.utf8");

		unsigned int errors = 0;

		if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")) {
			std::cerr << "SDL: The render driver could not be set to OpenGL, rendering issues may occur.\n";
			errors++;
		}

		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			std::cerr << "SDL: Could not initialize SDL_INIT_VIDEO properly, Error: " << SDL_GetError() << '\n';
			errors++;
		}

		if (SDL_Init(SDL_INIT_AUDIO) != 0) {
			std::cerr << "SDL: Could not initialize SDL_INIT_AUDIO properly, Error: " << SDL_GetError() << '\n';
			errors++;
		}

		// Emspripten bug: https://github.com/emscripten-ports/SDL2_image/issues/3
		#ifndef __EMSCRIPTEN__
		if (IMG_Init(IMG_INIT_JPG) == 0) {
			std::cerr << "SDL: Could not initialize IMG_INIT_JPG, Error: " << SDL_GetError() << '\n';
			errors++;
		}

		if (IMG_Init(IMG_INIT_PNG) == 0) {
			std::cerr << "SDL: Could not initialize IMG_INIT_PNG, Error: " << SDL_GetError() << '\n';
			errors++;
		}
		#endif

		if (TTF_Init() != 0) {
			std::cerr << "SDL: Could not initialize TTF_Init, Error: " << SDL_GetError() << '\n';
			errors++;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cerr << "SDL: Could not initialize SDL_mixer, Error: " << Mix_GetError() << "\n";
			errors++;
		}

		Mix_AllocateChannels(2048);

		return errors;
	}

	// Splits string from a character
	inline std::vector<std::wstring> split(std::wstring str, const wchar_t chr = L' ') {
		str += chr;
		std::vector<std::wstring> ret;
		std::wstring retstr;
		for (wchar_t chrc : str) {
			if (chrc == chr) {
				ret.push_back(retstr);
				retstr = L"";
			}
			else retstr += chrc;
		}
		return ret;
	}

	// Uppercase each character of the string
	inline std::wstring upper(const std::wstring& str) {
		std::wstring res;
		for (wchar_t wchr : str)
			res += (wchar_t)std::toupper(wchr);
		return res;
	}

	// Lowercase each character of the string
	inline std::wstring lower(const std::wstring& str) {
		std::wstring res;
		for (wchar_t wchr : str)
			res += (wchar_t)std::tolower(wchr);
		return res;
	}

	// Converts a string to int, returns 0 on error
	inline I64 strToInt(const std::wstring& str) {
		try {
			return std::stoll(str);
		}
		catch (...) {
			return 0;
		}
	}

	// Converts a string to floating point (is double), returns 0 on error
	inline double strToPoint(const std::wstring& str) {
		try {
			return std::stod(str);
		}
		catch (...) {
			return 0;
		}
	}

	// Generates a pseudo-random number between...
	inline double random(double from, double to) {
		return from + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (to - from)));
	}

	// Converts UTF8 encoded std::string to std::wstring; Source: https://stackoverflow.com/a/28875347/14022623
	inline std::wstring utf8ToWstring(const std::string& str) {
		return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
	}

	// Converts std::wstring to UTF8 encoded std::string; Source: https://stackoverflow.com/a/28875347/14022623
	inline std::string wstringToUtf8(const std::wstring& str) {
		return boost::locale::conv::utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
	}
}
