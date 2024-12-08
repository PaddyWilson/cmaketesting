#pragma once
#include <raylib.h>

#include "../BaseScene.h"

#include "ScryfallJson.h"
#include "MTGComps.h"

class MTGGameScene : public BaseScene
{
private:
	MTGDeck deck;

public:
	MTGGameScene() { SceneName = "MTG Game"; };
	void Init();
	void End();

	void Update(float deltaTime);
	void Render();
	void RenderUI();

	void PlayCard();
	void DrawCard();
	void ReturnCard();
	void TapCard();
	void UntapAll();
	void OnTop();
	void OnBottom();
	void Shuffle();
	void FlipOver();
	void NextToken();
	void ScearchDeck();
	void ToGrave();
	void ToExile();
	void ViewGrave();
	void ViewExile();
};

