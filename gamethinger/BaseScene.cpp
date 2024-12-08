#include "BaseScene.h"

void BaseScene::ClearScene()
{
    //delete world;
    b2DestroyWorld(worldId);
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2{0.0f, -10.0f};
    worldId = b2CreateWorld(&worldDef);
    world = &worldDef;

    registry->clear();
    systemManager->ResetSystems();
}

void BaseScene::Update(float deltaTime)
{
    // b2World_Step(worldId, deltaTime, 4);
    if (world != nullptr)
        b2World_Step(worldId, deltaTime, 4);
    systemManager->Update(deltaTime);
}

entt::entity BaseScene::CreateEntity()
{
    auto e = registry->create();
    return e;
}

void BaseScene::Save(std::string filePath)
{
    // serializer.Save(filePath, this);
}

void BaseScene::Load(std::string filePath)
{
    ClearScene();
    // serializer.Load(filePath, this);
}