/*!
 *	AtomEngine
 *	util
 *	@brief		ユーティリティー
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_UTIL_H_INCLUDE_GUARD_7111_1_3893_29_10__
#define __ATOM_ENGINE_UTIL_H_INCLUDE_GUARD_7111_1_3893_29_10__


#include	<string>
#include	<algorithm>
#include	<cassert>
#include	<vector>


namespace aen
{
/// ユーティリティー関連
namespace util
{


/*! 大文字小文字を区別しないで文字列を比較する。
 * @brief		大文字小文字を区別しないで文字列を比較する。
 * @param s1		比較する文字列１
 * @param s2		比較する文字列２
 * @return		同じであれば true そうでなければ false
*/
extern	bool		CompareString(const std::string& s1, const std::string& s2);
extern	bool		CompareString(const std::string& s1, const char* s2);
extern	bool		CompareString(const char* s1, const std::string& s2);
extern	bool		CompareString(const char* s1, const char* s2);


// 大文字/小文字を無視して部分文字列を検索する
// str:検索する文字列。substr:探す部分文字列
// 戻り値：一致した場合は先頭の反復子、一致しない場合はstr.end()
extern	std::string::iterator SearchString(std::string& str, const std::string& substr);


/// 度からラジアンをだす。
extern	double		GetRad(double n);
/// ラジアンを度に変換
extern	double		GetDo(double rad);


/*! パスで指定したディレクトリ内のファイルと、フォルダへのパスを返す。
 * @brief		パスで指定したディレクトリ内のファイルと、フォルダへのパスを返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ディレクトリ内のディレクトリと、ファイルへのパス。
 */
extern	std::vector<std::string>		GetListFileAndDirectory(std::string urlPath);
/*! 指定したディレクトリ内にあるディレクトリ一覧を返す。
 * @brief		指定したディレクトリ内にあるディレクトリ一覧を返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ディレクトリ一覧。
 */
extern	std::vector<std::string>		GetListDirectory(std::string urlPath);
/*! 指定したディレクトリ内にあるファイル一覧を返す。
 * @brief		指定したファイル内にあるファイル一覧を返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ファイル一覧。
 */
 extern	std::vector<std::string>		GetListFile(std::string urlPath);

/*! ディレクトリがあれば true を返す。
 * @brief		指定したディレクトリがあれば true を返す。
 * @param urlPath	ディレクトリまでのアドレス。
 * @return		ディレクトリあり true なし false
 */
extern	bool		IsExistDirectory(const char* const urlPath);
/*! 指定したファイルがあれば true を返す。
 * @brief		指定したファイルがあれば true を返す。
 * @param urlPath	ファイルまでのアドレス。
 * @return		ファイルあり true なし false
 */
extern	bool		IsExistFile(const char* const urlPath);

/*! すべての文字を小文字にする。
 * @brief		すべての文字を小文字にする。
 * @param str		文字列へのポインタ
 * @return		なし
 */
extern	void		ConvertStringLower(char* str);
/*! 文字列をコピーするアレ
 * @brief		文字列をコピーするアレ
 * @param dest		コピー先
 * @param source	コピー元
 * @param size		コピーするバイト数
 * @return		なし
 */
extern	void		StrCopy(char* dest, const char* const source, int size);


// UTF-8 を ShitJis に変換する。
// http://www.lab.its55.com/?p=33
extern	bool		ConvUtf8toSJis(char* pSource, char* pDist, int* pSize);


// . で区切った結果をかえす。
extern	std::vector<std::string>	GetListLexDot(const char* const str);
// スペースとカンまで区切った結果をかえす。
extern	std::vector<std::string>	GetListLex(char* const str);
// コマンドラインオプション文字が含まれていれば true を返す。
extern	bool		IsOption(char* const str, char* const option);


/// 文字列の中にその文字があるか検索。あれば true
extern	bool		IsFoundStr(const char* const s, char k);
/*! 文字列の中にその文字があるか検索。あれば存在するバイト位置、なければ -1
 * @brief		文字列中の文字を検索、あれば発見バイト位置、なければ -1
 * @param source	文字列
 * @param k		検索する文字
 * @param n		発見をスキップする。
 * @return		発見 バイト位置、存在しない -1
 */
extern	int		GetFoundStr(const char* const source, char k, int n);


//---------------------------------------------------------


inline bool CompareString(const std::string& s1, const char* s2)
{
	assert(s2 && "CompareString parameter s2 is nullpointer.");
	if (!s2) {
		return (false);
	}

	return (CompareString(s1, std::string(s2)));
}


inline bool CompareString(const char* s1, const std::string& s2)
{
	assert(s1 && "CompareString parameter s1 is nullpointer.");
	if (!s1) {
		return (false);
	}

	return (CompareString(std::string(s1), s2));
}


inline bool CompareString(const char* s1, const char* s2)
{
	assert(s1 && "CompareString parameter s1 is nullpointer.");
	assert(s2 && "CompareString parameter s2 is nullpointer.");

	if (!s1 || !s2) {
		return (false);
	}

	return (CompareString(std::string(s1), std::string(s2)));
}


// 大文字小文字を区別しないで文字列を比較する。
// 同じであればtrueを、そうではなければfalseを返す。
inline bool CompareString(const std::string& s1, const std::string& s2)
{
	if (s1.length() != s2.length()) {
		return (false);
	}

	// 大文字小文字を区別しない比較を行う。
	return (std::equal(
		s1.begin(), s1.end(),
		s2.begin(), [](const char c1, const char c2) -> bool { return ((tolower(c1) == tolower(c2))); }));
}


// 大文字/小文字を無視して部分文字列を検索する
// str:検索する文字列。substr:探す部分文字列
// 戻り値：一致した場合は先頭の反復子、一致しない場合はstr.end()
inline std::string::iterator SearchString(std::string& str, const std::string& substr)
{
	return (std::search(
			str.begin(), str.end(),
			substr.begin(), substr.end(),
			[](const char c1, const char c2) -> bool { return ((tolower(c1) == tolower(c2))); }));
}


// 度からラジアンをだす。
inline double GetRad(double n)
{
const double RADIAN = (3.1415 / 180.0);
	return (RADIAN * n);
}


// ラジアンを度に変換
inline double GetDo(double rad)
{
const double DO = rad * 180.0 / 3.1415;
	return (DO);
}


} } // namespace aen::util


#endif // __ATOM_ENGINE_UTIL_H_INCLUDE_GUARD_7111_1_3893_29_10__

