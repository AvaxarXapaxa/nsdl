#pragma once

#include <nsdl/declarations/surfrenderer.hpp>


nk::SurfRenderer::SurfRenderer(nk::Surface& surface) {
	if (!surface.isValid()) {
		std::cerr << "NSDL: A SurfRenderer was initialized with an invalid surface!\n";
		return;
	}

	this->nk_surface = &surface;

	this->sdl_renderer = SDL_CreateSoftwareRenderer(surface.getSDLSurface());
	if (this->sdl_renderer == nullptr)
		std::cerr << "SDL: Unable to make a software renderer, Error: " << SDL_GetError() << '\n';
}

nk::SurfRenderer::~SurfRenderer() {
	if (this->sdl_renderer != nullptr)
		SDL_DestroyRenderer(this->sdl_renderer);
}
