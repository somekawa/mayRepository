#include "DxLib.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SelectScene.h"

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
	auto Mouse = GetMouseInput();                //�}�E�X�̓��͏�Ԏ擾
	GetMousePoint(&x, &y);					     //�}�E�X�̍��W�擾
	if (Mouse & MOUSE_INPUT_LEFT && !(_oldMouse & MOUSE_INPUT_LEFT)) {	 //�}�E�X�̍��{�^����������Ă�����
		// �����蔻��(NORMAL�I����)
		if (x >= 250 && x <= 250 + 400 && y >= 100 && y <= 100 + 150)
		{
			return std::make_unique<GameScene>();
		}
		// �����蔻��(�^�C�g���֖߂�)
		if (x >= 650 && x <= 650 + 200 && y >= 450 && y <= 450 + 100)
		{
			return std::make_unique<TitleScene>();
		}
	}

	_oldMouse = Mouse;
	Draw();

	// ������Scene�̃��j�[�N�|�C���^��Ԃ� ���L�����Y�ꂸ��!
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

	return true;
}

void SelectScene::Draw(void)
{
	ClsDrawScreen();
	DrawGraph(0, 0, _backPNG, true);
	DrawGraph(650, 450, _titleBackPNG, true);
	DrawGraph(250, 100, _normalPNG, true);
	DrawGraph(250, 300, _hardPNG, true);
	ScreenFlip();
}