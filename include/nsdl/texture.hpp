#pragma once

#include <nsdl/declarations/texture.hpp>


void nk::Texture::_makeBlank(nk::Renderer& renderer, const Uint2& size) {
	this->nk_renderer = &renderer;
	if (!renderer.isValid())
		std::cerr << "NSDL: A texture was created with an invalid renderer\n!";

	this->sdl_texture = SDL_CreateTexture(renderer.getSDLRenderer(), NSDL_PIXEL_FORMAT,
		SDL_TEXTUREACCESS_STREAMING, size.x, size.y
	);
	if (this->sdl_texture == nullptr)
		std::cerr << "SDL: Unable to initialize an empty texture, Error: " << SDL_GetError() << '\n';
}

void nk::Texture::_loadSurface(nk::Renderer& renderer, nk::Surface& surface) {
	this->_makeBlank(renderer, surface.getSize());

	if (!this->isValid())
		return;

	if (!surface.isValid()) {
		std::cerr << "NSDL: A texture was created with an invalid surface!\n";
		return;
	}

	this->interact([&](U32* texture_pixels, Uint2 size) {
		size_t pixel_amount = (size_t)size.x * size.y;

		surface.interact([&](U32* pixels, Uint2 size) {
			for (size_t i = 0; i < pixel_amount; i++)
				texture_pixels[i] = pixels[i];
		});
	});
}

nk::Texture::Texture(nk::Texture& copy) : Texture(*copy.nk_renderer, copy.getSize()) {
	if (!this->isValid())
		return;

	this->interact([&](U32* dest_pixels, Uint2 dest_size) {
		copy.interact([&](U32* pixels, Uint2 size) {
			size_t pixel_amount = (size_t)size.x * size.y;

			for (size_t i = 0; i < pixel_amount; i++)
				dest_pixels[i] = pixels[i];
		});
	});
}

nk::Texture::Texture(nk::Renderer& renderer, const std::wstring& file_name) {
	nk::Surface image_surface(file_name);
	
	if (!image_surface.isValid())
		return;

	this->_loadSurface(renderer, image_surface);
}

nk::Texture::Texture(nk::Renderer& renderer, nk::Font& font, const std::wstring& text, const Color& color) {
	nk::Surface text_surface(font, text, color);

	if (!text_surface.isValid())
		return;

	this->_loadSurface(renderer, text_surface);
}

nk::Texture::~Texture() {
	if (this->sdl_texture != nullptr)
		SDL_DestroyTexture(this->sdl_texture);
}

// Sets the color/alpha modulation
void nk::Texture::setModulation(Color filter_color) {
	if (!this->isValid())
		return;

	SDL_SetTextureColorMod(this->sdl_texture, filter_color.r, filter_color.g, filter_color.b);
	SDL_SetTextureAlphaMod(this->sdl_texture, filter_color.a);
}

// Draws the texture to the render target at a position, with a scale, and a clipping rect of the current texture
void nk::Texture::draw(const Int2& pos, const Vec2& scale, const nk::Rect& clip) {
	if (!this->isValid())
		return;

	Uint2 texture_size = this->getSize();
	SDL_Rect clip_rect = { (int)clip.position.x, (int)clip.position.y, (clip.position.x == 0) ? (int)texture_size.x : (int)clip.size.x, (clip.size.y == 0) ? (int)texture_size.y : (int)clip.size.y };
	SDL_Rect destination_rect = { (int)pos.x, (int)pos.y, (int)(clip_rect.w * scale.x), (int)(clip_rect.h * scale.y) };
	SDL_RenderCopy(this->nk_renderer->getSDLRenderer(), this->sdl_texture, &clip_rect, &destination_rect);
}

// Draws the texture to the render target at a position and angle according to the origin, with a scale, additional clippings, and a clipping rect of the current texture
void nk::Texture::draw(const Int2& pos, const double angle, const Int2& origin,
	const Vec2& scale, const bool flip_x, const bool flip_y, const nk::Rect& clip) {

	if (!this->isValid())
		return;

	Uint2 texture_size = this->getSize();

	// Some explicit casts because GCC compilers aren't that smart.
	SDL_Rect clip_rect = { (int)clip.position.x, (int)clip.position.y, (clip.position.x == 0) ? (int)texture_size.x : (int)clip.size.x, (clip.size.y == 0) ? (int)texture_size.y : (int)clip.size.y };
	SDL_Rect destination_rect = { (int)pos.x - (int)(origin.x * scale.x), (int)pos.y - (int)(origin.y * scale.y), (int)(clip_rect.w * scale.x), (int)(clip_rect.h * scale.y) };
	SDL_Point origin_point = { (int)(origin.x * scale.x), (int)(origin.y * scale.y) };

	SDL_RenderCopyEx(
		this->nk_renderer->getSDLRenderer(), this->sdl_texture,
		&clip_rect, &destination_rect,
		angle, &origin_point,
		(SDL_RendererFlip)((flip_x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flip_y ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
	);
}

// Gets the texture's size
Uint2 nk::Texture::getSize() const {
	if (!this->isValid())
		return Uint2{ 0, 0 };

	int width, height;
	SDL_QueryTexture(this->sdl_texture, nullptr, nullptr, &width, &height);
	return Uint2{ width, height };
}

// Gets the texture's color/alpha modulation
Color nk::Texture::getModulation() const {
	if (!this->isValid())
		return Color{ 0, 0, 0, 0 };

	U8 red, green, blue, alpha;
	SDL_GetTextureColorMod(this->sdl_texture, &red, &green, &blue);
	SDL_GetTextureAlphaMod(this->sdl_texture, &alpha);

	return Color{ red, green, blue, alpha };
}

// Lets a (lambda) function be given the texture's pixels content and allows it to read/write to it
void nk::Texture::interact(const std::function<void(U32*, Uint2)>& action) {
	if (!this->isValid())
		return;

	void* content = nullptr;
	int pitch;

	SDL_LockTexture(this->sdl_texture, nullptr, &content, &pitch);

	if (content == nullptr) {
		std::cerr << "SDL: Could not lock a texture to be interacted with, Error: " << SDL_GetError() << '\n';
		return;
	}

	action((U32*)content, this->getSize());

	SDL_UnlockTexture(this->sdl_texture);
}

// Saves the texture to a PNG file
bool nk::Texture::saveToPNG(const std::wstring& file_name) {
	if (!this->isValid())
		return false;

	nk::Surface file_surface(*this);
	return file_surface.saveToPNG(file_name);
}

// Saves the texture to a BMP file
bool nk::Texture::saveToBMP(const std::wstring& file_name) {
	if (!this->isValid())
		return false;

	nk::Surface file_surface(*this);
	return file_surface.saveToBMP(file_name);
}
