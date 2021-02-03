#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/font.hpp>


namespace nk {
	class Texture;

	class Surface {
	protected:
		SDL_Surface* sdl_surface = nullptr;

		void _makeBlank(const Uint2& size);
	public:
		Surface(const Uint2& size) { this->_makeBlank(size); }
		Surface(nk::Texture& texture);
		Surface(const std::wstring& file_name);
		Surface(nk::Font& font, const std::wstring& text, const Color& color = Color{ 255, 255, 255, 255 });
		Surface(const nk::Surface& copy);
		virtual ~Surface();

		void blit(nk::Surface& source, const Int2& pos, 
			const Vec2& scale = Vec2{ 1.f, 1.f }, const nk::Rect& clip = nk::Rect(0, 0, 0, 0));

		Uint2 getSize() const;

		void interact(const std::function<void(U32*, Uint2)>& action);
		bool saveToPNG(const std::wstring& file_name);
		bool saveToBMP(const std::wstring& file_name);

		inline SDL_Surface* getSDLSurface() { return this->sdl_surface; }
		inline bool isValid() const { return this->sdl_surface != nullptr; }
	};
}
