#pragma once
#include <memory>
#include "VECTOR2.h"

// �v���g�^�C�v�錾
class BaseScene;
class GameCtl;

// �ȗ��p
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	// unique_Base own : ������Scene�|�C���^ , const GameCtl &ctl : GameCtl�̓ǂݎ���p
	virtual unique_Base Update(unique_Base own, const GameCtl& ctl) = 0;	// �������z�֐�
private:
	virtual bool Init(void) = 0;		// �������z�֐�
	virtual void Draw(void) = 0;		// �������z�֐�
};