/*!
 *	AtomEngine
 *	@brief		AtomEngine
 *	@date		2012 / 03 / 09
 *	@author		jskny
*/


#include	<iostream>
#include	<sstream>
#include	<ostream>
#include	<fstream>
#include	<ctime>


#include	"aen.h"


using namespace std;


namespace aen
{


// ログ出力を std::ostream の拡張で行う。
// http://stackoverflow.com/questions/11836485/how-do-i-override-a-stdfilebuf
// http://www.jah.ne.jp/~naoyuki/Writings/ExtIos.html
class OfstreamExtendBuffer : public std::filebuf {
public:
	OfstreamExtendBuffer(const string& s) :
		m_tag(s)
	{
	}


protected:
	std::streampos	seekoff( 
		std::streamoff off, 
		std::ios::seek_dir dir, 
		int nMode = std::ios::in | std::ios::out )
	{
		return EOF;
	}
	std::streampos	seekpos( 
		std::streampos pos, 
		int nMode = std::ios::in | std::ios::out )
	{
		return EOF;
	}


virtual	int_type	sync()
	{
// 次の業とか。
cout << "[" << this->m_tag << "]\t";
		return std::filebuf::sync();
	}


virtual std::streamsize	xsputn(const char_type* p, std::streamsize n)
	{
//cout << "xsputn" << endl;
		return (filebuf::xsputn(p, n));
	}


virtual	int_type overflow(int c = EOF)
	{
//		if (filebuf::sync()==EOF) {// Change to sync() for the workaround.
//			return EOF;
//		}


		if (c == EOF) {
			return (EOF);
		}
//cout << "A";

		char buffer[2];
		buffer[0]=c;
		DWORD size;
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),buffer,1,&size,NULL);
/*
		char b[2];
		b[0] = c;
		cout << b;*/


		// 基底クラス呼び出し
		filebuf::overflow(c);
		return (1);
	}


private:
	string		m_tag;


};


// 失敗に終わった。
class OfstreamExtend : public std::ostream {
public:
	OfstreamExtend(const string& s) :
		m_exBuf(s),
		std::ostream( 0 )
	{
		init(&this->m_exBuf);
	}


	OfstreamExtend(const string& s, const char* urlFile, std::ios_base::openmode mode = std::ios_base::out) :
		m_exBuf(s),
		std::ostream( 0 )
	{
		init(&this->m_exBuf);
		this->open(urlFile, mode);
	}


	bool is_open() const
	{
		return (this->m_exBuf.is_open());
	}
	void close()
	{
		this->m_exBuf.close();
	}
	void open(const char* urlFile, std::ios_base::openmode mode = std::ios_base::out)
	{
		this->m_exBuf.open(urlFile, mode);
	}


	std::filebuf* rdbuf()
	{
		return (&this->m_exBuf);
	}


private:
	OfstreamExtendBuffer	m_exBuf;

};


/// AtomEngine が初期化されていれば true
static	bool g_flagAtomEngineInitialize = false;


// デバッグ用。
static	ofstream	g_outputStreamDebug;
static	ofstream	g_outputStreamError;
static	ofstream	g_damyStreamDebug;
static	clock_t		g_timeInitialize;
// 現在のログ優先度。
// 数値が大きいほど重要な情報。
static	int		g_outputLogLevel;


/// ログストリームを初期化。
extern	void		OpenStreamLog(void);
/// ログストリーム終了。
extern	void		CloseStreamLog(void);


/*! マトリックスの中身をシリアライズ
 * @brief		Matrixの中身を出力する。
 * @param oss		std::ostream の派生クラスを指定。
 * @param m		マトリックス。
 * @return		なし
 */
void SerializeMatrix(std::ostream& oss, const DxLib::MATRIX& matrix)
{
	for (int i = 0; i < 4; ++i) {
		oss << matrix.m[i][0] << "," << matrix.m[i][1] << "," << matrix.m[i][2] << "," << matrix.m[i][3] << endl;
	}


	return;
}


 /*! ベクター中身をシリアライズ
 * @brief		DxLib::VECTOR の中身を出力する。
 * @param oss		std::ostream の派生クラスを指定。
 * @param v		DxLib::VECTOR
 * @return		なし
 */
void SerializeVector(std::ostream& oss, const DxLib::VECTOR& v)
{
	oss << v.x << "," << v.y << "," << v.z << endl;
}


// 非推奨関数。
// Logic::initialize と、
// Logic::finalize を使え。


// AtomEngine は初期化済みか？
// 初期化済み : true
bool IsInitialize(void)
{
	return (g_flagAtomEngineInitialize);
}


/// AtomEngine を初期化。
bool Initialize(void) throw (aen::Exception)
{
	if (IsInitialize()) {
AEN_THROW_EXCEPTION("AtomEngine is already initialized.");
		return (false);
	}


#ifdef	ATOM_ENGINE_DEBUG
	// メモリーリークの検出
	// メモリーリークチェックして。
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	// プログラム終了前にリークをチェックする。
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif


	// ログストリームの初期化。
	OpenStreamLog();


GetStreamLogDebug(1) << "---------------------------------------" << endl;
GetStreamLogDebug() << "             AtomEngine" << endl;
GetStreamLogDebug() << "          Version:'" << ATOM_ENGINE_VERSION << "'" << endl;
GetStreamLogDebug() << "                              by jskny" << endl;
GetStreamLogDebug() << "---------------------------------------" << endl;
GetStreamLogDebug() << endl;


	g_timeInitialize = clock();
	g_flagAtomEngineInitialize = true;
	return (true);
}


/// AtomEngine を終了する。
bool Finalize(void) throw (aen::Exception)
{
	if (!IsInitialize()) {
AEN_THROW_EXCEPTION("AtomEngine is not initialized.");
		return (false);
	}


	clock_t start = g_flagAtomEngineInitialize;
	clock_t end = clock();


	// ログストリームの終了。
GetStreamLogDebug() << endl;
GetStreamLogDebug() << "---------------------------------------" << endl;
GetStreamLogDebug() << " AtomEngine Quit. (" << (end - start) << ":" << (end - start) / CLOCKS_PER_SEC << ")" << endl;
GetStreamLogDebug() << endl;
	CloseStreamLog();
	g_flagAtomEngineInitialize = false;


#ifdef	ATOM_ENGINE_DEBUG
	// メモリーリークの検出
	_CrtDumpMemoryLeaks();
#endif
	return (true);
}


/// ログストリームを初期化。
void OpenStreamLog(void)
{
	g_outputLogLevel = 0;
	g_outputStreamError.open("AtomEngine_Error.log", ios::out);
	g_outputStreamError.seekp(0, ios::beg); // 先頭に戻す。

	// デバッグ用。
//#ifdef ATOM_ENGINE_DEBUG
	g_outputStreamDebug.open("AtomEngine_Debug.log", ios::out);
	g_outputStreamDebug.seekp(0, ios::beg); // 先頭に戻す。
//#else
//	g_outputStreamDebug.close();
//#endif


	// ダミーストリーム。
	g_damyStreamDebug.close();


	return;
}


/// ログストリーム終了。
void CloseStreamLog(void)
{
	// ハンドルを閉じる。
	g_outputStreamDebug.flush();
	g_outputStreamDebug.close();

	g_outputStreamError.flush();
	g_outputStreamError.close();


	g_damyStreamDebug.flush();
	g_damyStreamDebug.close();


	g_outputLogLevel = 0;
	return;
}


// 出力するログの優先度をセット。
// 値が大きくなればなるほど、
// 重要な情報のみが出力される。
void SetStreamLogLevel(int level)
{
	assert(level >= 0 && "parameter 'level' is unjust.");
	g_outputLogLevel = level;
}


/// debug ストリームを取得。
std::ostream& GetStreamLogDebug(int level, const char* const fText)
{
	if (fText) {
		g_outputStreamDebug << fText << " >> ";
	}


	if (level < g_outputLogLevel) {
		// 出力する価値なき情報。
		g_damyStreamDebug.clear();
		return (g_damyStreamDebug);
	}


	return (g_outputStreamDebug);
}


/// error ストリームを取得。
std::ostream& GetStreamLogError(const char* const fText)
{
	if (fText) {
		g_outputStreamError << fText << " >> ";
	}


	return (g_outputStreamError);
}


/// AtomEngine の初期化。
bool Logic::initialize(void) throw (aen::Exception)
{
	bool rc = ::aen::Initialize();


	if (!rc) { return (false); }
	rc = this->m_window.initialize();
	return (rc);
}


/// AtomEngine の終了。
bool Logic::finalize(void) throw (aen::Exception)
{
	bool rc = ::aen::Finalize();


	if (!rc) { return (false); }
	rc = this->m_window.finalize();
	return (rc);
}


/*! 最終手段。
 * @brief		AtomEngine を強制終了させる。
 * @return		window オブジェクトへのポインタ
 */
void Logic::crash(const char* const expString) throw (aen::Exception)
{
	// ウインドウを閉じる。
	if (this->getWindow()->isOpen()) {
		this->getWindow()->close();
	}


	this->getWindow()->finalize();


ATOM_ENGINE_DEBUG_BREAK;


	AEN_STREAM_LOG_ERROR() << "System has crashed." << endl;
	this->finalize();


AEN_THROW_EXCEPTION("System has crashed.");
	return;
}


/// 警告の出力
void Logic::warn(const std::string& str, const char* const func, const char* const file, int line) const
{
	cout << str << endl;
}


//-------------------------------------
// 簡易当たり判定
//-------------------------------------


bool IsHitBox(
	const CollisionBox& a,
	const CollisionBox& b)
{
	bool test1 = (a.x < (b.x + b.xSize));
	bool test2 = (a.y < (b.y + b.ySize));
	bool test3 = (b.x < (a.x + a.xSize));
	bool test4 = (b.y < (a.y + a.ySize));


	return (test1 && test2 && test3 && test4);
}


// 当たり判定領域出力
void CollisionBox::draw(void) const
{
	DrawBox(this->x, this->y, this->x + this->xSize, this->y + this->ySize, this->dbColor, FALSE);
}


// 3D 当たり判定。
bool IsHitBox(const CollisionBox3d& a, const CollisionBox3d& b)
{
	bool ret = false;

	// Z 座標当たり判定。
	// num*0.5 は、 float だから ( num/2 と同じ意味 )
	float az1 = static_cast<float>(a.z - ((a.iz - a.bz)*0.5));
	float az2 = static_cast<float>(a.z + ((a.iz - a.bz)*0.5));
	float bz1 = static_cast<float>(b.z - ((b.iz - b.bz)*0.5));
	float bz2 = static_cast<float>(b.z + ((b.iz - b.bz)*0.5));


	if (az1 < bz2 && bz1 < az2) {
		ret = true;
//		a.serialize(Again::debugStream());
//		b.serialize(Again::debugStream());
	}


	// Z であたってなければあたってないでしょ。
	if (!ret) {
		return (false);
	}


	ret = false;
	float ax1 = static_cast<float>(a.x - ((a.ix - a.bx)*0.5));
	float ax2 = static_cast<float>(a.x + ((a.ix - a.bx)*0.5));
	float ay1 = static_cast<float>(a.y - ((a.iy - a.by)*0.5));
	float ay2 = static_cast<float>(a.y + ((a.iy - a.by)*0.5));

	float bx1 = static_cast<float>(b.x - ((b.ix - b.bx)*0.5));
	float bx2 = static_cast<float>(b.x + ((b.ix - b.bx)*0.5));
	float by1 = static_cast<float>(b.y - ((b.iy - b.by)*0.5));
	float by2 = static_cast<float>(b.y + ((b.iy - b.by)*0.5));


//cout << a.x << "," << a.y << "," << a.z << ":" << b.x << "," << b.y << "," << b.z << endl;
//cout << ax1 << "," << ax2 << ":" << ay1 << "," << ay2 << ":";
//cout << bx1 << "," << bx2 << ":" << by1 << "," << by2 << endl;
	// 当たった時の処理
	if((ax1 < bx2) &&
	   (bx1 < ax2) &&
	   (ay1 < by2) &&
	   (by1 < ay2))
	{
//a.serialize(Again::debugStream());
//b.serialize(Again::debugStream());
		ret = true;
	}


	return (ret);
}


void CollisionBox3d::draw(int colorI, int colorB) const
{
	float ax1 = 0.0f;
	float ax2 = 0.0f;
	float ay1 = 0.0f;
	float ay2 = 0.0f;
	float az1 = 0.0f;
	float az2 = 0.0f;


//	int colorI = GetColor(0xFF, 0xFF, 0x00);
//	int colorB = GetColor(0xFF, 0x00, 0x00);


	// イメージボックス。
// POINT:
// 2011 / 09 / 19
// jskny
	// モデル出力時に、
	// 中心座標を、 2D　と同じように、左上に変更しているのでこれで動くが、
	// 移植した時とかは、まあ、
	// this->x - (this->ix/2)
	// this->x + (this->ix/2)
	// とか、書かなきゃダメでしょうね。
	ax1 = static_cast<float>(this->x - (this->ix*0.5f));
	ax2 = static_cast<float>(this->x + (this->ix*0.5f));
	ay1 = static_cast<float>(this->y - (this->iy*0.5f));
	ay2 = static_cast<float>(this->y + (this->iy*0.5f));
	az1 = static_cast<float>(this->z - (this->iz*0.5f));
	az2 = static_cast<float>(this->z + (this->iz*0.5f));


//cout << ax1 << "," << ax2 << endl;

	// 前
	// top
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax2, ay1, az1), colorI);
	// bottom
	DrawLine3D(VGet(ax1, ay2, az1), VGet(ax2, ay2, az1), colorI);
	// left
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax1, ay2, az1), colorI);
	// right
	DrawLine3D(VGet(ax2, ay1, az1), VGet(ax2, ay2, az1), colorI);


	// 奥
	// top
	DrawLine3D(VGet(ax1, ay1, az2), VGet(ax2, ay1, az2), colorI);
	// bottom
	DrawLine3D(VGet(ax1, ay2, az2), VGet(ax2, ay2, az2), colorI);
	// left
	DrawLine3D(VGet(ax1, ay1, az2), VGet(ax1, ay2, az2), colorI);
	// right
	DrawLine3D(VGet(ax2, ay1, az2), VGet(ax2, ay2, az2), colorI);


	// top (L)
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax1, ay1, az2), colorI);
	// bottom (L)
	DrawLine3D(VGet(ax1, ay2, az1), VGet(ax1, ay2, az2), colorI);
	// top (R)
	DrawLine3D(VGet(ax2, ay1, az1), VGet(ax2, ay1, az2), colorI);
	// bottom (R)
	DrawLine3D(VGet(ax2, ay2, az1), VGet(ax2, ay2, az2), colorI);


	// コリジョンボックス
	ax1 = static_cast<float>(this->x - ((this->ix - this->bx)*0.5));
	ax2 = static_cast<float>(this->x + ((this->ix - this->bx)*0.5));
	ay1 = static_cast<float>(this->y - ((this->iy - this->by)*0.5));
	ay2 = static_cast<float>(this->y + ((this->iy - this->by)*0.5));
	az1 = static_cast<float>(this->z - ((this->iz - this->bz)*0.5));
	az2 = static_cast<float>(this->z + ((this->iz - this->bz)*0.5));
	// 前
	// top
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax2, ay1, az1), colorB);
	// bottom
	DrawLine3D(VGet(ax1, ay2, az1), VGet(ax2, ay2, az1), colorB);
	// left
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax1, ay2, az1), colorB);
	// right
	DrawLine3D(VGet(ax2, ay1, az1), VGet(ax2, ay2, az1), colorB);


	// 奥
	// top
	DrawLine3D(VGet(ax1, ay1, az2), VGet(ax2, ay1, az2), colorB);
	// bottom
	DrawLine3D(VGet(ax1, ay2, az2), VGet(ax2, ay2, az2), colorB);
	// left
	DrawLine3D(VGet(ax1, ay1, az2), VGet(ax1, ay2, az2), colorB);
	// right
	DrawLine3D(VGet(ax2, ay1, az2), VGet(ax2, ay2, az2), colorB);


	// top
	DrawLine3D(VGet(ax1, ay1, az1), VGet(ax1, ay1, az2), colorB);
	// bottom
	DrawLine3D(VGet(ax1, ay2, az1), VGet(ax1, ay2, az2), colorB);
	// top
	DrawLine3D(VGet(ax2, ay1, az1), VGet(ax2, ay1, az2), colorB);
	// bottom
	DrawLine3D(VGet(ax2, ay2, az1), VGet(ax2, ay2, az2), colorB);
}


// Matrix 2 Vector 平行移動行列を取り出す。
VECTOR MatrixToPos(const MATRIX& m)
{
	MATRIX tmp = MGetIdent();
	VECTOR pos;


	pos.x = tmp.m[3][0];
	pos.y = tmp.m[3][1];
	pos.z = tmp.m[3][2];
	return (pos);
}


} // namespace aen

