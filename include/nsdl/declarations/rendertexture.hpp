#pragma once

#include <nsdl/collection.hpp>
#include <nsdl/texture.hpp>


namespace nk {
	class RenderTexture : public nk::Texture {
	protected:
		// PLACEHOLDERS
		RenderTexture(nk::RenderTexture&) {}
		void interact(const std::function<void(U32*, Uint2)>& action) {}
		bool saveToPNG(const std::wstring& file_name) { return false; }
		bool saveToBMP(const std::wstring& file_name) { return false; }

		void _makeBlank(nk::Renderer& renderer, const Uint2& size);
		void _loadSurface(nk::Renderer& renderer, nk::Surface& surface);
	public:
		RenderTexture(nk::Renderer& renderer, const Uint2& size) { this->_makeBlank(renderer, size); }
		RenderTexture(nk::Renderer& renderer, nk::Surface& surface) { this->_loadSurface(renderer, surface); }
		RenderTexture(nk::Renderer& renderer, nk::Texture& texture);

		RenderTexture(nk::Renderer& renderer, const std::wstring& file_name);
		RenderTexture(nk::Renderer& renderer, nk::Font& font, const std::wstring& text, const Color& color = Color{ 255, 255, 255, 255 });

		void target();
	};
}
