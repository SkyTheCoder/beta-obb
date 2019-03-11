//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
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

#include <BetaObject.h>
#include "GameObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Space;
class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public functions declared in the header.
class GameObjectManager : public BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	GameObjectManager(Space* space);

	// Destructor
	~GameObjectManager();

	// Update all objects in the active game objects list.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Shutdown the game object manager, destroying all active objects.
	void Shutdown(void) override;

	// Unload the game object manager, destroying all object archetypes.
	void Unload(void) override;

	// Add a game object to the active game object list.
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	void AddObject(GameObject& gameObject);

	// Add a game object to the game object archetype list.
	// Params:
	//	 gameObject = Reference to the game object to be added to the list.
	void AddArchetype(GameObject& gameObject);

	// Returns a pointer to the first active game object matching the specified name.
	// Params:
	//	 objectName = The name of the object to be returned, if found.
	// Returns:
	//   If the named object is found,
	//	   then return the pointer to the named game object,
	//	   else return nullptr.
	GameObject* GetObjectByName(const std::string& objectName) const;

	// Returns a pointer to the first game object archetype matching the specified name.
	// Params:
	//	 objectName = The name of the archetype to be returned, if found.
	// Returns:
	//   If the named archetype is found,
	//	   then return the pointer to the named game object archetype,
	//	   else return nullptr.
	GameObject* GetArchetypeByName(const std::string& objectName) const;

	// Returns the number of active objects with the given name.
	// Params:
	//   objectName = The name of the objects that should be counted.
	unsigned GetObjectCount(const std::string& objectName) const;

private:
	// Disable copy constructor and assignment operator
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	// Update object logic using variable timestep.
	void VariableUpdate(float dt);

	// Update object physics using fixed timestep.
	void FixedUpdate(float dt) override;

	// Destroy any objects marked for destruction.
	void DestroyObjects();

	// Draw all game objects in the active game object list.
	void Draw(void) override;

	// Check for collisions between each pair of objects
	void CheckCollisions();
	
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------
	
	// Objects
	std::vector<GameObject*> gameObjectActiveList;
	
	// Archetypes
	std::vector<GameObject*> gameObjectArchetypes;
	
	// Time
	const float fixedUpdateDt;
	const float maxFixedUpdateTime;
	float timeAccumulator;
};

//------------------------------------------------------------------------------
