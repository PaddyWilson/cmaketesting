#pragma once
#include "ImguiEntt.h"

//Used to display values in ImguiEntt editor
namespace MM {
	template <>
	void ComponentEditorWidget<Position>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& t = reg.get<Position>(e);
		ImGui::DragFloat("x", &t.position.x, 0.5f);
		ImGui::DragFloat("y", &t.position.y, 0.5f);
		ImGui::DragFloat("rotation", &t.rotation, 0.5f);
		ImGui::DragFloat("off x", &t.offset.x, 0.5f);
		ImGui::DragFloat("off y", &t.offset.y, 0.5f);
	}

	template <>
	void ComponentEditorWidget<Velocity>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& t = reg.get<Velocity>(e);
		ImGui::DragFloat("Velocity x", &t.velocity.x, 0.5f);
		ImGui::DragFloat("Velocity y", &t.velocity.y, 0.5f);
		ImGui::DragFloat("rotation", &t.rotation, 0.5f);
	}

	template <>
	void ComponentEditorWidget<TextComp>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& v = reg.get<TextComp>(e);

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), v.text.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			v.text = std::string(buffer);
		}
		//ImGui::DragInt("Size", &v.size, 0.5f);

		/*int r = v.color.r;
		int b = v.color.b;
		int g = v.color.g;
		int a = v.color.a;

		ImGui::DragInt("r", &r, 1);
		ImGui::DragInt("g", &g, 1);
		ImGui::DragInt("b", &b, 1);
		ImGui::DragInt("a", &a, 1);

		v.color.r = r;
		v.color.b = b;
		v.color.g = g;
		v.color.a = a;*/
	}


	template <>
	void ComponentEditorWidget<RectangleComp>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& v = reg.get<RectangleComp>(e);
		//ImGui::DragFloat("w", &v.rec.width, 0.5f);
		//ImGui::DragFloat("h", &v.rec.height, 0.5f);

		/*int r = v.color.r;
		int b = v.color.b;
		int g = v.color.g;
		int a = v.color.a;

		ImGui::DragInt("r", &r, 1);
		ImGui::DragInt("g", &g, 1);
		ImGui::DragInt("b", &b, 1);
		ImGui::DragInt("a", &a, 1);

		v.color.r = r;
		v.color.b = b;
		v.color.g = g;
		v.color.a = a;*/
	}
}