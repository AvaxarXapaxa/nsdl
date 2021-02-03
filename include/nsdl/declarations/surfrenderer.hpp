#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/renderer.hpp>
#include <nsdl/surface.hpp>


namespace nk {
	class SurfRenderer : public nk::Renderer {
	protected:
		nk::Surface* nk_surface = nullptr;
	public:
		SurfRenderer(nk::Surface& surface);
		virtual ~SurfRenderer();

		virtual Uint2 getSize() const { return this->nk_surface->getSize(); }

		inline nk::Surface& getSurface() { return *this->nk_surface; }
		inline bool isValid() const { return this->nk_surface->isValid() && this->sdl_renderer != nullptr; }
	};
}
