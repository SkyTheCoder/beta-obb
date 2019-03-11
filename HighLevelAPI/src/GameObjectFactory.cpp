//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.cpp
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

#include "GameObjectFactory.h"

// Systems
#include "GameObject.h"
#include "Component.h"
#include <Mesh.h>
#include "SpriteSource.h"
#include "Parser.h"
#include "Sprite.h"

// Components
#include "ColliderCircle.h"
#include "ColliderPoint.h"
#include "ColliderRectangle.h"
#include "ColliderTilemap.h"
#include "ColliderLine.h"
#include "Transform.h"
#include "Animation.h"
#include "Sprite.h"
#include "SpriteTilemap.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a single instance of the specified game object.
// Loads the object from a text file (if it exists).
// Params:
//   name = The name of the object.
//   mesh = The mesh used for the object's sprite (if any).
//   spriteSource = The sprite source used for the object's sprite (if any).
// Returns:
//	 If the object file exists and can be loaded, then return a pointer to a new instance 
//    of the specified game object type, else nullptr.
GameObject* GameObjectFactory::CreateObject(const std::string& name, Mesh* mesh, SpriteSource* spriteSource)
{
	// Create a new game object.
	GameObject* gameObject = new GameObject(name);

	// Open the object file.
	Parser parser("Assets/Objects/" + name + ".txt", std::fstream::in);

	try
	{
		// Deserialize the object.
		gameObject->Deserialize(parser);

		// If the object has a sprite component, set its mesh and sprite source.
		Sprite* sprite = gameObject->GetComponent<Sprite>();
		if (sprite != nullptr)
		{
			sprite->SetMesh(mesh);
			sprite->SetSpriteSource(spriteSource);
		}
	}
	catch (const ParseException& parseException)
	{
		// If something went horribly wrong deserializing the object, print an error message and return nullptr.
		std::cout << parseException.what() << std::endl;
		delete gameObject;
		return nullptr;
	}

	return gameObject;
}

// Create a single instance of the specified component.
// Params:
//   name = The name of the component.
// Returns:
//	 If the component exists, then return a pointer to a new instance 
//    of the specified component, else nullptr.
Component* GameObjectFactory::CreateComponent(const std::string& name)
{
	// Find a registered component with the given name.
	for (size_t i = 0; i < registeredComponents.size(); i++)
	{
		// If the component's name matches, return a clone.
		if (std::string(typeid(*registeredComponents[i]).name()).substr(6) == name)
		{
			return registeredComponents[i]->Clone();
		}
	}

	return nullptr;
}

// Saves an object to an archetype file so it can be loaded later.
void GameObjectFactory::SaveObjectToFile(GameObject* object)
{
	// Open the object file.
	std::string filePath = "Assets/Objects/" + object->GetName() + ".txt";
	Parser parser(filePath, std::fstream::out);

	// Serialize the object.
	object->Serialize(parser);

	std::cout << "Object " + object->GetName() + " has been written to " + filePath << std::endl;
}

// Returns an instance of the factory.
GameObjectFactory& GameObjectFactory::GetInstance()
{
	static GameObjectFactory instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
GameObjectFactory::GameObjectFactory()
{
	// Register all default components.
	RegisterComponent<ColliderCircle>();
	RegisterComponent<ColliderPoint>();
	RegisterComponent<ColliderRectangle>();
	RegisterComponent<ColliderTilemap>();
	RegisterComponent<ColliderLine>();
	RegisterComponent<Transform>();
	RegisterComponent<Animation>();
	RegisterComponent<Sprite>();
	RegisterComponent<SpriteTilemap>();
	RegisterComponent<Physics>();
}

// Destructor is private to prevent accidental destruction
GameObjectFactory::~GameObjectFactory()
{
	// Delete every component in the array.
	for (auto it = registeredComponents.begin(); it != registeredComponents.end();)
	{
		delete *it;
		it = registeredComponents.erase(it);
	}
}

//------------------------------------------------------------------------------
