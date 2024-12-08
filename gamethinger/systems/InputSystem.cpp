#include "InputSystem.h"

#include <imgui.h>

void InputSystem::Update(float deltaTime)
{
	// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	//ImGuiIO& io = ImGui::GetIO();

	// (2) ONLY forward mouse data to your underlying app/game.
	//if (io.WantCaptureMouse)
		//return;

	auto view = registry->view<InputComp>();
	for (auto [entity, input] : view.each())
	{
		for (size_t i = 0; i < input.inputs.size(); i++)
		{
			bool thisUpdate = false;

			if (input.inputs[i].buttonState == InputAction::DOWN)
				thisUpdate = IsKeyDown(input.inputs[i].key);
			else if (input.inputs[i].buttonState == InputAction::PRESSED)
				thisUpdate = IsKeyPressed(input.inputs[i].key);
			else if (input.inputs[i].buttonState == InputAction::RELEASED)
				thisUpdate = IsKeyReleased(input.inputs[i].key);
			else if (input.inputs[i].buttonState == InputAction::UP)
				thisUpdate = !IsKeyDown(input.inputs[i].key);

			if (thisUpdate)
				input.inputs[i].func();

			//if (thisUpdate)
			//	int asdfasdf = 0;

			//Clickable::Action action = Clickable::Action::NONE;
			//if (!click.lastUpdate && thisUpdate)
			//	action = Clickable::Action::PRESSED;
			//else if (click.lastUpdate && thisUpdate)//has one frame of lag
			//	action = Clickable::Action::DOWN;
			//else if (click.lastUpdate && !thisUpdate)
			//	action = Clickable::Action::RELEASED;
			//else if (!click.lastUpdate && !thisUpdate)//has one frame of lag
			//	action = Clickable::Action::UP;

			//if (click.action == action && click.func != NULL)
			//	click.func();

			//click.lastUpdate = thisUpdate;
		}
	}
}
