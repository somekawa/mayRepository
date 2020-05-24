#include "DxLib.h"
#include "Player.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Menu.h"
#include "SelectScene.h"

// static変数の実体<型>クラス名::変数名 = 初期化;
MODE SelectScene::modeTest = MODE::NON;

SelectScene::SelectScene()
{
	Init();
}

SelectScene::~SelectScene()
{
}

unique_Base SelectScene::Update(unique_Base own, const GameCtl& ctl)
{
	int x = 0;
	int y = 0;
	auto Mouse = GetMouseInput();                //マウスの入力状態取得
	GetMousePoint(&x, &y);					     //マウスの座標取得
	if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //マウスの左ボタンが押されていたら
		// 当たり判定(NORMAL選択時)
		if (x >= 250 && x <= 250 + 400 && y >= 100 && y <= 100 + 150)
		{
			modeTest = MODE::NORMAL;
			//Player::LoadTest();
			Menu::LoadTest();
			return std::make_unique<GameScene>();
		}
		// 当たり判定(HARD選択時)
		if (x >= 250 && x <= 250 + 400 && y >= 300 && y <= 300 + 150)
		{
			modeTest = MODE::HARD;
			return std::make_unique<GameScene>();
		}

		// 当たり判定(タイトルへ戻る)
		if (x >= 650 && x <= 650 + 200 && y >= 450 && y <= 450 + 100)
		{
			return std::make_unique<TitleScene>();
		}
	}

	_oldMouse = Mouse;
	Draw();

	// 明るくしたり暗くする処理
	if (!_lightFlg)
	{
		if (_pngLight <= 255)
		{
			_pngLight++;
			if (_pngLight == 255)
			{
				_lightFlg = true;
			}
		}
	}

	if (_lightFlg)
	{
		_pngLight--;
		if (_pngLight <= 128)
		{
			_lightFlg = false;
		}
	}

	// 自分のSceneのユニークポインタを返す 所有権も忘れずに!
	return std::move(own);
}

bool SelectScene::Init(void)
{
	std::string normal = "image/normal.png";
	_normalPNG = LoadGraph(normal.c_str());

	std::string hard = "image/hard.png";
	_hardPNG = LoadGraph(hard.c_str());

	std::string renga = "image/renga.png";
	_backPNG = LoadGraph(renga.c_str());

	std::string titleBackButton = "image/titleBackButton.png";
	_titleBackPNG = LoadGraph(titleBackButton.c_str());

	_pngLight = 128;
	_lightFlg = false;
	return true;
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	//αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pngLight);
	DrawGraph(0, 0, _backPNG, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawGraph(650, 450, _titleBackPNG, true);
	DrawGraph(250, 100, _normalPNG, true);
	DrawGraph(250, 300, _hardPNG, true);
	ScreenFlip();
}