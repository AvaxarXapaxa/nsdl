#pragma once

#include <nsdl/declarations/renderer.hpp>


// Updates the changes
void nk::Renderer::update() {
	if (!this->isValid())
		return;

	SDL_RenderPresent(this->sdl_renderer);
}

// Targets the renderer as the render target
void nk::Renderer::target() {
	if (!this->isValid())
		return;

	SDL_SetRenderTarget(this->sdl_renderer, nullptr);
}

// Clears/fills the renderer with a color
void nk::Renderer::clear(const Color& color) {
	if (!this->isValid())
		return;

	SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(this->sdl_renderer);
}

// Draws a point/pixel at a specific position and a color
void nk::Renderer::drawPoint(const Int2& pos, const Color& color) {
	if (!this->isValid())
		return;

	SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(this->sdl_renderer, pos.x, pos.y);
}

// Draws a line from A to B with a color
void nk::Renderer::drawLine(const Int2& pos_a, const Int2& pos_b, const Color& color) {
	if (!this->isValid())
		return;

	SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(this->sdl_renderer, pos_a.x, pos_a.y, pos_b.x, pos_b.y);
}

// Draws an outline of a rectangle with a specific position and size with a color
void nk::Renderer::drawRect(const nk::Rect& rect, const Color& color) {
	if (!this->isValid())
		return;

	SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, color.a);

	SDL_Rect sdl_rect = rect.getSDLRect();
	SDL_RenderDrawRect(this->sdl_renderer, &sdl_rect);
}

// Draws a filled rectangle with a specific position and size with a color
void nk::Renderer::drawFilledRect(const nk::Rect& rect, const Color& color) {
	if (!this->isValid())
		return;

	SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, color.a);

	SDL_Rect sdl_rect = rect.getSDLRect();
	SDL_RenderFillRect(this->sdl_renderer, &sdl_rect);
}

// Sets the viewport rect
void nk::Renderer::setViewport(const nk::Rect& rect) {
	if (!this->isValid())
		return;

	SDL_Rect sdl_rect = rect.getSDLRect();
	SDL_RenderSetViewport(this->sdl_renderer, &sdl_rect);
}

// Gets the viewport rect
nk::Rect nk::Renderer::getViewport() const {
	if (!this->isValid())
		return nk::Rect(0, 0, 0, 0);

	SDL_Rect sdl_rect = { 0, 0, 0, 0 };
	SDL_RenderGetViewport(this->sdl_renderer, &sdl_rect);

	return nk::Rect(sdl_rect.x, sdl_rect.y, sdl_rect.w, sdl_rect.h);
}
