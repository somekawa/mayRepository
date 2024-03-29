#pragma once
#include <mutex>	
#include <memory>	
#include "BaseScene.h"

class GameCtl;

#define lpSceneMng SceneMng::GetInstance()

// シングルトン
class SceneMng
{
public:
	static SceneMng& GetInstance(void)	// 情報を読み取るだけの関数 
	{
		static SceneMng s_Instance;		// インスタンスする前にアドレスを確定させる必要があるのでstaticを利用
		return (s_Instance);			// s_Instanceの情報を返す
	}

	void Run(void);						// 呼び出し用
private:
	SceneMng();
	~SceneMng();
	bool SysInit(void);					// システムの初期化

	std::unique_ptr<GameCtl> gameCtl;	// GameCtlのユニークポインタ
	unique_Base activeScene;			// activeのsceneを保持する
	const int screen_sizeX;				// 画面解像度 横
	const int screen_sizeY;				// 画面解像度 縦
};