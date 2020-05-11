#include "DxLib.h"
#include "SceneMng.h"
#include "GameCtl.h"
#include "GameScene.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own, const GameCtl& ctl)
{
	if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F1]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F1]))
	{
		return std::make_unique<GameScene>();
	}

	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

bool TitleScene::Init(void)
{
	return true;
}

void TitleScene::Draw(void)
{
}
