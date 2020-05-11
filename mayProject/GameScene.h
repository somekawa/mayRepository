#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

#define BOX 50		// �l�p�̔��̐�
#define BOX_SIZE 50	// �l�p�̃T�C�Y

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �I�u�W�F�N�g�ƃV�[���̊Ǘ�
private:
	bool Init(void);					// ������
	void Draw(void);					// �`��

	int _mouse;
	VECTOR2 _cursorPos;
	
	VECTOR2 _testPos[BOX];				// ����̎l�p
};