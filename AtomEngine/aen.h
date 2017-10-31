/*!
 *	AtomEngine
 *	@brief		AtomEngine
 *	@date		2012 / 03 / 09
 *	@author		jskny
*/


/*!
 * @mainpage Index page
 *
 * @section これは何？
 * ゲーム作るのめんどいヤダっ！
 *
 * ...そう。だから楽に作るのだっ！
 *
 * 目指すはゲーム作りを楽にするライブラリ
 *
 * @section Developer
 * jskny (\@jskny_tw)
 *
 *
 * and you...
 *
 * - @subpage status
 *
 * - @subpage contact
 */


/*! @page status Status
只今作成中です。

@ref contact "Contact"
*/


/*! @page contact Contact
\@jskny_tw までー

@ref status "status"
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_H_INCLUDE_GUARD_367103_1_348920_02_5_24_13__
#define __ATOM_ENGINE_AEN_H_INCLUDE_GUARD_367103_1_348920_02_5_24_13__


#include	<ostream>
#include	<fstream>
#include	<cstdint>


#include	<boost/noncopyable.hpp>


#include	"def.h"
#include	"aen_hash.h"
#include	"aen_handle.h"
#include	"aen_scene.h"
#include	"aen_profiler.h"
#include	"aen_util.h"
#include	"aen_scripter.h"
#include	"aen_system.h"


#include	"aen_gl.h"


#ifdef AEN_USE_LUAPLUS
	#include	"aen_import_lua.h"
#endif


/// AtomEngine が所属するネームスペース
namespace aen
{


/*!
 * ゲームロジックベース
 * @brief	ゲームロジックベース。これを継承してゲームを作ってくれ。
 */
class Logic;


//-------------------------------------
// 非推奨関数。
// Logic::initialize と、
// Logic::finalize を使え。
//-------------------------------------


/*! 非推奨関数
 * @brief		AtomEngine を初期化。
 * @warning		Logic オブジェクトがラップしている。
 * @return		成功 true 失敗 false
 */
extern	bool		Initialize(void) throw (aen::Exception);
/*! 非推奨関数
 * @brief		AtomEngine を終了する。
 * @warning		Logic オブジェクトがラップしている。
 * @return		成功 true 失敗 false
 */
extern	bool		Finalize(void) throw (aen::Exception);
/*! 非推奨関数
 * @brief		AtomEngine が初期化済みなら true を返す。
 * @warning		Logic オブジェクトがラップしている。
 * @return		成功 true 失敗 false
 */
extern	bool		IsInitialize(void);


// ゲームロジック
class Logic :private boost::noncopyable
{
public:
	Logic() {};
virtual	~Logic()
	{
		if (this->isInitialize()) {
			this->finalize();
		}
	}


	/// AtomEngine の初期化。
virtual	bool		initialize(void) throw (aen::Exception);
inline	bool		isInitialize(void) const { return (IsInitialize()); };
	/// AtomEngine の終了。
virtual	bool		finalize(void) throw (aen::Exception);


	/*! 最終手段。
	 * @brief		AtomEngine を強制終了させる。
	 * @return		window オブジェクトへのポインタ
	 */
virtual	void		crash(const char* const expString) throw (aen::Exception);


	/*! Window オブジェクトへのポインタを返す
	 * @brief		Window オブジェクトへのポインタを返す
	 * @return		window オブジェクトへのポインタ
	 */
	aen::gl::Window*	getWindow(void) { return (&this->m_window); };
	/*! DxLib の初期化
	 * @brief		DxLib の初期化
	 * @param title		ウインドウのタイトル。
	 * @param w		ウインドウの X サイズ。
	 * @param h		ウインドウの Y サイズ。
	 * @param flagFull	true なら、フルスクリーン。
	 * @return		成功 true 失敗 false
	 */
inline	bool		openWindow(const std::string& title, int w, int h, bool flagFull) throw (aen::Exception)
	{
		return (this->getWindow()->open(title, w, h, flagFull));
	}
	/*! DxLib の終了
	 * @brief		DxLib を終了させる。
	 * @return		成功 true 失敗 false
	 */
inline	bool		closeWindow(void) throw (aen::Exception)
	{
		return (this->getWindow()->close());
	}


	/// 警告の出力
virtual	void		warn(const std::string& str, const char* const func = NULL, const char* const file = NULL, int line = 0) const;


private:
	// window
	aen::gl::Window	m_window;


};


} // namespace aen


#endif // __ATOM_ENGINE_AEN_H_INCLUDE_GUARD_367103_1_348920_02_5_24_13__

