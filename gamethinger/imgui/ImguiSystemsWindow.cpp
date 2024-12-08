#include "ImguiSystemsWindow.h"

#include <typeinfo>

void ImguiSystemsWindow::Draw()
{
	if (!IsOpen)
		return;

	if (ImGui::Begin(WindowName.c_str(), &IsOpen, ImGuiWindowFlags_NoCollapse))
	{
        ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

        if (ImGui::BeginTable("Systems Table", 4, flags))
        {
            ImGui::TableNextColumn();
            ImGui::TableHeader("System");
            ImGui::TableNextColumn();
            ImGui::TableHeader("Update");
            ImGui::TableNextColumn();
            ImGui::TableHeader("Render");
            ImGui::TableNextColumn();
            ImGui::TableHeader("RenderUI");

            for (size_t i = 0; i < systemManager->systems.size(); i++)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                std::string name = systemManager->systems[i]->systemName;
                ImGui::Text(name.c_str());

                ImGui::TableNextColumn();
                std::string update = name + "Update";
                ImGui::PushID(update.c_str());
                ImGui::Checkbox("Update", &systemManager->systems[i]->updateEnabled);
                ImGui::PopID();

                ImGui::TableNextColumn();
                std::string render = name + "Render";
                ImGui::PushID(render.c_str());
                ImGui::Checkbox("Render", &systemManager->systems[i]->renderEnabled);
                ImGui::PopID();
                
                ImGui::TableNextColumn();
                std::string renderUI = name + "RenderUI";
                ImGui::PushID(renderUI.c_str());
                ImGui::Checkbox("RenderUI", &systemManager->systems[i]->renderUIEnabled);
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
		ImGui::End();//window
	}
}
