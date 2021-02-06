#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/rect.hpp>
#include <nsdl/renderer.hpp>
#include <nsdl/surface.hpp>


namespace nk {
	class Texture {
	protected:
		SDL_Texture* sdl_texture = nullptr;
		nk::Renderer* nk_renderer = nullptr;

		Texture() {} // PLACEHOLDER

		void _makeBlank(nk::Renderer& renderer, const Uint2& size);
		void _loadSurface(nk::Renderer& renderer, nk::Surface& surface);
	public:
		Texture(nk::Renderer& renderer, const Uint2& size) { this->_makeBlank(renderer, size); }
		Texture(nk::Renderer& renderer, nk::Surface& surface) { this->_loadSurface(renderer, surface); }
		Texture(nk::Texture& copy);

		Texture(nk::Renderer& renderer, const std::wstring& file_name);
		Texture(nk::Renderer& renderer, nk::Font& font, const std::wstring& text, const Color& color = Color{ 255, 255, 255, 255 });
		
		virtual ~Texture();

		void setModulation(Color filter_color);

		void draw(const Int2& pos, const Vec2& scale = Vec2{ 1.f, 1.f },
			const nk::Rect& clip = nk::Rect(0, 0, 0, 0));
		void draw(const Int2& pos, const double angle, const Int2& origin,
			const Vec2& scale = Vec2{ 1.f, 1.f },
			const bool flip_x = false, const bool flip_y = false,
			const nk::Rect& clip = nk::Rect(0, 0, 0, 0));

		Uint2 getSize() const;
		Color getModulation() const;

		void interact(const std::function<void(U32*, Uint2)>& action);
		bool saveToPNG(const std::wstring& file_name);
		bool saveToBMP(const std::wstring& file_name);

		inline SDL_Texture* getSDLTexture() { return this->sdl_texture; }
		inline nk::Renderer& getRenderer() { return *this->nk_renderer; }
		inline bool isValid() const { return this->sdl_texture != nullptr && this->nk_renderer != nullptr && this->nk_renderer->isValid(); }
	};
}
