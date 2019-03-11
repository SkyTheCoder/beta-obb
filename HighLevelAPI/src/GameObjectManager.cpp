//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	David Cohen (david.cohen)
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

#include "stdafx.h"

#include "GameObjectManager.h"

// Systems
#include "Space.h"
#include <Vector2D.h>

// Components
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
GameObjectManager::GameObjectManager(Space* space) : BetaObject("GameObjectmanager", space),
	fixedUpdateDt(1.0f / 60.0f), maxFixedUpdateTime(0.2f), timeAccumulator(0.0f)
{
}

// Destructor
GameObjectManager::~GameObjectManager()
{
	Shutdown();
	Unload();
}

// Update all objects in the active game objects list.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManager::Update(float dt)
{
	Space* space = static_cast<Space*>(GetParent());
	if (!space->IsPaused())
	{
		VariableUpdate(dt);
		FixedUpdate(dt);
	}

	DestroyObjects();

	Draw();
}

// Shutdown the game object manager, destroying all active objects.
void GameObjectManager::Shutdown(void)
{
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		delete *it;
	}

	gameObjectActiveList.clear();
}

// Unload the game object manager, destroying all object archetypes.
void GameObjectManager::Unload(void)
{
	for (auto it = gameObjectArchetypes.begin(); it != gameObjectArchetypes.end(); it++)
	{
		delete *it;
	}

	gameObjectArchetypes.clear();
}

// Add a game object to the active game object list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddObject(GameObject& gameObject)
{
	gameObjectActiveList.push_back(&gameObject);
	gameObject.SetParent(GetParent());
	gameObject.Initialize();
}

// Add a game object to the game object archetype list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddArchetype(GameObject& gameObject)
{
	gameObjectArchetypes.push_back(&gameObject);
}

// Returns a pointer to the first active game object matching the specified name.
// Params:
//	 objectName = The name of the object to be returned, if found.
// Returns:
//   If the named object is found,
//	   then return the pointer to the named game object,
//	   else return nullptr.
GameObject* GameObjectManager::GetObjectByName(const std::string& objectName) const
{
	// Loop through every game object and check if its name matches.
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		if ((*it)->GetName() == objectName)
			return *it;
	}

	return nullptr;
}

// Returns a pointer to the first game object archetype matching the specified name.
// Params:
//	 objectName = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return nullptr.
GameObject* GameObjectManager::GetArchetypeByName(const std::string& objectName) const
{
	// Loop through every archetype and check if its name matches.
	for (auto it = gameObjectArchetypes.begin(); it != gameObjectArchetypes.end(); it++)
	{
		if ((*it)->GetName() == objectName)
			return *it;
	}

	return nullptr;
}

// Returns the number of active objects with the given name.
// Params:
//   objectName = The name of the objects that should be counted.
unsigned GameObjectManager::GetObjectCount(const std::string& objectName) const
{
	unsigned count = 0;

	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end(); it++)
	{
		// If the object is not destroyed and its name matches the search, increment count.
		if (!(*it)->IsDestroyed() && (*it)->GetName() == objectName)
			++count;
	}

	return count;
}

// Update object logic using variable timestep.
void GameObjectManager::VariableUpdate(float dt)
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Update(dt);
	}
}

// Update object physics using fixed timestep.
void GameObjectManager::FixedUpdate(float dt)
{
	timeAccumulator += dt;

	// Clamp the time accumulator so the engine never freezes due to too many collisions.
	if (timeAccumulator > maxFixedUpdateTime)
		timeAccumulator = maxFixedUpdateTime;

	// Call the FixedUpdate functions the appropriate amount of times.
	while (timeAccumulator >= fixedUpdateDt)
	{
		for (size_t i = 0; i < gameObjectActiveList.size(); i++)
		{
			gameObjectActiveList[i]->FixedUpdate(fixedUpdateDt);
		}

		CheckCollisions();

		timeAccumulator -= fixedUpdateDt;
	}
}

// Destroy any objects marked for destruction.
void GameObjectManager::DestroyObjects()
{
	for (auto it = gameObjectActiveList.begin(); it != gameObjectActiveList.end();)
	{
		// If the object is destroyed, delete it and erase it from the vector.
		if ((*it)->IsDestroyed())
		{
			delete *it;
			it = gameObjectActiveList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManager::Draw(void)
{
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		gameObjectActiveList[i]->Draw();
	}
}

// Check for collisions between each pair of objects.
void GameObjectManager::CheckCollisions()
{
	// Iterate over every object.
	for (size_t i = 0; i < gameObjectActiveList.size(); i++)
	{
		// If the current object is marked for destruction, skip it.
		if (gameObjectActiveList[i]->IsDestroyed())
			continue;

		// Get the current object's collider.
		Collider* collider = gameObjectActiveList[i]->GetComponent<Collider>();

		// If the current object does not have a collider, skip it.
		if (collider == nullptr)
			continue;

		// Iterate over all remaining objects.
		for (size_t j = i + 1; j < gameObjectActiveList.size(); j++)
		{
			// If the current object is marked for destruction, skip it.
			if (gameObjectActiveList[j]->IsDestroyed())
				continue;

			// Get the current object's collider.
			Collider* collider2 = gameObjectActiveList[j]->GetComponent<Collider>();

			// If the current object does not have a collider, skip it.
			if (collider2 == nullptr)
				continue;

			// Check for collisions between the two objects.
			collider->CheckCollision(*collider2);
		}
	}
}

//------------------------------------------------------------------------------
