/*!
 *	AtomEngine
 *	ハッシュ関数。
 *	@brief		ハッシュ関数。
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#include	<string>
#include	<cassert>


#include	"aen_hash.h"
#include	"aen_util.h"


using namespace std;


namespace aen
{
namespace hash
{


//---------------------------------------------------------
// ハッシュ
// example :
//	char* buf = "AAaaadsbuyg8";
//	int hash = adler32((unsigned char*)buf, strlen(buf));
//	cout << hash << endl;
//
// http://ja.wikipedia.org/wiki/Adler-32 からコピペ。
//---------------------------------------------------------
unsigned int Adler(unsigned char* data, int size)
{
const	unsigned int MOD_ADLER = 65521;
	unsigned int a = 1, b = 0;


	if (!data) {
		// null pointer.
		assert(data && "null pointer.");
		return (0);
	}


	size_t len = 0;
	if (size == -1) {
		len = strlen(reinterpret_cast<char*>(data));
	}
	else {
		len = size;
	}


	if (len <= 0) {
		return (0);
	}


	while (len > 0) {
		size_t tlen = len > 5550 ? 5550 : len;
		len -= tlen;
		do {
			a += *data++;
			b += a;
		} while (--tlen);

		a %= MOD_ADLER;
		b %= MOD_ADLER;
	}


	return ((b << 16) | a);
}


/*! ハッシュ関数 すべて小文字化する
 * @brief		文字列を小文字にしてから、ハッシュ化
 * @param data		ハッシュ化する文字列
 * @param size		ハッシュ化するデータのサイズ これが -1 だと配列の中の、\0 までをハッシュ化する。
 * @warning		size に -1 を入れると、\0 を探すループに入るので、極力サイズを指定してください。
 * @warning		極めて質の悪いハッシュ値になります。
 * @warning		文字列を内部でコピーするため重いです。
 * @return		ハッシュ値
 */
unsigned int AdlerStringLower(const char* const data, int size)
{
	if (!data) {
		assert(data && "null pointer.");
		return (NULL);
	}


	size_t len = 0;
	if (size == -1) {
		len = strlen(data);
	}
	else {
		len = size;
	}


	if (len <= 0) {
		return (NULL);
	}


	char* buf = new char[len+2];
	assert(buf && "null pointer.");
	if (!buf) {
		return (NULL);
	}


	// コピれば安全だろ。
	util::StrCopy(buf, data, len);
	// これがないと、止まらない。
	buf[len] = '\0';

	// 小文字化
	util::ConvertStringLower(buf);


	unsigned int ret = Adler(reinterpret_cast<unsigned char*>(buf), len);
	delete[] (buf);


	return (ret);
}



}} // namespace aen::hash

