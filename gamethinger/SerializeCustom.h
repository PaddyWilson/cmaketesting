#pragma once

#include "Serialize.h"

#include <raylib.h>
#include "Components.h"
#include "CuteC2Components.h"

#include "ScriptRegistry.h"

namespace YAML {
	template<>
	struct convert<Color> {
		static Node encode(const Color& rhs) {
			Node node;
			node.push_back((int)rhs.r);
			node.push_back((int)rhs.g);
			node.push_back((int)rhs.b);
			node.push_back((int)rhs.a);
			return node;
		}

		static bool decode(const Node& node, Color& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.r = node[0].as<uint8_t>();
			rhs.g = node[1].as<uint8_t>();
			rhs.b = node[2].as<uint8_t>();
			rhs.a = node[3].as<uint8_t>();
			return true;
		}
	};

	template<>
	struct convert<Vector2> {
		static Node encode(const Vector2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, Vector2& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<std::vector<Vector2>> {
		static Node encode(const std::vector<Vector2>& rhs) {
			Node node;

			for (size_t i = 0; i < rhs.size(); i++)
			{
				node.push_back(rhs[i].x);
				node.push_back(rhs[i].y);
			}

			return node;
		}

		static bool decode(const Node& node, std::vector<Vector2>& rhs) {
			if (!node.IsSequence() || node.size() == 0) {
				return false;
			}

			for (size_t i = 0; i < node.size(); i++)
			{
				rhs.push_back(Vector2(node[i][0].as<float>(), node[i][1].as<float>()));
			}
			return true;
		}
	};

#pragma region cute_c2

	template<>
	struct convert<c2v> {
		static Node encode(const c2v& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, c2v& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

#pragma endregion

}

#pragma region EmittersOverloads

YAML::Emitter& operator<<(YAML::Emitter& out, const Color& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << (int)v.r << (int)v.g << (int)v.b << (int)v.a << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<Vector2>& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq;
	for (size_t i = 0; i < v.size(); i++)
	{
		out << v[i];// .x << v[i].y;
	}
	out << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const c2v& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

#pragma endregion

template<>
void SaveComponent<UniqueID>(YAML::Emitter& out, entt::registry& reg, UniqueID* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "ID" << YAML::Value << comp->id;
	out << YAML::EndMap;
};

template<>
void LoadComponent<UniqueID>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<UniqueID>(entity, data["ID"].as<uint64_t>());
}

template<>
void SaveComponent<Tag>(YAML::Emitter& out, entt::registry& reg, Tag* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "Tag" << YAML::Value << comp->tag;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Tag>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Tag>(entity, data["Tag"].as<std::string>());
}

template<>
void SaveComponent<Position>(YAML::Emitter& out, entt::registry& reg, Position* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << pos->position;
	out << YAML::Key << "Offset" << YAML::Value << pos->offset;
	out << YAML::Key << "Rotation" << YAML::Value << pos->rotation;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Position>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Position>(entity,
		data["Position"].as<Vector2>(),
		data["Offset"].as<Vector2>(),
		data["Rotation"].as<float>());
}

template<>
void SaveComponent<Velocity>(YAML::Emitter& out, entt::registry& reg, Velocity* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Velocity" << YAML::Value << pos->velocity;
	out << YAML::Key << "Rotation" << YAML::Value << pos->rotation;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Velocity>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Velocity>(entity,
		data["Velocity"].as<Vector2>(),
		data["Rotation"].as<float>());
}

template<>
void SaveComponent<Camera2D>(YAML::Emitter& out, entt::registry& reg, Camera2D* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << pos->target;
	out << YAML::Key << "Offset" << YAML::Value << pos->offset;
	out << YAML::Key << "Rotation" << YAML::Value << pos->rotation;
	out << YAML::Key << "Zoom" << YAML::Value << pos->zoom;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Camera2D>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Camera2D>(entity,
		data["Offset"].as<Vector2>(),
		data["Position"].as<Vector2>(),
		data["Rotation"].as<float>(),
		data["Zoom"].as<float>());
}

template<>
void SaveComponent<Rigidbody2D>(YAML::Emitter& out, entt::registry& reg, Rigidbody2D* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "BodyType" << YAML::Value << Rigidbody2D::TypeToString(comp->type);
	out << YAML::Key << "FixedRotation" << YAML::Value << comp->fixedRotation;
	out << YAML::Key << "Bullet" << YAML::Value << comp->isBullet;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Rigidbody2D>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Rigidbody2D>(entity,
		Rigidbody2D::StringToType(data["BodyType"].as<std::string>()),
		data["FixedRotation"].as<bool>(),
		data["Bullet"].as<bool>()
	);
}

template<>
void SaveComponent<Rigidbody2DRectangle>(YAML::Emitter& out, entt::registry& reg, Rigidbody2DRectangle* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "HalfSize" << YAML::Value << comp->halfSize;
	out << YAML::Key << "Offset" << YAML::Value << comp->offset;
	out << YAML::Key << "Density" << YAML::Value << comp->density;
	out << YAML::Key << "Friction" << YAML::Value << comp->friction;
	out << YAML::Key << "Restitution" << YAML::Value << comp->restitution;
	out << YAML::Key << "RestitutionThreshhold" << YAML::Value << comp->restitutionThreshhold;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Rigidbody2DRectangle>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Rigidbody2DRectangle>(entity,
		data["HalfSize"].as<Vector2>(),
		data["Offset"].as<Vector2>(),
		data["Density"].as<float>(),
		data["Friction"].as<float>(),
		data["Restitution"].as<float>(),
		data["RestitutionThreshhold"].as<float>()
	);
}

template<>
void SaveComponent<Rigidbody2DCircle>(YAML::Emitter& out, entt::registry& reg, Rigidbody2DCircle* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "Radius" << YAML::Value << comp->radius;
	out << YAML::Key << "Offset" << YAML::Value << comp->offset;
	out << YAML::Key << "Density" << YAML::Value << comp->density;
	out << YAML::Key << "Friction" << YAML::Value << comp->friction;
	out << YAML::Key << "Restitution" << YAML::Value << comp->restitution;
	out << YAML::Key << "RestitutionThreshhold" << YAML::Value << comp->restitutionThreshhold;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Rigidbody2DCircle>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Rigidbody2DCircle>(entity,
		data["Radius"].as<float>(),
		data["Offset"].as<Vector2>(),
		data["Density"].as<float>(),
		data["Friction"].as<float>(),
		data["Restitution"].as<float>(),
		data["RestitutionThreshhold"].as<float>()
	);
}

template<>
void SaveComponent<SpriteComp>(YAML::Emitter& out, entt::registry& reg, SpriteComp* sprite) {
	out << YAML::BeginMap;
	out << YAML::Key << "Texture" << YAML::Value << sprite->texture;
	out << YAML::Key << "Size" << YAML::Value << sprite->halfSize; 
	out << YAML::Key << "Color" << YAML::Value << sprite->color;
	out << YAML::EndMap;
};

template<>
void LoadComponent<SpriteComp>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	SpriteComp& spr = reg.emplace<SpriteComp>(entity,
		data["Texture"].as<std::string>(),
		data["Size"].as<Vector2>(),
		data["Color"].as<Color>()
	);
}

template<>
void SaveComponent<RectangleComp>(YAML::Emitter& out, entt::registry& reg, RectangleComp* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "HalfSize" << YAML::Value << comp->halfSize;
	out << YAML::Key << "Color" << YAML::Value << comp->color;
	out << YAML::EndMap;
};

template<>
void LoadComponent<RectangleComp>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<RectangleComp>(entity, 
		data["HalfSize"].as<Vector2>(),
		data["Color"].as<Color>()
	);
}

template<>
void SaveComponent<CircleComp>(YAML::Emitter& out, entt::registry& reg, CircleComp* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "Radius" << YAML::Value << comp->radious;
	out << YAML::Key << "Color" << YAML::Value << comp->color;
	out << YAML::EndMap;
};

template<>
void LoadComponent<CircleComp>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<CircleComp>(entity, 
		data["Radius"].as<float>(),
		data["Color"].as<Color>()
	);
}

template<>
void SaveComponent<TextComp>(YAML::Emitter& out, entt::registry& reg, TextComp* comp) {
	out << YAML::BeginMap;
	out << YAML::Key << "Text" << YAML::Value << comp->text;
	out << YAML::Key << "Size" << YAML::Value << comp->size;
	out << YAML::Key << "Padding" << YAML::Value << comp->padding;
	out << YAML::Key << "Color" << YAML::Value << comp->color;
	out << YAML::EndMap;
};

template<>
void LoadComponent<TextComp>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<TextComp>(entity,
		data["Text"].as<std::string>(),
		data["Size"].as<float>(),
		data["Padding"].as<float>(),
		data["Color"].as<Color>()
	);
}

template<>
void SaveComponent<Renderable>(YAML::Emitter& out, entt::registry& reg, Renderable* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Show" << YAML::Value << pos->show;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Renderable>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<Renderable>(entity, data["Show"].as<bool>());
}

template<>
void SaveComponent<RenderableUI>(YAML::Emitter& out, entt::registry& reg, RenderableUI* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Show" << YAML::Value << pos->show;
	out << YAML::EndMap;
};

template<>
void LoadComponent<RenderableUI>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<RenderableUI>(entity, data["Show"].as<bool>());
}

template<>
void SaveComponent<ScriptComp>(YAML::Emitter& out, entt::registry& reg, ScriptComp* comp) {
	out << YAML::BeginMap;

	std::string name = "NoScriptInstance";
	if (comp->scriptInstance != nullptr)
		name = comp->scriptInstance->scriptName;

	out << YAML::Key << "ScriptName" << YAML::Value << name;
	out << YAML::EndMap;
};

template<>
void LoadComponent<ScriptComp>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	ScriptRegistry::AddScriptToEntity(data["ScriptName"].as<std::string>(), &reg, entity);
}

#pragma region Parent Child Relationship

template<>
void SaveComponent<Parent>(YAML::Emitter& out, entt::registry& reg, Parent* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "ParentUniqueID" << YAML::Value << reg.get<UniqueID>(pos->parent).id;
	out << YAML::Key << "Depth" << YAML::Value << (int)pos->depth;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Parent>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<ParentLoad>(entity,
		data["ParentUniqueID"].as<uint64_t>(),
		data["Depth"].as<uint8_t>()
	);
}

template<>
void SaveComponent<ChildPosition>(YAML::Emitter& out, entt::registry& reg, ChildPosition* pos) {
	out << YAML::BeginMap;
	out << YAML::Key << "Position" << YAML::Value << pos->position;
	out << YAML::Key << "Offset" << YAML::Value << pos->offset;
	out << YAML::Key << "Rotation" << YAML::Value << pos->rotation;
	out << YAML::EndMap;
};

template<>
void LoadComponent<ChildPosition>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	reg.emplace<ChildPosition>(entity,
		data["Position"].as<Vector2>(),
		data["Offset"].as<Vector2>(),
		data["Rotation"].as<float>()
	);
}

//don't realy have to save children of parent
// just use parent component
//just keeping this incase
template<>
void SaveComponent<Children>(YAML::Emitter& out, entt::registry& reg, Children* pos) {
	out << YAML::BeginMap;

	out << YAML::Key << "Children" << YAML::Value;

	out << YAML::Flow;
	out << YAML::BeginSeq;
	for (size_t i = 0; i < pos->children.size(); i++)
	{
		out << reg.get<UniqueID>(pos->children[i]).id;
	}
	 out << YAML::EndSeq;
	out << YAML::EndMap;
};

template<>
void LoadComponent<Children>(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	/*reg.emplace<Children>(entity,
		data["Position"].as<Vector2>(),
		data["Offset"].as<Vector2>(),
		data["Rotation"].as<float>()
	);*/
}

#pragma endregion


#pragma region CuteC2

template<>
void SaveComponent<BB_AABB>(YAML::Emitter& out, entt::registry& reg, BB_AABB* aabb) {
	out << YAML::BeginMap;
	out << YAML::Key << "Static" << YAML::Value << aabb->isStatic;
	out << YAML::Key << "DebugColor" << YAML::Value << aabb->debugColor;
	out << YAML::Key << "HalfSize" << YAML::Value << aabb->halfSize;
	out << YAML::EndMap;
};

template<>
void LoadComponent<BB_AABB>(entt::registry& reg, entt::entity ent, YAML::Node& aabb) {
	auto p = reg.get<Position>(ent);
	auto* comp = &reg.emplace<BB_AABB>(ent,
		p.position,
		aabb["HalfSize"].as<Vector2>(),
		aabb["Static"].as<bool>());
	comp->debugColor = aabb["DebugColor"].as<Color>();
}

template<>
void SaveComponent<BB_RECTANGLE>(YAML::Emitter& out, entt::registry& reg, BB_RECTANGLE* aabb) {
	out << YAML::BeginMap;
	out << YAML::Key << "Static" << YAML::Value << aabb->isStatic;
	out << YAML::Key << "DebugColor" << YAML::Value << aabb->debugColor;
	out << YAML::Key << "HalfSize" << YAML::Value << aabb->halfSize;
	out << YAML::EndMap;
};

template<>
void LoadComponent<BB_RECTANGLE>(entt::registry& reg, entt::entity ent, YAML::Node& aabb) {
	auto p = reg.get<Position>(ent);
	auto* comp = &reg.emplace<BB_RECTANGLE>(ent,
		p.position,
		aabb["HalfSize"].as<Vector2>(),
		aabb["Static"].as<bool>());
	comp->debugColor = aabb["DebugColor"].as<Color>();
}

template<>
void SaveComponent<BB_CIRCLE>(YAML::Emitter& out, entt::registry& reg, BB_CIRCLE* circle) {
	//out << YAML::Key << "BB_CIRCLEComp";
	out << YAML::BeginMap;
	out << YAML::Key << "Static" << YAML::Value << circle->isStatic;
	out << YAML::Key << "DebugColor" << YAML::Value << circle->debugColor;
	out << YAML::Key << "Radius" << YAML::Value << circle->circle.r;
	out << YAML::EndMap;
};

template<>
void LoadComponent<BB_CIRCLE>(entt::registry& reg, entt::entity ent, YAML::Node& circle) {
	auto p = reg.get<Position>(ent);
	auto* comp = &reg.emplace<BB_CIRCLE>(ent,
		p.position,
		circle["Radius"].as<float>(),
		circle["Static"].as<bool>()
	);
	comp->debugColor = circle["DebugColor"].as<Color>();
}

template<>
void SaveComponent<BB_POLY>(YAML::Emitter& out, entt::registry& reg, BB_POLY* poly) {
	//out << YAML::Key << "BB_POLYComp";
	out << YAML::BeginMap;
	out << YAML::Key << "Static" << YAML::Value << poly->isStatic;
	out << YAML::Key << "DebugColor" << YAML::Value << poly->debugColor;
	out << YAML::Key << "Verts" << poly->points;
	out << YAML::EndMap;
};

template<>
void LoadComponent<BB_POLY>(entt::registry& reg, entt::entity ent, YAML::Node& poly) {
	auto p = reg.get<Position>(ent);
	auto* comp = &reg.emplace<BB_POLY>(ent,
		p.position,
		poly["Verts"].as<std::vector<Vector2>>(),
		poly["Static"].as<bool>()
	);
	comp->debugColor = poly["DebugColor"].as<Color>();
}

template<>
void SaveComponent<BB_CAP>(YAML::Emitter& out, entt::registry& reg, BB_CAP* cap) {
	//out << YAML::Key << "BB_CAPComp";
	out << YAML::BeginMap;
	out << YAML::Key << "Static" << YAML::Value << cap->isStatic;
	out << YAML::Key << "DebugColor" << YAML::Value << cap->debugColor;
	out << YAML::Key << "P1" << cap->p1;
	out << YAML::Key << "P2" << cap->p2;
	out << YAML::Key << "Radius" << YAML::Value << cap->cap.r;
	out << YAML::EndMap;
};

template<>
void LoadComponent<BB_CAP>(entt::registry& reg, entt::entity ent, YAML::Node& cap) {
	auto p = reg.get<Position>(ent);
	auto* comp = &reg.emplace<BB_CAP>(ent,
		p.position,
		cap["P1"].as<Vector2>(),
		cap["P2"].as<Vector2>(),
		cap["Radius"].as<float>(),
		cap["Static"].as<bool>()
	);
	comp->debugColor = cap["DebugColor"].as<Color>();
}

#pragma endregion
