#pragma once

#include <unordered_map>
#include <iostream>
#include <functional>
#include <string>

#include "Components.h"
#include "Script.h"

class ScriptRegistry
{
public:
	struct ScriptRegistryData {
		std::string scriptName = "Default Script";
		std::function<void(std::string, entt::registry*, entt::entity)> addScript;
		//std::function<void(entt::registry*, entt::entity)> removeScript;
	};

private:
	static std::unordered_map<std::string, ScriptRegistryData> scriptRegistry;

	template<typename T>
	static void AddScript(std::string name, entt::registry* reg, entt::entity entity)
	{
		reg->emplace<ScriptComp>(entity).Bind<T>();
	};
public:

	static void AddScriptToEntity(std::string scriptName, entt::registry* reg, entt::entity entity);

	static std::vector<std::string> GetScriptList();	

	template<typename T>
	static void RegisterScript()
	{
		static_assert(std::is_base_of<Script, T>::value, "T must derive from Script");

		T temp;
		//set functions to trysave / tryload
		ScriptRegistryData data;
		data.scriptName = temp.scriptName;
		data.addScript = [&](std::string name ,entt::registry* reg, entt::entity entity) {
			AddScript<T>(name, reg, entity); };

		//data.removeScript = [&](entt::registry* reg, entt::entity entity) {
		//	RemoveScript<T>(reg, entity); };

		scriptRegistry.insert_or_assign(data.scriptName, data);
	}
};


