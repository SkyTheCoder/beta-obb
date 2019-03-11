//------------------------------------------------------------------------------
//
// File Name:	ColorChange.cpp
// Author(s):	David Cohen (david.cohen)
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

#include "ColorChange.h"

// Systems
#include "GameObject.h"
#include <Parser.h>

// Components
#include "Sprite.h"
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	void ColorChangeCollisionHandler(GameObject& object, GameObject& other);

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   normalColor = The color that the object reverts to when not colliding.
	//   collidedColor = The color that the object changes to when colliding.
	//   collidedColorTime = The amount of time the object will retain the collided color.
	ColorChange::ColorChange(const Color& normalColor, const Color& collidedColor, float collidedColorTime) :
		Component("ColorChange"), sprite(nullptr), normalColor(normalColor), collidedColor(collidedColor),
		collidedColorTime(collidedColorTime), collided(false), timer(0.0f)
	{
	}

	// Return a new copy of the component.
	Component* ColorChange::Clone() const
	{
		return new ColorChange(*this);
	}

	// Initialize data for this object.
	void ColorChange::Initialize()
	{
		// Store the required components for ease of access.
		sprite = GetOwner()->GetComponent<Sprite>();

		// Set the object's collision handler to ours.
		Collider* collider = GetOwner()->GetComponent<Collider>();
		collider->SetCollisionHandler(&Behaviors::ColorChangeCollisionHandler);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void ColorChange::Deserialize(Parser& parser)
	{
		parser.ReadVariable("normalColor", normalColor);
		parser.ReadVariable("collidedColor", collidedColor);
		parser.ReadVariable("collidedColorTime", collidedColorTime);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void ColorChange::Serialize(Parser& parser) const
	{
		parser.WriteVariable("normalColor", normalColor);
		parser.WriteVariable("collidedColor", collidedColor);
		parser.WriteVariable("collidedColorTime", collidedColorTime);
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void ColorChange::Update(float dt)
	{
		if (collided)
		{
			// If the object is colliding with something, change its color.
			sprite->SetColor(collidedColor);

			// Mark that we have acknowledged this collision.
			collided = false;

			// Set the timer so the color persists for a short time.
			timer = collidedColorTime;
		}
		else if (timer <= 0.0f)
		{
			// Restore the object's color to its old color if the object has not collided with anything recently.
			sprite->SetColor(normalColor);
		}

		timer -= dt;
	}

	// Collision handler for ColorChange objects.
	// Params:
	//   object = The first object.
	//   other  = The other object the first object is colliding with.
	void ColorChangeCollisionHandler(GameObject& object, GameObject& other)
	{
		UNREFERENCED_PARAMETER(other);

		// Mark the object as collided.
		ColorChange* colorChange = object.GetComponent<ColorChange>();
		colorChange->collided = true;
	}
}
