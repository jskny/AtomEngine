/*
 *	EvenShooter
 *	スプライトシステム。
 *	2012 / 04 / 08
 *	jskny
*/


#pragma once

#ifndef __EVENSHOOTER_SPRITE_INCLUDE_GUARD_0371_13_13985639_234_21__
#define __EVENSHOOTER_SPRITE_INCLUDE_GUARD_0371_13_13985639_234_21__


#include	<aen.h>
#include	<vector>
#include	"object_game.h"


namespace sprite
{


// スプライト
class Sprite;
// スプライトシステム
class System;


// スプライト
class Sprite
{
public:
	// ゲームオブジェクトを参照する値を取得。
	int		getKey(void) const { return (this->m_key); };
	// ゲームオブジェクトを参照する値をセット
	void		setKey(int key) { this->m_key = key; };


	// 描画するときは true を返す。
	bool		isViwe(void) const { return (this->m_flagViwe); };
	// 描画する true
	void		setFlagViwe(bool flag) { this->m_flagViwe = flag; };


	// 画像をセット
	void		setGr(const aen::gl::Picture& a) { this->m_gr = a; };
	// 何枚目の画像を出力するか。
	void		setGrIndex(int index) { this->m_grIndex = index; };
	// 出力。
	// 重い処理を中で行います。
	void		draw(void);


	// 殺すか？
	bool		isKill(void) const { return (this->m_flagKill); };
	void		kill(void) { this->m_flagKill = true; };


	Sprite() :
		m_flagViwe(false), m_flagKill(false),
		m_key(0), m_grIndex(0),
		m_gr()
	{
	}


private:
	bool		m_flagViwe;
	bool		m_flagKill;
	// ゲームオブジェクトを参照する値。
	int		m_key;


	// 何枚目の画像を出力するか、
	int		m_grIndex;


	// 画像
aen::gl::Picture	m_gr;


};


// スプライトシステム
class System
{
public:
	// 新しいオブジェクトの追加。
	// 描画イベントが発生したら追加。
	void		add(int key, const aen::gl::Picture& gr, int index)
	{
		Sprite a;
		a.setKey(key);
		a.setGrIndex(index);
		a.setFlagViwe(true);
		a.setGr(gr);


		this->m_list.push_back(a);
	}


	// 登録されているスプライトを描画
	void		run(void);
	// 登録されているスプライトをすべて破棄
	void		clear(void)
	{
		this->m_list.clear();
	}


private:
	std::list<Sprite>	m_list;


};


} // namespace sprite


#endif // __EVENSHOOTER_SPRITE_INCLUDE_GUARD_0371_13_13985639_234_21__

