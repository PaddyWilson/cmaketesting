#include <fstream>
#include <iostream>

#include "Serialize.h"
#include "EventManager.h"
#include "Events.h"
#include "BaseScene.h"

void Serialize::SerializeEntity(YAML::Emitter& out, entt::registry& reg, entt::entity entity)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity";

	out << YAML::BeginMap;

	//loop compReg
	for (auto& [type_id, function] : compsRegistry)
	{
		//get storage pool for components by hash value
		auto* storage_ptr = reg.storage(type_id);

		if (storage_ptr == nullptr)
			continue;

		bool contians = storage_ptr->contains(entity);
		if (contians)
			function.save(out, reg, entity, function.nodeName);
	}

	out << YAML::EndMap;
	out << YAML::EndMap;
}

void Serialize::DeserializeEntity(YAML::Node& data, entt::registry& reg)
{
	auto entity = reg.create();

	//there is most likely a better way
	for (size_t i = 0; i < 10; i++)
	{
		//loop compReg
		for (auto& [type_id, function] : compsRegistry)
		{
			if (function.priority == i)
				function.load(reg, entity, data, function.nodeName);
		}
	}
}

void Serialize::Save(std::string file, BaseScene* scene)
{
	//yaml test
	YAML::Emitter out;

	//save sceneData
	out << YAML::BeginMap;
	out << YAML::Key << "SceneName" << YAML::Value << scene->SceneName;

	out << YAML::Key << "WorldGravity_X" << YAML::Value << scene->gravity.x;
	out << YAML::Key << "WorldGravity_Y" << YAML::Value << scene->gravity.y;
	out << YAML::Key << "WorldVelocityIterations" << YAML::Value << scene->velocityIterations;
	out << YAML::Key << "WorldPositionIterations" << YAML::Value << scene->positionIterations;

	//save entities
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

	if (compsRegistry.size() == 0)
		std::cout << "No components are registered to save" << std::endl;

	scene->registry->each([&](entt::entity entity) {
		//if (compsRegistry.size() != 0)
		SerializeEntity(out, *scene->registry, entity);
		});//for each entity
	//done saving entities
	out << YAML::EndSeq;

	//end of first map
	out << YAML::EndMap;

	std::ofstream fout(file);
	fout << out.c_str();
	fout.close();
}

void Serialize::Load(std::string file, BaseScene* scene)
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(file);
	}
	catch (YAML::ParserException e)
	{
		std::cout << "Error loading file \"" << file << "\"." << e.what() << std::endl;
		return;
	}

	//load scene data
	std::string sceneName = data["SceneName"].as<std::string>();
	scene->gravity.x = data["WorldGravity_X"].as<float>();
	scene->gravity.y = data["WorldGravity_Y"].as<float>();;
	scene->velocityIterations = data["WorldVelocityIterations"].as<int>();
	scene->velocityIterations = data["WorldPositionIterations"].as<int>();

	//load entities
	auto entities = data["Entities"];

	if (compsRegistry.size() == 0)
		std::cout << "No components are registered to load" << std::endl;

	for (size_t i = 0; i < entities.size(); i++)
	{
		auto entityNode = entities[i]["Entity"];
		//no entity
		if (!entityNode)
			continue;

		if (compsRegistry.size() != 0)
			DeserializeEntity(entityNode, *scene->registry);
	}

	EVENTS::SerializeOnLoadFinished ev;
	ev.reg = scene->registry;
	EventManager::TriggerEvent<EVENTS::SerializeOnLoadFinished>(ev);
}