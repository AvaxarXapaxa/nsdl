#pragma once

#include <nsdl/declarations/surface.hpp>
#include <nsdl/texture.hpp>


void nk::Surface::_makeBlank(const Uint2& size) {
	SDL_Surface* blank_surface = SDL_CreateRGBSurface(0, size.x, size.y, 1, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	if (blank_surface == nullptr) {
		std::cerr << "SDL: Could not make blank surface, Error: " << SDL_GetError() << '\n';
		return;
	}

	SDL_PixelFormat* format = SDL_AllocFormat(NSDL_PIXEL_FORMAT);
	this->sdl_surface = SDL_ConvertSurface(blank_surface, format, 0);
	SDL_FreeSurface(blank_surface);
	SDL_FreeFormat(format);

	if (this->sdl_surface == nullptr)
		std::cerr << "SDL: Could not convert the pixel format of the blank surface, Error: " << SDL_GetError() << '\n';
}

nk::Surface::Surface(nk::Texture& texture) {
	this->_makeBlank(texture.getSize());
	if (!this->isValid())
		return;

	this->interact([&](U32* pixels, Uint2 size) {
		size_t pixel_amount = (size_t)size.x * size.y;

		texture.interact([&](U32* texture_pixels, Uint2 size) {
			for (size_t i = 0; i < pixel_amount; i++)
				pixels[i] = texture_pixels[i];
		});
	});
}

nk::Surface::Surface(const std::wstring& file_name) {
	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	SDL_Surface* image_surf = IMG_Load(utf8_file_path.c_str());

	if (image_surf == nullptr)
		std::cerr << "SDL: Unable to load an image file \"" << utf8_file_path << "\", Error: " << IMG_GetError() << '\n';
	else {
		SDL_PixelFormat* format = SDL_AllocFormat(NSDL_PIXEL_FORMAT);
		this->sdl_surface = SDL_ConvertSurface(image_surf, format, 0);
		SDL_FreeSurface(image_surf);
		SDL_FreeFormat(format);

		if (this->sdl_surface == nullptr)
			std::cerr << "SDL: Unable to convert the format of a loaded image file surface, Error: " << SDL_GetError() << '\n';
	}
}

nk::Surface::Surface(nk::Font& font, const std::wstring& text, const Color& color) {
	std::string utf8_text = nk::wstringToUtf8(text);
	if (font.isValid()) {
		SDL_Surface* text_surf = TTF_RenderText_Solid(font.getSDLFont(), utf8_text.c_str(), SDL_Color{ color.r, color.g, color.b, color.a });
		if (text_surf == nullptr) {
			std::cerr << "SDL: Unable to render text from font into a surface, Error: " << TTF_GetError() << '\n';
			return;
		}

		SDL_PixelFormat* format = SDL_AllocFormat(NSDL_PIXEL_FORMAT);
		this->sdl_surface = SDL_ConvertSurface(text_surf, format, 0);
		SDL_FreeSurface(text_surf);
		SDL_FreeFormat(format);

		if (this->sdl_surface == nullptr)
			std::cerr << "SDL: Unable to convert the format of a rendered text surface, Error: " << SDL_GetError() << '\n';
	}
	else
		std::cerr << "NSDL: Surface was created with an invalid font!\n";
}

nk::Surface::Surface(const nk::Surface& copy) {
	if (this->sdl_surface != nullptr) {
		this->sdl_surface = SDL_DuplicateSurface(copy.sdl_surface);
		if (this->sdl_surface == nullptr)
			std::cerr << "SDL: Could not copy surface, Error: " << SDL_GetError() << '\n';
	}
}

nk::Surface::~Surface() {
	if (this->sdl_surface != nullptr)
		SDL_FreeSurface(this->sdl_surface);
}

// Blits/draws a surface to the current one at a position, with a scale, and a clipping rect of the source
void nk::Surface::blit(nk::Surface& source, const Int2& pos, const Vec2& scale, const nk::Rect& clip) {
	if (!this->isValid())
		return;

	Uint2 surface_size = this->getSize();
	SDL_Rect clip_rect = { (int)clip.position.x, (int)clip.position.y, (clip.position.x == 0) ? (int)surface_size.x : (int)clip.size.x, (clip.size.y == 0) ? (int)surface_size.y : (int)clip.size.y };
	SDL_Rect destination_rect = { (int)pos.x, (int)pos.y, (int)(clip_rect.w * scale.x), (int)(clip_rect.h * scale.y) };
	SDL_BlitSurface(source.sdl_surface, &clip_rect, this->sdl_surface, &destination_rect);
}

// Gets the surface's size
Uint2 nk::Surface::getSize() const {
	if (!this->isValid())
		return Uint2{ 0, 0 };

	return Uint2{ (U32)this->sdl_surface->w, (U32)this->sdl_surface->h };
}

// Lets a (lambda) function be given the surface's pixels content and allows it to read/write to it
void nk::Surface::interact(const std::function<void(U32*, Uint2)>& action) {
	if (!this->isValid())
		return;

	SDL_LockSurface(this->sdl_surface);

	action((U32*)this->sdl_surface->pixels, this->getSize());

	if (this->sdl_surface->locked)
		SDL_UnlockSurface(this->sdl_surface);
}

// Saves the surface to a PNG file
bool nk::Surface::saveToPNG(const std::wstring& file_name) {
	if (!this->isValid())
		return false;

	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	return IMG_SavePNG(this->sdl_surface, utf8_file_path.c_str()) == 0;
}
 
// Saves the surface to a BMP file
bool nk::Surface::saveToBMP(const std::wstring& file_name) {
	if (!this->isValid())
		return false;

	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	return SDL_SaveBMP(this->sdl_surface, utf8_file_path.c_str()) == 0;
}
