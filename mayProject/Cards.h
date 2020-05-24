#pragma once
#include <map>
#include <array>
#include "CARD_MEMBER.h"
#include "Player.h"
#include "Menu.h"
#include "VECTOR2.h"

class Cards
{
public:
	Cards();
	~Cards();
	void Update(void);						// アップデート関数
	void Draw(Player* player,Menu* menu);	// 描画
	void SetTurn(int turn);					// 敵の攻撃までに動かしていい回数の設定
	int GetTurn(void);						// 残り移動回数の取得
	void SetDamage(int damage);				// プレイヤーの攻撃後、0に戻すのに必要
	int GetDamage(void);					// プレイヤーの攻撃力に+するカードの威力取得用関数
	void SetGuard(int guard);				// プレイヤーの防御後、0に戻すのに必要
	int GetGuard(void);						// プレイヤーの防御力に+するカードの威力取得用関数
	void SetHeal(int heal);					// プレイヤーの回復後、0に戻すのに必要
	int GetHeal(void);						// プレイヤーの最大HPに対して何割回復するかの数値を取得する関数
	CARDS_SYURUI GetCardsSyurui(void);		// 使用したカードの種類を取得する
	void SetCardsSyurui(CARDS_SYURUI cards);// 使用後にNONに戻すときに使う
private:
	bool Init(void);						// 初期化
	void Move(int id);						// カードの動き(手元で組み合わせる)
	void UseCard(int id);					// カードを敵や自分に対して使用する

	// マウス関係
	int _mouse;								// マウスの状態
	VECTOR2 _cursorPos;						// マウスカーソルの座標保存用変数

	VECTOR2 _pngPos;						// 画像の座標保存用変数
	bool _clickFlg;							// マウスを左クリックしたことを判定するフラグ(離した後に使う)
	int _setCardPos[5];						// カードが置かれるであろう場所の保存用配列(X座標で考える)
	int _cardPNG[static_cast<int>(CARD_MEMBER::MAX)-1];	// カード画像
	int _id;								// カードのid

	int _turnNum;							// カードを動かす毎に-1
	int _damageNum;							// カードの攻撃力保存用変数
	int _gaurdNum;							// カードの防御力保存用変数
	int _sumGaurdNum;						// 防御カード連続使用時に防御力を+していくための変数
	int _healNum;							// カードの回復力保存用変数

	CARDS_SYURUI _cardsSyurui;				// 使用したカードの種類情報を保存する

	// カードの置き場所(_setCardPos[5])をキー、CARD_MODEを値として扱う連想配列
	// これで、知りたい場所を指定することによってカードの種類を知ることができる。かも。しれない。
	std::map<int, CARD_MEMBER> _cardMap;

	// CARD_MODEと_cardPNGをセットで考えたい
	// CARD_MODEを元に、画像を決められるようにしたい
	// ①CARD_MODEが決まる
	// ②CARD_MODEに合った画像がpngSelに入れられる
	// ③pngSelをつかってDrawされる
	std::pair<int, CARD_MEMBER>_cardPngSel_pair[static_cast<int>(CARD_MEMBER::MAX)];

	int _se;								// 重ねたときのSE
};