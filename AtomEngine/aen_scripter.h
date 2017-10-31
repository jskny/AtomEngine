/*!
 *	AtomEngine
 *	スクリプター
 *	@brief		スクリプト関連。
 *	@date		2012 / 04 / 08
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_SCRIPTER_H_INCLUDE_GUARD_399014_20011_2453257835_13__
#define __ATOM_ENGINE_AEN_SCRIPTER_H_INCLUDE_GUARD_399014_20011_2453257835_13__


#include	"def.h"


#ifdef AEN_USE_SCRIPTER_JAVASCRIPT


#include	<string>
#include	<boost/noncopyable.hpp>
#include	<v8.h>


// V8 を使用するのに必要。
#ifdef _DEBUG
	#pragma comment(lib, "v8_g.lib")
	#pragma comment(lib, "v8preparser_g.lib")
#else
	#pragma comment(lib, "v8.lib")
	#pragma comment(lib, "v8preparser.lib")
#endif
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")


namespace aen
{
/// スクリプト言語関連
namespace scripter
{


/// V8Engine ラッパー
class ScripterV8;


// オブジェクトを UTF-8 文字列に変換する。
extern	std::string	ObjectToStringUTF8(v8::Local<v8::Value> value);
// オブジェクトを ASCII 文字列に変換する。
// 注意。UTF-8 を UTF-16 にしてから ASCII に変換するのですっごく重たい。
extern	std::string	ObjectToStringASCII(v8::Local<v8::Value> value);


/// スクリプトラッパー
class ScripterV8 :private boost::noncopyable
{
public:
	// 簡単が良い。
typedef	v8::Handle<v8::Value>	FuncRet;
typedef	const std::vector<v8::Local<v8::Value>>&	FuncParam;
typedef	ScripterV8::FuncRet(*FuncNative)(ScripterV8::FuncParam);


	// スクリプトの読み込み
	// 成功 true
	bool		load(const char* const urlFile);
	bool		isLoad(void) const { return (this->m_flagLoad); };


	// 関数の登録
	// 成功 true
	bool		setFunction(const std::string& funcName, const FuncNative callback);


	// 実行。
	bool		run(void);


	// 初期化
	void		clear(void);
inline	void		reset(void) { this->clear(); };


	// オブジェクトをASCII 文字列に変換し返す。
static	std::string	GetString(const v8::Local<v8::Value>& value) { return (ObjectToStringASCII(value)); };
	// オブジェクトをUTF8 文字列に変換し返す。
static	std::string	GetStringUTF8(const v8::Local<v8::Value>& value) { return (ObjectToStringUTF8(value)); }


	// オブジェクトを int 型に変換し返す。
static	int		GetInt32(const v8::Local<v8::Value>& value) { return ((int)value->Int32Value()); };
	// オブジェクトを unsigned int 型に変換し返す。
static	unsigned int	GetUInt32(const v8::Local<v8::Value>& value) { return ((unsigned int)value->Uint32Value()); };


	ScripterV8();
virtual	~ScripterV8() {};


private:
	bool		m_flagLoad;
	bool		m_flagCreateContext;


v8::HandleScope		m_handleScopeV8;
v8::Handle<v8::Script>	m_script;
boost::shared_ptr<v8::Context::Scope>	m_scope;


};


} } // namespace aen::scripter


#endif // AEN_USE_SCRIPTER_JAVASCRIPT
#endif // __ATOM_ENGINE_AEN_SCRIPTER_H_INCLUDE_GUARD_399014_20011_2453257835_13__

