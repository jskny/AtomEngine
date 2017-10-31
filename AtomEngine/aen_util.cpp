/*!
 *	AtomEngine
 *	util
 *	@brief		ユーティリティー
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#include	<sstream>
#include	<vector>


#include	<windows.h>
#include	<shlwapi.h> // PathFileExists など
#pragma	comment(lib, "shlwapi.lib")


#include	"def.h"
#include	"aen_util.h"


using namespace std;


namespace aen
{
namespace util
{


/*! パスで指定したディレクトリ内のファイルと、フォルダへのパスを返す。
 * @brief		パスで指定したディレクトリ内のファイルと、フォルダへのパスを返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ディレクトリ内のディレクトリと、ファイルへのパス。
 */
std::vector<std::string> GetListFileAndDirectory(std::string urlPath)
{
	std::vector<std::string> vec;
	std::ostringstream oss;
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA fd;


	// 最後の / を必ずつけること。
	// 空であるのなら
	// 実行ファイルがあるディレクトリを見るのであろう。
	if (!urlPath.empty()) {
		// 最後に / をなければつける。
		if (urlPath.back() != '/') {
			urlPath.push_back('/');
		}
	}


	// アドレス生成。
	oss << urlPath << "*.*";


	// 全てのファイルを列挙する
	hSearch = FindFirstFile(TEXT(oss.str().c_str()), &fd);
	if (hSearch == INVALID_HANDLE_VALUE) {
// cout << "NOT FOUND PATH : " << oss.str() << endl;
		return (vec);
	}
	while (true) {
		if (fd.cFileName[0] == '.' && fd.cFileName[1] == '\0') {
			// "." はスルーする。
			goto labelNextFind;
		}
		else if (fd.cFileName[0] == '.' && fd.cFileName[1] == '.' && fd.cFileName[2] == '\0') {
			// ".." はスルーする。
			goto labelNextFind;
		}


		// アドレスをプッシュ。
		vec.push_back(fd.cFileName);


labelNextFind:
		if (!FindNextFile(hSearch, &fd)) {
			if( GetLastError() == ERROR_NO_MORE_FILES ) {
// cout << "FINISH." << endl;
				break;
			}
			else {
// cerr << "ERR" << endl;
				break;
			}
		}
	}


	FindClose(hSearch);
	hSearch = NULL;


	return (vec);
}


/*! 指定したディレクトリ内にあるディレクトリ一覧を返す。
 * @brief		指定したディレクトリ内にあるディレクトリ一覧を返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ディレクトリ一覧。
 */
std::vector<std::string> GetListDirectory(std::string urlPath)
{
	std::vector<std::string> vec;
	vec = GetListFileAndDirectory(urlPath);


	// ディレクトリ以外を消してゆく。
	auto itr = vec.begin();
	while (itr != vec.end()) {
		if (IsExistFile(itr->c_str())) {
			// ディレクトリではない。消す。
			itr = vec.erase(itr);
			continue;
		}


		++itr;
	}


	return (vec);
}


/*! 指定したディレクトリ内にあるファイル一覧を返す。
 * @brief		指定したファイル内にあるファイル一覧を返す。
 * @param urlPath	ルートとなるディレクトリ。
 * @warning		ディレクトリを潜っては行かない。
 * @return		ファイル一覧。
 */
std::vector<std::string> GetListFile(std::string urlPath)
{
	std::vector<std::string> vec;
	vec = GetListFileAndDirectory(urlPath);


	// ファイル以外を以外を消してゆく。
	auto itr = vec.begin();
	while (itr != vec.end()) {
		if (IsExistDirectory(itr->c_str())) {
			// ファイルではない。消す。
			itr = vec.erase(itr);
			continue;
		}


		++itr;
	}


	return (vec);
}


/*! ディレクトリがあれば true を返す。
 * @brief		指定したディレクトリがあれば true を返す。
 * @param urlPath	ディレクトリまでのアドレス。
 * @return		ディレクトリあり true なし false
 */
bool IsExistDirectory(const char* const urlPath)
{
	if (!urlPath) {
assert(urlPath && "null pointer.");
		return (false);
	}


	bool flagFile = PathFileExists(urlPath) == TRUE ? true : false;
	bool flagDir = PathIsDirectory(urlPath) == TRUE ? true : false;
	if (flagFile && !flagDir) {
		// 指定されたパスにファイルが存在、かつディレクトリでない
		return (false);
	}
	else if (flagDir) {
		// 指定されたパスがディレクトリである
		return (true);
	}


	return (false);
}


/*! 指定したファイルがあれば true を返す。
 * @brief		指定したファイルがあれば true を返す。
 * @param urlPath	ファイルまでのアドレス。
 * @return		ファイルあり true なし false
 */
bool IsExistFile(const char* const urlPath)
{
	if (!urlPath) {
assert(urlPath && "null pointer.");
		return (false);
	}


	bool flagFile = PathFileExists(urlPath) == TRUE ? true : false;
	bool flagDir = PathIsDirectory(urlPath) == TRUE ? true : false;
	if (flagFile && !flagDir) {
		// 指定されたパスにファイルが存在、かつディレクトリでない
		return (true);
	}


	return (false);
}


/*! すべての文字を小文字にする。
 * @brief		すべての文字を小文字にする。
 * @param str		文字列へのポインタ
 * @return		なし
 */
void ConvertStringLower(char* str)
{
	if (!str) {
		AEN_ASSERT_KILL(str, "null pointer");
AEN_THROW_EXCEPTION("null pointer.");
	}


	for (int i = 0; str[i] != '\0'; i++) {
		if (isalpha((unsigned char)str[i])) {
			// アルファベットは小文字へ変換。
			str[i] = tolower((unsigned char)str[i]);
		}
	}


	return;
}


/*! 文字列をコピーするアレ
 * @brief		文字列をコピーするアレ
 * @param dest		コピー先
 * @param source	コピー元
 * @param size		コピーするバイト数
 * @return		なし
 */
void StrCopy(char* dest, const char* const source, int size)
{
	for (int i = 0; i < size; i++) {
		dest[i] = source[i];
	}


	return;
}


// UTF-8 を ShitJis に変換する。
// http://www.lab.its55.com/?p=33
bool ConvUtf8toSJis(char* pSource, char* pDist, int* pSize)
{
	*pSize = 0;

	// UTF-8からUTF-16へ変換
	const int nSize = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pSource, -1, NULL, 0 );
	BYTE* buffUtf16 = new BYTE[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pSource, -1, (LPWSTR)buffUtf16, nSize );

	//UTF-16からShift-JISへ変換
	const int nSizeSJis = ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)buffUtf16, -1, NULL, 0, NULL, NULL );
	if( !pDist ){
		*pSize = nSizeSJis;
		delete buffUtf16;
		return true;
	}

	BYTE* buffSJis = new BYTE[ nSizeSJis * 2 ];
	ZeroMemory( buffSJis, nSizeSJis * 2 );
	::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)buffSJis, nSizeSJis, NULL, NULL );

	*pSize = lstrlen( (char*)buffSJis );
	memcpy( pDist, buffSJis, *pSize );

	delete buffUtf16;
	delete buffSJis;

	return false;
}


// . で区切った結果をかえす。
std::vector<std::string> GetListLexDot(const char* const str)
{
	vector<string> vec;


	// \ のぶんも追加。
const	int size = sizeof(char) * (strlen(str)+1);
	char* buf = new char[size];


	memcpy(buf, str, sizeof(char) * size);


	// 元の文字列を破壊する極めて悪質な標準関数を使っています。
	// 空白または、カンマで区切る
const	char key[] = ".";
	char* tok = NULL;


	tok = strtok(buf, key);
	while (tok != NULL) {
		vec.push_back(tok);
// cout << tok << ":" << endl;
		// 二回目。
tok = strtok(NULL, key);
	}


	delete[](buf);
	return (vec);
}


// スペースとカンまで区切った結果をかえす。
std::vector<std::string> GetListLex(char* const str)
{
	vector<string> vec;


	// \ のぶんも追加。
const	int size = sizeof(char) * (strlen(str)+1);
	char* buf = new char[size];


	memcpy(buf, str, sizeof(char) * size);


	// 元の文字列を破壊する極めて悪質な標準関数を使っています。
	// 空白または、カンマで区切る
const	char key[] = " ,";
	char* tok = NULL;


	tok = strtok(buf, key);
	while (tok != NULL) {
		vec.push_back(tok);
// cout << tok << ":" << endl;
		// 二回目。
tok = strtok(NULL, key);
	}


	delete[](buf);
	return (vec);
}


// コマンドラインオプション文字が含まれていれば true を返す。
bool IsOption(char* const str, char* const option)
{
	vector<string> bufOp = GetListLex(option);
	vector<string> bufStr = GetListLex(str);


	for (auto itr = bufOp.begin(); itr != bufOp.end(); ++itr) {
// cout << "OP : " << *itr << endl;
		for (auto itr2 = bufStr.begin(); itr2 != bufStr.end(); ++itr2) {
			if (*itr == *itr2) {
				return (true);
			}


			string text;
			string str = *itr;
			text = "-";
			text += str;
			if (text == *itr2) {
				return (true);
			}


			text = "--";
			text += str;
			if (text == *itr2) {
				return (true);
			}

			text = "/";
			text += str;
			if (text == *itr2) {
				return (true);
			}
		}
	}


	return (false);
}


/// 文字列の中にその文字があるか検索。あれば true
bool IsFoundStr(const char* const s, char k)
{
	if (!s) {
		return (false);
	}


	int l = strlen(s);
	for (int i = 0; i < l; ++i) {
		if (s[i] == k) {
			return (true);
		}
	}


	return (false);
}


/*! 文字列の中にその文字があるか検索。あれば存在するバイト位置、なければ -1
 * @brief		文字列中の文字を検索、あれば発見バイト位置、なければ -1
 * @param source	文字列
 * @param k		検索する文字
 * @param n		発見をスキップする。
 * @return		発見 バイト位置、存在しない -1
 */
int GetFoundStr(const char* const s, char k, int n)
{
	int count = 0;


	if (!s) {
		return (-1);
	}


	int l = strlen(s);
	for (int i = 0; i < l; ++i) {
		if (s[i] == k) {
			// 発見したらば、その場所を返す。
			if (count == n) {
				return (i);
			}
			else {
				count++;
			}
		}
	}


	return (-1);
}


} } // namespace aen::util

