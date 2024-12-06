#include <iostream>
#include <vector>
#include <string>

// #include "rapidjson/document.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
using namespace rapidjson;

#include <raylib.h>
#include <raymath.h>
// #include "rlImGui.h"

#include <yaml-cpp/yaml.h>
#include <nfd.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <entt/entt.hpp>

#include <box2d/box2d.h>

#define ENET_IMPLEMENTATION
#include "enet.h"

// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"

// #include <NativeFileDialog/nfd.h>
// #include <stdio.h>
// #include <stdlib.h>

#include "more.hpp"
#include "morex2.hpp"

using namespace std;
// using namespace rapidjson;

int main()
{
    vector<string> msg{"Hello", "C++", "World", "from", "VS sghjldkfsfgdhjCode", "and the C++ extension!"};

    for (const string &word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    if (enet_initialize() != 0)
    {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }

    enet_deinitialize();

    print3("hel4sdfgsdfg5lo");
    print3x2("morex2fds");

    YAML::Emitter out;
    out << YAML::BeginSeq;
    out << "eggs";
    out << "bread";
    out << "milk";
    out << YAML::EndSeq;

    std::cout << "Here's the output YAML:\n"
              << out.c_str(); // prints "Hello, World!"

    InitWindow(800, 450, "raylib [core] example - basic window");
    // rlImGuiSetup(true);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        // rlImGuiBegin();

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first barhhhhhh window!", 190, 200, 20, LIGHTGRAY);

        // rlImGuiEnd();
        EndDrawing();
    }
    // rlImGuiShutdown();
    CloseWindow();

    NFD_Init();
    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[2] = {{"Source code", "c,cpp,cc"}, {"Headers", "h,hpp"}};
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 2;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
    if (result == NFD_OKAY)
    {
        puts("Success!");
        puts(outPath);
        NFD_FreePathU8(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        puts("User pressed cancel.");
    }
    else
    {
        printf("Error: %s\n", NFD_GetError());
    }

    NFD_Quit();

    entt::registry registry;

    // create JSON arrays
    json j_no_init_list = json::array();
    json j_empty_init_list = json::array({});
    json j_nonempty_init_list = json::array({1, 2, 3, 4});
    json j_list_of_pairs = json::array({{"one", 1}, {"two", 2}});

    // serialize the JSON arrays
    std::cout << j_no_init_list << '\n';
    std::cout << j_empty_init_list << '\n';
    std::cout << j_nonempty_init_list << '\n';
    std::cout << j_list_of_pairs << '\n';
    //  1. Parse a JSON string into DOM.
    const char *json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value &s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);

    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){0.0f, 4.0f};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;
    b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;
    for (int i = 0; i < 10; ++i)
    {
        b2World_Step(worldId, timeStep, subStepCount);
        b2Vec2 position = b2Body_GetPosition(bodyId);
        b2Rot rotation = b2Body_GetRotation(bodyId);
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation));
    }

    return 0;
}