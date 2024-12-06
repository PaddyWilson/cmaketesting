#pragma once

#include <unordered_map>
#include <iostream>
#include <functional>
#include <string>
#include <entt/entt.hpp>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

//forward declair BaseScene
class BaseScene;

//can use Template Specialization to customize this
template<typename T>
void SaveComponent(YAML::Emitter& out, entt::registry& reg, T* comp) {
	std::cout << "SaveComponent() not implimented for \"" << typeid(*comp).name() << "\"" << std::endl;
};

//can use Template Specialization to customize this
template<typename T>
void LoadComponent(entt::registry& reg, entt::entity entity, YAML::Node& data) {
	std::cout << "LoadComponent() not implimented for \"" << typeid(T).name() << "\"" << std::endl;
};

class Serialize
{
private:
	struct RegistryData {
		int priority = 10;
		std::string nodeName;
		std::function<void(YAML::Emitter&, entt::registry&, entt::entity, std::string)> save;
		std::function<void(entt::registry&, entt::entity, YAML::Node&, std::string)> load;
	};
	//i dont really want this static
	std::unordered_map<entt::id_type, RegistryData> compsRegistry;

	template<typename T>
	void TrySaveComponent(YAML::Emitter& out, entt::registry& reg, entt::entity entity, std::string name)
	{
		T* comp = reg.try_get<T>(entity);
		if (comp != nullptr) {
			out << YAML::Key << name;
			SaveComponent<T>(out, reg, comp);
		}
	};

	template<typename T>
	void TryLoadComponent(entt::registry& reg, entt::entity entity, YAML::Node& node, std::string nodeName)
	{
		if (compsRegistry.size() == 0)
		{
			std::cout << "No components in registered to load" << std::endl;
			return;
		}
		auto n = node[nodeName];
		if (n)
			LoadComponent<T>(reg, entity, n);
	};

public:
	void Save(std::string file, BaseScene* scene);
	void Load(std::string file, BaseScene* scene);

	void SerializeEntity(YAML::Emitter& out, entt::registry& reg, entt::entity entity);
	void DeserializeEntity(YAML::Node& data, entt::registry& reg);

	template<typename T>
	void RegisterComponent(std::string nodeName, int priority = 5)
	{
		//get comp hash to use as index
		constexpr entt::id_type index = entt::type_hash<T>::value();

		//set functions to trysave / tryload
		RegistryData data;
		data.priority = priority;
		data.nodeName = nodeName;
		//this does not work? bind might
		/*data.save = &Serialize::TrySaveComponent<T>;
		data.load = &Serialize::TryLoadComponent<T>;*/
		data.save = [&](YAML::Emitter& out, entt::registry& reg, entt::entity entity, std::string name) {
			this->TrySaveComponent<T>(out, reg, entity, name); };
		data.load = [&](entt::registry& reg, entt::entity entity, YAML::Node& node, std::string nodeName) {
			this->TryLoadComponent<T>(reg, entity, node, nodeName); };

		compsRegistry.insert_or_assign(index, data);
	}
};
