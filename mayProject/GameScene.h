#pragma once
#include "BaseScene.h"
#include "VECTOR2.h"

#define BOX 50		// 四角の箱の数
#define BOX_SIZE 50	// 四角のサイズ

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
	void Draw(void);					// 描画

	int _mouse;
	VECTOR2 _cursorPos;
	
	VECTOR2 _testPos[BOX];				// 左上の四角
};