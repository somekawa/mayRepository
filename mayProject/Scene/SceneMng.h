#pragma once
#include <mutex>	
#include <memory>	
#include "BaseScene.h"

class GameCtl;

#define lpSceneMng SceneMng::GetInstance()

// �V���O���g��
class SceneMng
{
public:
	static SceneMng& GetInstance(void)	// ����ǂݎ�邾���̊֐� 
	{
		static SceneMng s_Instance;		// �C���X�^���X����O�ɃA�h���X���m�肳����K�v������̂�static�𗘗p
		return (s_Instance);			// s_Instance�̏���Ԃ�
	}

	void Run(void);						// �Ăяo���p
private:
	SceneMng();
	~SceneMng();
	bool SysInit(void);					// �V�X�e���̏�����

	std::unique_ptr<GameCtl> gameCtl;	// GameCtl�̃��j�[�N�|�C���^
	unique_Base activeScene;			// active��scene��ێ�����
	const int screen_sizeX;				// ��ʉ𑜓x ��
	const int screen_sizeY;				// ��ʉ𑜓x �c
};