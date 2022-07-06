#pragma once
#include "gameobject.h"

#include <glm/vec3.hpp>
#include <memory>

class Sprite;

class StaticObject : public GameObject {
public:
	StaticObject(const std::shared_ptr<Sprite>& sprite, const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction);
	virtual ~StaticObject() = default;
protected:
	std::shared_ptr<Sprite> m_pSprite;
};