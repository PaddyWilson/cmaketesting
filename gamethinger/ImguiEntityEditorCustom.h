#pragma once

#include "ImguiEntityEditor.h"
#include "Components.h"
#include <rlImGuiColors.h>
#include "ScriptRegistry.h"

void DragFloatVector2(std::string label, Vector2& vec, float amount = 0.25f)
{
	float p[2];
	p[0] = vec.x;
	p[1] = vec.y;
	ImGui::DragFloat2(label.c_str(), p, amount);
	vec.x = p[0];
	vec.y = p[1];
}

void EditString(std::string label, std::string& editString)
{
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strncpy_s(buffer, sizeof(buffer), editString.c_str(), sizeof(buffer));
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
	{
		editString = std::string(buffer);
	}
}

void EditColor4(std::string label, Color& color)
{
	float c[4];
	c[0] = color.r / 255.f;
	c[1] = color.g / 255.f;
	c[2] = color.b / 255.f;
	c[3] = color.a / 255.f;

	if (ImGui::ColorEdit4("Color", c, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB))
	{
		color.r = c[0] * 255.f;
		color.g = c[1] * 255.f;
		color.b = c[2] * 255.f;
		color.a = c[3] * 255.f;
	}
}


template<>
void CustomComponentWidget<UniqueID>(entt::registry* reg, UniqueID* comp)
{
	ImGui::Text(std::to_string(comp->id).c_str());
	ImGui::SameLine();
	if (ImGui::Button("Re roll"))
		comp->id = UUID::Generate();
};

template<>
void CustomComponentWidget<Tag>(entt::registry* reg, Tag* comp)
{
	EditString("Tag", comp->tag);
};

template <>
void CustomComponentWidget<Position>(entt::registry* reg, Position* comp)
{
	DragFloatVector2("Position", comp->position);
	ImGui::DragFloat("Angle", &comp->rotation, 0.5f);
	DragFloatVector2("Offset", comp->offset);
}

template<>
void CustomComponentWidget<Parent>(entt::registry* reg, Parent* comp)
{
	ImGui::Text("P Id = ");
	ImGui::SameLine();
	ImGui::Text(std::to_string((int)comp->parent).c_str());
	ImGui::SameLine();
	ImGui::Text(" Depth = ");
	ImGui::SameLine();
	ImGui::Text(std::to_string(comp->depth).c_str());
};

template <>
void CustomComponentWidget<ChildPosition>(entt::registry* reg, ChildPosition* comp)
{
	DragFloatVector2("Position", comp->position);
	ImGui::DragFloat("Angle", &comp->rotation, 0.5f);
	DragFloatVector2("Offset", comp->offset);
}

template <>
void CustomComponentWidget<Velocity>(entt::registry* reg, Velocity* comp)
{
	DragFloatVector2("Vel", comp->velocity);
	ImGui::DragFloat("Angle", &comp->rotation, 0.5f);
}

template <>
void CustomComponentWidget<Camera2D>(entt::registry* reg, Camera2D* comp)
{
	//DragFloatVector2("Target", comp->target);
	//DragFloatVector2("Offset", comp->offset);
	ImGui::DragFloat("Angle", &comp->rotation, 0.5f);
	ImGui::DragFloat("Zoom", &comp->zoom, 0.05f);
}

template <>
void CustomComponentWidget<ScriptComp>(entt::registry* reg, ScriptComp* comp)
{
	std::string name = "No Script";
	if (comp->scriptInstance != nullptr)
		name = comp->scriptInstance->scriptName;

	if (ImGui::BeginCombo("ScriptNames", name.c_str()))
	{
		auto list = ScriptRegistry::GetScriptList();

		for (size_t i = 0; i < list.size(); i++)
		{
			bool selected = false;
			if (list[i] == name)
				selected = true;

			if (ImGui::Selectable(list[i].c_str(), selected))
			{
				if (selected)
					continue;

				if (comp->DestroyScript != nullptr)
					comp->DestroyScript(comp);
				auto e = entt::to_entity(*reg, *comp);

				reg->remove<ScriptComp>(e);
				ScriptRegistry::AddScriptToEntity(list[i], reg, e);
			}
		}

		ImGui::EndCombo();
	}
	//ImGui::DragFloat("Angle", &comp->rotation, 0.5f);
}

template <>
void CustomComponentWidget<Rigidbody2D>(entt::registry* reg, Rigidbody2D* comp)
{
	const char* items[] = { "Static", "Dynamic", "Kinematic" };
	std::string f = Rigidbody2D::TypeToString(comp->type);
	const char* current_item = f.c_str();
	if (ImGui::BeginCombo("Type", current_item))
	{
		for (int n = 0; n < 3; n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				comp->type = Rigidbody2D::StringToType(items[n]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Checkbox("Fixed Rotation", &comp->fixedRotation);
	ImGui::Checkbox("Bullit", &comp->isBullet);
}

template <>
void CustomComponentWidget<Rigidbody2DRectangle>(entt::registry* reg, Rigidbody2DRectangle* comp)
{
	DragFloatVector2("HalfSize", comp->halfSize);
	DragFloatVector2("Offset", comp->offset);
	ImGui::DragFloat("Density", &comp->density, 0.5f);
	ImGui::DragFloat("Friction", &comp->friction, 0.5f);
	ImGui::DragFloat("Restitution", &comp->restitution, 0.5f);
	ImGui::DragFloat("RestitutionThreshhold", &comp->restitutionThreshhold, 0.5f);
}

template <>
void CustomComponentWidget<Rigidbody2DCircle>(entt::registry* reg, Rigidbody2DCircle* comp)
{
	ImGui::DragFloat("Radius", &comp->radius);
	DragFloatVector2("Offset", comp->offset);
	ImGui::DragFloat("Density", &comp->density, 0.5f);
	ImGui::DragFloat("Friction", &comp->friction, 0.5f);
	ImGui::DragFloat("Restitution", &comp->restitution, 0.5f);
	ImGui::DragFloat("RestitutionThreshhold", &comp->restitutionThreshhold, 0.5f);
}

template <>
void CustomComponentWidget<SpriteComp>(entt::registry* reg, SpriteComp* comp)
{
	EditString("Texture", comp->texture);
	DragFloatVector2("Half Size", comp->halfSize); 
	EditColor4("Color", comp->color);
}

template <>
void CustomComponentWidget<RectangleComp>(entt::registry* reg, RectangleComp* comp)
{
	DragFloatVector2("Half Size", comp->halfSize);
	EditColor4("Color", comp->color);
}

template <>
void CustomComponentWidget<CircleComp>(entt::registry* reg, CircleComp* comp)
{
	ImGui::DragFloat("Radius", &comp->radious);
	EditColor4("Color", comp->color);
}

template <>
void CustomComponentWidget<TextComp>(entt::registry* reg, TextComp* comp)
{
	EditString("Text", comp->text);
	ImGui::DragFloat("Size", &comp->size, 0.5f);
	ImGui::DragFloat("Padding", &comp->padding, 0.5f);
	EditColor4("Color", comp->color);
}

template <>
void CustomComponentWidget<Renderable>(entt::registry* reg, Renderable* comp)
{
	ImGui::Checkbox("Show", &comp->show);
}

template <>
void CustomComponentWidget<RenderableUI>(entt::registry* reg, RenderableUI* comp)
{
	ImGui::Checkbox("Show", &comp->show);
}

template <>
void CustomComponentWidget<BB_AABB>(entt::registry* reg, BB_AABB* comp)
{
	ImGui::Checkbox("Static", &comp->isStatic);
	DragFloatVector2("Half Size", comp->halfSize);
	EditColor4("Color", comp->debugColor);
}

template <>
void CustomComponentWidget<BB_RECTANGLE>(entt::registry* reg, BB_RECTANGLE* comp)
{
	ImGui::Checkbox("Static", &comp->isStatic);
	DragFloatVector2("Half Size", comp->halfSize);
	EditColor4("Color", comp->debugColor);
}

template <>
void CustomComponentWidget<BB_CIRCLE>(entt::registry* reg, BB_CIRCLE* comp)
{
	ImGui::Checkbox("Static", &comp->isStatic);
	ImGui::DragFloat("Radius", &comp->circle.r);
	EditColor4("Color", comp->debugColor);
}

template <>
void CustomComponentWidget<BB_POLY>(entt::registry* reg, BB_POLY* comp)
{
	ImGui::Checkbox("Static", &comp->isStatic);

	ImGui::SeparatorText("Verts");
	for (size_t i = 0; i < comp->points.size(); i++)
		DragFloatVector2("P" + std::to_string(i), comp->points[i], 0.025f);

	EditColor4("Color", comp->debugColor);
}

template <>
void CustomComponentWidget<BB_CAP>(entt::registry* reg, BB_CAP* comp)
{
	ImGui::Checkbox("Static", &comp->isStatic);

	DragFloatVector2("P1", comp->p1);
	DragFloatVector2("P2", comp->p2);
	ImGui::DragFloat("Radius", &comp->radius);

	EditColor4("Color", comp->debugColor);
}