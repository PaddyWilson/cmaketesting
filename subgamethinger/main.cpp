#include <iostream>
#include <vector>
#include <string>

//#include <entt/entt.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
#include <yaml-cpp/parser.h>

#include <raylib.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>

#include <Collisions.h>
#include <physac.h>
#include <box2d/box2d.h>
#include <cute_c2.h>


#include <nfd.h>
//#include <stdio.h>
//#include <stdlib.h>

// #include "more.hpp"
// #include "morex2.hpp"
#include <curl/curl.h>

using namespace std;
//using namespace rapidjson;

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS sghjldkfsfgdhjCode", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    // src2::print3("hel4sdfgsdfg5lo hello 3 \n");
    // src2::print3x2("morex2fds");

    //1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(json);
    
    // 2. Modify it by DOM.
    rapidjson::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    
    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
    
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    
    NFD_Init();
    
    nfdu8char_t *outPath;
    nfdu8filteritem_t filters[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
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


    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
       BeginDrawing();
           ClearBackground(RAYWHITE);
           DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
       EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
