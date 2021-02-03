#pragma once

#include <nsdl/declarations/font.hpp>


nk::Font::Font(const std::wstring& file_name, const U16 size) {
	std::string utf8_file_path = nk::wstringToUtf8(file_name);
	this->sdl_font = TTF_OpenFont(utf8_file_path.c_str(), size);
	if (this->sdl_font == nullptr)
		std::cerr << "SDL: Could not load font file, Error: " << TTF_GetError() << '\n';
}

nk::Font::~Font() {
	if (this->sdl_font != nullptr)
		TTF_CloseFont(this->sdl_font);
}

// Sets the style of the font
void nk::Font::setStyle(const bool bold, const bool italic, const bool underlined, const bool strikethrough) {
	if (!this->isValid())
		return;

	TTF_SetFontStyle(this->sdl_font,
		(bold ? TTF_STYLE_BOLD : TTF_STYLE_NORMAL) |
		(italic ? TTF_STYLE_ITALIC : 0) |
		(underlined ? TTF_STYLE_UNDERLINE : 0) |
		(strikethrough ? TTF_STYLE_STRIKETHROUGH : 0)
	);
}

// Returns if the font is bold or not
bool nk::Font::isBold() const {
	if (!this->isValid())
		return false;

	return (TTF_GetFontStyle(this->sdl_font) & TTF_STYLE_BOLD) != 0;
}

// Returns if the font is italic or not
bool nk::Font::isItalic() const {
	if (!this->isValid())
		return false;

	return (TTF_GetFontStyle(this->sdl_font) & TTF_STYLE_ITALIC) != 0;
}

// Returns if the font is underlined or not
bool nk::Font::isUnderlined() const {
	if (!this->isValid())
		return false;

	return (TTF_GetFontStyle(this->sdl_font) & TTF_STYLE_UNDERLINE) != 0;
}

// Returns if the font is strikethrough or not
bool nk::Font::isStrikethrough() const {
	if (!this->isValid())
		return false;

	return (TTF_GetFontStyle(this->sdl_font) & TTF_STYLE_STRIKETHROUGH) != 0;
}
