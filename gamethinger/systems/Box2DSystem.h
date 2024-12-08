#pragma once
#include "../BaseSystem.h"
class Box2DSystem : public BaseSystem
{
public:
	Box2DSystem()
	{
		systemName = "Box2DSystem";
	}
	void Init();
	void Reset();

	void Update(float deltaTime);
	void Render();

	//used to run init on Rigidbody components
	void OnRigidbody2DAdd();
	void OnRigidbody2DRemove();
};

