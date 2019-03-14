//------------------------------------------------------------------------------
//
// File Name:	Level4.cpp
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

#include "Level4.h"

// Systems
#include <Texture.h>
#include "SpriteSource.h"
#include "GameObject.h"
#include "MeshHelper.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Mesh.h>
#include <GameObjectFactory.h>

// Components
#include "MeshHelper.h"
#include "Transform.h"
#include "Physics.h"

// Levels
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level5.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 4.
	Level4::Level4() : Level("Level4")
	{
	}

	// Load the resources associated with Level 4.
	void Level4::Load()
	{
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Rectangle", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ControllableRectangle", resourceManager.GetMesh("Quad")));
	}

	// Initialize the memory associated with Level 4.
	void Level4::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Add various physics objects to the scene.

		// Rectangles.
		GameObject* rectangle = new GameObject(*objectManager.GetArchetypeByName("Rectangle"));
		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(150.0f, -100.0f));
		rectangle->GetComponent<Transform>()->SetRotation(M_PI_F * 3.0f / 8.0f);
		objectManager.AddObject(*rectangle);

		// Controllable rectangles.
		GameObject* controllableRectangle = new GameObject(*objectManager.GetArchetypeByName("ControllableRectangle"));
		objectManager.AddObject(*controllableRectangle);
	}

	// Update Level 4.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level4::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel<Level1>();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel<Level2>();
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->SetLevel<Level3>();
		}
		else if (input.CheckTriggered('4'))
		{
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('5'))
		{
			GetSpace()->SetLevel<Level5>();
		}
	}

	// Unload the resources associated with Level 4.
	void Level4::Unload()
	{
	}
}

//----------------------------------------------------------------------------
