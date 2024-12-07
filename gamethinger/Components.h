#pragma once
#include <vector>
#include <raylib.h>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "UUIDGen.h"

#include "Script.h"

// user to remove entity at end of frame
struct DestroyFlag
{
	bool yes = true;
};

struct Networkable
{
	std::vector<entt::id_type> sendComps;
};

class UniqueID
{
public:
	uint64_t id;

	UniqueID()
	{
		id = UUID::Generate();
	}

	UniqueID(uint64_t id) : id(id) {}
};

struct Tag
{
	std::string tag;
};

// raylib has a transform struct

/// <summary>
/// position in world space
/// </summary>
struct Position
{
	Vector2 position;
	Vector2 offset;
	float rotation;
};

struct Velocity
{
	Vector2 velocity;
	float rotation;
};

class ScriptComp
{
public:
	Script *scriptInstance = nullptr;

	Script *(*InstantiateScript)() = nullptr;
	void (*DestroyScript)(ScriptComp *) = nullptr;

	//~ScriptComp() {
	//	//if (scriptInstance != nullptr)
	//	//{
	//		delete scriptInstance;
	//		scriptInstance = nullptr;
	//		InstantiateScript = nullptr;
	//		DestroyScript = nullptr;
	//	//}
	//}

	template <typename T>
	void Bind()
	{
		InstantiateScript = []()
		{ return static_cast<Script *>(new T()); };
		DestroyScript = [](ScriptComp *nsc)
		{ delete nsc->scriptInstance; nsc->scriptInstance = nullptr; };
	}
};

struct Health
{
	int health;
};
struct Damage
{
	int damage;
};
struct DamageOverTime
{
	int damage;
	float time;
};

struct IsHovered
{
};
struct Hoverable
{
	Color hoverColor;
};

struct Button
{
	Color disabledColor;
	std::function<void()> onClick = NULL;
	std::function<void()> onRelease = NULL;
};

#pragma region box2dComponents

struct Rigidbody2D
{

	enum class RigidbodyType
	{
		Static,
		Dynamic,
		Kinematic
	};

	RigidbodyType type = RigidbodyType::Static;
	bool fixedRotation = false;
	// prevents tunneling on Dynamic fast moving objects
	bool isBullet = false;
	b2BodyId bodyId = b2_nullBodyId;

	b2BodyId Init(b2WorldId world, const Position &position)
	{
		b2BodyDef bodyDef = b2DefaultBodyDef();

		if (type == Rigidbody2D::RigidbodyType::Static)
			bodyDef.type = b2_staticBody;
		else if (type == Rigidbody2D::RigidbodyType::Dynamic)
			bodyDef.type = b2_dynamicBody;
		else if (type == Rigidbody2D::RigidbodyType::Kinematic)
			bodyDef.type = b2_kinematicBody;

		// this could be handy
		// bodyDef.userData.pointer = uintptr_t(this);

		bodyDef.position = b2Vec2{position.position.x, position.position.y};
		bodyDef.rotation = b2MakeRot(position.rotation);
		bodyDef.fixedRotation = fixedRotation;
		bodyDef.isBullet = isBullet;

		bodyId = b2CreateBody(world, &bodyDef);
		return bodyId;
	}

	static std::string TypeToString(const RigidbodyType &type)
	{
		if (type == Rigidbody2D::RigidbodyType::Static)
			return "Static";
		else if (type == Rigidbody2D::RigidbodyType::Dynamic)
			return "Dynamic";
		else if (type == Rigidbody2D::RigidbodyType::Kinematic)
			return "Kinematic";
	}

	static RigidbodyType StringToType(const std::string &type)
	{
		if (type == "Static")
			return Rigidbody2D::RigidbodyType::Static;
		else if (type == "Dynamic")
			return Rigidbody2D::RigidbodyType::Dynamic;
		else if (type == "Kinematic")
			return Rigidbody2D::RigidbodyType::Kinematic;
	}
};

class Rigidbody2DRectangle
{
public:
	Vector2 halfSize{0.5f, 0.5f};
	Vector2 offset{0.f, 0.f};

	float density = 1.0f;
	float friction = 0.1f;
	float restitution = 0.f;
	float restitutionThreshhold = 0.1f;

	b2Polygon box;
	b2ShapeId shapeId;

	b2ShapeId Init(b2BodyId bodyId)
	{
		box = b2MakeBox(halfSize.x, halfSize.y);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = density;
		shapeDef.friction = friction;
		shapeDef.restitution = restitution;

		b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

		// fixture = b2MakeBox(halfSize.x, halfSize.y);
		// b2PolygonShape box;
		// box.SetAsBox(halfSize.x, halfSize.y, b2Vec2(offset.x, offset.y), 0.0f);
		// b2FixtureDef fixtureDef;
		// fixtureDef.shape = &box;

		// fixtureDef.density = density;
		// fixtureDef.friction = friction;
		// fixtureDef.restitution = restitution;
		// fixtureDef.restitutionThreshold = restitutionThreshhold;

		// fixture = body->CreateFixture(&fixtureDef);
		return shapeId;
	}
};

class Rigidbody2DCircle
{
public:
	float radius = 0.5f;
	Vector2 offset{ 0.f, 0.f };

	float density = 1.0f;
	float friction = 0.1f;
	float restitution = 0.f;
	float restitutionThreshhold = 0.1f;

	// b2Body *body;

	b2Circle circle;
	b2ShapeId shapeId;

	b2ShapeId Init(b2BodyId bodyId)
	{
		// circle.center = (b2Vec2){position.x, position.y};
		circle.radius = radius;

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = density;
		shapeDef.friction = friction;
		shapeDef.restitution = restitution;

		b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
		// b2CircleShape circle;
		// circle.m_radius = radius;

		// b2FixtureDef fixtureDef;
		// fixtureDef.shape = &circle;

		// fixtureDef.density = density;
		// fixtureDef.friction = friction;
		// fixtureDef.restitution = restitution;
		// fixtureDef.restitutionThreshold = restitutionThreshhold;

		// fixture = body->CreateFixture(&fixtureDef);
		return shapeId;
	}
};

#pragma endregion

struct CircleComp
{
	float radious;
	Color color = BLACK;
};

struct RectangleComp
{
	Vector2 halfSize;
	Color color = BLACK;
};

struct SpriteComp
{
	// Texture2D texture;
	std::string texture; // using
	Vector2 halfSize;
	Color color = BLACK;
};

struct TextComp
{
	std::string text;
	float size;
	float padding = 0.1f;
	Color color = BLACK;
};

#pragma region ParentChild

/// <summary>
/// flag to know if Position has been updated yet
/// cleared at the end of movement system update
/// </summary>
struct WorldPositionUpdated
{
};

struct Parent
{
	entt::entity parent = entt::null;
	uint8_t depth = 0;
};

// used to construct parent child relations on load of scene
struct ParentLoad
{
	uint64_t parentID = 0;
	uint8_t depth = 0;
};

struct Children
{
	std::vector<entt::entity> children;
};

/// <summary>
/// position relative of parent
/// </summary>
struct ChildPosition
{
	Vector2 position;
	Vector2 offset;
	float rotation;
};

#pragma endregion

// have refactored above comps;
// to do are below
// input
struct InputAction
{
	enum ButtonState
	{
		UP,
		DOWN,
		PRESSED,
		RELEASED
	};
	KeyboardKey key;
	std::function<void()> func = NULL;
	ButtonState buttonState = ButtonState::DOWN;

	// bool lastUpdate = false;
	// float time = 0.f;
};

struct InputComp
{
	std::vector<InputAction> inputs;
};

// clicking
struct Clickable
{
	enum Action
	{
		NONE,
		DOWN,
		PRESSED,
		RELEASED,
		UP
	};
	Action action;
	std::function<void()> func = NULL;
	MouseButton mouseButton = MOUSE_BUTTON_LEFT;
	bool lastUpdate = false;

	bool enabled = true;
};

struct FollowMouse
{
	bool follow = true;
};

// renderable comps
struct LineComp
{
	Vector2 point1; // maybe don't need
	Vector2 point2;
	float thickness = 1.0f;
	Color color;
};

// flags for where to render
// can't have no variables if want to serialize
struct Renderable
{
	bool show = true;
};

struct RenderableUI
{
	bool show = true;
};