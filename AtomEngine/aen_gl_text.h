/*!
 *	AtomEngine
 *	Text
 *	@brief		テキスト
 *	@date		2012 / 04 / 03
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_TEXT_H_INCLUDE_GUARD_211977715_719011335_1__
#define __ATOM_ENGINE_AEN_GL_TEXT_H_INCLUDE_GUARD_211977715_719011335_1__


#include	<vector>
#include	"aen_handle.h"


namespace aen
{
namespace gl
{


/// テキスト
class Text;


/// テキスト
class Text
{
public:
	enum TypeFont {
		/// 通常
		FT_N = 2000,
		/// エッジ
		FT_E,
		/// アンチエイリアス
		FT_A,
		/// エッジ付きアンチエイリアス
		FT_AE
	};


	/*! フォントのセット。
	 * @brief		フォントを読み込む。
	 * @param fontName	フォント名
	 * @param size		フォントの大きさ
	 * @param thinck	フォントの太さ
	 * @param t		フォントタイプ
	 * @warning		すでにセットされていた場合上書きとなる。
	 * @return		成功 true, 失敗 false
	 */
	bool		load(
		const char* const fontName = "ＭＳ ゴシック",
		const unsigned int size = 16,
		const unsigned int thinck = 0,
		const TypeFont t = FT_AE
	);


	/// ハンドルの開放。
	void		del(void);
inline	void		clear(void) { this->del(); };
inline	void		reset(void) { this->del(); };


	/*! 読み込まれていれば true を返す。
	 * @brief		読み込まれていれば true ただし、ハンドルの有効性までは判定しない。
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		読み込まれている true, 読み込まれていない false
	 */
	bool		isLoad(void) const { return (this->m_handle.get() ? true : false); };


	/*! 指定した文字列を描画した時の横幅を返す。
	 * @brief		指定した文字列をこのオブジェクトで描画した時の横幅を返す。
	 * @param str		文字列
	 * @return		描画した時の横幅 ( ピクセル単位 )
	 */
unsigned int		getW(const char* const str) const;
	/*! フォントの縦幅を返す。
	 * @brief		このオブジェクトが持っているフォントの縦幅を返す。
	 * @return		フォントの縦幅 ( ピクセル単位 )
	 */
unsigned int		getH(void) const { return (this->m_fontSize); };


	/*! 文字列の出力。
	 * @brief		文字列を出力する。
	 * @param x		出力 x 座標
	 * @param y		出力 y 座標
	 * @param color		出力色
	 * @param str		出力文字列
	 * @return		なし。
	 */
	void		draw(int x, int y, int color, const char* const str) const;


	Text();
virtual	~Text() { this->del(); }


private:
	boost::shared_ptr<handle::HandleDxLib>	m_handle;
	int		m_fontSize;


};


inline Text::Text() :
	m_fontSize(0)
{
	this->clear();
}


} } // namespace aen::gl


#endif // __ATOM_ENGINE_AEN_GL_TEXT_H_INCLUDE_GUARD_211977715_719011335_1__

