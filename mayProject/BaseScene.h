#pragma once
#include <memory>
#include "VECTOR2.h"

// プロトタイプ宣言
class BaseScene;
class GameCtl;

// 省略用
using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	virtual ~BaseScene();

	// unique_Base own : 自分のSceneポインタ , const GameCtl &ctl : GameCtlの読み取り専用
	virtual unique_Base Update(unique_Base own, const GameCtl& ctl) = 0;	// 純粋仮想関数
private:
	virtual bool Init(void) = 0;		// 純粋仮想関数
	virtual void Draw(void) = 0;		// 純粋仮想関数
};