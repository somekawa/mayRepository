#pragma once
#include <map>
#include <string>
#include <memory>
#include "Common/VECTOR2.h"
#include "Item/ITEMCLASS.h"

class MouseCtl;

// メニュー画面の項目
enum class MENU {
	NON,
	STATUS,
	ITEM,
	TO_GAME,
	TO_TITLE,
	SAVE,
	MAX
};

class GameScene;
class Player;
class Item;
class Monster;
class Cards;
class MouseCtl;

class Menu
{
public:
	Menu();
	~Menu();

	void Update(GameScene* game, const std::shared_ptr<Player>& player, const std::shared_ptr<Monster>& monster, const std::shared_ptr<Cards>& cards,const std::shared_ptr<MouseCtl>& mouse);	
	void Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const std::shared_ptr<Monster>& monster);		
	void MenuButton_NonEnemy(const std::shared_ptr<MouseCtl>& mouse);						// メニューボタン処理(非戦闘時)
	void MenuButton_Enemy(const std::shared_ptr<MouseCtl>& mouse);							// メニューボタン処理(戦闘時)

	void SetItem(const ITEM& item, const int& png);	// 取得したアイテムを格納する
	bool GetMenuFlg(void)const;						// メニュー画面が表示状態か取得する
	MENU GetMenu(void)const;						// 現在のメニュー項目を取得する
	int GetCanHaveItem(void)const;					// 後何個アイテムが持てるか調べる

	int GetEquipDamage(void)const;					// 装備武器によるダメージの増加量を取得する
	int GetEquipGuard(void)const;					// 装備防具によるダメージの軽減量を取得する

	int GetPowUp(void)const;						// パワーアップアイテムの効果を取得する
	void SetPowUp(const int& num);					// 攻撃すると-1ずつ減少していくときに使う

	bool GetNonDamageFlg(void)const;				// 無敵効果のアイテムを使用状態を取得する
	void SetNonDamageFlg(const bool& flag);			// 敵からの攻撃を受けた後に無敵効果の状態を消すときに使う

	bool GetMenuBackPngFlg(void)const;				// アイテム背景が描画されているか取得する

	bool GetEscapeFlg(void)const;					// 戦闘から逃げるアイテムの情報を取得する		
	void SetEscapeFlg(const bool& flag);			// 戦闘から逃げるアイテムの情報を設定する

	bool GetAppraisalFlg(void)const;				// 宝箱の中身を鑑定するアイテムの情報を取得する
	void SetAppraisalFlg(const bool& flag);			// 宝箱の中身を鑑定するアイテムの情報を設定する

	void Save(const std::shared_ptr<Player>& player);		// セーブ
	static void Load(void);									// ロード
private:
	bool Init(void);								// 初期化
	void PngInit(void);								// 画像関係初期化

	MENU _menu;										// メニュー項目の保存用変数
	ITEM _itemAction;								// アイテム使用時の効果を分けるときに使う
	ITEM _itemExplanation;							// メニューで説明を出すときに使う
	VECTOR2 _equipSwordPos;							// 装備武器に"E"を付けるときに使う
	VECTOR2 _equipShieldPos;						// 装備防具に"E"を付けるときに使う
	VECTOR2 _choicePos;								// 選択中のアイテムに色枠を表示するために使う(わかりやすくするため)

	/*メニュー関係*/
	int _chooseNum;									// 選択中のアイテムの場所番号				
	bool _menuBackPngFlg;							// メニュー画面の背景画像を表示するかのフラグ
	bool _menuSelPngFlg;							// メニュー項目を表示するかのフラグ
	std::pair<VECTOR2, MENU>menu_pair[static_cast<int>(MENU::MAX)];		// メニュー項目
	VECTOR2 buttonSize[static_cast<int>(MENU::MAX)];// メニュー項目ボタンサイズ

	/*アイテム関係*/
	int _powUpNum;									// 一時的に攻撃力が上昇するアイテムの効果用
	int _equipDamage;								// 武器による攻撃力の上昇
	int _equipGuard;								// 盾による防御力の上昇
	bool _useOrThrowAway;							// アイテム選択時にtrue
	bool _nonNeedFlg;								// アイテムを使っても効果がないときにtrue
	bool _nonDamageFlg;								// 無敵効果のアイテム用フラグ
	bool _escapeFlg;								// 逃走のアイテム用フラグ
	bool _AppraisalFlg;								// 宝箱の中身を鑑定するアイテム用フラグ

	/*画像関係*/
	int _menuSelPNG[3];
	std::map<std::string, int> _menuImages;

	int _soundSE[5];
	static bool _loadFlg;							// true時に所持アイテムをボックスに格納する処理を行う
};