#pragma once

#include <nsdl/declarations/rendertexture.hpp>


void nk::RenderTexture::_makeBlank(nk::Renderer& renderer, const Uint2& size) {
	this->nk_renderer = &renderer;
	if (!renderer.isValid())
		std::cerr << "NSDL: A render texture was created with an invalid renderer\n!";

	this->sdl_texture = SDL_CreateTexture(renderer.getSDLRenderer(), NSDL_PIXEL_FORMAT,
		SDL_TEXTUREACCESS_TARGET, size.x, size.y
	);
	if (this->sdl_texture == nullptr)
		std::cerr << "SDL: Unable to initialize an empty render texture, Error: " << SDL_GetError() << '\n';
}

void nk::RenderTexture::_loadSurface(nk::Renderer& renderer, nk::Surface& surface) {
	this->_makeBlank(renderer, surface.getSize());

	if (!this->isValid())
		return;

	if (!surface.isValid()) {
		std::cerr << "NSDL: A render texture was created with an invalid surface!\n";
		return;
	}

	nk::Texture surf_texture(renderer, surface);

	SDL_Texture* previous_target = SDL_GetRenderTarget(renderer.getSDLRenderer());
	this->target();
	SDL_RenderCopy(renderer.getSDLRenderer(), surf_texture.getSDLTexture(), nullptr, nullptr);
	SDL_SetRenderTarget(renderer.getSDLRenderer(), previous_target);
}

nk::RenderTexture::RenderTexture(nk::Renderer& renderer, nk::Texture& texture) {
	if (!texture.isValid()) {
		std::cerr << "NSDL: A render texture was created with an invalid texture!\n";
		return;
	}

	this->_makeBlank(renderer, texture.getSize());
	SDL_Texture* previous_target = SDL_GetRenderTarget(renderer.getSDLRenderer());
	this->target();
	SDL_RenderCopy(renderer.getSDLRenderer(), texture.getSDLTexture(), nullptr, nullptr);
	SDL_SetRenderTarget(renderer.getSDLRenderer(), previous_target);
}

nk::RenderTexture::RenderTexture(nk::Renderer& renderer, const std::wstring& file_name) {
	nk::Surface image_surface(file_name);

	if (!image_surface.isValid())
		return;

	this->_loadSurface(renderer, image_surface);
}

nk::RenderTexture::RenderTexture(nk::Renderer& renderer, nk::Font& font, const std::wstring& text, const Color& color) {
	nk::Surface text_surface(font, text, color);

	if (!text_surface.isValid())
		return;

	this->_loadSurface(renderer, text_surface);
}

// Targets the render texture as the render target
void nk::RenderTexture::target() {
	SDL_SetRenderTarget(this->nk_renderer->getSDLRenderer(), this->sdl_texture);
}
