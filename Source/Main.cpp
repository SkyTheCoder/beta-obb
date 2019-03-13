p n//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
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

// BetaFramework Engine
#include <Engine.h>

// Engine modules
#include "Space.h"

// Initial game state
#include "Level1.h"

// Game object factory
#include <GameObjectFactory.h>

// Components
#include "ColorChange.h"
#include "MonkeyAnimation.h"
#include "MonkeyMovement.h"
#include "PlayerShip.h"
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "PlayerProjectile.h"
#include "Asteroid.h"
#include "MouseFollow.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);

	GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();

	// Register components with the game object factory so game objects can be fully deserialized.
	objectFactory.RegisterComponent<Behaviors::ColorChange>();
	objectFactory.RegisterComponent<Behaviors::MonkeyAnimation>();
	objectFactory.RegisterComponent<Behaviors::MonkeyMovement>();
	objectFactory.RegisterComponent<Behaviors::PlayerShip>();
	objectFactory.RegisterComponent<Behaviors::ScreenWrap>();
	objectFactory.RegisterComponent<Behaviors::TimedDeath>();
	objectFactory.RegisterComponent<Behaviors::PlayerProjectile>();
	objectFactory.RegisterComponent<Behaviors::Asteroid>();
	objectFactory.RegisterComponent<Behaviors::MouseFollow>();

	// Create a new space called "GameSpace"
	Space* space = new Space("GameSpace");

	// Set initial level to the second level.
	space->SetLevel<Levels::Level1>();

	// Add additional modules to engine
	Engine::GetInstance().AddModule(space);
	
	// Game engine goes!
	Engine::GetInstance().Start(800, 600, 200);

	return 0;
}
