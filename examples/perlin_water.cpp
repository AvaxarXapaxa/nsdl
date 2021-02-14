#include <nsdl.hpp>


bool is_running = true;
double start = nk::getTime();

float x_scale = 4, y_scale = 16;

nk::Window* window;
nk::Texture* perlin;
nk::Texture* monke;
nk::Texture* sand;

void loop() {
	if (!is_running)
		return;

	window->clear();
	window->handleEvents();
	if (!is_running) return;

	double now = nk::getTime() - start;

	sand->draw(Int2{ 0, 0 }, Vec2{ 1.3f, 1.3f });
	monke->draw(Int2{ 256, 427 }, 0.0, Int2{ 256, 256 }, Vec2{ .75f, .75f });
	perlin->interact([&](U32* pixels, Uint2 size) {
		for (U32 x = 0; x < size.x; x++) {
			for (U32 y = 0; y < size.y; y++) {
				U32& pixel = pixels[y * size.x + x];
				U8& blue = *((U8*)(&pixel) + 0);
				U8& green   = *((U8*)(&pixel) + 1);
				U8& red = *((U8*)(&pixel) + 2);
				U8& alpha  = *((U8*)(&pixel) + 3);

				float noise = glm::simplex(Vec3{ (float)x * x_scale / size.x, (float)y * y_scale / size.y, now }) * 127 + 127;
				red = noise * .6f;
				green = noise * .6f;
				blue = 255;
				alpha = noise / 2.f + 127;
			}
		}
	});
	perlin->draw(Int2{ 0, 0 });

	window->setTitle(L"FPS: " + std::to_wstring(window->fps));
	window->update();
}

int main() {
	nk::setup();

	window = new nk::Window(Uint2{ 512, 512 }, L"Perlin noise", false, false);
	perlin = new nk::Texture(*window, Uint2{ 512, 512 });
	monke = new nk::Texture(*window, L"examples/assets/monke.png");
	sand = new nk::Texture(*window, L"examples/assets/sand.jpg");

	SDL_SetTextureBlendMode(perlin->getSDLTexture(), SDL_BLENDMODE_BLEND);

	window->events.handler.onQuit = [&](auto& window) {
		is_running = false;
		window.close();
	};
	window->events.handler.onKeyPress = [&](auto& window, auto key, auto pressed) {
		if (pressed) {
			switch (key) {
			case SDLK_UP:
				y_scale += 1;
				break;

			case SDLK_DOWN:
				y_scale -= 1;
				break;

			case SDLK_LEFT:
				x_scale -= 1;
				break;

			case SDLK_RIGHT:
				x_scale += 1;
				break;
			}
		}
	};

	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(loop, 0, 1);
	#else
	while (is_running)
		loop();
	#endif

	delete perlin;
	delete monke; // ;( we'll miss you
	delete sand;
	delete window;
}
