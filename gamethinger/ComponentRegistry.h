#pragma once
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>

#include "EventManager.h"

template<typename T>
class ComponentRegistryAdd 
{
public:
	std::string compName = "Default Comp";
	int priority = 5;
	entt::id_type compType;
	//std::function<void(A...)> widgetDisplay;
};

class ComponentRegistry
{
private:
	//std::unordered_map<entt::id_type, RegistryData> compsRegistry;
	//entt::registry* reg;

	/*template<typename T>
	void DisplayCustomWidget(entt::registry* reg, entt::entity entity, std::string compName)
	{
		T* comp = reg->try_get<T>(entity);
		if (comp != nullptr) {
			if (ImGui::TreeNode(compName.c_str())) {
				CustomComponentWidget<T>(reg, comp);
				ImGui::TreePop();
			}
		}
	};*/

public:
	template<typename T>
	static void RegisterComponent(std::string compName, int priority = 5)
	{
		//get comp hash to use as index
		constexpr entt::id_type index = entt::type_hash<T>::value();

		//set functions to trysave / tryload
		ComponentRegistryAdd<T> data;
		data.priority = priority;
		data.compName = compName;
		data.compType = index;

		//compsRegistry.insert_or_assign(index, data);

		EventManager::TriggerEvent<ComponentRegistryAdd<T>>(data);
	};
};

