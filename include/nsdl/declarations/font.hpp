#pragma once

#include <nsdl/collection.hpp>


namespace nk {
	class Font {
	protected:
		TTF_Font* sdl_font = nullptr;

		Font(const nk::Font&) {} // PLACEHOLDER
	public:
		Font(const std::wstring& file_name, const U16 size);
		~Font();

		void setStyle(const bool bold, const bool italic, const bool underlined, const bool strikethrough);

		bool isBold() const;
		bool isItalic() const;
		bool isUnderlined() const;
		bool isStrikethrough() const;

		inline TTF_Font* getSDLFont() { return this->sdl_font; }
		inline bool isValid() const { return this->sdl_font != nullptr; }
	};
}
