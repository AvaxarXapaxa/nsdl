#pragma once

#include <nsdl/declarations/sound.hpp>


nk::Sound::Sound(const std::wstring& file_name) {
	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	this->sdl_chunk = Mix_LoadWAV(utf8_file_path.c_str());
	if (this->sdl_chunk == nullptr)
		std::cerr << "SDL: Could not load a sound file, Error: " << Mix_GetError() << '\n';
}

nk::Sound::Sound(void* memory, const size_t size) {
	this->sdl_chunk = Mix_QuickLoad_RAW((U8*)memory, size);
	if (this->sdl_chunk == nullptr)
		std::cerr << "SDL: Could not load a sound from a memory buffer, Error: " << Mix_GetError() << '\n';
}

nk::Sound::~Sound() {
	if (this->sdl_chunk != nullptr)
		Mix_FreeChunk(this->sdl_chunk);
}

// Plays the sound
void nk::Sound::play(const U32 loops, const bool loop_forever) {
	if (!this->isValid())
		return;

	Mix_PlayChannel(-1, this->sdl_chunk, loop_forever ? -1 : loops);
}
