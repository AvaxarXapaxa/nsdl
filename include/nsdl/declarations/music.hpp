#pragma once

#include <nsdl/collection.hpp>


namespace nk {
	class Music {
	protected:
		Mix_Music* sdl_music = nullptr;

		Music(const nk::Music&) {} // PLACEHOLDER
	public:
		Music(const std::wstring& file_name);
		~Music();

		void play(const U32 loops = 0, const bool loop_forever = false);
		inline static void pause() { Mix_PauseMusic(); }
		inline static void resume() { Mix_ResumeMusic(); }
		inline static void stop() { Mix_HaltMusic(); }
		
		inline static bool isPlaying() { return Mix_PlayingMusic(); }
		inline static bool isPaused() { return Mix_PausedMusic(); }

		inline Mix_Music* getSDLMusic() { return this->sdl_music; }
		inline bool isValid() const { return this->sdl_music != nullptr; }
	};
}
