#include "ParentChildSystem.h"

#include "../EventManager.h"
#include "../Events.h"
#include "../Components.h"

void ParentChildSystem::Init()
{
	this->registry->on_construct<Parent>().connect<&ParentChildSystem::AddParent>(this);
	this->registry->on_destroy<Parent>().connect<&ParentChildSystem::RemoveParent>(this);

	EventManager::AddListener<EVENTS::SerializeOnLoadFinished>([&](EVENTS::SerializeOnLoadFinished e) { OnSerializeLoad(e.reg); });
}

void ParentChildSystem::Update(float deltaTime)
{

}

void ParentChildSystem::AddParent(entt::registry& reg, entt::entity entity)
{
	//get parent comp
	auto& p = reg.get<Parent>(entity);

	//get parents parent comp to check depth
	Parent* pd = reg.try_get<Parent>(p.parent);
	if (pd)
	{
		p.depth = pd->depth + 1;
	}

	//get parents children comp to add to
	Children* pChildren = reg.try_get<Children>(p.parent);
	//if parent does not have a children comp add one
	if (!pChildren)
		pChildren = &reg.emplace<Children>(p.parent);

	if (pChildren)
		pChildren->children.push_back(entity);

	UpdateParentDepthOfChildren(reg, entity, p.depth);

	ChildPosition* hasChildPosition = reg.try_get<ChildPosition>(entity);
	if (hasChildPosition)//don't need to add ChildPosition if one is present
		return;

	auto& cp = reg.emplace<ChildPosition>(entity);
	auto& pos = reg.get<Position>(entity);

	cp.offset = pos.offset;
	cp.position = pos.position;
	cp.rotation = pos.rotation;
}

void ParentChildSystem::RemoveParent(entt::registry& reg, entt::entity entity)
{
	//get parent comp
	auto parentEntity = reg.get<Parent>(entity).parent;

	//get parents parent comp to check depth
	Children* children = reg.try_get<Children>(parentEntity);
	if (children)
	{
		auto& vector = children->children;

		std::vector<entt::entity>::iterator position = std::find(vector.begin(), vector.end(), entity);
		if (position != vector.end()) // == vector.end() means the element was not found
			vector.erase(position);

		if (children->children.size() == 0) {
			reg.remove<Children>(parentEntity);
			reg.get<Parent>(entity).depth = -1;
		}
	}

	UpdateParentDepthOfChildren(reg, entity, reg.get<Parent>(entity).depth);

	reg.remove<ChildPosition>(entity);
}

void ParentChildSystem::UpdateParentDepthOfChildren(entt::registry& reg, entt::entity entity, int depth)
{
	Parent* parent = reg.try_get<Parent>(entity);
	if (parent)
		parent->depth = depth;

	Children* children = reg.try_get<Children>(entity);
	if (children)
	{
		for (size_t i = 0; i < children->children.size(); i++)
		{
			UpdateParentDepthOfChildren(reg, children->children[i], depth + 1);
		}
	}
}

void ParentChildSystem::OnSerializeLoad(entt::registry* reg)
{
	auto view = reg->view<ParentLoad>();
	for (auto [entity, parent] : view.each())
	{
		//find entity with tag
		auto view2 = reg->view<UniqueID>();
		for (auto [entityParent, id] : view2.each())
		{
			if (id.id == parent.parentID)
			{
				reg->emplace<Parent>(entity, entityParent);
				reg->remove<ParentLoad>(entity);
				break;
			}
		}
	}
}
