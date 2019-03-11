//------------------------------------------------------------------------------
//
// File Name:	GameObject.cpp
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

#include "GameObject.h"

// Systems
#include "Component.h"
#include "Space.h"
#include "Parser.h"
#include "GameObjectFactory.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new game object.
// Params:
//	 name = The name of the game object being created.
GameObject::GameObject(const std::string& name) : BetaObject(name), isDestroyed(false)
{
}

// Clone a game object from another game object.
// Params:
//	 other = A reference to the object being cloned.
GameObject::GameObject(const GameObject& other) : BetaObject(other.GetName(), other.GetParent()), isDestroyed(false)
{
	// Reserve how many components we need so there's only 1 allocation.
	components.reserve(other.components.size());

	// Loop through all of the other game object's components and clone them.
	for (auto it = other.components.begin(); it != other.components.end(); it++)
	{
		Component* component = (*it)->Clone();
		component->SetParent(this);
		components.push_back(component);
	}
}

// Free the memory associated with a game object.
GameObject::~GameObject()
{
	// Destroy all components.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		delete *it;
	}
}

// Initialize this object's components and set it to active.
void GameObject::Initialize()
{
	// Initialize all components.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Initialize();
	}
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void GameObject::Deserialize(Parser& parser)
{
	parser.ReadSkip(GetName());
	parser.ReadSkip('{');

	// Read the number of components in the file.
	unsigned numComponents;
	parser.ReadVariable("numComponents", numComponents);

	// Reserve how many components we need so there's only 1 allocation.
	components.reserve(numComponents);

	for (unsigned i = 0; i < numComponents; i++)
	{
		// Read the next component's name.
		std::string componentName;
		parser.ReadValue(componentName);

		// Attempt to create the specified component.
		Component* component = GameObjectFactory::GetInstance().CreateComponent(componentName);
		if (component == nullptr)
		{
			throw ParseException(GetName(), "Could not find component: " + componentName);
		}

		AddComponent(component);

		// Deserialize the component from the file.
		parser.ReadSkip('{');
		component->Deserialize(parser);
		parser.ReadSkip('}');
	}

	parser.ReadSkip('}');
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void GameObject::Serialize(Parser& parser) const
{
	parser.WriteValue(GetName());
	parser.BeginScope();

	// Write the number of components to the file.
	unsigned numComponents = static_cast<unsigned>(components.size());
	parser.WriteVariable("numComponents", numComponents);

	for (unsigned i = 0; i < numComponents; i++)
	{
		// Write the next component's name.
		parser.WriteValue(std::string(typeid(*components[i]).name()).substr(6));

		// Serialize the component to the file.
		parser.BeginScope();
		components[i]->Serialize(parser);
		parser.EndScope();
	}

	parser.EndScope();
}

// Update any components attached to the game object.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObject::Update(float dt)
{
	if (isDestroyed)
		return;

	// Call the Update function on every component.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}
}

// Updates components using a fixed timestep (usually just physics)
// Params:
//	 dt = A fixed change in time, usually 1/60th of a second.
void GameObject::FixedUpdate(float dt)
{
	if (isDestroyed)
		return;

	// Call the FixedUpdate function on every component.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->FixedUpdate(dt);
	}
}

// Draw any visible components attached to the game object.
void GameObject::Draw()
{
	// Call the Draw function on every component.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		(*it)->Draw();
	}
}

// Adds a component to the object.
void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
	component->SetParent(this);
}

// Retrieves the component with the given name if it exists.
// Params:
//   name = The name of the component to find.
// Returns:
//  A pointer to the component if it exists, nullptr otherwise.
Component* GameObject::GetComponent(const std::string& name_)
{
	// Loop through every component and check if its name matches.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetName() == name_)
			return *it;
	}

	return nullptr;
}

// Mark an object for destruction.
void GameObject::Destroy()
{
	isDestroyed = true;
}

// Whether the object has been marked for destruction.
// Returns:
//		True if the object will be destroyed, false otherwise.
bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

// Get the space that contains this object.
Space* GameObject::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

//------------------------------------------------------------------------------
