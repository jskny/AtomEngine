/*!
 *	AtomEngine
 *	ハッシュ関数。
 *	@brief		ハッシュ関数。
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_HASH_H_INCLUDE_GUARD_2884001_4781_91_1_34__
#define __ATOM_ENGINE_AEN_HASH_H_INCLUDE_GUARD_2884001_4781_91_1_34__


namespace aen
{
/// ハッシュ関連
namespace hash
{


/*! ハッシュ関数
 * @brief		ハッシュ関数 http://ja.wikipedia.org/wiki/Adler-32 からコピペ。
 * @param data		ハッシュ化するデータ
 * @param size		ハッシュ化するデータのサイズ これが -1 だと配列の中の、\0 までをハッシュ化する。
 * @warning		size に -1 を入れると、\0 を探すループに入るので、極力サイズを指定してください。
 * @return		ハッシュ値
 */
extern	unsigned int	Adler(unsigned char* data, int size = -1);
/*! ハッシュ関数 文字列をすべて小文字化した状態でハッシュ値を計算する。
 * @brief		文字列を小文字にしてから、ハッシュ化
 * @param data		ハッシュ化する文字列
 * @param size		ハッシュ化するデータのサイズ これが -1 だと配列の中の、\0 までをハッシュ化する。
 * @warning		size に -1 を入れると、\0 を探すループに入るので、極力サイズを指定してください。
 * @warning		極めて質の悪いハッシュ値になります。
 * @warning		文字列を内部でコピーするため重いです。
 * @return		ハッシュ値
 */
extern	unsigned int	AdlerStringLower(const char* const data, int size = -1);
extern	unsigned int	AdlerStringLower(const std::string& str);


inline unsigned int	AdlerStringLower(const std::string& str)
{
	return (AdlerStringLower(str.c_str()));
}


}} // namespace aen::hash


#endif // __ATOM_ENGINE_AEN_HASH_H_INCLUDE_GUARD_2884001_4781_91_1_34__

