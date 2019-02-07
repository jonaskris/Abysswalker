#include "PComponent.h"

namespace abyssengine {
	PComponent::PComponent() : Component(Component::componentType::PComponentType)
	{

	}

	PComponent::PComponent(math::vec3 position) : Component(Component::componentType::PComponentType)
	{
		this->position = position;
	}

	PComponent::~PComponent()
	{

	}

	void PComponent::update()
	{
		velocity += acceleration;
		position += velocity;
	}
}