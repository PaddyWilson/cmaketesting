// GameThinger.cpp : This file contains the 'main' function. Program execution begins and ends there.

#define CUTE_C2_IMPLEMENTATION
#include <cute_c2.h>

//raylib flags
//#define SUPPORT_DEFAULT_FONT (default)
//#define SUPPORT_CAMERA_SYSTEM
//#define SUPPORT_GESTURES_SYSTEM
//#define SUPPORT_MOUSE_GESTURES
//#define SUPPORT_BUSY_WAIT_LOOP
//#define SUPPORT_PARTIALBUSY_WAIT_LOOP
#define SUPPORT_SCREEN_CAPTURE
#define SUPPORT_GIF_RECORDING
//#define SUPPORT_COMPRESSION_API
#define SUPPORT_AUTOMATION_EVENTS

//entt flags
//for pointer stability
//#define ENTT_PACKED_PAGE 1024
//#define ENTT_USE_ATOMIC

#include <iostream>
#include <fstream>
#include <raylib.h>

#include <imgui-master/imgui.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>
#include <entt/entt.hpp>

#include <box2d/box2d.h>

#include "Global.h"
#include "TextureManager.h"

//systems
#include "SystemHeaders.h"

//scenes
#include "ScenesHeader.h"

//imgui
#include "ImguiHeader.h"
#include "ImguiEntityEditorCustom.h"

#include "EventManager.h"

#include "Serialize.h"
#include "SerializeCustom.h"

#include "ScriptRegistry.h"
#include "TestScripts.h"

#include "TheadPool.h"

using namespace std;

SystemManager* AddSystem(SystemManager* systemManager) {
	//create systems
	systemManager->AddSystem(new DestroySystem());
	systemManager->AddSystem(new InputSystem());
	systemManager->AddSystem(new ParentChildSystem());
	systemManager->AddSystem(new ScriptSystem());
	systemManager->AddSystem(new MovementSystem());
	systemManager->AddSystem(new Box2DSystem());
	systemManager->AddSystem(new CuteC2System());
	systemManager->AddSystem(new HoverSystem());
	systemManager->AddSystem(new Camera2DSystem());
	systemManager->AddSystem(new ClickableSystem());
	systemManager->AddSystem(new RenderSystem());
	return systemManager;
}

template<typename T>
void RegisterComponet(Serialize& ser, ImguiEntityEditor& editor, std::string compName, int priority = 5)
{
	ser.RegisterComponent<T>(compName, priority);
	editor.RegisterComponent<T>(compName, priority);
}

int main(void)
{
	ThreadPool* threadPool = ThreadPool::GetInstance(1);

	SetTraceLogLevel(7);//debug level. 7 hides everything

	// i know you can just or them together
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	//SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(Global::ScreenWidth, Global::ScreenHeight, "raylib [core] example - basic window");

	InitAudioDevice();
	SetTargetFPS(200);

	rlImGuiSetup(true);//imgui
	TextureManager::Init();

	ScriptRegistry::RegisterScript<TestController>();
	ScriptRegistry::RegisterScript<TestController2>();
	ScriptRegistry::RegisterScript<SpaceShipController>();
	ScriptRegistry::RegisterScript<MeteorController>();
	ScriptRegistry::RegisterScript<MeteorSpawnerScript>();

	//create ecs
	SceneManager sceneManager;
	sceneManager.AddScene(new BlankScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new TestScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new NetworkTestingScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new MTGGameScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new Box2dTestScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new Cute_c2TestScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();
	sceneManager.AddScene(new SpaceShooterScene());
	AddSystem(sceneManager.GetActiveScene()->GetSystemManager());
	sceneManager.GetActiveScene()->Init();

	/*Image largeimage = GenImageColor(MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, GREEN);
	Texture2D texture = LoadTextureFromImage(largeimage);
	UnloadImage(largeimage);*/

	vector<ImguiWindow*> imguiWindows;

	ImguiSystemsWindow systemWindow;
	imguiWindows.push_back(&systemWindow);

	ImguiEntityEditor editor2;
	editor2.IsOpen = true;
	imguiWindows.push_back(&editor2);

	//maybe have one static class to register components
	Serialize serializer;
	RegisterComponet<UniqueID>(serializer, editor2, "UniqueID", 0);
	RegisterComponet<Tag>(serializer, editor2, "Tag", 0);
	RegisterComponet<Position>(serializer, editor2, "Position", 0);
	RegisterComponet<Velocity>(serializer, editor2, "Velocity", 0);
	RegisterComponet<Rigidbody2D>(serializer, editor2, "Rigidbody2D");
	RegisterComponet<Rigidbody2DRectangle>(serializer, editor2, "Rigidbody2DRectangle");
	RegisterComponet<Rigidbody2DCircle>(serializer, editor2, "Rigidbody2DCircle");
	RegisterComponet<Camera2D>(serializer, editor2, "Camera2D");
	RegisterComponet<TextComp>(serializer, editor2, "TextComp");
	RegisterComponet<CircleComp>(serializer, editor2, "CircleComp");
	RegisterComponet<RectangleComp>(serializer, editor2, "RectangleComp");
	//RegisterComponet<ColorComp>(serializer, editor2, "ColorComp");
	RegisterComponet<SpriteComp>(serializer, editor2, "SpriteComp");
	RegisterComponet<Renderable>(serializer, editor2, "Renderable");
	RegisterComponet<RenderableUI>(serializer, editor2, "RenderableUI");
	RegisterComponet<BB_AABB>(serializer, editor2, "BB_AABB");
	RegisterComponet<BB_RECTANGLE>(serializer, editor2, "BB_RECTANGLE");
	RegisterComponet<BB_CIRCLE>(serializer, editor2, "BB_CIRCLE");
	RegisterComponet<BB_POLY>(serializer, editor2, "BB_POLY");
	RegisterComponet<BB_CAP>(serializer, editor2, "BB_CAP");
	RegisterComponet<Parent>(serializer, editor2, "Parent");
	//RegisterComponet<Children>(serializer, editor2, "Children");
	RegisterComponet<ChildPosition>(serializer, editor2, "ChildPosition");
	RegisterComponet<ScriptComp>(serializer, editor2, "ScriptComp");
	RegisterComponet<DestroyFlag>(serializer, editor2, "DestroyFlag");

	RenderTestWindow testWindow;
	imguiWindows.push_back(&testWindow);

	bool Quit = false,
		ImGuiDemoOpen = false,
		showFPS = true,
		capFPS = true;

	//Setup ImGui menu bar
	ImguiMenu::SetSceneManager(&sceneManager);

	ImguiMenu::AddFileMenu("Open", [&] {
		string filePath;
		cout << filePath << endl;
		if (OpenFileDialog(GetWorkingDirectory(), filePath))
			serializer.Load(filePath, sceneManager.GetActiveScene());
		});
	ImguiMenu::AddFileMenu("Load", [&] {
		sceneManager.GetActiveScene()->ClearScene();
		serializer.Load("Testing.yml", sceneManager.GetActiveScene());
		//this should be some where else
		//re add bodies to world
		auto registry = sceneManager.GetActiveScene()->GetRegistry();
		auto view = registry->view<Position, Rigidbody2D>();
		for (auto [entity, position, rigidbody] : view.each()) {

			auto* body = rigidbody.Init(*sceneManager.GetActiveScene()->GetWorld(), position);

			auto rec = registry->try_get<Rigidbody2DRectangle>(entity);
			if (rec)
				rec->Init(body);

			auto cir = registry->try_get<Rigidbody2DCircle>(entity);
			if (cir)
				cir->Init(body);
		}
		});
	ImguiMenu::AddFileMenu("Save", [&] { serializer.Save("Testing.yml", sceneManager.GetActiveScene()); });
	ImguiMenu::AddFileMenu("Save as", [&] {
		string filePath;
		cout << filePath << endl;
		if (SaveFileDialog(GetWorkingDirectory(), filePath))
			serializer.Save(filePath, sceneManager.GetActiveScene());
		});
	ImguiMenu::AddFileMenu("Clear", [&] { sceneManager.GetActiveScene()->ClearScene(); });
	ImguiMenu::AddFileMenu("Quit", &Quit);

	ImguiMenu::AddWindowMenu("Show FPS", &showFPS);
	ImguiMenu::AddWindowMenu("Cap FPS", [&] {if (capFPS) { SetTargetFPS(0); capFPS = !capFPS; } else { SetTargetFPS(120); capFPS = !capFPS; } });
	ImguiMenu::AddWindowMenu("ImGui Demo", &ImGuiDemoOpen);

	ImguiMenu::AddCustomWindow(&editor2);
	ImguiMenu::AddCustomWindow(&testWindow);
	ImguiMenu::AddCustomWindow(&systemWindow);

	for (size_t i = 0; i < imguiWindows.size(); i++)
		imguiWindows[i]->Init();

	//event testing
	enum Tests { test };

	EventManager::AddListener<Tests>([&](Tests t) { cout << "Tests" << endl; });
	EventManager::AddListener<Tests>([&](Tests t) { cout << "Tests 2" << endl; });

	Tests tes = Tests::test;
	EventManager::TriggerEvent<Tests>(tes);

	//Downloader::Download("https://cards.scryfall.io/png/front/9/4/94eea6e3-20bc-4dab-90ba-3113c120fb90.png?1670031714", (string)GetWorkingDirectory() + "\\card.png");

	/*auto asdf = LoadDirectoryFiles(GetWorkingDirectory());
	for (size_t i = 0; i < asdf.count; i++)
	{
		cout << IsPathFile(asdf.paths[i]) << " " << asdf.paths[i] << endl;
	}*/

	while (!Quit)
	{
		if (WindowShouldClose())
			break;

		TextureManager::Update();
		rlImGuiBegin();
		//start update 

		bool updateScreenSize = false;
		updateScreenSize = GetScreenHeight() != Global::ScreenHeight;
		if (GetScreenWidth() != Global::ScreenWidth)
			updateScreenSize = true;

		if (updateScreenSize)
		{
			Global::ScreenHeight = GetScreenHeight();
			Global::ScreenWidth = GetScreenWidth();
		}

		//if (IsKeyPressed(KEY_S))
		//	TakeScreenshot("test.png");

		sceneManager.Update(GetFrameTime());
		testWindow.Update();

		//end update
		//----------------------------
		//start draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//in camera

		//moved to render system
		//BeginMode2D(camera);		

		sceneManager.Render();

		//end camera
		//EndMode2D();

		//render ui
		sceneManager.RenderUI();
		testWindow.Draw();

		//DrawRing(pos, 100, 200, 0, 390, 2, RED);

		if (ImGuiDemoOpen) {
			// show ImGui Content
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);
		}

		//menu bar
		ImguiMenu::DrawMenu();

		//editor.SetRegistryAndEntity(sceneManager.GetActiveScene()->registry, &editorEntity);
		editor2.SetRegistry(sceneManager.GetActiveScene()->GetRegistry());
		systemWindow.systemManager = sceneManager.GetActiveScene()->GetSystemManager();

		for (size_t i = 0; i < imguiWindows.size(); i++)
			imguiWindows[i]->Draw();

		rlImGuiEnd();//imgui end

		if (showFPS)
			DrawFPS(10, 20);

		/*Vector2 points[4];
		points[0] = Vector2{ 100, 100 };
		points[1] = Vector2{ 100, 100 };
		points[2] = Vector2{ 100, 100 };
		points[3] = Vector2{ 100, 100 };

		DrawSplineLinear(&points, 4, 2, PURPLE);*/

		//std::cout << "" << GetMousePosition().x <<":" << GetMousePosition().y << endl;

		//end draw
		EndDrawing();

		//SwapScreenBuffer();
		//PollInputEvents();
		//WaitTime(double seconds);
	}

	threadPool->StopThreads();

	TextureManager::End();
	rlImGuiShutdown();		// cleans up ImGui

	CloseAudioDevice();
	CloseWindow();
	return 0;
}