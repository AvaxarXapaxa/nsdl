#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/renderer.hpp>


namespace nk {
	typedef SDL_Keycode Key;
	bool IS_EVENT_MUTEX_LOCKED = false;
	std::mutex EVENT_MUTEX;
	std::vector<SDL_Event> EVENT_STACK;

	class Window : public nk::Renderer {
	private:
		bool is_destroyed = false;
	protected:
		SDL_Window* sdl_window = nullptr;
		
		Window(const nk::Window&) {} // PLACEHOLDER
	public:
		struct WindowEvent {
			struct WindowEventHandler {
				std::function<void(nk::Window&, const bool)> onKeyboardFocus = [](nk::Window&, const bool) {};
				std::function<void(nk::Window&, const bool)> onMouseFocus = [](nk::Window&, const bool) {};
				std::function<void(nk::Window&, const bool)> onMinimize = [](nk::Window&, const bool) {};
				std::function<void(nk::Window&, const bool)> onShow = [](nk::Window&, const bool) {};

				std::function<void(nk::Window&, const bool)> onLeftMouseButtonPress = [](nk::Window&, const bool) {};
				std::function<void(nk::Window&, const bool)> onMiddleMouseButtonPress = [](nk::Window&, const bool) {};
				std::function<void(nk::Window&, const bool)> onRightMouseButtonPress = [](nk::Window&, const bool) {};

				std::function<void(nk::Window&, const Int2&, const Int2&)> onMouseMove = [](nk::Window&, const Int2&, const Int2&) {};
				std::function<void(nk::Window&, const short, const short)> onMouseScroll = [](nk::Window&, const short, const short) {};

				std::function<void(nk::Window&, const nk::Key, const bool)> onKeyPress = [](nk::Window&, const nk::Key, const bool) {};

				std::function<void(nk::Window&)> onQuit = [](nk::Window&) {};
				std::function<void(nk::Window&, const Uint2&)> onResize = [](nk::Window&, const Uint2&) {};
			} handler;

			bool keyboard_focus = false;
			bool mouse_focus = false;
			bool minimized = false;
			bool shown = false;

			bool left_mouse_button_pressed = false;
			bool middle_mouse_button_pressed = false;
			bool right_mouse_button_pressed = false;
			
			Int2 mouse_position = Int2{ 0, 0 };

			std::unordered_map<nk::Key, bool> key_pressed;
		} events;

		U64 last_update = nk::getMicros();
		double delta = 0.0;
		double fps = 0.0;

		Window(const Uint2& size = Uint2{ 800, 600 }, const std::wstring& title = L"", const bool resizable = false, const bool fullscreen = false);
		virtual ~Window();

		void close();
		void update();
		void handleEvents();

		void setResizable(const bool resizable = true);
		void setFullscreen(const bool fullscreen = true);
		void toggleFullscreen();

		void setSize(const Uint2& size);
		void setTitle(const std::wstring& title = L"");

		virtual Uint2 getSize() const;
		std::wstring getTitle() const;

		bool isResizable() const;
		bool isFullscreen() const;

		inline SDL_Window* getSDLWindow() { return this->sdl_window; }
		inline bool isValid() const { return this->sdl_window != nullptr && this->sdl_renderer != nullptr && !this->is_destroyed; }
	};
}
