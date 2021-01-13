#include "DxLib.h"
#include "Enemy_weak.h"
#include "Cards.h"

#define CARD_SIZE_X 126		// カード1枚の横サイズ
#define CARD_SIZE_Y 180

struct cardsPNG
{
	VECTOR2 pos;			// 座標
	CARD_MEMBER cardMem;	// 種類
	int num;				// 番号
	int pngSel;				// 画像
} card[9];

Cards::Cards()
{
	Init();
}

Cards::~Cards()
{
	DeleteSoundMem(_se);
}

void Cards::Update(void)
{
	_mouse = GetMouseInput();						 //マウスの入力状態取得
	GetMousePoint(&_cursorPos.x, &_cursorPos.y);	 //マウスの座標取得
	if (!_clickFlg)
	{
		if (_mouse & MOUSE_INPUT_LEFT) {			 //マウスの左ボタンが押されていたら
			// 画像とマウスクリックの当たり判定
			for (int i = 0; i <= 5; i++)
			{
				if (_cursorPos.x >= card[i].pos.x - CARD_SIZE_X / 2 && _cursorPos.x <= card[i].pos.x + CARD_SIZE_X / 2 && _cursorPos.y >= card[i].pos.y - CARD_SIZE_Y / 2 && _cursorPos.y <= card[i].pos.y + CARD_SIZE_Y / 2)
				{
					_clickFlg = true;
					_id = i;
				}
			}
		}
		else
		{
			// 左クリックしていないときはidを-1にしておく
			_id = -1;
		}
	}

	// 左クリックで押されている間だけ動かせるようにする処理
	if (_clickFlg)
	{
		card[_id].pos = VECTOR2(_cursorPos);
	}
	else
	{
		_id = -1;
	}

	// カードの動きの処理
	if (_id >= 0)
	{
		Move(_id);
	}
}

void Cards::SetTurn(const int& turn)
{
	_turnNum = turn;
}

int Cards::GetTurn(void)const
{
	return _turnNum;
}

void Cards::SetDamage(const int& damage)
{
	_damageNum = damage;
}

int Cards::GetDamage(void)const
{
	return _damageNum;
}

void Cards::SetGuard(const int& guard)
{
	_gaurdNum = guard;
	_sumGaurdNum = guard;
}

int Cards::GetGuard(void)const
{
	return _sumGaurdNum;
}

void Cards::SetHeal(const int& heal)
{
	_healNum = heal;
}

int Cards::GetHeal(void) const
{
	return _healNum;
}

CARDS_SYURUI Cards::GetCardsSyurui(void)const
{
	return _cardsSyurui;
}

void Cards::SetCardsSyurui(const CARDS_SYURUI& cards)
{
	_cardsSyurui = cards;
}

bool Cards::Init(void)
{
	_se = LoadSoundMem("sound/se/cards.mp3");

	_turnNum = 3;	// 敵によって増減
	_damageNum = 0;
	_gaurdNum = 0;
	_sumGaurdNum = 0;
	_healNum = 0;

	_cardsSyurui = CARDS_SYURUI::NON;

	// カードの分割読み込み
	std::string cardpng = "image/card.png";
	LoadDivGraph(cardpng.c_str(), 9, 1, 9, 126, 180, _cardPNG);

	_clickFlg = false;
	_cursorPos = VECTOR2(0, 0);

	// ボックス位置の決定
	for (int i = 0; i <= 4; i++)
	{
		_setCardPos[i] = 140 * (i + 1);
		// キーが存在しないときに構築する
		_cardMap.try_emplace(_setCardPos[i], CARD_MEMBER::NON);
	}

	for (auto member : CARD_MEMBER())
	{
		_cardPngSel_pair[static_cast<int>(member) - 1] = std::make_pair(_cardPNG[static_cast<int>(member) - 1], member);
	}

	for (int i = 0; i <= 4; i++)
	{
		card[i].pos = VECTOR2(_setCardPos[i] - 140 / 2, 500);
		//0 3 6のカードのみだしたい
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[i].pngSel = _cardPngSel_pair[userand].first;
		card[i].cardMem = _cardPngSel_pair[userand].second;
		card[i].num = i;
		// カードをセットするところに種別情報を入れる
		_cardMap[_setCardPos[i]] = card[i].cardMem;
	}

	_id = -1;
	return true;
}

void Cards::Draw(Player* player, Menu* menu)
{
	//ClsDrawScreen();

	for (int i = 0; i <= 4; i++)
	{
		// ここでカードをすべてDrawしてしまうと、
		// クリックしたカードが右側のカードよりしたに描画されてしまうのでわける
		//Draw(card[i].pos, card[i].pngSel);
		if (_id >= 0)
		{
			// 左クリックして所持しているカードがあるとき
			// 左クリックされているカード以外の描画
			if (_id != i)
			{
				// 所持カードの種類が異なるとき(薄く表示)
				if (card[i].cardMem != card[_id].cardMem)
				{
					// 描画ブレンドモード変更
					SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
					DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
					// 描画ブレンドモードをノーブレンドにする
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else
				{
					// 所持カードの種類が同じとき
					DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
				}
			}
		}
		else
		{
			// 通常はここでDraw
			DrawRotaGraph(card[i].pos.x, card[i].pos.y, 1.0f, 0, card[i].pngSel, false);
		}
	}

	// 縦線
	for (int i = 141; i < 702;)
	{
		DrawLine(i, 400, i, 600, GetColor(255, 0, 0), 2);
		i += 140;
	}

	// 横線
	DrawLine(0, 400, 900, 400, GetColor(255, 0, 0), 2);

	// idが入っているときはそのカードが一番上にくるようにここでDrawする
	// 左クリックされているカードの描画
	if (_id >= 0)
	{
		DrawRotaGraph(card[_id].pos.x, card[_id].pos.y, 1.0f, 0, card[_id].pngSel, false);
	}

	VECTOR2 _offset = { -80,90 };
	// カード情報の表示
	// GetHealとかの数字はまだusecardの関数に入る前なので使えない
	if (_id != -1)
	{
		if (card[_id].pos.y > 0 && card[_id].pos.y <= 300)
		{
			std::string cardType;
			int cardMember = static_cast<int>(card[_id].cardMem);	// 操作中のカードの情報を取得
			// 取得したカードの種類によって計算処理が別れる
			if (cardMember >= static_cast<int>(CARD_MEMBER::ATTACK_1) && cardMember <= static_cast<int>(CARD_MEMBER::ATTACK_3))
			{
				cardType = "攻撃";
				DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:敵に%dのダメージ", cardType.c_str(),
					  (cardMember + (cardMember - 1))
					* (cardMember + (cardMember - 1)) 
					+  player->GetAttackDamage() + menu->GetEquipDamage() + menu->GetPowUp());
			}
			if (cardMember >= static_cast<int>(CARD_MEMBER::GUARD_1) && cardMember <= static_cast<int>(CARD_MEMBER::GUARD_3))
			{
				cardType = "防御";
				DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:ダメージ%d割の軽減", cardType.c_str(),
					  ((cardMember % static_cast<int>(CARD_MEMBER::GUARD_1)) + 1)
					+ (cardMember % static_cast<int>(CARD_MEMBER::GUARD_1)) + (cardMember / NUM_CAL(CARD_MEMBER::GUARD_1)));
			}
			if (cardMember >= static_cast<int>(CARD_MEMBER::HEAL_1) &&  cardMember <= static_cast<int>(CARD_MEMBER::HEAL_3))
			{
				cardType = "回復";
				DrawFormatString(card[_id].pos.x + _offset.x, card[_id].pos.y + _offset.y, 0xffffff, "%s:体力%d割の回復", cardType.c_str(),
					  ((cardMember % static_cast<int>(CARD_MEMBER::HEAL_1)) + 1)
					+ (cardMember % static_cast<int>(CARD_MEMBER::HEAL_1)) + (cardMember / NUM_CAL(CARD_MEMBER::HEAL_1)));
			}
		}
	}

	//ScreenFlip();
}

void Cards::Move(int id)
{
	// 範囲外処理
	if (card[id].pos.x < 0)
	{
		card[id].pos.x = 0;
	}
	else if (card[id].pos.x > 700)
	{
		card[id].pos.x = 700;
	}

	// 横線より上にだしたら手札を使う処理を行う
	if (card[id].pos.y <= 300)
	{
		// 左クリック離した後
		if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
		{
			UseCard(id);
			// 元の位置に戻す(戻さなかったらカードがずっと使用される状態になるから位置を戻さないといけない)
			card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
			card[id].pos.y = 500;
			_clickFlg = false;
		}
	}

	if (card[id].pos.y > 300)
	{
		// 左クリック離した後
		if (_clickFlg && !(_mouse & MOUSE_INPUT_LEFT))
		{
			// 左端
			if (card[id].pos.x >= 0 && card[id].pos.x <= _setCardPos[0])
			{
				// 左端に何も入っていないとき
				if (_cardMap[_setCardPos[0]] == CARD_MEMBER::NON)
				{
					// numを書き換える前にnonにする
					_cardMap[_setCardPos[card[id].num]] = CARD_MEMBER::NON;

					// 移動させていい
					card[id].pos.x = _setCardPos[0] - 140 / 2;
					card[id].pos.y = 500;
					card[id].num = 0;

					// 新しく移動した場所にカードの種類を登録
					_cardMap[_setCardPos[0]] = card[id].cardMem;
				}
				else if (_cardMap[_setCardPos[0]] == card[id].cardMem && card[id].num != 0)
				{
					// 3以上のカードが作れないようにする
					if (card[id].cardMem == CARD_MEMBER::ATTACK_3 || card[id].cardMem == CARD_MEMBER::GUARD_3 || card[id].cardMem == CARD_MEMBER::HEAL_3)
					{
						// 元の位置に戻す
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;
					}
					else
					{
						// 同じカードの種類で重ねる時に行う処理
						PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);

						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;

						card[0].pngSel = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].first;
						card[0].cardMem = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].second;
						_cardMap[_setCardPos[0]] = card[0].cardMem;

						// 動かし元のスペースに新しいカードの生成を行う
						//card[id] = card[card[id].num];
						// 次に出るカードはランダムにする
						//0 3 6のカードのみだしたい
						int rand = GetRand(8);
						int userand = rand % 3 * 3;
						card[id].pngSel = _cardPngSel_pair[userand].first;
						card[id].cardMem = _cardPngSel_pair[userand].second;
						_cardMap[_setCardPos[id]] = card[id].cardMem;
						_turnNum -= 1;
					}
				}
				else
				{
					// 移動させることができないときとかに元の位置に戻す
					card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
					card[id].pos.y = 500;
				}
				_clickFlg = false;
			}

			for (int i = 0; i <= 3; i++)
			{
				if (card[id].pos.x > _setCardPos[i] && card[id].pos.x <= _setCardPos[i + 1])
				{
					if (_cardMap[_setCardPos[i + 1]] == CARD_MEMBER::NON)
					{
						// numを書き換える前にnonにする
						_cardMap[_setCardPos[card[id].num]] = CARD_MEMBER::NON;

						// 移動させていい
						card[id].pos.x = _setCardPos[i + 1] - 140 / 2;
						card[id].pos.y = 500;
						card[id].num = i + 1;

						// 新しく移動した場所にカードの種類を登録
						_cardMap[_setCardPos[i + 1]] = card[id].cardMem;
					}
					else if (_cardMap[_setCardPos[i + 1]] == card[id].cardMem && card[id].num != i + 1)
					{
						// 3以上のカードが作れないようにする
						if (card[id].cardMem == CARD_MEMBER::ATTACK_3 || card[id].cardMem == CARD_MEMBER::GUARD_3 || card[id].cardMem == CARD_MEMBER::HEAL_3)
						{
							// 元の位置に戻す
							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;
						}
						else
						{
							// 同じカードの種類で重ねる時に行う処理
							PlaySoundMem(_se, DX_PLAYTYPE_BACK, true);

							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;

							// _cardPngSel_pairがnonから始まるわけではないので、+1しなくていい
							card[i + 1].pngSel = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].first;
							card[i + 1].cardMem = _cardPngSel_pair[static_cast<int>(card[id].cardMem)].second;
							_cardMap[_setCardPos[i + 1]] = card[i + 1].cardMem;

							// 動かし元のスペースに新しいカードの生成を行う
							//card[id] = card[card[id].num];
							// 次に出るカードはランダムにする
							//0 3 6のカードのみだしたい
							int rand = GetRand(8);
							int userand = rand % 3 * 3;
							card[id].pngSel = _cardPngSel_pair[userand].first;
							card[id].cardMem = _cardPngSel_pair[userand].second;
							_cardMap[_setCardPos[id]] = card[id].cardMem;
							_turnNum -= 1;
						}
					}
					else
					{
						// 元の位置に戻す
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;
					}
					_clickFlg = false;
				}
			}
		}
	}
}

void Cards::UseCard(int id)
{
	// 使用したカードがattackのとき
	if (card[id].cardMem == CARD_MEMBER::ATTACK_1 || card[id].cardMem == CARD_MEMBER::ATTACK_2 || card[id].cardMem == CARD_MEMBER::ATTACK_3)
	{
		_cardsSyurui = CARDS_SYURUI::ATTACK;

		// 敵のHPを減らす処理
		// ダメージは、カードの大きさ+プレイヤーの基礎攻撃力 +武器の攻撃力に-2～+2の変動値があるといい感じかもしれない
		//_damageNum = static_cast<int>(card[id].cardMem) * 2;
		if (card[id].cardMem == CARD_MEMBER::ATTACK_1)
		{
			//1
			_damageNum = 1*1;
		}
		else if (card[id].cardMem == CARD_MEMBER::ATTACK_2)
		{
			//9
			_damageNum = 3*3;
		}
		else
		{
			//25
			_damageNum = 5*5;
		}

		// 敵の待機ターン -1
		_turnNum -= 1;

		// 動かし元のスペースに新しいカードの生成を行う
		// 次に出るカードはランダムにする
		// 0 3 6のカードのみだしたい
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].cardMem = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].cardMem;
		return;
	}

	// 使用したカードがguardのとき
	if (card[id].cardMem == CARD_MEMBER::GUARD_1 || card[id].cardMem == CARD_MEMBER::GUARD_2 || card[id].cardMem == CARD_MEMBER::GUARD_3)
	{
		_cardsSyurui = CARDS_SYURUI::GUARD;

		// 敵の待機ターン -1
		_turnNum -= 1;

		// 受ける攻撃の1/3とかでカットできるようにする
		if (card[id].cardMem == CARD_MEMBER::GUARD_1)
		{
			_gaurdNum = 1;
		}
		else if(card[id].cardMem == CARD_MEMBER::GUARD_2)
		{
			_gaurdNum = 3;
		}
		else
		{
			_gaurdNum = 8;
		}

		// 複数回のターンで同じ効果のカードを使用するときに合計が必要になる
		_sumGaurdNum += _gaurdNum;

		// 使ったカードの効果を出して、違うカードを使用した場所に出す

		// 動かし元のスペースに新しいカードの生成を行う
		// 次に出るカードはランダムにする
		// 0 3 6のカードのみだしたい
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].cardMem = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].cardMem;
		return;
	}

	// 使用したカードがhealのとき
	if (card[id].cardMem == CARD_MEMBER::HEAL_1 || card[id].cardMem == CARD_MEMBER::HEAL_2 || card[id].cardMem == CARD_MEMBER::HEAL_3)
	{
		_cardsSyurui = CARDS_SYURUI::HEAL;

		// 敵の待機ターン -1
		_turnNum -= 1;

		// プレイヤーの最大HPの何割で回復する
		if (card[id].cardMem == CARD_MEMBER::HEAL_1)
		{
			_healNum = 1;
		}
		else if(card[id].cardMem == CARD_MEMBER::HEAL_2)
		{
			_healNum = 3;
		}
		else
		{
			_healNum = 8;
		}

		// 使ったカードの効果を出して、違うカードを使用した場所に出す

		// 動かし元のスペースに新しいカードの生成を行う
		// 次に出るカードはランダムにする
		// 0 3 6のカードのみだしたい
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[id].pngSel = _cardPngSel_pair[userand].first;
		card[id].cardMem = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].cardMem;
		return;
	}
}