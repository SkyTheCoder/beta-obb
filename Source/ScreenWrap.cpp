//------------------------------------------------------------------------------
//
// File Name:	ScreenWrap.cpp
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

#include "ScreenWrap.h"

// Systems
#include "GameObject.h"
#include <Graphics.h>

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	ScreenWrap::ScreenWrap() : Component("ScreenWrap")
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* ScreenWrap::Clone() const
	{
		return new ScreenWrap(*this);
	}

	// Initialize this component (happens at object creation).
	void ScreenWrap::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		physics = GetOwner()->GetComponent<Physics>();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void ScreenWrap::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Vector2D translation = transform->GetTranslation();
		Vector2D scale = transform->GetScale();
		Vector2D velocity = physics->GetVelocity();
		BoundingRectangle screenDimensions = Graphics::GetInstance().GetScreenWorldDimensions();

		// Check if the object is moving off the right edge of the screen, and wrap around if so.
		if (velocity.x > 0.0f && translation.x > screenDimensions.right + scale.x)
		{
			translation.x = screenDimensions.left - scale.x;
		}

		// Check if the object is moving off the left edge of the screen, and wrap around if so.
		if (velocity.x < 0.0f && translation.x < screenDimensions.left - scale.x)
		{
			translation.x = screenDimensions.right + scale.x;
		}

		// Check if the object is moving off the top edge of the screen, and wrap around if so.
		if (velocity.y > 0.0f && translation.y > screenDimensions.top + scale.y)
		{
			translation.y = screenDimensions.bottom - scale.y;
		}

		// Check if the object is moving off the bottom edge of the screen, and wrap around if so.
		if (velocity.y < 0.0f && translation.y < screenDimensions.bottom - scale.y)
		{
			translation.y = screenDimensions.top + scale.y;
		}

		// Set the udpated translation.
		transform->SetTranslation(translation);
	}
}

//------------------------------------------------------------------------------
