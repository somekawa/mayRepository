#pragma once
#include "VECTOR2.h"
#include "ITEMCLASS.h"

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

class Menu
{
public:
	Menu();
	~Menu();

	void Update(GameScene* game,Player* player, Monster* monster, Cards* cards);	// アップデート関数
	void Draw(Player* player, Item* item, Monster* monster);		// 描画
	void MenuButton_NonEnemy(void);		// メニューボタン処理(非戦闘時)
	void MenuButton_Enemy(void);		// メニューボタン処理(戦闘時)

	void Setitem(ITEM item, int png);	// 取得したアイテムを格納する
	bool GetMenuFlg(void);				// メニュー画面が表示状態か取得する
	MENU GetMenu(void);					// 現在のメニュー項目を取得する
	int GetCanHaveItem(void);			// 後何個アイテムが持てるか調べる

	int GetEquipDamage(void);			// 装備武器によるダメージの増加量を取得する
	int GetEquipGuard(void);			// 装備防具によるダメージの軽減量を取得する

	int GetPowUp(void);					// パワーアップアイテムの効果を取得する
	void SetPowUp(int num);				// 攻撃すると-1ずつ減少していくときに使う

	bool GetNonDamageFlg(void);			// 無敵効果のアイテムを使用状態を取得する
	void SetNonDamageFlg(bool flag);	// 敵からの攻撃を受けた後に無敵効果の状態を消すときに使う

	bool GetMenuBackPngFlg(void);		// アイテム背景が描画されているか取得する

	bool GetEscapeFlg(void);			
	void SetEscapeFlg(bool flag);	

	bool GetMeganeFlg(void);
	void SetMeganeFlg(bool flag);
private:
	void Init(void);					// 初期化
	void pngInit(void);					// 画像関係初期化

	/*マウス関係*/
	VECTOR2 _cursorPos;					// マウスカーソルの座標保存用変数
	int _mouse;							// マウスの入力状態取得

	MENU _menu;							// メニュー項目の保存用変数
	//ITEM chooseItem;					
	ITEM _itemAction;					// アイテム使用時の効果を分けるときに使う
	ITEM _itemSetumei;					// メニューで説明を出すときに使う
	VECTOR2 _equipSwordPos;				// 装備武器に"E"を付けるときに使う
	VECTOR2 _equipShieldPos;			// 装備防具に"E"を付けるときに使う
	VECTOR2 _choicePos;					// 選択中のアイテムに色枠を表示するために使う(わかりやすくするため)

	/*メニュー関係*/
	int _chooseNum;						// 選択中のアイテムの場所番号				
	bool _menuBackPngFlg;				// メニュー画面の背景画像を表示するかのフラグ
	bool _menuSelPngFlg;				// メニュー項目を表示するかのフラグ
	// メニュー項目
	std::pair<VECTOR2, MENU>menu_pair[static_cast<int>(MENU::MAX)];
	VECTOR2 buttonSize[static_cast<int>(MENU::MAX)];	// メニュー項目ボタンサイズ

	/*アイテム関係*/
	int _powUpNum;						// 一時的に攻撃力が上昇するアイテムの効果用
	int _equipDamage;					// 武器による攻撃力の上昇
	int _equipGuard;					// 盾による防御力の上昇
	bool _useOrThrowAway;				// アイテム選択時にtrue
	bool _nonNeedFlg;					// アイテムを使っても効果がないときにtrue
	bool _nonDamageFlg;					// 無敵効果のアイテム用フラグ
	bool _escapeFlg = false;			// 逃走のアイテム用フラグ
	bool _meganeFlg = false;			// 宝箱の中身を鑑定するアイテム用フラグ

	/*画像関係*/
	// メニュー項目画像
	int _menuSelPNG[3];
	// タイトルへ戻るの文字画像
	int _menuTitleBackPNG;
	// セーブの文字画像
	int _menuSavePNG;
	// メニューボタン画像
	int _menuPNG;
	// メニュー画面背景画像
	int _menuBackPNG;
	//「使用」の文字画像
	int _usePNG;
	//「捨てる」の文字画像
	int _suteruPNG;
	//「戻る」の文字画像
	int _backPNG;
	// 説明文のバック画像
	int _setumeiPNG;
	// アイテムボックス画像
	int _itemBoxPNG;
	// 選択アイテムを見やすくする画像
	int _itemChoicePNG;

	// クリック音
	int _seClick;
};