//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Level1.h"

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
#include "Level2.h"
// David Wong: Added the third level
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

	// Creates an instance of Level 1.
	Level1::Level1() : Level("Level1"),
		circleSpeed(0.0f), pointSpeed(0.0f)
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");

		// Load the circle texture and sprite source.
		resourceManager.GetSpriteSource("Circle.png");

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Rectangle", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Circle", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ControllableRectangle", resourceManager.GetMesh("Quad")));
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Add various physics objects to the scene.

		// Rectangles.
		GameObject* rectangle = new GameObject(*objectManager.GetArchetypeByName("Rectangle"));
		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(-200.0f, 250.0f));
		rectangle->GetComponent<Transform>()->SetRotation(-M_PI_F / 8.0f);
		rectangle->GetComponent<Physics>()->SetVelocity(Vector2D(50.0f, -75.0f));
		rectangle->GetComponent<Physics>()->SetAngularVelocity(M_PI_F / 2.0f);
		objectManager.AddObject(*rectangle);

		rectangle = new GameObject(*objectManager.GetArchetypeByName("Rectangle"));
		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(50.0f, -150.0f));
		rectangle->GetComponent<Transform>()->SetRotation(M_PI_F / 8.0f);
		rectangle->GetComponent<Physics>()->SetVelocity(Vector2D(0.0f, 0.0f));
		objectManager.AddObject(*rectangle);

		// Controllable rectangles.
		GameObject* controllableRectangle = new GameObject(*objectManager.GetArchetypeByName("ControllableRectangle"));
		objectManager.AddObject(*controllableRectangle);

#if 0
		// Circles.
		GameObject* circle = objectFactory.CreateObject("Circle", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png"));
		objectManager.AddObject(*circle);

		circle = objectFactory.CreateObject("Circle", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png"));
		circle->GetComponent<Transform>()->SetTranslation(Vector2D(125.0f, 375.0f));
		circle->GetComponent<Physics>()->SetVelocity(Vector2D(-25.0f, -75.0f));
		objectManager.AddObject(*circle);

		// Points.
		GameObject* point = objectFactory.CreateObject("Point", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png"));
		point->GetComponent<Transform>()->SetTranslation(Vector2D(0.0f, 100.0f));
		point->GetComponent<Physics>()->SetVelocity(Vector2D(0.0f, -50.0f));
		objectManager.AddObject(*point);
#endif
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel<Level2>();
		}
		// David Wong: Added the third level
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->SetLevel<Level3>();
		}
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
	}
}

//----------------------------------------------------------------------------
