#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

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
};