//------------------------------------------------------------------------------
//
// File Name:	MouseFollow.cpp
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

#include "MouseFollow.h"

// Systems
#include "GameObject.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Parser.h>
#include <Engine.h>
#include <Graphics.h>
#include <Camera.h>

// Components
#include "Transform.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   forwardThrust = Amount of force added to the ship when thrusters are activated.
	//   maximumSpeed  = Maximum attainable speed of the ship.
	//   rotationSpeed = Speed at which the ship rotates.
	//   bulletSpeed   = Speed at which bullets move when fired by ship.
	MouseFollow::MouseFollow(float rotationSpeed) : Component("MouseFollow"), rotationSpeed(rotationSpeed), transform(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* MouseFollow::Clone() const
	{
		return new MouseFollow(*this);
	}

	// Initialize this component (happens at object creation).
	void MouseFollow::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void MouseFollow::Serialize(Parser& parser) const
	{
		parser.WriteVariable("rotationSpeed", rotationSpeed);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void MouseFollow::Deserialize(Parser& parser)
	{
		parser.ReadVariable("rotationSpeed", rotationSpeed);
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void MouseFollow::FixedUpdate(float dt)
	{
		Input& input = Input::GetInstance();
		Graphics& graphics = Graphics::GetInstance();
		transform->SetTranslation(graphics.ScreenToWorldPosition(input.GetCursorPosition(), &graphics.GetCurrentCamera()));

		if (input.IsKeyDown(VK_LBUTTON))
			transform->SetRotation(transform->GetRotation() + rotationSpeed * dt);

		if (input.IsKeyDown(VK_RBUTTON))
			transform->SetRotation(transform->GetRotation() - rotationSpeed * dt);
	}
}

//------------------------------------------------------------------------------
