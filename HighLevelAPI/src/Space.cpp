//------------------------------------------------------------------------------
//
// File Name:	Space.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaEngine
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Space.h"

// Systems
#include "Level.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
Space::Space(const std::string& name) : BetaObject(name),
	paused(false), currentLevel(nullptr), nextLevel(nullptr), objectManager(this)
{
}

// Destructor
Space::~Space()
{
	Shutdown();
}

// Updates the state manager and object manager.
// Params:
//   dt = The change in time since the last call to this function.
void Space::Update(float dt)
{
	objectManager.Update(dt);

	// If there is a next level, handle level changing logic.
	if (nextLevel != nullptr)
	{
		ChangeLevel();
	}

	// If the current level exists and is not paused, update it.
	if (currentLevel != nullptr && !paused)
	{
		currentLevel->Update(dt);
	}
}

// Shuts down the object manager
void Space::Shutdown()
{
	// Shutdown the current level if there is one.
	if (currentLevel != nullptr)
		currentLevel->Shutdown();

	// Shutdown and unload the object manager.
	objectManager.Shutdown();
	objectManager.Unload();

	// Unload the current level if there is one.
	if (currentLevel != nullptr)
	{
		currentLevel->Unload();
		resourceManager.OnLevelUnload();
	}

	resourceManager.Unload();

	// Free the current level's memory.
	delete currentLevel;

	// Safely set the current level to a nullptr.
	currentLevel = nullptr;
}

// Returns a boolean indicating whether objects in this space are paused.
// Returns:
//   True if the space is paused, false otherwise.
bool Space::IsPaused() const
{
	return paused;
}

// Returns the name of the level currently running in this space.
const std::string& Space::GetLevelName() const
{
	// Get the current level's name. If there is no current level, that sounds like a you problem.
	return currentLevel->GetName();
}

// Pauses the space, preventing objects from being updated, but objects are still drawn.
// Params:
//   value = A true/false value that indicates whether the space should be paused.
void Space::SetPaused(bool value)
{
	paused = value;
}

// Sets the level that the space is using after unloading the current level.
// Params:
//   level = The next level that the space will be using.
void Space::SetLevel(Level* level)
{
	// If Setlevel was called multiple times in the same frame, delete the previous level.
	if (nextLevel != nullptr)
		delete nextLevel;

	nextLevel = level;

	// If the next level exists, set its parent to this space (necessary for Level::GetParent to work properly).
	if (nextLevel != nullptr)
	{
		nextLevel->SetParent(this);
	}
}

// Restarts the current level (next level = current)
void Space::RestartLevel()
{
	nextLevel = currentLevel;
}

// Returns the object manager, which you can use to retrieve and add objects.
GameObjectManager& Space::GetObjectManager()
{
	return objectManager;
}

// Returns the resource manager, which you can use to load resources.
ResourceManager& Space::GetResourceManager()
{
	return resourceManager;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Updates current/next level pointers and calls shutdown/unload/load/init
void Space::ChangeLevel()
{
	// If the current level exists, shut it down.
	if (currentLevel != nullptr)
		currentLevel->Shutdown();

	// Shutdown the object manager.
	objectManager.Shutdown();

	// Check if we are changing levels.
	if (nextLevel != currentLevel)
	{
		// Unload the object manager.
		objectManager.Unload();

		// If the current level exists, unload it.
		if (currentLevel != nullptr)
		{
			currentLevel->Unload();
			resourceManager.OnLevelUnload();
		}

		// Free the current level's memory.
		delete currentLevel;

		// Set the current level to the next level.
		currentLevel = nextLevel;

		// Load the next level.
		currentLevel->Load();
		resourceManager.OnLevelLoad();
	}

	// Initialize the next level.
	currentLevel->Initialize();

	// Set the next level to a nullptr 
	nextLevel = nullptr;
}

//------------------------------------------------------------------------------
