#pragma once
#include <DxLib.h>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "BaseScene.h"
#include "../Common/VECTOR2.h"
#include "../Card/Cards.h"
#include "../Enemy/Monster.h"
#include "../Player.h"
#include "../Menu.h"
#include "../Item/Item.h"
#include "../Event/Event.h"

class MouseCtl;

// ���摜
enum class MAP {
	STRAIGHT,		// ���i
	RIGHT,			// �E�܂̂�
	LEFT,			// ���܂̂� 
	STOP,			// �s���~�܂�(�C�x���g���Ȃ�)
	TJI,			// T���H
	TONOJI_SR,		// �g���^(���i�ƉE�ւ̓�)
	TONOJI_SL,		// �g���^(�����ƍ��ւ̓�)
	INN,			// �h
	MERCHANT,		// ���l
	BUTTON,			// �{�^��
	CHEST,			// ��
	DRINK,			// ���ݕ�
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

// �}�b�v�ړ��O��
struct MapMake {
	VECTOR2 pos;			// ���ݍ��W
	int num;				// ���݈ʒu�ԍ�
	float rota;				// �摜��]�p�x
	PL_DIRECTION dir;		// �v���C���[�̌���
};

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own, const GameCtl& ctl) override;

	std::shared_ptr<MouseCtl> mouse;
	EVENT_STATE eventState;				// �C�x���g�̏�ԊǗ��p
	bool moveFlg;						// �������[�V�����Ǘ��p�t���O
	bool shakeFlg;						// �G�̑ҋ@�^�[����0�ɂȂ�����true�ɂ��ĉ�ʂ�h�炵�n�߂�
	bool blinkFlg;						// �G�̓_�ŗp�t���O
	bool backFlg;						// �������֖߂鏈��
	bool openDoor;						// �����J����
	static int plPosX;					// �v���C���[�̕��s���p�ϐ�x
	static int plPosY;					// �v���C���[�̕��s���p�ϐ�y
	static bool monsterFlg;				// �G�Ƃ̃G���J�E���g�p�t���O
	static bool bossClearFlg;			// �{�X���j��true�ɂ���
private:
	bool Init(void) override;			// ������
	void PngInit(void);					// �摜�p������
	void Draw(void) override;			// �`��
	void AllMapDraw(void);				// �S�̃}�b�v�̕`��
	void SmallMapDraw(void);			// �����}�b�v�̕`��
	void ShakeDraw(void);				// ��ʂ������h�炷
	void MouseClick_Go(const GameCtl& ctl);		// ���N���b�N���āu�i�ށv���������Ƃ��̏���
	void EventUpdate(void);				// �C�x���g�̃A�b�v�f�[�g�֐�
	void Pl_TurnEndAfter(void);			// �v���C���[�̃^�[�����I�������̏���
	void Pl_Attack(void);				// �v���C���[�̍U��
	void Pl_Heal(void);					// �v���C���[�̉�
	void Pl_Dead(void);					// �v���C���[���S��
	void Walk(void);					// �����ƃL�[���͂̎�t����������
	void ChangeBGM(void);				// �ʏ�BGM�Ɛ퓬BGM�̐ؑ�
	void EnemyItemDrop(void);			// �G����̃A�C�e���h���b�v����
	void CardEffect(void);				// �J�[�h�̌���
	void DungeonFog(void);				// ���̌���
	void ButtleCaution(void);			// �����퓬�̈ē����o��
	void GameReset(void);				// �Q�[���I�[�o�[���ɂ��Ȃ�����I�������Ƃ��ɍĐݒ肷��ϐ����W�߂��֐�

	std::shared_ptr<Monster> _monster[1];
	std::shared_ptr<Cards> _cards;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Menu> _menu;
	std::shared_ptr<Item> _item;
	std::unique_ptr<Event> _event;

	// ���֌W
	float _doorExpand;					// ���̊g��p�ϐ�
	float _degree;						// �������[�V�����p
	int _doorOpenTiming;				// �����Ă�������J���܂ł̎��Ԓ����p
	float _lastDoorExpand;				// �N���A�O�̌�����̊g�厞�Ɏg�p
	int _lastmoveTime;					// ��莞�Ԉȉ��Ȃ�g��𑱂��鎞�Ɏg��
	bool _changeToClear;				// ��莞�Ԉȏ�Ńt���O��true�ɂȂ�A�����I�ɃN���A��ʂɈڍs����

	// ��ʗh�炵�֌W
	bool _shakeChangeFlg;				// ���l�ȏ㍶�E�ǂ��炩�ɗh�ꂽ�甽�Ε����֗h�炷���߂̃t���O
	float _shakeTime;					// �h�炷����
	VECTOR2 _shackPos;					// �h�ꕝ

	// �_���W�����֌W
	void Direct(void);					// �ړ��ƌ�����ς���֐�
	void Key(void);						// �L�[�����֐�
	std::pair<bool, int> _dungeonMap[10][10];				// �}�b�v(�ʂ������Ƃ̂���Ƃ����true)
	std::vector<MapMake> _mapVec;							// �}�b�v�ړ��O�Ղ̕ۑ��p�ϐ�
	int _plNowPoint;					// �v���C���[�̌��ݒn
	int _plOldPoint;					// �v���C���[�̑O�񂢂��ʒu
	bool _leftFlg;						// ���܂�����true
	bool _rightFlg;						// �E�܂�����true
	double _directRota;					// �����A�C�R���̌��������p
	PL_DIRECTION _plDirect;				// �v���C���[�̌����Ă������
	PL_DIRECTION _plDirectOld;			// �v���C���[�̑O������Ă�������
	VECTOR2 _mapChipDrawOffset;			// �}�b�v�`�b�v�̕`��ʒu�I�t�Z�b�g
	VECTOR2 _plNowMark;					// �����A�C�R���`��ʒu�����p
	bool _allMapFlg;					// �S�̃}�b�v�\���̂Ƃ���true

	// ���̑�
	int _blinkCnt;						// �U����H������Ƃ��ɋN����_��
	int _monsTimeCnt;					// �G�o����������p
	int _walkDirect;					// ���܂ƉE�܂Ŏ��_���񂹂Ă����Ƃ��Ɏg�p
	int _plDeadChangeWinColor;			// �v���C���[���S���A��ʂ̐F�𔽓]������Ƃ��Ɏg��
	int _poisonCnt;						// �퓬���̓Ō��ʂ̎��F��ʂ̎���(�����l��255���傫���K�v������)
	bool _onceFlg;						// �G����̃h���b�v�A�C�e���\���Ɏg�p�ƓG�̑ҋ@�^�[����0�̎��Ɏg�p
	bool _anounceFlg;					// �G����̃h���b�v�A�C�e���Ŏ������������ς��̎��Ɉē����o��
	float _fog[2];						// ��
	VECTOR2 _bossPos;					// �{�X�o��ʒu
	bool _keyFlg;						// �L�[�������ꂽ��true�ɂȂ葫��SE���Ȃ�I�������false�ɂȂ�
	int _levelUpAnounceTime;			// ���x���A�b�v���̈ē��\������
	int _forcedButtlePngMoveCnt;		// �u�����퓬�v�̕����摜�̈ړ������p
	bool _turnEndOnceFlg;				// �v���C���[�^�[���I������true�ɂ���
	VECTOR2 _bossEmergencyPos;			// �{�X�O�̌x���摜���o�����W
	int _mapChipSize;					// �}�b�v�`�b�v��1������̃T�C�Y
	const int screen_sizeX;				// ��ʉ𑜓x ��
	const int screen_sizeY;				// ��ʉ𑜓x �c
	bool _guideFlg;						// ��莞�ԃL�[���삪�s���Ȃ������ꍇ��true�ɂȂ�A����K�C�h���\�������悤�ɂ���
	int _guideVisibleTime;				// �L�[���삪�s���Ă��Ȃ����Ԃ��v��
	int _guideMove;						// �K�C�h�`�掞�ɔw�i�̘g�����R�ό`�œ������Ƃ��Ɏg��
	float _guideExrMove;				// �K�C�h�`�掞�̊g��/�k��
	bool _buttleGuideFlg;				// �퓬�K�C�h�̕\��/��\���ؑ�

	std::map<int, EVENT_STATE> _eventStateMap;	// �C�x���g�ԍ���enum class����v�����ăC�x���g�������s���₷������

	/*�摜�֌W*/
	std::map<std::string, int> _drawHandle;		
	int _turnPNG[6];							// �G�^�[���܂ł̃J�E���g�p�摜
	int _room[3];								// ����(���J����)�̉摜
	int _roadPNG[static_cast<int>(MAP::MAX)];	// �_���W�����摜
	int _chipPNG[static_cast<int>(MAP::MAX)];	// �}�b�v�`�b�v�摜
	int _dungeonFogPNG[2];						// ���摜

	/*���֌W*/
	// SE�֘A
	int _soundSE[8];					// SE
	int _seCnt;							// ����������h�A���܂ł̖�Ԋu
	bool _soundWalk;					// ����������h�A���ɂȂ��Ƃ��ɕK�v�ȃt���O
	float _walkCnt;						// �������ƃL�[�̓��͎�t�^�C�~���O�̒���
	// BGM�֘A
	int _gameBGM;						// �ʏ�BGM
	int _battleBGM;						// �퓬BGM
};