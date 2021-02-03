#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/rect.hpp>


namespace nk {
	class Renderer {
	protected:
		SDL_Renderer* sdl_renderer = nullptr;
	public:
		virtual ~Renderer() {};

		void update();
		void target();

		void clear(const Color& color = Color{ 0, 0, 0, 0 });
		void drawPoint(const Int2& pos, const Color& color);
		void drawLine(const Int2& pos_a, const Int2& pos_b, const Color& color);
		void drawRect(const nk::Rect& rect, const Color& color);
		void drawFilledRect(const nk::Rect& rect, const Color& color);

		void setViewport(const nk::Rect& rect);
		nk::Rect getViewport() const;

		virtual Uint2 getSize() const { return Uint2{ 0, 0 }; };

		inline SDL_Renderer* getSDLRenderer() { return this->sdl_renderer; }
		inline bool isValid() const { return this->sdl_renderer != nullptr; }
	};
}
