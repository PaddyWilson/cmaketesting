#pragma once
#include "ImguiWindow.h"

#include <stdint.h>
#include <entt/entt.hpp>

template<typename T>
void CustomComponentWidget(entt::registry* reg, T* comp)
{
	ImGui::Text("Nothing is here");
};

class ImguiEntityEditor : public ImguiWindow
{
private:
	entt::registry* reg;
	entt::entity selected;

	struct RegistryData {
		int priority = 5;
		std::string compName = "Default Comp";
		std::function<void(entt::registry*, entt::entity, std::string)> widgetDisplay;
		std::function<void(entt::registry*, entt::entity)> addComponent;
		std::function<void(entt::registry*, entt::entity)> removeComponent;
	};

	std::unordered_map<entt::id_type, RegistryData> compsRegistry;

	template<typename T>
	void DisplayCustomWidget(entt::registry* reg, entt::entity entity, std::string compName)
	{
		T* comp = reg->try_get<T>(entity);
		if (comp != nullptr) {
			if (ImGui::TreeNode(compName.c_str())) {
				CustomComponentWidget<T>(reg, comp);
				ImGui::TreePop();
			}
		}
	};

	void DisplayEntity(entt::entity entity);

	template<typename T>
	void AddComponent(entt::registry* reg, entt::entity entity)
	{
		T* comp = reg->try_get<T>(entity);
		if (comp == nullptr) {//no component found of type so add one
			reg->emplace<T>(entity);
		}
	};

	template<typename T>
	void RemoveComponent(entt::registry* reg, entt::entity entity)
	{
		T* comp = reg->try_get<T>(entity);
		if (comp) {//component found of type so remove it
			reg->remove<T>(entity);
		}
	};

public:
	void Init();
	//void End();
	//void Update();
	void Draw();

	void SetRegistry(entt::registry* reg)
	{
		//if (reg != this->reg)
			//selected = 0;
		this->reg = reg;
	}

	template<typename T>
	void RegisterComponent(std::string compName, int priority = 5)
	{
		//get comp hash to use as index
		constexpr entt::id_type index = entt::type_hash<T>::value();

		//set functions to trysave / tryload
		RegistryData data;
		data.priority = priority;
		data.compName = compName;
		//this does not work? bind might
		data.widgetDisplay = [&](entt::registry* reg, entt::entity entity, std::string name) {
			this->DisplayCustomWidget<T>(reg, entity, name); };

		data.addComponent = [&](entt::registry* reg, entt::entity entity) {
			this->AddComponent<T>(reg, entity); };

		data.removeComponent = [&](entt::registry* reg, entt::entity entity) {
			this->RemoveComponent<T>(reg, entity); };

		compsRegistry.insert_or_assign(index, data);
	}
};

