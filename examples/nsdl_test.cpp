#include <nsdl.hpp>


bool is_running = true;
double start = nk::getTime();
nk::Window* window;
nk::Texture* logo;
nk::Sound* explosion;
nk::Font* font;

void loop() {
	if (!is_running)
		return;

	window->clear();
	window->handleEvents();
	if (!is_running) return;

	double now = nk::getTime() - start;

	logo->setModulation(Color{ 255, 255, 255, 128 });
	logo->draw(Uint2{ 400, 300 }, now * 90, Uint2{ 192, 192 }, Vec2{ std::sin(now) / 2 + 1.5f, std::sin(now) / 2 + 1.5f }, false, false);
	logo->setModulation(Color{ 0, 0, 255, 255 });
	logo->draw(Uint2{ 400, 300 }, now * 90, Uint2{ 192, 192 }, Vec2{ std::sin(now) / 2 + 1.5f, std::sin(now) / 2 + 1.5f }, false, false, nk::Rect(64, 64, 128, 128));

	window->setTitle(L"FPS: " + std::to_wstring(window->fps));
	window->update();

}

int main() {
	nk::setup();

	window = new nk::Window(Uint2{ 800, 600 }, L"NSDL test!", true, false);
	logo = new nk::Texture(*window, L"examples/assets/neaxture.jpg");
	explosion = new nk::Sound(L"examples/assets/explosion.wav");
	font = new nk::Font(L"examples/assets/tahoma.ttf", 24);

	font->setStyle(true, true, true, false);

	window->events.handler.onQuit = [&](auto& window) {
		std::cout << "Closing window...\n";
		is_running = false;
		window.close();
	};

	window->events.handler.onLeftMouseButtonPress = [&](auto& window, auto pressed) {
		if (pressed) {
			std::cout << "BOOM!\n";
			explosion->play();
		}
	};

	window->events.handler.onMouseMove = [&](auto& window, auto position, auto relative) {
		nk::Texture(window, *font,
			L"I am moved to " + std::to_wstring((int)position.x) + L", " + std::to_wstring((int)position.y))
			.draw(position);
	};

	window->events.handler.onKeyPress = [&](auto& window, auto key, auto pressed) {
		if (pressed)
			switch (key) {
			case SDLK_UP:
				std::cout << "UP!\n";
				break;
			}
	};

	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(loop, 0, 1);
	#else
	while (is_running)
		loop();
	#endif

	delete window;
	delete logo;
	delete explosion;
	delete font;
}
