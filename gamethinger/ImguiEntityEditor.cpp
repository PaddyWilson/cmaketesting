#include "ImguiEntityEditor.h"
#include "Components.h"
#include "PreFabs.h"

bool IsEntityLower(entt::registry* reg, entt::entity find, entt::entity search)
{
	if (search == find)
		return true;

	auto* parentComp = reg->try_get<Parent>(search);
	if (parentComp) {
		return IsEntityLower(reg, find, parentComp->parent);
	}
	return false;
}

void DragAndDropEntityInTree(entt::registry* reg, entt::entity& selected, entt::entity treeEntity) {
	if (ImGui::BeginDragDropTarget()) {
		// Some processing...

		if (const auto payload = ImGui::AcceptDragDropPayload("MoveEntity")) {
			const uint32_t dropEntity = (const uint32_t)payload->Data;
			//selected = (entt::entity)dropEntity;
			//std::cout << "Drop " << (uint32_t)dropEntity << " " << (uint32_t)selected << std::endl;

			//make sure selectedEntity is not in the lower in the tree
			if (!IsEntityLower(reg, selected, treeEntity))
			{
				//get parent of selected and remove referance from children comp
				auto* parentComp = reg->try_get<Parent>(selected);
				if (parentComp)
				{
					reg->remove<Parent>(selected);
				}
				if (treeEntity != entt::null)
					reg->emplace<Parent>(selected, treeEntity);
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void ImguiEntityEditor::DisplayEntity(entt::entity entity)
{

	std::string nodeName = "No Tag";

	Tag* tag = reg->try_get<Tag>(entity);
	if (tag)
		nodeName = tag->tag;

	nodeName = nodeName + "##" + std::to_string((int)entity);// std::to_string(id->id);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (entity == selected)
		flags |= ImGuiTreeNodeFlags_Selected;

	//dont display arrow if no children
	Children* children = reg->try_get<Children>(entity);
	if (!children || children->children.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(nodeName.c_str(), flags);

	DragAndDropEntityInTree(reg, selected, entity);

	if (ImGui::BeginDragDropSource()) {
		// Some processing...
		const uint32_t source = (uint32_t)selected;
		ImGui::SetDragDropPayload("MoveEntity", &source, sizeof(uint32_t));
		//std::cout << "Drag " << source << std::endl;
		selected = entity;

		ImGui::EndDragDropSource();
	}

	if (ImGui::IsItemClicked())
		selected = entity;

	if (opened)
	{
		if (children)
		{
			for (size_t i = 0; i < children->children.size(); i++)
			{
				DisplayEntity(children->children[i]);
			}
		}
		ImGui::TreePop();
	}
}

void ImguiEntityEditor::Init()
{
	WindowName = "Entity Editor";
}

void ImguiEntityEditor::Draw()
{
	if (!IsOpen)
		return;

	//ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(400, 400));
	if (ImGui::Begin(WindowName.c_str(), &IsOpen, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SeparatorText("Entity List");

		ImGui::Text("Entity Count:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(reg->size()).c_str());
		ImGui::SameLine();

		if (ImGui::Button("New Entity"))
		{
			Prefabs::CreateBasicEntity(reg, Vector2(10, 10), "New Entity");
		}

		ImGui::BeginChild("Entities#123123", ImVec2(ImGui::GetContentRegionAvail().x * 1.f, 350), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);


		//display tree of entities
		auto view = reg->view<Tag>(entt::exclude<Parent>);
		for (auto [entity, tag] : view.each())
		{
			DisplayEntity(entity);
		}

		if (ImGui::BeginPopupContextWindow())
		{

			ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New Entity"))
			{
				selected = Prefabs::CreateBasicEntity(reg, Vector2(10, 10), "New Entity");
			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::Selectable("Test"))
			{
			}

			ImGui::EndPopup();
		}

		ImGui::EndChild();


		//for some reason child window DragDrop has to be after EndChild() not in
		DragAndDropEntityInTree(reg, selected, entt::null);

		//display components
		ImGui::SeparatorText("Edit Components");

		ImGui::BeginChild("Edit Components#123123", ImVec2(ImGui::GetContentRegionAvail().x * 1.f, 350), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		std::vector<RegistryData> compToAdd;

		for (size_t i = 0; i < 10; i++)
		{
			//loop compReg
			for (auto& [type_id, function] : compsRegistry)
			{
				if (function.priority != i)
					continue;

				//get storage pool for components by hash value
				auto* storage_ptr = reg->storage(type_id);

				if (storage_ptr == nullptr)
					continue;

				bool contians = storage_ptr->contains(selected);
				if (contians) {
					function.widgetDisplay(reg, selected, function.compName);

					if (ImGui::BeginPopupContextWindow())
					{
						ImGui::MenuItem("Remove", NULL, false, false);
						if (ImGui::MenuItem(function.compName.c_str()))
						{
							function.removeComponent(reg, selected);
						}
						ImGui::EndPopup();
					}
				}
				else
				{
					compToAdd.push_back(function);
				}
			}
		}

		if (ImGui::BeginPopupContextWindow())
		{
			ImGui::MenuItem("Components", NULL, false, false);

			for (size_t i = 0; i < compToAdd.size(); i++)
			{
				if (ImGui::MenuItem(compToAdd[i].compName.c_str()))
				{
					compToAdd[i].addComponent(reg, selected);
					//Prefabs::CreateBasicEntity(reg, Vector2(10, 10), "New Entity");
				}
			}

			/*if (ImGui::MenuItem("New Entity"))
			{
				Prefabs::CreateBasicEntity(reg, Vector2(10, 10), "New Entity");
			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::Selectable("Clear"))
			{
			}*/

			ImGui::EndPopup();
		}

		ImGui::EndChild();
		//used to drop entities onto level 0 hierarchy

		ImGui::End();
	}
}
