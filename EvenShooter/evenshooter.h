/*
 *	EvenShooter
 *	2012 / 03 / 24
 *	jskny
*/


#pragma once

#ifndef __EVENSHOOTER_EVENSHOOTER_INCLUDE_GUARD_57827400193321637842_471__
#define __EVENSHOOTER_EVENSHOOTER_INCLUDE_GUARD_57827400193321637842_471__


#include	<map>
#include	<boost/shared_ptr.hpp>


#include	<aen.h>
#include	"sprite.h"
#include	"input.h"


#define		WINDOW_W		(800)
#define		WINDOW_H		(600)


class EvenLogic;


// 入力系はこっちで管理しやすいのがいいから。
// typedef	aen::gl::input::Mouse		Mouse;
// typedef	aen::gl::input::Keyboard	Keyboard;


// ゲームロジック本体
extern	EvenLogic*		EvenShooter;


class EvenLogic :public aen::Logic
{
public:
	// プロファイルマネージャーを取得。
aen::profiler::Manager*	getManagerProfile(void) { return (&this->m_managerProfile); };


	// ゲームオブジェクトを取得。
boost::shared_ptr<ObjectGame>	getObjectGame(int key) { return (this->m_mapObjectGame[key]); };


	// スプライトマネージャーを取得。
sprite::System*		getSystemSprite(void) { return (&this->m_spriteSystem); }


	// イベントマネージャー取得
aen::system::event::Manager*	getManagerEvent(void) { return (&this->m_managerEvent); };


	bool		initialize(void);
	bool		finalize(void);


	// スクリプト・シーン実行
	void		run(void);
	bool		setScene(const char* const name);
	// システムループ呼び出し
	bool		systemLoop(void);


	EvenLogic();
	~EvenLogic();


private:
	aen::profiler::Manager	m_managerProfile;
std::map<unsigned int, boost::shared_ptr<ObjectGame>>	m_mapObjectGame;
	sprite::System		m_spriteSystem;


	// イベントマネージャー
	aen::system::event::Manager	m_managerEvent;


aen::scripter::ScripterV8	m_scripter;


};


// EvenShooter システムイベントリスナー
class EvenShooterEventListener :public aen::system::event::Listener
{
public:
	bool		handleEvent(aen::system::event::Event& event);


};


#endif // __EVENSHOOTER_EVENSHOOTER_INCLUDE_GUARD_57827400193321637842_471__

