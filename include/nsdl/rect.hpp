#pragma once

#include <nsdl/declarations/rect.hpp>


// Returns if the other rect collides with the current rect
bool nk::Rect::collide(const nk::Rect& rect) const {
	SDL_Rect this_rect = this->getSDLRect();
	SDL_Rect other_rect = rect.getSDLRect();

	return SDL_HasIntersection(&this_rect, &other_rect);
}

// Returns if a point collides (is inside) the rect
bool nk::Rect::collide(const Int2& point) const {
	return
		(
			(this->position.x < point.x && point.x < this->position.x + this->size.x) ||
			(this->position.x + this->size.x < point.x && point.x < this->position.x) // In case the size is negative
		) &&
		(
			(this->position.y < point.y && point.y < this->position.y + this->size.y) ||
			(this->position.y + this->size.y < point.y && point.y < this->position.y)
		);
}

// Returns the intersection rect with the other rect
nk::Rect nk::Rect::intersect(const nk::Rect& rect, bool* has_intersection) const {
	SDL_Rect this_rect = this->getSDLRect();
	SDL_Rect other_rect = rect.getSDLRect();
	SDL_Rect intersection_rect = { 0, 0, 0, 0 };

	bool intersection = SDL_IntersectRect(&this_rect, &other_rect, &intersection_rect);
	if (has_intersection != nullptr)
		*has_intersection = intersection;

	return nk::Rect(intersection_rect.x, intersection_rect.y, intersection_rect.w, intersection_rect.h);
}

// Returns the bounding/union rect of the current and other rect
nk::Rect nk::Rect::unite(const nk::Rect& rect) const {
	SDL_Rect this_rect = this->getSDLRect();
	SDL_Rect other_rect = rect.getSDLRect();
	SDL_Rect union_rect = { 0, 0, 0, 0 };

	SDL_UnionRect(&this_rect, &other_rect, &union_rect);
	return nk::Rect(union_rect.x, union_rect.y, union_rect.w, union_rect.h);
}
