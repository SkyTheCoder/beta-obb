//------------------------------------------------------------------------------
//
// File Name:	Asteroid.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Asteroid.h"

// Systems
#include <GameObject.h>
#include <Engine.h>
#include <Parser.h>
#include <Random.h>
#include <Graphics.h>
#include <GameObjectManager.h>
#include <Space.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <Collider.h>
#include <ColliderCircle.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	// The collision handling function for Asteroids.
	// Params:
	//	 asteroid = The asteroid.
	//   otherObject = The other object.
	void CollisionHandlerAsteroid(GameObject& asteroid, GameObject& otherObject)
	{
		const std::string otherObjectName = otherObject.GetName();

		// Split if the asteroid collided with a bullet or a spaceship.
		if (otherObjectName == "Bullet" || otherObjectName == "PlayerShip")
		{
			asteroid.GetComponent<Asteroid>()->SpawnNewAsteroids();
			asteroid.Destroy();
		}
	}

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Asteroid constructor
	// Params:
	//   speedMin = Minimum of range for asteroid movement speed.
	//   speedMax = Maximum of range for asteroid movement speed.
	//   basePointsValue = Point value for largest asteroid.
	//   sizePointsModifier = Multiplicative scale applied to points for smaller asteroids.
	//   spawnScaleModifier = Percentage of original asteroid's scale to use for new asteroids.
	Asteroid::Asteroid(float speedMin, float speedMax, unsigned basePointsValue,
		float sizePointsModifier, float spawnScaleModifier)
		: Component("Asteroid"), speedMin(speedMin), speedMax(speedMax),
		basePointsValue(basePointsValue), sizePointsModifier(sizePointsModifier), spawnScaleModifier(spawnScaleModifier),
		size(SizeLarge), location(LocationTopLeft)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Asteroid::Clone() const
	{
		return new Asteroid(*this);
	}

	// Initialize this component (happens at object creation).
	void Asteroid::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();

		// Add the collision handler to the collider.
		GetOwner()->GetComponent<Collider>()->SetCollisionHandler(CollisionHandlerAsteroid);

		// Update position & velocity.
		SetPosition();
		SetVelocity();
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Asteroid::Serialize(Parser& parser) const
	{
		parser.WriteVariable("speedMin", speedMin);
		parser.WriteVariable("speedMax", speedMax);
		parser.WriteVariable("basePointsValue", basePointsValue);
		parser.WriteVariable("sizePointsModifier", sizePointsModifier);
		parser.WriteVariable("spawnScaleModifier", spawnScaleModifier);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Asteroid::Deserialize(Parser& parser)
	{
		parser.ReadVariable("speedMin", speedMin);
		parser.ReadVariable("speedMax", speedMax);
		parser.ReadVariable("basePointsValue", basePointsValue);
		parser.ReadVariable("sizePointsModifier", sizePointsModifier);
		parser.ReadVariable("spawnScaleModifier", spawnScaleModifier);
	}

	// Get the value of this object for increasing the player's score.
	unsigned Asteroid::GetPointValue() const
	{
		// Calculate the points based on size.
		switch (size)
		{
		case SizeLarge:
			return basePointsValue;
		case SizeMedium:
			return static_cast<unsigned>(basePointsValue * sizePointsModifier);
		case SizeSmall:
			return static_cast<unsigned>(basePointsValue * sizePointsModifier * sizePointsModifier);
		}

		// Shouldn't ever get here.
		return 0;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Set position of asteroid based on origin
	void Asteroid::SetPosition()
	{
		// Check if the asteroid's size is large.
		if (size == SizeLarge)
		{
			// Choose a random location.
			location = static_cast<Location>(RandomRange(0, 3));
			BoundingRectangle screenDimensions = Graphics::GetInstance().GetScreenWorldDimensions();

			float margin = 50.0f;

			// Set the translation to the appropriate screen corner.
			switch (location)
			{
			case LocationTopLeft:
				transform->SetTranslation(Vector2D(screenDimensions.left + margin, screenDimensions.top - margin));
				break;
			case LocationTopRight:
				transform->SetTranslation(Vector2D(screenDimensions.right - margin, screenDimensions.top - margin));
				break;
			case LocationBottomRight:
				transform->SetTranslation(Vector2D(screenDimensions.right - margin, screenDimensions.bottom + margin));
				break;
			case LocationBottomLeft:
				transform->SetTranslation(Vector2D(screenDimensions.left + margin, screenDimensions.bottom + margin));
				break;
			}
		}
	}

	// Set velocity based on size
	void Asteroid::SetVelocity()
	{
		float angle = RandomRange(0.0f, 360.0f);
		float speed = RandomRange(speedMin, speedMax);
		physics->SetVelocity(Vector2D::FromAngleDegrees(angle) * speed);
	}

	// Generate new asteroids based off this asteroid
	void Asteroid::SpawnNewAsteroids()
	{
		if (GetOwner()->IsDestroyed())
			return;

		// The number of clones to create.
		unsigned clones = 0;
		// The size of the clones.
		Size cloneSize = SizeLarge;

		// Choose random clone count based on this asteroid's size.
		switch (size)
		{
		case SizeLarge:
		{
			clones = RandomRange(2, 3);
			cloneSize = SizeMedium;
			break;
		}
		case SizeMedium:
		{
			clones = RandomRange(1, 2);
			cloneSize = SizeSmall;
			break;
		}
		}

		for (unsigned i = 0; i < clones; i++)
		{
			// Clone this object.
			GameObject* asteroid = new GameObject(*GetOwner());

			// Update the clone's scale.
			Transform* asteroidTransform = asteroid->GetComponent<Transform>();
			asteroidTransform->SetScale(transform->GetScale() * spawnScaleModifier);

			// Updat the clone's collider.
			ColliderCircle* asteroidCollider = asteroid->GetComponent<ColliderCircle>();
			asteroidCollider->SetRadius(asteroidTransform->GetScale().x / 2.0f);

			// Update the clone's size.
			Asteroid* asteroidAsteroid = asteroid->GetComponent<Asteroid>();
			asteroidAsteroid->size = cloneSize;

			// Add the clone to the object manager.
			GetOwner()->GetSpace()->GetObjectManager().AddObject(*asteroid);
		}
	}
}

//------------------------------------------------------------------------------
