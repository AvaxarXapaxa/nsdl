#pragma once

#include <nsdl/declarations/music.hpp>


nk::Music::Music(const std::wstring& file_name) {
	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	this->sdl_music = Mix_LoadMUS(utf8_file_path.c_str());
	if (this->sdl_music == nullptr)
		std::cerr << "SDL: Could not load a music file, Error: " << Mix_GetError() << '\n';
}

nk::Music::~Music() {
	if (this->sdl_music != nullptr)
		Mix_FreeMusic(this->sdl_music);
}

void nk::Music::play(const U32 loops, const bool loop_forever) {
	if (!this->isValid())
		return;

	Mix_PlayMusic(this->sdl_music, loop_forever ? -1 : loops);
}
