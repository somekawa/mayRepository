#pragma once
#include <memory>
#include "../Common/VECTOR2.h"

class BaseScene;
class GameCtl;

using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	// unique_Base own : ������Scene�|�C���^ , const GameCtl &ctl : GameCtl�̓ǂݎ���p
	virtual unique_Base Update(unique_Base own, const GameCtl& ctl) = 0;
private:
	virtual bool Init(void) = 0;		
	virtual void Draw(void) = 0;		
};