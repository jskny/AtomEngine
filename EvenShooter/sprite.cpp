/*
 *	EvenShooter
 *	スプライトシステム。
 *	2012 / 04 / 08
 *	jskny
*/


#include	<iostream>
#include	<sstream>
#include	<string>


#include	<aen.h>


#include	"evenshooter.h"
#include	"sprite.h"


using namespace std;


namespace sprite
{


// 出力。
void Sprite::draw(void)
{
	// 描画しない。
	if (!this->isViwe()) {
		return;
	}


	// 画像なし。
	if (!this->m_gr.isLoad()) {
AEN_ERROR("画像がそもそもセットされてねーよ！！");
		return;
	}


	// 参照ハンドル未初期化。
	if (this->getKey() == 0) {
AEN_ERROR("参照ハンドルが未初期化だぞ。");
		return;
	}


	ObjectGamePtr a = EvenShooter->getObjectGame(this->getKey());
	if (!a.get()) {
AEN_ERROR("参照ハンドルの参照先が不正。");
		return;
	}


	// 出力。
	this->m_gr.draw((int)a->x, (int)a->y, this->m_grIndex);
}


// 登録されているスプライトを描画
void System::run(void)
{
	// Kill 駆除
	// リバースイテレーターは erase が使えない・・・
	// こんな・・・こんな無駄な・・・
{
	auto itr = this->m_list.begin();
	while (itr != this->m_list.end()) {
		if (itr->isKill()) {
			itr = this->m_list.erase(itr);
			continue;
		}


		++itr;
	}
}


	// 逆順逐次
	auto itr = this->m_list.rbegin();
	while (itr != this->m_list.rend()) {
		if (itr->isViwe()) {
			itr->draw();
		}


		++itr;
	}


	return;
}


} // namespace sprite


