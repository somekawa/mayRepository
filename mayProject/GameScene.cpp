#include "DxLib.h"
#include <string>
#include "SceneMng.h"
#include "GameCtl.h"
#include "TitleScene.h"
#include "GameScene.h"

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

	if (_mouse & MOUSE_INPUT_LEFT) {	 //マウスの左ボタンが押されていたら

		// 左上の四角と当たり判定テスト
		//if (_cursorPos.x >= _testPos[0].x && _cursorPos.x <= _testPos[0].x + BOX_SIZE && _cursorPos.y >= _testPos[0].y && _cursorPos.y <= _testPos[0].y + BOX_SIZE)
		//{
		//	int a = 0;
		//	int b = a;
		//}

		// クリックしたボックスがわかるようにする
		for (int i = 0; i <= BOX; i++)
		{
			if (_cursorPos.x >= _testPos[i].x && _cursorPos.x <= _testPos[i].x + BOX_SIZE && _cursorPos.y >= _testPos[i].y && _cursorPos.y <= _testPos[i].y + BOX_SIZE)
			{
				int a = i;
				int b = a;
			}
		}
	}
	_mouse = GetMouseInput();						 //マウスの入力状態取得
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //マウスの座標取得

	Draw();

	return std::move(own);
}

bool GameScene::Init(void)
{
	// ボックスの位置を配列で追加する
	for (int i = 0; i < BOX; i++)
	{
		//int a = 50 * (i % 10);
		//int b = ((i / 10) * 50);
		_testPos[i] = { 50 * (i % 10),300 + ((i / 10) * 50) };
	}
	return true;
}

void GameScene::Draw(void)
{
	ClsDrawScreen();
	// 縦線
	for (int i = 0; i <= 500;)
	{
		DrawLine(i, 300, i, 550, GetColor(255, 0, 0), 2);
		i += 50;
	}

	// 横線
	for (int i = 300; i < 600;)
	{
		DrawLine(0, i, 500, i, GetColor(255, 0, 0), 2);
		i += 50;
	}
	ScreenFlip();
}