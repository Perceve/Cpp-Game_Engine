//Codigo propiedad de Aitor Izurrategui

#pragma once

#include <Transform.hpp>
#include <b2_body.h>

namespace engine
{
	/// <summary>
	/// Contiene la informacion necesaria para manejar los cuerpos rigidos
	/// </summary>
	class PhysicsComponent : public Component
	{
	public:

		b2Body * body;

		PhysicsComponent(Entity* entity);
		~PhysicsComponent() = default;

		void SetStatic(const bool _value)
		{
			if (_value)
			{
				body->SetType(b2_staticBody);
			}
			else
			{
				body->SetType(b2_dynamicBody);
			}
		}
	};
}