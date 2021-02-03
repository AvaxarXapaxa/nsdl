#pragma once

#include <nsdl/declarations/window.hpp>


nk::Window::Window(const Uint2& size, const std::wstring& title, const bool resizable, const bool fullscreen) {
	std::string utf8_title = nk::wstringToUtf8(title);

	this->sdl_window = SDL_CreateWindow(utf8_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		size.x, size.y,
		SDL_WINDOW_SHOWN | (resizable ? SDL_WINDOW_RESIZABLE : 0)
		#ifndef __EMSCRIPTEN__
		| (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
		#endif
	);

	#ifdef __EMSCRIPTEN__
	this->setFullscreen(fullscreen);
	#endif

	if (this->sdl_window == nullptr)
		std::cerr << "SDL: Could not initialize window, Error: " << SDL_GetError() << '\n';

	this->sdl_renderer = SDL_CreateRenderer(this->sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (this->sdl_renderer == nullptr)
		std::cerr << "SDL: Could not initialize window renderer, Error: " << SDL_GetError() << '\n';
}

nk::Window::~Window() {
	this->close();
}

// Closes and cleans up the window and its events
void nk::Window::close() {
	if (this->isValid()) {
		U32 window_id = SDL_GetWindowID(this->sdl_window);

		if (!nk::IS_EVENT_MUTEX_LOCKED)
			nk::EVENT_MUTEX.lock();

		for (size_t i_event = 0; i_event < nk::EVENT_STACK.size(); i_event++)
			if (nk::EVENT_STACK[i_event].window.windowID == window_id) {
				nk::EVENT_STACK.erase(nk::EVENT_STACK.begin() + i_event);
				i_event--;
			}

		if (!nk::IS_EVENT_MUTEX_LOCKED)
			nk::EVENT_MUTEX.unlock();
	}

	if (this->sdl_window != nullptr)
		SDL_DestroyWindow(this->sdl_window);

	if (this->sdl_renderer != nullptr)
		SDL_DestroyRenderer(this->sdl_renderer);

	this->is_destroyed = true;
}

// Updates the window
void nk::Window::update() {
	if (!this->isValid())
		return;

	SDL_RenderPresent(this->sdl_renderer);

	this->delta = ((double)nk::getMicros() - (double)this->last_update) / 1000000.0;

	if (this->delta == 0.0)
		this->fps = 0.0;
	else
		this->fps = 1.0 / delta;

	this->last_update = nk::getMicros();
}

// Handles the incoming events and pass it to the callback functions
void nk::Window::handleEvents() {
	if (!this->isValid())
		return;

	U32 window_id = SDL_GetWindowID(this->sdl_window);
	nk::EVENT_MUTEX.lock();
	nk::IS_EVENT_MUTEX_LOCKED = true;

	SDL_Event incoming_event;
	while (SDL_PollEvent(&incoming_event))
		nk::EVENT_STACK.push_back(incoming_event);

	for (size_t i_event = 0; i_event < nk::EVENT_STACK.size(); i_event++) {
		if (i_event >= nk::EVENT_STACK.size())
			break;

		SDL_Event& event = nk::EVENT_STACK[i_event];

		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.windowID != window_id)
				continue;

			switch (event.window.event) {
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				this->events.mouse_focus = true;
				this->events.handler.onMouseFocus(*this, true);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				this->events.mouse_focus = false;
				this->events.handler.onMouseFocus(*this, false);
				break;

			case SDL_WINDOWEVENT_ENTER:
				this->events.keyboard_focus = true;
				this->events.handler.onKeyboardFocus(*this, true);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				this->events.keyboard_focus = false;
				this->events.handler.onKeyboardFocus(*this, false);
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				this->events.minimized = true;
				this->events.handler.onMinimize(*this, true);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				this->events.minimized = false;
				this->events.handler.onMinimize(*this, false);
				break;

			case SDL_WINDOWEVENT_SHOWN:
				this->events.shown = true;
				this->events.handler.onShow(*this, true);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				this->events.shown = false;
				this->events.handler.onShow(*this, false);
				break;

			case SDL_WINDOWEVENT_CLOSE:
				this->events.handler.onQuit(*this);
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				this->events.handler.onResize(*this, Uint2{ event.window.data1, event.window.data2 });
				SDL_RenderPresent(this->sdl_renderer);
				break;
			} break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.windowID != window_id)
				continue;

			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				this->events.left_mouse_button_pressed = true;
				this->events.handler.onLeftMouseButtonPress(*this, true);
				break;
			case SDL_BUTTON_MIDDLE:
				this->events.middle_mouse_button_pressed = true;
				this->events.handler.onMiddleMouseButtonPress(*this, true);
				break;
			case SDL_BUTTON_RIGHT:
				this->events.right_mouse_button_pressed = true;
				this->events.handler.onRightMouseButtonPress(*this, true);
				break;
			} break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.windowID != window_id)
				continue;

			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				this->events.left_mouse_button_pressed = false;
				this->events.handler.onLeftMouseButtonPress(*this, false);
				break;
			case SDL_BUTTON_MIDDLE:
				this->events.middle_mouse_button_pressed = false;
				this->events.handler.onMiddleMouseButtonPress(*this, false);
				break;
			case SDL_BUTTON_RIGHT:
				this->events.right_mouse_button_pressed = false;
				this->events.handler.onRightMouseButtonPress(*this, false);
				break;
			} break;

		case SDL_MOUSEMOTION:
			if (event.motion.windowID != window_id)
				continue;

			this->events.mouse_position = Int2{ event.motion.x, event.motion.y };
			this->events.handler.onMouseMove(*this, Int2{ event.motion.x, event.motion.y }, Int2{ event.motion.xrel, event.motion.yrel });
			break;

		case SDL_MOUSEWHEEL:
			if (event.wheel.windowID != window_id)
				continue;

			this->events.handler.onMouseScroll(*this, event.wheel.x, event.wheel.y);
			break;

		case SDL_KEYDOWN:
			if (event.key.windowID != window_id)
				continue;
			
			this->events.key_pressed[event.key.keysym.sym] = true;
			this->events.handler.onKeyPress(*this, event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			if (event.key.windowID != window_id)
				continue;

			this->events.key_pressed[event.key.keysym.sym] = false;
			this->events.handler.onKeyPress(*this, event.key.keysym.sym, false);
			break;
			
		default:
			break;
		}

		if (this->is_destroyed)
			break;

		nk::EVENT_STACK.erase(nk::EVENT_STACK.begin() + i_event);
		i_event--;
	}

	nk::IS_EVENT_MUTEX_LOCKED = false;
	nk::EVENT_MUTEX.unlock();
}

void nk::Window::setResizable(const bool resizable) {
	if (!this->isValid())
		return;

	SDL_SetWindowResizable(this->sdl_window, (SDL_bool)resizable);
}

void nk::Window::setFullscreen(const bool fullscreen) {
	if (!this->isValid())
		return;

	#ifdef __EMSCRIPTEN__
	if (this->isFullscreen() != fullscreen)
		this->toggleFullscreen();
	#else
	SDL_SetWindowFullscreen(this->sdl_window, fullscreen);
	#endif
}

void nk::Window::toggleFullscreen() {
	if (!this->isValid())
		return;

	#ifdef __EMSCRIPTEN__
	Uint2 window_size = this->getSize();

	emscripten_run_script("Module.requestFullscreen(false, false)");
	emscripten_set_canvas_size((int)window_size.x, (int)window_size.y);
	#else
	SDL_SetWindowFullscreen(this->sdl_window, !this->isFullscreen());
	#endif
}

// Sets the size of the window
void nk::Window::setSize(const Uint2& size) {
	if (!this->isValid())
		return;

	SDL_SetWindowSize(this->sdl_window, size.x, size.y);
	#ifdef __EMSCRIPTEN__
	emscripten_set_canvas_size(size.x, size.y);
	#endif
}

// Sets the title of the window
void nk::Window::setTitle(const std::wstring& title) {
	if (!this->isValid())
		return;

	std::string utf8_title = nk::wstringToUtf8(title);
	SDL_SetWindowTitle(this->sdl_window, utf8_title.c_str());
}

// Gets the size of the window
Uint2 nk::Window::getSize() const {
	if (!this->isValid())
		return Uint2{ 0, 0 };

	int width, height;
	SDL_GetWindowSize(this->sdl_window, &width, &height);
	return Uint2{ width, height };
}

// Gets the title of the window
std::wstring nk::Window::getTitle() const {
	if (!this->isValid())
		return L"";

	std::string title = SDL_GetWindowTitle(this->sdl_window);
	return nk::utf8ToWstring(title);
}

// Checks if the window is resizable or not
bool nk::Window::isResizable() const {
	if (!this->isValid())
		return false;

	return (SDL_GetWindowFlags(this->sdl_window) & SDL_WINDOW_RESIZABLE) > 0;
}

// Checks if the window is on fullscreen or not
bool nk::Window::isFullscreen() const {
	if (!this->isValid())
		return false;

	#ifdef __EMSCRIPTEN__
	return EM_ASM_INT({ return document.fullscreen; });
	#else
	return (SDL_GetWindowFlags(this->sdl_window) & SDL_WINDOW_FULLSCREEN) > 0;
	#endif
}
