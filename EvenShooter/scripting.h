/*
 *	EvenShooter
 *	スクリプティング
 *	2012 / 04 / 15
 *	jskny
*/


#pragma once

#ifndef __EVENSHOOTER_SCRIPTING_INCLUDE_GUARD_936_1_3012864_553918732_1_3231__
#define __EVENSHOOTER_SCRIPTING_INCLUDE_GUARD_936_1_3012864_553918732_1_3231__


#include	<aen.h>



// 似非ガベージコレクション
// メモリ馬鹿食い・・・せめてもの・・・
extern	void		ScriptingPush(void);
extern	void		ScriptingPop(void);


// EvenShooter の関数をスクリプターに登録
extern	void		SetEvenFunc(aen::scripter::ScripterV8* scripter);


// EvenShooter の設定。
// EvConfig("key", value);
extern	aen::scripter::ScripterV8::FuncRet V8_EvConfig(aen::scripter::ScripterV8::FuncParam args);


// 画像ハンドルの読み込み
// EvLoadGraph(url)
// EvLoadGraph(url, imax, imaxX, imaxY, imageOneW, imageOneH)
extern	aen::scripter::ScripterV8::FuncRet	V8_EvLoadGraph(aen::scripter::ScripterV8::FuncParam args);
// 描画！
// EvDrawGraph(x, y, handle)
// EvDrawGraph(x, y, handle, angle, extend)
extern	aen::scripter::ScripterV8::FuncRet	V8_EvDrawGraph(aen::scripter::ScripterV8::FuncParam args);


// エンジン側に処理を戻す。
// EvSystem
extern	aen::scripter::ScripterV8::FuncRet	V8_EvSystem(aen::scripter::ScripterV8::FuncParam args);
// シーンを変える。
// EvChangeScene(name)
extern	aen::scripter::ScripterV8::FuncRet	V8_EvChangeScene(aen::scripter::ScripterV8::FuncParam args);
// 画面消去
extern	aen::scripter::ScripterV8::FuncRet	V8_EvScreenClear(aen::scripter::ScripterV8::FuncParam args);
// 画面更新
extern	aen::scripter::ScripterV8::FuncRet	V8_EvScreenFlip(aen::scripter::ScripterV8::FuncParam args);
// Sleep
// EvSleep(time)
extern	aen::scripter::ScripterV8::FuncRet	V8_EvSleep(aen::scripter::ScripterV8::FuncParam args);


#endif // __EVENSHOOTER_SCRIPTING_INCLUDE_GUARD_936_1_3012864_553918732_1_3231__

