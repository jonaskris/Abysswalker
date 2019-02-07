#pragma once
#include <iostream>
#include "../../math/vec3.h"
#include "Event.h"

namespace abyssengine {
	class Entity;
	struct Action
	{
		enum types
		{
			// No data
			// Float
			SETACCELERATION, CHANGEACCELERATION,	// Vec3
			SETVELOCITY, CHANGEVELOCITY,
			SETPOSITION, CHANGEPOSITION,
			ADDEVENT								// Event*
		};

		types type;

		Action(types type);

		virtual void executeOn(Entity* entity);

		types getType();
	};

	struct FloatAction : public Action
	{
		float scalar;

		FloatAction(types type, float scalar);

		void executeOn(Entity* entity);
	};

	struct VecAction : public Action
	{
		math::vec3 vector;

		VecAction(types type, math::vec3 vector);

		void executeOn(Entity* entity);
	};

	struct EventAction : public Action
	{
		Event* event = NULL;

		EventAction(types type, Event* vector);

		void executeOn(Entity* entity);
	};
}