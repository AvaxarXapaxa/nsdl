#pragma once

#include <nsdl/collection.hpp>


namespace nk {
	struct Rect {
		Int2 position;
		Int2 size;

		Rect(const Int2 _position, const Int2 _size) : position(_position), size(_size) {}
		Rect(const int x, const int y, const int width, const int height) : position(Int2{ x, y }), size(Int2{ width, height }) {}

		bool collide(const nk::Rect& rect) const;
		bool collide(const Int2& point) const;
		nk::Rect intersect(const nk::Rect& rect, bool* has_intersection = nullptr) const;
		nk::Rect unite(const nk::Rect& rect) const;

		inline Int2 topLeft() const { return Int2{ std::min(this->position.x, this->position.x + this->size.x), std::min(this->position.y, this->position.y + this->size.y) }; }
		inline Int2 topRight() const { return Int2{ std::max(this->position.x, this->position.x + this->size.x), std::min(this->position.y, this->position.y + this->size.y) }; }
		inline Int2 bottomLeft() const { return Int2{ std::min(this->position.x, this->position.x + this->size.x), std::max(this->position.y, this->position.y + this->size.y) }; }
		inline Int2 bottomRight() const { return Int2{ std::max(this->position.x, this->position.x + this->size.x), std::max(this->position.y, this->position.y + this->size.y) }; }

		inline SDL_Rect getSDLRect() const { return SDL_Rect{ this->position.x, this->position.y, this->size.x, this->size.y }; }
	};
}
