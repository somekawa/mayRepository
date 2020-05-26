#pragma once
#include <vector>
#include <tuple>
#include "BaseScene.h"
#include "VECTOR2.h"
#include "Cards.h"
#include "Monster.h"
#include "Player.h"
#include "Menu.h"
#include "Item.h"
#include "Event.h"

//#define EVENT_NUM 25

// �C�x���g�ʂ̍ő�l
#define EVENT_MAXNUM 99

// ���摜
enum class MAP {
	STRAIGHT,		// ���i
	RIGHT,			// �E�܂̂�
	LEFT,			// ���܂̂� 
	STOP,			// �s���~�܂�(�C�x���g���Ȃ�)
	TJI,			// T���H
	TONOJI_SR,		// �g���^(���i�ƉE�ւ̓�)
	TONOJI_SL,		// �g���^(�����ƍ��ւ̓�)
	YADO,
	SHOUNIN,
	BUTTON,
	CHEST,
	DRINK,
	TRAP,			// �����g���b�v
	EVE_MONS,		// �C�x���g�����X�^�[(�ʘH�摜�̗\��)
	GOAL,			// �S�[��
	MAX
};

// �v���C���[�̐i�s����
enum class PL_DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	// unique_Base own : ������Scene�|�C���^ ,  const GameCtl &ctl : GameCtl�̓ǂݎ���p
	unique_Base Update(unique_Base own, const GameCtl& ctl);	// �I�u�W�F�N�g�ƃV�[���̊Ǘ�

	// �}�E�X�֌W
	VECTOR2 cursorPos;					// �}�E�X�J�[�\���̍��W�ۑ��p�ϐ�
	int mouse;							// �}�E�X�̏��
	int mouseOld;

	EVENT_STATE eventState;				// �C�x���g�̏�ԊǗ��p
	bool moveFlg;						// �������[�V�����Ǘ��p�t���O
	bool shakeFlg;						// �G�̑ҋ@�^�[����0�ɂȂ�����true�ɂ��ĉ�ʂ�h�炵�n�߂�
	bool blinkFlg;						// �G�̓_�ŗp�t���O
	int walkCnt;						// ��������
	//int chestFate = -1;
	bool _backFlg = false;
	static int testx;			// �v���C���[�̕��s���p�ϐ�x
	static int testy;			// �v���C���[�̕��s���p�ϐ�y
	bool _openDoor;						// �����J����
	static bool monsterFlg;
private:
	bool Init(void);					// ������
	void pngInit(void);					// �摜�p������
	void Draw(void);					// �`��
	void MouseClick_Go(const GameCtl& ctl);		// �}�E�X�����N���b�N���āu�i�ށv���������Ƃ��̏���
	void EventUpdate(void);				// �C�x���g�̃A�b�v�f�[�g�֐�
	void pl_TurnEndAfter(void);			// �v���C���[�̃^�[�����I�������̏���
	void pl_Attack(void);				// �v���C���[�̍U��
	void pl_Heal(void);					// �v���C���[�̉�
	void shakeDraw(void);				// ��ʂ������h�炷
	void doorWalk(void);				// �����ƃh�A���̂Ȃ�
	void changeBGM(void);				// �ʏ�BGM�Ɛ퓬BGM�̐ؑ�
	void plDead(void);					// �v���C���[���S��
	void enemyItemDrop(void);			// �G����̃A�C�e���h���b�v����
	void cardEffect(void);				// �J�[�h�̌���

	Monster* _monster[1];
	Cards* _cards;
	Player* _player;
	Menu* _menu;
	Item* _item;
	Event* _event;

	// ���֌W
	//bool doorFlg;						// �g�債�Ă��Ȃ��� �\���Ǘ��p�t���O
	//bool _openDoor;						// �����J����
	float _doorExpand;					// ���̊g��p�ϐ�
	float _degree;						// �������[�V�����p
	int _doorOpenTiming;				// �����Ă�������J���܂ł̎��Ԓ����p
	float _lastDoorExpand;				// �N���A�O�̌�����̊g�厞�Ɏg�p
	int _lastmoveTime;					// ��莞�Ԉȉ��Ȃ�g��𑱂��鎞�Ɏg��
	bool _changeToClear;				// ��莞�Ԉȏ�Ńt���O��true�ɂȂ�A�����I�ɃN���A��ʂɈڍs����

	// �C�x���g�֌W
	//int walkCnt;						// ��������
	//int _goalCnt;						// �S�[���܂łɕK�v�ȕ���
	int _eventNum[EVENT_MAXNUM];			// �C�x���g�������������
	int _bossEventNum;					// �{�X�̏o�Ă���^�C�~���O
	// �C�x���g���e�̊Ǘ�
	int _eventChoiceNum;				// _eventChoiceNumOld�̊i�[�ꏊ��ς���̂Ɏg��
	int _eventChoiceNumOld[3];			// �ߋ��̃C�x���g����3�񕪕ۑ�����̂Ɏg��

	// ��ʂ�h�炵�֌W
	bool _shakeChangeFlg;				// ���l�ȏ㍶�E�ǂ��炩�ɗh�ꂽ�甽�Ε����֗h�炷���߂̃t���O
	float _shakeTime;					// �h�炷����
	VECTOR2 _shackPos;					// �h�ꕝ

	// �_�Ŋ֌W
	int _blinkCnt;						// �U����H������Ƃ��ɋN����_��
	//bool blinkFlg;					// �G�̓_�ŗp�t���O

	// ���̑�
	int _plDeadChangeWinColor;			// �v���C���[���S���A��ʂ̐F�𔽓]������Ƃ��Ɏg��
	int _poisonCnt;						// �퓬���̓Ō��ʂ̎��F��ʂ̎���(�����l��255���傫���K�v������)
	bool _onceFlg;						// �G����̃h���b�v�A�C�e���\���Ɏg�p�ƓG�̑ҋ@�^�[����0�̎��Ɏg�p
	bool _anounceFlg;					// �G����̃h���b�v�A�C�e���Ŏ������������ς��̎��Ɉē����o��

	//// �X�L���֌W
	//int _skillCharge = 1;				// �X�L���`���[�W
	//int _skillIconPNG;					// �X�L���A�C�R��(��ō����ւ���)
	//int _skillAnnouncePNG;				// �X�L���g�p�\�ē�
	//bool _skillFlg = false;				// �X�L�����g�p�\�ɂȂ�����true

	/*�摜�֌W*/
	// �G�^�[���܂ł̃J�E���g�p�摜
	int _turnPNG[6];						
	// HP�o�[�摜
	int _hpBarPl;
	int _hpBarEn;
	int _hpBarBack;
	// �i�s�x�Q�[�W�摜
	int _gaugeBackPNG;
	int _gaugePNG;
	// ����(���J����)�̉摜
	int _room[3];		
	//�u�i�ށv�̕����摜
	//int _walkPNG;	
	// �_�ŗp���摜
	int _whitePNG;
	// �G���摜
	int _enemyInfoPNG;
	//�u���v�̕����摜
	int _getItemPNG;
	// ���S���̂��Ȃ����̉摜
	int _retryPNG;
	// ���S���̂�����߂�̉摜
	int _deadPNG;
	// �����g���b�v�Ŏ��S��(�G�����O)�̉摜
	int _messageDeathPNG;
	// �����g���b�v�Ŏ��S��(�G������)�̉摜
	int _messageDeathPNG2;
	// �{�X��O�x���摜
	int bossEmergencyPNG;
	// ��
	int kiriPNG[2];
	float kiri1 = 0.0f;
	float kiri2 = -900.0f;


	// �_���W����
	// ���i
	//int straightPNG;
	// �E�܂̂�
	//int rightPNG;
	// ���܂̂�
	//int leftPNG;
	// ��(���͒��i�ƉE�܂̂�)
	int roadPNG[static_cast<int>(MAP::MAX)];
	int chipPNG[static_cast<int>(MAP::MAX)];

	/*���֌W*/
	// SE�֘A
	int _soundSE[8];
	int _seCnt;		// ����������h�A���܂ł̖�Ԋu
	bool _soundWalk;	// ����������h�A���ɂȂ��Ƃ��ɕK�v�ȃt���O
	// BGM�֘A
	int _gameBGM;
	int _battleBGM;

	int testCnt = 0;

	//int num[3][3];		// �}�b�v�I�Ȃ���
	std::pair<bool, int> numkai[10][10];	// �}�b�v(�ʂ������Ƃ̂���Ƃ����true)
	int plNowPoint;		// �v���C���[�̌��ݒn
	int plOldPoint;
	//static int testx;			// �v���C���[�̕��s���p�ϐ�x
	//static int testy;			// �v���C���[�̕��s���p�ϐ�y
	//std::map<bool, std::pair<int, int>> mapTest;
	//std::map<int, int> mapTest;
	//VECTOR2 movePos[4];		// �}�b�v�ړ��O��
	std::vector<std::tuple<VECTOR2, int,float>> vec;	// �}�b�v�ړ��O��(�ʒu�ƃ~�j�}�b�v�p�Ɍ��݂̔ԍ��ɉ摜��])
	void TestDirect(void);
	void TestKey(void);
	PL_DIRECTION _plDirect = PL_DIRECTION::UP;
	PL_DIRECTION _plDirectOld = PL_DIRECTION::UP;
	bool leftFlg = false;
	bool rightFlg = false;
	int directPNG;
	float directRota = 0.0f;
	int monsTimeCnt = 3;

	VECTOR2 offset = {0,0};
	VECTOR2 plNowMark = { 0,0 };

	VECTOR2 bossPos = { 8,5 };
	//VECTOR2 bossPos = { 0,2 };
};