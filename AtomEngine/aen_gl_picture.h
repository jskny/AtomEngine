/*!
 *	AtomEngine
 *	Picture
 *	@brief		画像 ( 連番 )
 *	@date		2012 / 03 / 25
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_PICTURE_H_INCLUDE_GUARD_1822333_42_357_021__
#define __ATOM_ENGINE_AEN_GL_PICTURE_H_INCLUDE_GUARD_1822333_42_357_021__


#include	<vector>
#include	"aen_handle.h"


namespace aen
{
namespace gl
{


/// 画像 ( 連番 )
class Picture
{
public:
	/*! 画像のセット
	 * @brief		画像のセット
	 * @param urlFile	画像へのパス
	 * @param num_g		画像数
	 * @param num_gx	横に何枚画像があるか
	 * @param num_gy	縦に何枚画像があるか
	 * @param imageOneW	画像一つの横幅
	 * @param imageOneH	画像一つの縦幅
	 * @warning		すでにセットされていた場合上書きとなる。
	 * @return		成功 true, 失敗 false
	 */
	bool		load(
		const char* const urlFile,
		const unsigned int numG,
		const unsigned int numGx,
		const unsigned int numGy,
		const unsigned int imageOneW,
		const unsigned int imageOneH
	);


	/*! 画像のセット
	 * @brief		一枚絵として画像を読み込む
	 * @param urlFile	画像へのパス
	 * @warning		すでにセットされていた場合上書きとなる。
	 * @return		成功 true, 失敗 false
	 */
	bool		load(const char* const urlFile);


	/// ハンドルの開放。
	void		del(void);
inline	void		clear(void) { this->del(); };
inline	void		reset(void) { this->del(); };


	/*! 画像の出力
	 * @brief		画像出力
	 * @param x		出力する x 座標
	 * @param y		出力する y 座標
	 * @param index		何枚目の画像かを指定。
	 * @return		なし
	 */
	void		draw(const long int x, const long int y, const unsigned long int index) const;
	void		draw(const long int x, const long int y, const unsigned long int index, const long double angle, const long double extend = 1.0) const;


	/*! 読み込まれていれば true を返す。
	 * @brief		読み込まれていれば true ただし、ハンドルの有効性までは判定しない。
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		読み込まれている true, 読み込まれていない false
	 */
inline	bool		isLoad(void) const { return (this->m_flagLoad); };
	/*! 一枚絵として読み込まれていれば true を返す。
	 * @brief		一枚絵として読み込まれていれば true ただし、ハンドルの有効性までは判定しない。
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		読み込まれている true, 読み込まれていない false
	 */
inline	bool		isTexture(void) const { return (this->m_flagTexture); };
	/*! 分割絵として読み込まれていれば true を返す。
	 * @brief		分割絵として読み込まれていれば true ただし、ハンドルの有効性までは判定しない。
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		読み込まれている true, 読み込まれていない false
	 */
inline	bool		isDevPic(void) const { return (!this->m_flagTexture); };


	/*! 画像ハンドルを返す。
	 * @brief		画像ハンドルを返す。
	 * @param index		配列参照用
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		画像ハンドル
	 */
	int		getHandle(unsigned int index) const;
	int		getSize(void) const { return (this->m_handle.size()); };


	/// 画像一枚の横幅を取得
	int		getImageW(void) const;
	/// 画像一枚の縦幅を取得
	int		getImageH(void) const;


	Picture();
virtual	~Picture() { this->del(); }


private:
std::vector<boost::shared_ptr<handle::HandleDxLib>>	m_handle;
	bool		m_flagLoad;
	bool		m_flagTexture;


	int		m_imageW, m_imageH;


};


inline Picture::Picture() :
	m_flagLoad(false), m_flagTexture(false),
	m_imageW(0), m_imageH(0)
{
	this->clear();
}


/*! 画像ハンドルを返す。
 * @brief		画像ハンドルを返す。
 * @param index		配列参照用
 * @warning		ハンドルの有効性までは判定しない。
 * @return		画像ハンドル
 */
inline int Picture::getHandle(unsigned int index) const
{
	int a = 0;


// AEN_ASSERT_KILL(index >= 0, "over flow.");
AEN_ASSERT_KILL(index < this->m_handle.size(), "over flow.");
AEN_ASSERT_KILL(this->m_handle.at(index), "over flow.");
	a = this->m_handle.at(index)->getHandle();
AEN_ASSERT_KILL(a, "over flow.");


	return (a);
}


} } // namespace aen::gl


#endif // __ATOM_ENGINE_AEN_GL_PICTURE_H_INCLUDE_GUARD_1822333_42_357_021__

