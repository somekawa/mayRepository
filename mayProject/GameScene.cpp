#include "DxLib.h"
#include <string>
#include "SceneMng.h"
#include "GameCtl.h"
#include "TitleScene.h"
#include "GameScene.h"

#define PI 3.141592653589793

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

unique_Base GameScene::Update(unique_Base own, const GameCtl& ctl)
{
	if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F2]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F2]))
	{
		return std::make_unique<TitleScene>();
	}

	return std::move(own);
}

bool GameScene::Init(void)
{
	return true;
}