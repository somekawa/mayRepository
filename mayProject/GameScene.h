#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	// unique_Base own : 自分のSceneポインタ ,  const GameCtl &ctl : GameCtlの読み取り専用
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// オブジェクトとシーンの管理
private:
	bool Init(void);					// 初期化
};