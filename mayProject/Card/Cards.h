#pragma once
#include <map>
#include <array>
#include <memory>
#include "CARD_MEMBER.h"
#include "../Player.h"
#include "../Menu.h"
#include "../Common/VECTOR2.h"

// カードの威力計算式
#define NUM_CAL(X) 3 * 3 - (static_cast<int>(X) - 1)

class Cards
{
public:
	Cards();
	~Cards();
	void Update(void);						
	void Draw(const std::shared_ptr<Player>& player, const std::shared_ptr<Menu>& menu);	
	void SetTurn(const int& turn);			// 敵の攻撃までに動かしていい回数の設定
	int  GetTurn(void)const;				// 残り移動回数の取得
	void SetDamage(const int& damage);		// プレイヤーの攻撃後、0に戻すのに必要
	int  GetDamage(void)const;				// プレイヤーの攻撃力に+するカードの威力取得用関数
	void SetGuard(const int& guard);		// プレイヤーの防御後、0に戻すのに必要
	int  GetGuard(void) const;				// プレイヤーの防御力に+するカードの威力取得用関数
	void SetHeal(const int& heal);			// プレイヤーの回復後、0に戻すのに必要
	int  GetHeal(void)  const;				// プレイヤーの最大HPに対して何割回復するかの数値を取得する関数
	CARDS_TYPE GetCardsType(void) const;	// 使用したカードの種類を取得する
	void SetCardsType(const CARDS_TYPE& cards);// 使用後にNONに戻すときに使う
private:
	bool Init(void);						
	void SetCardPos(int id,int posId);		// カードの座標設定
	void Move(int id);						// カードの動き(手元で組み合わせる)
	void UseCard(int id);					// カードを敵や自分に対して使用する
	void CreateCard(int id);				// カードの生成や重ねた際の処理を行う

	// マウス関係
	int _mouse;								// マウスの状態
	VECTOR2 _cursorPos;						// マウスカーソルの座標保存用変数

	VECTOR2 _pngPos;						// 画像の座標保存用変数
	bool _clickFlg;							// マウスを左クリックしたことを判定するフラグ(離した後に使う)
	int  _setCardPos[5];					// カードが置かれるであろう場所の保存用配列(X座標で考える)
	int  _cardPNG[static_cast<int>(CARD_MEMBER::MAX)-1];	// カード画像
	int  _id;								// カードのid

	int _turnNum;							// カードを動かす毎に-1
	int _damageNum;							// カードの攻撃力保存用変数
	int _gaurdNum;							// カードの防御力保存用変数
	int _sumGaurdNum;						// 防御カード連続使用時に防御力を加算していく為の変数
	int _healNum;							// カードの回復力保存用変数

	CARDS_TYPE _cardsType;					// 使用したカードの種類情報を保存する
	std::map<int, CARD_MEMBER> _cardMap;	// カード座標の設定用
	std::map<CARD_MEMBER, int> _cardPower;	// カード威力の設定用
	std::pair<int, CARD_MEMBER>_cardPngSel_pair[static_cast<int>(CARD_MEMBER::MAX)];

	int _se;								// 重ねたときのSE
};