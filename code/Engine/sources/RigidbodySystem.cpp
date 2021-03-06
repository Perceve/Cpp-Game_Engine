#include <RigidbodySystem.hpp>

namespace engine
{
	std::list<Rigidbody*> RigidbodySystem::rigidbodies;
	btDiscreteDynamicsWorld * RigidbodySystem::dynamicsWorld;

	/// <summary>
	/// Crea y prepara el mundo fisico
	/// </summary>
	RigidbodySystem::RigidbodySystem()
	{
		btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher * collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
		btDbvtBroadphase * overlappingPairCache = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver * constraintSolver = new btSequentialImpulseConstraintSolver();

		dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher,
													overlappingPairCache,
													constraintSolver,
													collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	}

	void RigidbodySystem::Run(bool& shouldExit)
	{
		UpdatePhysicWorld(1.0f / 60.0f);
	}

	/// <summary>
	/// Actualiza el mundo fisico segun la tasa de refreso, moviendo cada cuerpo fisico a su posicion primero segun el transform
	/// </summary>
	void RigidbodySystem::UpdatePhysicWorld(const float step)
	{
		
		for (auto& currentRigidbody : rigidbodies)
		{
			glm::vec3 worldPosition = currentRigidbody->gameobject->transform->position;
			currentRigidbody->body->getWorldTransform().setOrigin(btVector3(worldPosition.x, worldPosition.y, worldPosition.z));

			Transform * currentTransform = currentRigidbody->gameobject->transform.get();

			btQuaternion rotation = btQuaternion(currentTransform->rotation.x, currentTransform->rotation.y, 
												 currentTransform->rotation.z, currentTransform->rotation.w);

			currentRigidbody->body->getWorldTransform().setRotation(rotation);
		}

		dynamicsWorld->stepSimulation(step);

		for (auto& currentRigidbody : rigidbodies)
		{
			btVector3 worldPosition = currentRigidbody->body->getWorldTransform().getOrigin();
			currentRigidbody->gameobject->transform->position = glm::vec3(worldPosition.x(), worldPosition.y(), worldPosition.z());

			btQuaternion currentRotation = currentRigidbody->body->getWorldTransform().getRotation();
			currentRigidbody->gameobject->transform->rotation = glm::quat((float)currentRotation.getW(), (float)currentRotation.getX(),
																		  (float)currentRotation.getY(), (float)currentRotation.getZ());
		}
	}
}