#include "DxLib.h"
#include "Enemy_weak.h"
#include "Cards.h"

#define CARD_SIZE_X 126		// カード1枚の横サイズ
#define CARD_SIZE_Y 180

struct cardsPNG
{
	VECTOR2 pos;
	CARD_MEMBER syurui;
	int num;
	int pngSel;
} card[9];

Cards::Cards()
{
	Init();
}

Cards::~Cards()
{
}

void Cards::Update(void)
{
	//if ((ctl.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_F2]) & ~(ctl.GetCtl(KEY_TYPE_OLD)[KEY_INPUT_F2]))
	//{
	//	return std::make_unique<TitleScene>();
	//}

	_mouse = GetMouseInput();					 //マウスの入力状態取得
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

	//for (int i = 0; i <= 4; i++)
	//{
	//	// ここでカードをすべてDrawしてしまうと、
	//	// クリックしたカードが右側のカードよりしたに描画されてしまうのでわける
	//	//Draw(card[i].pos, card[i].pngSel);
	//	if (id >= 0)
	//	{
	//		// 左クリックして所持しているカードがあるとき
	//		// 左クリックされているカード以外の描画
	//		if (id != i)
	//		{
	//			// 所持カードの種類が異なるとき(薄く表示)
	//			if (card[i].syurui != card[id].syurui)
	//			{
	//				// 描画ブレンドモード
	//				SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
	//				Draw(card[i].pos, card[i].pngSel);
	//				// 描画ブレンドモードをノーブレンドにする
	//				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//			}
	//			else
	//			{
	//				// 所持カードの種類が同じとき
	//				Draw(card[i].pos, card[i].pngSel);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		// 通常はここでDraw
	//		Draw(card[i].pos, card[i].pngSel);
	//	}
	//}
	//// idが入っているときはそのカードが一番上にくるようにここでDrawする
	//// 左クリックされているカードの描画
	//if (id >= 0)
	//{
	//	Draw(card[id].pos, card[id].pngSel);
	//}

	//Draw();

	//Draw(card[0].pos, card[0].pngSel);
	//Draw(card[1].pos, card[1].pngSel);
	//Draw(card[2].pos, card[2].pngSel);
	//Draw(card[3].pos, card[3].pngSel);
	//Draw(card[4].pos, card[4].pngSel);
	//Draw(card[5].pos, card[5].pngSel);

}

void Cards::SetTurn(int turn)
{
	_turnNum = turn;
}

int Cards::GetTurn(void)
{
	return _turnNum;
}

void Cards::SetDamage(int damage)
{
	_damageNum = damage;
}

int Cards::GetDamage(void)
{
	return _damageNum;
}

void Cards::SetGuard(int guard)
{
	_gaurdNum = guard;
	_sumGaurdNum = guard;
}

int Cards::GetGuard(void)
{
	return _sumGaurdNum;
}

void Cards::SetHeal(int heal)
{
	_healNum = heal;
}

int Cards::GetHeal(void)
{
	return _healNum;
}

//CARD_MEMBER Cards::GetCardMem(void)
//{
//	return card[_id].syurui;
//}
//
//VECTOR2 Cards::GetCardsPos(void)
//{
//	return card[_id].pos;
//}

CARDS_SYURUI Cards::GetCardsSyurui(void)
{
	return _cardsSyurui;
}

void Cards::SetCardsSyurui(CARDS_SYURUI cards)
{
	_cardsSyurui = cards;
}

//
//bool Cards::GetCardAtkUseFlg(void)
//{
//	return cardAttackUseFlg;
//}
//
//void Cards::SetCardAtkUseFlg(bool flag)
//{
//	cardAttackUseFlg = flag;
//}
//
//bool Cards::GetCardHEALUseFlg(void)
//{
//	return healUseFlg;
//}
//
//void Cards::SetCardHEALUseFlg(bool flag)
//{
//	healUseFlg = flag;
//}
//
//bool Cards::GetCardGUseFlg(void)
//{
//	return guardUseFlg;
//}
//
//void Cards::SetCardGUseFlg(bool flag)
//{
//	guardUseFlg = flag;
//}

bool Cards::Init(void)
{
	_turnNum = 3;	// 敵によって増減
	_damageNum = 0;
	_gaurdNum = 0;
	_sumGaurdNum = 0;
	_healNum = 0;

	_cardsSyurui = CARDS_SYURUI::NON;

	// _cardPNG = attack, 2 = gaurd, 3 = heal として画像を入れられるようにしてみる
	// for文にする
	//std::string attack_1 = "image/test_s_1.png";
	//std::string attack_2 = "image/attack_2.png";
	//std::string guard_1 = "image/test_si_1.png";
	//std::string guard_2 = "image/guard_2.png";
	//std::string heal_1 = "image/test.png";
	//std::string heal_2 = "image/heal_2.png";

	// カードの分割読み込み
	std::string cardpng = "image/card.png";
	LoadDivGraph(cardpng.c_str(), 9, 1, 9, 126, 180, _cardPNG);

	// 画像のメモリへの読みこみ
	//_cardPNG[0] = LoadGraph(attack_1.c_str());
	//_cardPNG[1] = LoadGraph(attack_2.c_str());
	//_cardPNG[2] = LoadGraph(guard_1.c_str());
	//_cardPNG[3] = LoadGraph(guard_2.c_str());
	//_cardPNG[4] = LoadGraph(heal_1.c_str());
	//_cardPNG[5] = LoadGraph(heal_2.c_str());

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

	//_cardPngSel_pair[0] = std::make_pair(_cardPNG[0], CARD_MEMBER::ATTACK_1);
	//_cardPngSel_pair[1] = std::make_pair(_cardPNG[1], CARD_MEMBER::ATTACK_2);
	//_cardPngSel_pair[2] = std::make_pair(_cardPNG[2], CARD_MEMBER::GUARD_1);
	//_cardPngSel_pair[3] = std::make_pair(_cardPNG[3], CARD_MEMBER::GUARD_2);
	//_cardPngSel_pair[4] = std::make_pair(_cardPNG[4], CARD_MEMBER::HEAL_1);
	//_cardPngSel_pair[5] = std::make_pair(_cardPNG[5], CARD_MEMBER::HEAL_2);

	for (int i = 0; i <= 4; i++)
	{
		card[i].pos = VECTOR2(_setCardPos[i] - 140 / 2, 500);
		//0 3 6のカードのみだしたい
		int rand = GetRand(8);
		int userand = rand % 3 * 3;
		card[i].pngSel = _cardPngSel_pair[userand].first;
		card[i].syurui = _cardPngSel_pair[userand].second;

		//card[i].pngSel = _cardPngSel_pair[i].first;
		//card[i].syurui = _cardPngSel_pair[i].second;
		card[i].num = i;

		// カードをセットするところに種別情報を入れる
		_cardMap[_setCardPos[i]] = card[i].syurui;
	}

	//card[0].pos = VECTOR2(_setCardPos[0] - 140 / 2 , 500);
	//card[0].syurui = _cardPngSel_pair[0].second;
	//card[0].num = 0;
	//card[0].pngSel = _cardPngSel_pair[0].first;
	//
	//card[1].pos = VECTOR2(_setCardPos[1] - 140 / 2 , 500);
	//card[1].syurui = _cardPngSel_pair[0].second;
	//card[1].num = 1;
	//card[1].pngSel = _cardPngSel_pair[0].first;
	//
	//card[2].pos = VECTOR2(_setCardPos[2] - 140 / 2 , 500);
	//card[2].syurui = _cardPngSel_pair[4].second;
	//card[2].num = 2;
	//card[2].pngSel = _cardPngSel_pair[4].first;
	//
	//card[3].pos = VECTOR2(_setCardPos[3] - 140 / 2 , 500);
	//card[3].syurui = _cardPngSel_pair[0].second;
	//card[3].num = 3;
	//card[3].pngSel = _cardPngSel_pair[0].first;
	//
	//card[4].pos = VECTOR2(_setCardPos[4] - 140 / 2, 500);
	//card[4].syurui = _cardPngSel_pair[0].second;
	//card[4].num = 4;
	//card[4].pngSel = _cardPngSel_pair[0].first;
	// 5は使っていない
	//card[5].pos = VECTOR2(_setCardPos[5] - 140 / 2, 500);
	//card[5].syurui = CARD_MODE::NON;
	//card[5].num = 5;

	// カードをセットするところに種別情報を入れる
	//_cardMap[_setCardPos[0]] = card[0].syurui;
	//_cardMap[_setCardPos[1]] = card[1].syurui;
	//_cardMap[_setCardPos[2]] = card[2].syurui;
	//_cardMap[_setCardPos[3]] = card[3].syurui;
	//_cardMap[_setCardPos[4]] = card[4].syurui;
	//5は使っていない
	//maptest[_setCardPos[5]] = card[5].syurui;

	// 値の書き換え方のテスト
	//maptest[_setCardPos[0]] = CARD_MODE::ATTACK_1;
	// 値が書き換えられたか確認するためのテスト
	//auto b = maptest[_setCardPos[0]];

	/*_setCardPos[0] = CARD_SIZE_X;	//左端
	_setCardPos[1] = CARD_SIZE_X*2;	//左端から2番目
	_setCardPos[2] = CARD_SIZE_X*3;	//左端から3番目
	_setCardPos[3] = CARD_SIZE_X*4;	//左端から4番目
	_setCardPos[4] = CARD_SIZE_X*5;	//左端から5番目
	_setCardPos[5] = CARD_SIZE_X*6;  //左端から6番目*/


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
				if (card[i].syurui != card[_id].syurui)
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

	// 縦線(後で画像にする)
	// 一番右の広いところがいろいろ描画用
	for (int i = 141; i < 702;)
	{
		DrawLine(i, 400, i, 600, GetColor(255, 0, 0), 2);
		i += 140;
	}

	// 横線(後で画像にする)
	DrawLine(0, 400, 900, 400, GetColor(255, 0, 0), 2);

	// idが入っているときはそのカードが一番上にくるようにここでDrawする
	// 左クリックされているカードの描画
	if (_id >= 0)
	{
		DrawRotaGraph(card[_id].pos.x, card[_id].pos.y, 1.0f, 0, card[_id].pngSel, false);
	}

	// カード情報の表示
	// GetHealとかの数字はまだusecardの関数に入る前なので使えない
	if (card[_id].pos.y > 0 && card[_id].pos.y <= 300)
	{
		std::string moji;
		if (static_cast<int>(card[_id].syurui) == 1 || static_cast<int>(card[_id].syurui) == 2 || static_cast<int>(card[_id].syurui) == 3)
		{
			moji = "攻撃";
			//auto a = (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1)) * (static_cast<int>(_cards->GetCardMem()) + (static_cast<int>(_cards->GetCardMem()) - 1));
			DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:敵に%dのダメージ", moji.c_str(), (static_cast<int>(card[_id].syurui) + (static_cast<int>(card[_id].syurui) - 1)) * (static_cast<int>(card[_id].syurui) + (static_cast<int>(card[_id].syurui) - 1)) + player->GetAttackDamage() + menu->GetEquipDamage() + menu->GetPowUp());
		}
		if (static_cast<int>(card[_id].syurui) == 4 || static_cast<int>(card[_id].syurui) == 5 || static_cast<int>(card[_id].syurui) == 6)
		{
			// 1 3 5
			// 1 3 8
			// 4 5 6
			// 4 / 3 = 1 * 3 = 3 - 3 = 0
			// 5 / 3 = 1 * 3 = 3 - 3 = 0
			// 6 / 3 = 2 * 3 = 6 - 3 = 3
			//auto tes = ((static_cast<int>(card[_id].syurui) % 4) + 1) + (static_cast<int>(card[_id].syurui) % 4) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 3);
			moji = "防御";
			DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:ダメージ%d割の軽減", moji.c_str(), ((static_cast<int>(card[_id].syurui) % 4) + 1) + (static_cast<int>(card[_id].syurui) % 4) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 3));
		}
		if (static_cast<int>(card[_id].syurui) == 7 || static_cast<int>(card[_id].syurui) == 8 || static_cast<int>(card[_id].syurui) == 9)
		{
			// 1 3 5
			// 1 3 8
			// 7 8 9
			// 7 / 3 = 2 * 3 = 6 - 6 = 0
			// 8 / 3 = 2 * 3 = 6 - 6 = 0
			// 9 / 3 = 3 * 3 = 9 - 6 = 3
			//auto tes = ((static_cast<int>(card[_id].syurui) % 7) + 1) + (static_cast<int>(card[_id].syurui) % 7) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 6);
			moji = "回復";
			DrawFormatString(card[_id].pos.x - 80, card[_id].pos.y + 90, GetColor(255, 255, 255), "%s:体力%d割の回復", moji.c_str(), ((static_cast<int>(card[_id].syurui) % 7) + 1) + (static_cast<int>(card[_id].syurui) % 7) + (static_cast<int>(card[_id].syurui) / 3 * 3 - 6));
		}
	}


	// 画面描画(画像中央を中心にする為にRotaGraphを使用中)
	//DrawRotaGraph(pos.x, pos.y, 1.0f, 0, pngSelect, false);

	/*DrawLine(141, 400, 141, 600, GetColor(255, 0, 0),2);
	DrawLine(281, 400, 281, 600, GetColor(255, 0, 0),2);
	DrawLine(421, 400, 421, 600, GetColor(255, 0, 0),2);
	DrawLine(561, 400, 561, 600, GetColor(255, 0, 0),2);
	DrawLine(701, 400, 701, 600, GetColor(255, 0, 0),2);*/

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

	// 横線よりいい感じで上にだしたら手札を使う処理を行う
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
					_cardMap[_setCardPos[0]] = card[id].syurui;
				}
				else if (_cardMap[_setCardPos[0]] == card[id].syurui && card[id].num != 0)
				{
					// 3以上のカードが作れないようにする
					if (card[id].syurui == CARD_MEMBER::ATTACK_3 || card[id].syurui == CARD_MEMBER::GUARD_3 || card[id].syurui == CARD_MEMBER::HEAL_3)
					{
						// 元の位置に戻す
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;
					}
					else
					{
						// 同じカードの種類で重ねる時に行う処理
						card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
						card[id].pos.y = 500;

						card[0].pngSel = _cardPngSel_pair[static_cast<int>(card[id].syurui)].first;
						card[0].syurui = _cardPngSel_pair[static_cast<int>(card[id].syurui)].second;
						_cardMap[_setCardPos[0]] = card[0].syurui;

						// 動かし元のスペースに新しいカードの生成を行う
						//card[id] = card[card[id].num];
						// 次に出るカードはランダムにする
						//0 3 6のカードのみだしたい
						int rand = GetRand(8);
						int userand = rand % 3 * 3;
						card[id].pngSel = _cardPngSel_pair[userand].first;
						card[id].syurui = _cardPngSel_pair[userand].second;
						_cardMap[_setCardPos[id]] = card[id].syurui;
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
						_cardMap[_setCardPos[i + 1]] = card[id].syurui;
					}
					else if (_cardMap[_setCardPos[i + 1]] == card[id].syurui && card[id].num != i + 1)
					{
						// 3以上のカードが作れないようにする
						if (card[id].syurui == CARD_MEMBER::ATTACK_3 || card[id].syurui == CARD_MEMBER::GUARD_3 || card[id].syurui == CARD_MEMBER::HEAL_3)
						{
							// 元の位置に戻す
							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;
						}
						else
						{
							// 同じカードの種類で重ねる時に行う処理
							card[id].pos.x = _setCardPos[card[id].num] - 140 / 2;
							card[id].pos.y = 500;

							// _cardPngSel_pairがnonから始まるわけではないので、+1しなくていい
							card[i + 1].pngSel = _cardPngSel_pair[static_cast<int>(card[id].syurui)].first;
							card[i + 1].syurui = _cardPngSel_pair[static_cast<int>(card[id].syurui)].second;
							_cardMap[_setCardPos[i + 1]] = card[i + 1].syurui;

							// 動かし元のスペースに新しいカードの生成を行う
							//card[id] = card[card[id].num];
							// 次に出るカードはランダムにする
							//0 3 6のカードのみだしたい
							int rand = GetRand(8);
							int userand = rand % 3 * 3;
							card[id].pngSel = _cardPngSel_pair[userand].first;
							card[id].syurui = _cardPngSel_pair[userand].second;
							_cardMap[_setCardPos[id]] = card[id].syurui;
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

			//	/*if (_pngPos.x > _setCardPos[0] && _pngPos.x <= _setCardPos[1])
			//	{
			//		_pngPos.x = _setCardPos[1] - CARD_SIZE_X / 2;
			//		_pngPos.y = 500;
			//		_clickFlg = false;
			//	}
			//	if (_pngPos.x > _setCardPos[1] && _pngPos.x <= _setCardPos[2])
			//	{
			//		_pngPos.x = _setCardPos[2] - CARD_SIZE_X / 2;
			//		_pngPos.y = 500;
			//		_clickFlg = false;
			//	}
			//	if (_pngPos.x > _setCardPos[2] && _pngPos.x <= _setCardPos[3])
			//	{
			//		_pngPos.x = _setCardPos[3] - CARD_SIZE_X / 2;
			//		_pngPos.y = 500;
			//		_clickFlg = false;
			//	}
			//	if (_pngPos.x > _setCardPos[3] && _pngPos.x <= _setCardPos[4])
			//	{
			//		_pngPos.x = _setCardPos[4] - CARD_SIZE_X / 2;
			//		_pngPos.y = 500;
			//		_clickFlg = false;
			//	}*/
		}
	}
}

void Cards::UseCard(int id)
{
	// 使用したカードがattackのとき
	if (card[id].syurui == CARD_MEMBER::ATTACK_1 || card[id].syurui == CARD_MEMBER::ATTACK_2 || card[id].syurui == CARD_MEMBER::ATTACK_3)
	{
		_cardsSyurui = CARDS_SYURUI::ATTACK;

		// 敵のHPを減らす処理
		// ダメージは、カードの大きさ+プレイヤーの基礎攻撃力 +武器の攻撃力に-2～+2の変動値があるといい感じかもしれない
		//_damageNum = static_cast<int>(card[id].syurui) * 2;
		if (card[id].syurui == CARD_MEMBER::ATTACK_1)
		{
			//1
			_damageNum = 1*1;
		}
		else if (card[id].syurui == CARD_MEMBER::ATTACK_2)
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
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}

	// 使用したカードがguardのとき
	if (card[id].syurui == CARD_MEMBER::GUARD_1 || card[id].syurui == CARD_MEMBER::GUARD_2 || card[id].syurui == CARD_MEMBER::GUARD_3)
	{
		_cardsSyurui = CARDS_SYURUI::GUARD;

		// 敵の待機ターン -1
		_turnNum -= 1;

		// 受ける攻撃の1/3とかでカットできるようにする
		if (card[id].syurui == CARD_MEMBER::GUARD_1)
		{
			_gaurdNum = 1;
		}
		else if(card[id].syurui == CARD_MEMBER::GUARD_2)
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
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}

	// 使用したカードがhealのとき
	if (card[id].syurui == CARD_MEMBER::HEAL_1 || card[id].syurui == CARD_MEMBER::HEAL_2 || card[id].syurui == CARD_MEMBER::HEAL_3)
	{
		_cardsSyurui = CARDS_SYURUI::HEAL;

		// 敵の待機ターン -1
		_turnNum -= 1;

		// プレイヤーの最大HPの何割で回復する
		if (card[id].syurui == CARD_MEMBER::HEAL_1)
		{
			_healNum = 1;
		}
		else if(card[id].syurui == CARD_MEMBER::HEAL_2)
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
		card[id].syurui = _cardPngSel_pair[userand].second;
		_cardMap[_setCardPos[id]] = card[id].syurui;
		return;
	}
}