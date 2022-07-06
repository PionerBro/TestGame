#include "staticobject.h"
#include "sprite.h"

StaticObject::StaticObject(const std::shared_ptr<Sprite>& sprite, const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction)
	: GameObject(position, size, direction)
	, m_pSprite(sprite)
{
}
