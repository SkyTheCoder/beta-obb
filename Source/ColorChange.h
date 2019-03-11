//------------------------------------------------------------------------------
//
// File Name:	ColorChange.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class
#include <Color.h>	// Color

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Sprite;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class ColorChange : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   normalColor = The color that the object reverts to when not colliding.
		//   collidedColor = The color that the object changes to when colliding.
		//   collidedColorTime = The amount of time the object will retain the collided color.
		ColorChange(const Color& normalColor = Colors::Green, 
			const Color& collidedColor = Colors::Red, float collidedColorTime = 0.1f);

		// Return a new copy of the component.
		Component* Clone() const;

		// Initialize data for this object.
		void Initialize();

		// Loads object data from a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Deserialize(Parser& parser) override;

		// Saves object data to a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Serialize(Parser& parser) const override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Collision handler for ColorChange objects.
		// Params:
		//   object = The first object.
		//   other  = The other object the first object is colliding with.
		friend void ColorChangeCollisionHandler(GameObject& object, GameObject& other);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		Sprite* sprite;

		// Properties
		Color normalColor;
		Color collidedColor;
		float collidedColorTime;

		// Other variables
		bool collided;
		float timer;
	};
}
