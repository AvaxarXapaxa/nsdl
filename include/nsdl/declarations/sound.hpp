#pragma once

#include <nsdl/collection.hpp>


namespace nk {
	class Sound {
	protected:
		Mix_Chunk* sdl_chunk = nullptr;

		Sound(const nk::Sound&) {} // PLACEHOLDER
	public:
		Sound(const std::wstring& file_name);
		Sound(void* memory, const size_t size);
		~Sound();

		void play(const U32 loops = 0, const bool loop_forever = false);

		inline Mix_Chunk* getSDLChunk() { return this->sdl_chunk; }
		inline bool isValid() const { return this->sdl_chunk != nullptr; }
	};
}
