//------------------------------------------------------------------------------
//
// File Name:	Archetypes.cpp
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

#include "Archetypes.h"

// Systems
#include "GameObject.h"
#include "Space.h"
#include "SpriteSource.h"
#include <Mesh.h>
#include <GameObjectFactory.h>

// Components
#include "Transform.h"
#include "Sprite.h"
#include "Physics.h"
#include "PlayerShip.h"
#include "TimedDeath.h"
#include "MonkeyMovement.h"
#include "Animation.h"
#include "ColliderPoint.h"
#include "ColliderCircle.h"
#include "ColliderRectangle.h"
#include "ColliderConvex.h"
#include "ColorChange.h"
#include "ScreenWrap.h"
#include "SpriteTilemap.h"
#include "ColliderTilemap.h"
#include "MonkeyAnimation.h"
#include "Shapes2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Archetypes
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create the ship game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateShip(Mesh* mesh)
	{
		// Create a new game object.
		GameObject* ship = new GameObject("Ship");
		
		// Create a new transform.
		ship->AddComponent(new Transform(Vector2D(), Vector2D(50.0f, 50.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		ship->AddComponent(sprite);

		// Create a new physics.
		ship->AddComponent(new Physics());

		// Create a new player ship.
		ship->AddComponent(new Behaviors::PlayerShip(100.0f, 100.0f, static_cast<float>(M_PI), 200.0f));

		GameObjectFactory::GetInstance().SaveObjectToFile(ship);

		return ship;
	}

	// Create the archetype for the bullet object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateBulletArchetype(Mesh* mesh)
	{
		// Create a new game object.
		GameObject* bullet = new GameObject("Bullet");

		// Create a new transform.
		bullet->AddComponent(new Transform(Vector2D(), Vector2D(7.0f, 7.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		bullet->AddComponent(sprite);

		// Create a new physics.
		bullet->AddComponent(new Physics());

		// Create a new timed death.
		bullet->AddComponent(new Behaviors::TimedDeath(5.0f));

		GameObjectFactory::GetInstance().SaveObjectToFile(bullet);

		return bullet;
	}

	// Create the monkey game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateMonkey(Mesh* mesh, SpriteSource* spriteSource)
	{
		// Create a new game object.
		GameObject* monkey = new GameObject("Monkey");

		// Create a new transform.
		monkey->AddComponent(new Transform(Vector2D(), Vector2D(100.0f, 100.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		monkey->AddComponent(sprite);

		// Create a new physics.
		monkey->AddComponent(new Physics());

		// Create a new rectangle collider.
		monkey->AddComponent(new ColliderRectangle(Vector2D(49.99f, 49.99f)));

		// Create a new animation.
		monkey->AddComponent(new Animation());

		// Create a new monkey movement.
		monkey->AddComponent(new Behaviors::MonkeyMovement());

		// Create a new MonkeyAnimation.
		monkey->AddComponent(new Behaviors::MonkeyAnimation(0, 8, 9, 12));

		GameObjectFactory::GetInstance().SaveObjectToFile(monkey);

		return monkey;
	}

	// Create the circle game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateCircle(Mesh* mesh, SpriteSource* spriteSource)
	{
		// Create a new game object.
		GameObject* circle = new GameObject("Circle");

		// Create a new transform.
		Transform* transform = new Transform(Vector2D(), Vector2D(100.0f, 100.0f));
		circle->AddComponent(transform);

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		circle->AddComponent(sprite);

		// Create a new physics.
		circle->AddComponent(new Physics());

		// Create a new circle collider.
		circle->AddComponent(new ColliderCircle(transform->GetScale().x / 2.0f));

		// Create a new color change.
		circle->AddComponent(new Behaviors::ColorChange(Colors::Yellow, Colors::Red));

		// Create a new screen wrap.
		circle->AddComponent(new Behaviors::ScreenWrap());

		GameObjectFactory::GetInstance().SaveObjectToFile(circle);

		return circle;
	}

	// Create the point game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	//   spriteSource = The sprite source to use for the object.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreatePoint(Mesh* mesh, SpriteSource* spriteSource)
	{
		// Create a new game object.
		GameObject* point = new GameObject("Point");

		// Create a new transform.
		point->AddComponent(new Transform(Vector2D(), Vector2D(20.0f, 20.0f)));

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		sprite->SetSpriteSource(spriteSource);
		point->AddComponent(sprite);

		// Create a new physics.
		point->AddComponent(new Physics());

		// Create a new point collider.
		point->AddComponent(new ColliderPoint());

		// Create a new color change.
		point->AddComponent(new Behaviors::ColorChange(Colors::Blue, Colors::Red));

		// Create a new screen wrap.
		point->AddComponent(new Behaviors::ScreenWrap());

		GameObjectFactory::GetInstance().SaveObjectToFile(point);

		return point;
	}

	// Create the rectangle game object.
	// Params:
	//   mesh  = The mesh to use for the object's sprite.
	// Returns:
	//	 A pointer to the newly constructed game object
	GameObject* CreateRectangle(Mesh* mesh)
	{
		// Create a new game object.
		GameObject* rectangle = new GameObject("Rectangle");

		// Create a new transform.
		Transform* transform = new Transform(Vector2D(), Vector2D(250.0f, 100.0f));
		rectangle->AddComponent(transform);

		// Create a new sprite.
		Sprite* sprite = new Sprite();
		sprite->SetMesh(mesh);
		rectangle->AddComponent(sprite);

		// Create a new physics.
		rectangle->AddComponent(new Physics());

		// Create a new rectangle collider.
		rectangle->AddComponent(new ColliderRectangle(transform->GetScale() / 2.0f));

		// Create a new color change.
		rectangle->AddComponent(new Behaviors::ColorChange(Colors::Green, Colors::Red));

		// Create a new screen wrap.
		rectangle->AddComponent(new Behaviors::ScreenWrap());

		GameObjectFactory::GetInstance().SaveObjectToFile(rectangle);

		return rectangle;
	}

	GameObject* CreateConvexObject1(Mesh* mesh)
	{
		GameObject* convex = new GameObject("Convex1");
		Transform* transform = new Transform(Vector2D(-200, 0), Vector2D(300, 300));
		Sprite* sprite = new Sprite;
		sprite->SetMesh(mesh);
		
		LineSegment segments[5] = {
			LineSegment(Vector2D(0.f, 0.5f), Vector2D(-0.5f, 0.f)),
			LineSegment(Vector2D(-0.5f, 0.0f), Vector2D(-0.25f, -0.25f)),
			LineSegment(Vector2D(-0.25f, -0.25f), Vector2D(0.25f, -0.25f)),
			LineSegment(Vector2D(0.25f, -0.25f), Vector2D(0.5f, 0.f)),
			LineSegment(Vector2D(0.5f, 0.f), Vector2D(0.f, 0.5f)),
		};

		ColliderConvex* collider = new ColliderConvex(segments, 5);

		convex->AddComponent(transform);
		convex->AddComponent(sprite);
		convex->AddComponent(collider);
		convex->AddComponent(new Behaviors::ColorChange(Colors::Blue, Colors::Red));

		return convex;
	}

	GameObject* CreateConvexObject2(Mesh* mesh)
	{
		GameObject* convex = new GameObject("Convex2");
		Transform* transform = new Transform(Vector2D(-200, 0), Vector2D(300, 300));
		Sprite* sprite = new Sprite;
		sprite->SetMesh(mesh);

		LineSegment segments[6] = {
			LineSegment(Vector2D(-0.25f, 0.25f), Vector2D(0.f, .5f)),
			LineSegment(Vector2D(0.f, 0.5f), Vector2D(0.25f, 0.25f)),
			LineSegment(Vector2D(0.25f, 0.25f), Vector2D(0.25f, -.25f)),
			LineSegment(Vector2D(0.25f, -.25f), Vector2D(0.f, -.5f)),
			LineSegment(Vector2D(0.f, -.5f), Vector2D(-0.25f, -.25f)),
			LineSegment(Vector2D(-0.25f, -.25f), Vector2D(-0.25f, 0.25f)),
		};

		ColliderConvex* collider = new ColliderConvex(segments, 6);

		convex->AddComponent(transform);
		convex->AddComponent(sprite);
		convex->AddComponent(collider);
		convex->AddComponent(new Behaviors::ColorChange(Colors::Blue, Colors::Red));

		return convex;
	}

	// Create a tilemap object
	// Params:
	//   mesh = The mesh to use for the sprite.
	//   spriteSource = The sprite source to use for the sprite.
	//   map = The map to use for tilemap related components.
	// Returns:
	//   A pointer to the newly constructed game object.
	GameObject* CreateTilemapObject(Mesh* mesh, SpriteSource* spriteSource, Tilemap* map)
	{
		// Create a new game object.
		GameObject* tilemap = new GameObject("Tilemap");

		// Create a new transform.
		Transform* transform = new Transform(Vector2D(-350.0f, 250.0f), Vector2D(100.0f, 100.0f));
		tilemap->AddComponent(transform);

		// Create a new sprite tilemap.
		SpriteTilemap* spriteTilemap = new SpriteTilemap();
		spriteTilemap->SetMesh(mesh);
		spriteTilemap->SetSpriteSource(spriteSource);
		spriteTilemap->SetTilemap(map);
		tilemap->AddComponent(spriteTilemap);
		 
		// Create a new collider tilemap.
		ColliderTilemap* colliderTilemap = new ColliderTilemap();
		colliderTilemap->SetTilemap(map);
		tilemap->AddComponent(colliderTilemap);

		GameObjectFactory::GetInstance().SaveObjectToFile(tilemap);

		return tilemap;
	}
}

//----------------------------------------------------------------------------
