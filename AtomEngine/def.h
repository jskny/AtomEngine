/*!
 *	AtomEngine
 *	@brief		設定等。
 *	@date		2012 / 03 / 09
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_DEF_H_INCLUDE_GUARD_7207302_362198_42_38_2_427810__
#define __ATOM_ENGINE_DEF_H_INCLUDE_GUARD_7207302_362198_42_38_2_427810__


#include	<stdexcept> // 標準例外
#include	<string>
#include	<cstdio>
#include	<DxLib.h>


#include	<boost/shared_ptr.hpp>
#include	<boost/noncopyable.hpp>


#define		ATOM_ENGINE_VERSION	("0.00.05a")


// スクリプト言語のラッパーを使用したい時は、コメントアウト
// #define		AEN_USE_SCRIPTER_JAVASCRIPT
// LuaPlus を使いたい場合 コメントアウト
// #define		AEN_USE_LUAPLUS


#ifdef _DEBUG
	#define		ATOM_ENGINE_DEBUG
	/// debug ブレークを発生させる。
	#define		ATOM_ENGINE_DEBUG_BREAK	do { __asm { int 3 }; } while (0)
#else
	#define		ATOM_ENGINE_DEBUG_BREAK
#endif


/// 出力用関数。
#define		AEN_OUTPUT_PRINTF	std::printf


#define		AEN_PRINTF_ERROR(text)		AEN_OUTPUT_PRINTF("%s\nFILE : %s\nLINE : %d\n", (text), __FILE__, __LINE__)
#define		AEN_PRINTF_DEBUG(text)		AEN_OUTPUT_PRINTF("%s\nFILE : %s\nLINE : %d\n", (text), __FILE__, __LINE__)


#define		AEN_SAFE_DELETE(p)		do { if (p) { delete	(p); (p) = 0; } else { AEN_PRINTF_ERROR("no new pointer delete. (this)"); } } while (0)
#define		AEN_SAFE_DELETE_ARRAY(p)	do { if (p) { delete[]	(p); (p) = 0; } else { AEN_PRINTF_ERROR("no new pointer delete. (array)"); } } while (0)


#define		AEN_ASSERT_KILL(exp, text)	do {	if ((exp)) {\
						}\
						else {\
						AEN_OUTPUT_PRINTF("%s\n", (text));\
						AEN_OUTPUT_PRINTF("%s\n", #exp);\
						AEN_PRINTF_ERROR("assert point.");\
						ATOM_ENGINE_DEBUG_BREAK;\
						/*std::getchar();*/\
						/*std::exit(1);*/\
						}\
					} while (0)


// #define		AEN_THROW_EXCEPTION(type, text)	do { throw (type((text), __FUNCTION__, __FILE__, __LINE__)); } while (0)
/// 例外を throw する。
#define		AEN_THROW_EXCEPTION(text)	do { throw (::aen::Exception((text), __FUNCTION__, __FILE__, __LINE__)); } while (0)
#define		AEN_NULL_TO_THROW_EXCEPTION(var)	do { if ((var)) {} else { AEN_THROW_EXCEPTION("variable '" #var "' is null pointer."); } } while (0)
#define		AEN_NULL_TO_THROW_EXCEPTION_PARAM(var)	do { if ((var)) {} else { AEN_THROW_EXCEPTION("parameter '" #var "' is null pointer."); } } while (0)


/// エラーログストリームの取得
#define		AEN_STREAM_LOG_ERROR()		::aen::GetStreamLogError(__FUNCTION__)
/// デバッグログストリームの取得
#define		AEN_STREAM_LOG_DEBUG(level)	::aen::GetStreamLogDebug(level, __FUNCTION__)

#define		AEN_ERROR			AEN_STREAM_LOG_ERROR
#define		AEN_DEBUG(level)		AEN_STREAM_LOG_DEBUG(level)


/// ロジックを使用しての警告
#define		AEN_WARN(L, text)		L->warn(text, __FUNCTION__, __FILE__, __LINE__)


// まとめてエラー出力
#define		AEN_O_EW(level, text)		do {\
							AEN_STREAM_LOG_DEBUG(level) << text << std::endl;\
							AEN_STREAM_LOG_ERROR() << text << std::endl;\
							AEN_PRINTF_ERROR(text);\
						} while (0)


#ifndef PHI_F
	// DxLib 3.08e 以降のマクロ名変更に対応。
	#define		PHI_F		DX_PI_F
	#define		TWO_PHI_F	DX_TWO_PI_F


	#define		PHI		DX_PI
	#define		TWO_PHI		DX_TWO_PI
#endif


namespace aen
{


/// ログレベル
enum {
	/// 低い情報
	DEBUG_LOG_LEVEL_LOW = 0,
	/// スクリプター関連
	DEBUG_LOG_LEVEL_SCRIPTER = 10,
	/// ハンドルに関連するログ
	DEBUG_LOG_LEVEL_HANDLE = 10,
	/// ゲームシステムとか
	DEBUG_LOG_LEVEL_GAMESYSTEM = 10,
	/// ある程度の情報
	DEBUG_LOG_LEVEL_HIGH = 100
};


/// 例外クラス。
class Exception;


/// 例外クラス。
class Exception :public std::runtime_error
{
public:
	/*! 例外クラスのコンストラクタ
	 * @brief 例外の中身をセットする。
	 * @param text エラーメッセージ
	 * @param funcName 例外が発生した関数名
	 * @param file 例外が発生したファイル名
	 * @param line 例外が発生したファイルの行数
	 * @return なし
	 */
	Exception(const std::string text, const std::string funcName = "unknown", const std::string file = "unknown", const long int line = -1) :
			runtime_error("AtomEngine Exception: " + text + ":" + funcName + ":" + file + ":" + std::string()),
			m_text(text), m_file(file),
			m_funcName(funcName), m_line(line)
	{
ATOM_ENGINE_DEBUG_BREAK;
	}


virtual	~Exception() {};


	/// serialize を呼ぶ。
inline	void		output(std::ostream& oss) { this->serialize(oss); };
	/// std::ostream を継承しているオブジェクトに例外の中身を出力。
virtual	void		serialize(std::ostream& oss) {
		oss << "text    :" << this->m_text << std::endl;
		oss << "funcName:" << this->m_funcName << std::endl;
		oss << "file    :" << this->m_file << std::endl;
		oss << "line    :" << this->m_line << std::endl;
	}	


public:
	std::string	m_text;
	std::string	m_file;
	std::string	m_funcName;
	long int	m_line;


};


/*! デバッグログで出力するレベルをセット。
 * @brief		値が大きくなればなるほど、重要な情報のみが出力される。
 * @return		なし
 */
extern	void		SetStreamLogLevel(int level);
/*! 非推奨関数
 * @brief		デバッグログストリームを返す。
 * @return		ログストリーム
 * @warning		AEN_STREAM_LOG_DEBUG マクロを使え。
 */
extern	std::ostream&	GetStreamLogDebug(int level = (int)DEBUG_LOG_LEVEL_LOW, const char* const fText = 0);
/*! 非推奨関数
 * @brief		エラーログストリームを返す。
 * @return		ログストリーム
 * @warning		AEN_STREAM_LOG_ERROR マクロを使え。
 */
extern	std::ostream&	GetStreamLogError(const char* const fText = 0);


/*! マトリックスの中身をシリアライズ
 * @brief		Matrixの中身を出力する。
 * @param oss		std::ostream の派生クラスを指定。
 * @param m		マトリックス。
 * @return		なし
 */
 extern	void		SerializeMatrix(std::ostream& oss, const DxLib::MATRIX& m);
 /*! ベクター中身をシリアライズ
 * @brief		DxLib::VECTOR の中身を出力する。
 * @param oss		std::ostream の派生クラスを指定。
 * @param v		DxLib::VECTOR
 * @return		なし
 */
 extern	void		SerializeVector(std::ostream& oss, const DxLib::VECTOR& v);


 // Matrix 2 Vector 平行移動行列を取り出す。
extern	VECTOR		MatrixToPos(const MATRIX& m);


//-------------------------------------
// 簡易当たり判定
//-------------------------------------


class CollisionBox;
class CollisionBox3d;
/*! 簡易当たり判定 2D
 * @brief		オブジェクト同士が当たっていれば true を返す。
 * @return		オブジェクト同士が当たっていれば true を返す。
 */
extern	bool		IsHitBox(
	const CollisionBox& a,
	const CollisionBox& b);
/*! 簡易当たり判定 3D
 * @brief		オブジェクト同士が当たっていれば true を返す。
 * @return		オブジェクト同士が当たっていれば true を返す。
 */
extern	bool		IsHitBox(
	const CollisionBox3d& a,
	const CollisionBox3d& b);


/// 2D 空間の当たり判定。
class CollisionBox
{
public:
	int	x, y;
	int	xSize, ySize;
	// デバッグ出力色
	int	dbColor;


	CollisionBox(int x = 0, int y = 0, int xSize = 0, int ySize = 0) :
		x(x), y(y),
		xSize(xSize), ySize(ySize),
		dbColor(0xFFFFFF)
	{
	}


inline	bool		isHit(const CollisionBox& a) { return (IsHitBox(a, *this)); };


	// 当たり判定出力
	void		draw(void) const;


};


/// 3D 空間の当たり判定。
class CollisionBox3d
{
public:
	float		x, y, z;
	float		ix, iy, iz;


	// 当たり判定領域。
	float		bx, by, bz;


	void		reset(void)
	{
		this->x = 0.0f, this->y = 0.0f, this->z = 0.0f;
		this->ix = 0.0f, this->iy = 0.0f, this->iz = 0.0f;
		this->bx = 0.0f, this->by = 0.0f, this->bz = 0.0f;
	}


	// 当たり判定領域を出力
	void		draw(int colorI = GetColor(0xFF, 0xFF, 0x00), int colorB = GetColor(0xFF, 0x00, 0x00)) const;
inline	bool		isHit(const CollisionBox3d& a) { return (IsHitBox(a, *this)); };


DxLib::VECTOR		getVec(void) const
	{
		return (VGet(this->x, this->y, this->z));
	}
inline	DxLib::VECTOR	getPos(void) const { return (this->getVec()); };


	void		setVec(const VECTOR& a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
	}
	void		setPos(float x, float y, float z)
	{
		this->setVec(VGet(x, y, z));
	}
	void		setPos(const VECTOR& a)
	{
		this->setVec(a);
	}


	CollisionBox3d() :
		x(0.0f), y(0.0f), z(0.0f),
		ix(0.0f), iy(0.0f), iz(0.0f),
		bx(0.0f), by(0.0f), bz(0.0f)
	{
	}


};



} // namespace aen


#endif // __ATOM_ENGINE_DEF_H_INCLUDE_GUARD_7207302_362198_42_38_2_427810__

