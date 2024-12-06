#include "ScriptRegistry.h"

std::unordered_map<std::string, ScriptRegistry::ScriptRegistryData> ScriptRegistry::scriptRegistry;// = {};

void ScriptRegistry::AddScriptToEntity(std::string scriptName, entt::registry* reg, entt::entity entity)
{
	if (scriptRegistry.contains(scriptName))
		scriptRegistry[scriptName].addScript(scriptName, reg, entity);
	else
		std::cout << "No Script registered with name \"" << scriptName << "\"" << std::endl;

}

std::vector<std::string> ScriptRegistry::GetScriptList()
{
	std::vector<std::string> names;
	for (auto var : scriptRegistry)
		names.push_back(var.first);
	return names;
}
