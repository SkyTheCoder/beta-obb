//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
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

#include "Level1.h"

// Archetypes
#include "Archetypes.h"

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
#include "MouseFollow.h"

// Levels
#include "Level2.h"
#include "Level3.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 3.
	Level3::Level3() : Level("Level3")
	{
	}

	// Load the resources associated with Level 3.
	void Level3::Load()
	{
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		// Create the quad mesh
		resourceManager.GetMesh("Quad");
	}

	// Initialize the memory associated with Level 3.
	void Level3::Initialize()
	{
		// Get the resource manager
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create the convex object
		GameObject* convex = GameObjectFactory::GetInstance().CreateObject("Convex1", resourceManager.GetMesh("Quad"));
		GameObject* followedConvex = GameObjectFactory::GetInstance().CreateObject("Convex2", resourceManager.GetMesh("Quad"));

		followedConvex->AddComponent(new Behaviors::MouseFollow);

		GetSpace()->GetObjectManager().AddObject(*convex);
		GetSpace()->GetObjectManager().AddObject(*followedConvex);
	}

	// Update Level 3.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level3::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel<Level3>();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel<Level2>();
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Unload the resources associated with Level 3.
	void Level3::Unload()
	{
	}
}

//----------------------------------------------------------------------------
