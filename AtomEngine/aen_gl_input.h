/*!
 *	AtomEngine
 *	Input
 *	@brief		入力関連。
 *	@date		2012 / 04 / 03
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_INPUT_H_INCLUDE_GUARD_65892143_1092761_122215492073926730911__
#define __ATOM_ENGINE_AEN_GL_INPUT_H_INCLUDE_GUARD_65892143_1092761_122215492073926730911__


#include	<boost/noncopyable.hpp>


namespace aen
{
namespace gl
{
/// 入力系
namespace input
{


/// マウス
class Mouse;
/// キーボード
class Keyboard;


/// 入力機器の状態を更新する。 ( AtomEngine が内部で呼ぶ )
extern	void		UpdateInputDevice(void);


/// マウス
class Mouse
{
public:
	enum KeyCode {
		/// 左
		KEY_L = 0,
		/// 右
		KEY_R = 1,
		/// 真ん中
		KEY_M = 2
	};


	/*! キーが押されていれば true を返す。
	 * @brief		キーが押されていれば true を返す。
	 * @param code		キー
	 * @return		押されていれば true, 押されていなければ false
	 */
	bool		isInput(KeyCode code);
	bool		isOn(KeyCode code) { return (this->isInput(code)); };


	/*! 現在のフレームで初めて押されていれば true を返す。
	 * @brief		現在のフレームで初めて押されていれば true を返す。
	 * @param code		キー
	 * @return		押されていれば true, 押されていなければ false
	 */
	bool		isNowInput(KeyCode code);
	bool		isNowOn(KeyCode code) { return (this->isNowInput(code)); };


	int		x(void) const;
	int		y(void) const;
	int		wheel(void) const;


	int		vx(void) const;
	int		vy(void) const;


	/// マウス座標を設定。
	bool		set(int x, int y);


};


/// キーボード
class Keyboard
{
public:
	/*! キーが押されていれば true を返す。
	 * @brief		キーが押されていれば true を返す。
	 * @param code		キー
	 * @return		押されていれば true, 押されていなければ false
	 */
static	bool		isInput(int code);
static	bool		isOn(int code) { return (Keyboard::isInput(code)); };


	/*! 現在のフレームで初めて押されていれば true を返す。
	 * @brief		現在のフレームで初めて押されていれば true を返す。
	 * @param code		キー
	 * @return		押されていれば true, 押されていなければ false
	 */
static	bool		isNowInput(int key);
static	bool		isNowOn(int key) { return (Keyboard::isNowInput(key)); };


	/*! 現在のキーマップを返す。
	 * @brief		現在のキーマップを返す。
	 * @param dest		データが入れられる配列
	 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
	 * @return		なし
	 */
static	void		getKey(char* dest);
	/*! 一フレーム前のキーマップを返す。
	 * @brief		一フレーム前のキーマップを返す。
	 * @param dest		データが入れられる配列
	 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
	 * @return		なし
	 */
static	void		getKeyOld(char* dest);
	/*! 現在のフレームで初めて押されたキーマップを返す。
	 * @brief		現在のフレームで初めて押されたキーマップを返す。
	 * @param dest		データが入れられる配列
	 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
	 * @return		なし
	 */
static	void		getKeyNowInput(char* dest);


};


// ジョイスティック
class Joystick
{
public:
/*	enum Key {
		UP = PAD_INPUT_UP, DOWN = PAD_INPUT_DOWN,
		RIGHT = PAD_INPUT_RIGHT, LEFT = PAD_INPUT_LEFT,
		A = PAD_INPUT_A, B = PAD_INPUT_B, C = PAD_INPUT_C,
		X = PAD_INPUT_X, Y = PAD_INPUT_Y, Z = PAD_INPUT_Z,
		START = PAD_INPUT_START, M = PAD_INPUT_M,
		L = PAD_INPUT_L, R = PAD_INPUT_R,
		LL = PAD_INPUT_LL, RR = PAD_INPUT_RR
	};*/


#define		K(tmp) static unsigned int tmp
	K(UP); K(DOWN); K(LEFT); K(RIGHT);
	K(A); K(B); K(X); K(Y);
	K(L); K(R); K(L2); K(R2); K(L3); K(R3);
	K(START); K(SELECT);
#undef K


	enum Pad {
		PAD_1 = 0,
		PAD_2, PAD_3, PAD_4
	};


	enum Config {
		MAX = 8
	};


	/*! キーが押されていれば true を返す。
	 * @brief		キーが押されていれば true を返す。
	 * @param p		パッドId
	 * @param key		キーコード
	 * @return		押されていれば true, 押されていなければ false
	 */
static	bool		isInput(const Pad p, unsigned int const key);
static	bool		isOn(const Pad p, unsigned int const key) { return (Joystick::isInput(p, static_cast<int>(key))); };


	/*! 現在のフレームで初めて押されていれば true を返す。
	 * @brief		現在のフレームで初めて押されていれば true を返す。
	 * @param p		パッドId
	 * @param key		キーコード
	 * @return		押されていれば true, 押されていなければ false
	 */
static	bool		isNowInput(Pad const p, unsigned int const key);
static	bool		isNowOn(Pad const p, unsigned int const key) { return (Joystick::isNowInput(p, static_cast<int>(key))); };
static	bool		isInputNow(Pad const p, unsigned int const key) { return (Joystick::isNowInput(p, static_cast<int>(key))); };
static	bool		isOnNow(Pad const p, unsigned int const key) { return (Joystick::isNowInput(p, static_cast<int>(key))); };


	/*! 指定したパッドの、左アナログスティック情報を返す。
	 * @brief		指定したパッドの、左アナログスティック情報を返す。
	 * @param p		パッドId
	 * @return		-1000 ～ 1000 の範囲で値を返す。
	 */
static	int		analogX(Pad const p);
static	int		analogY(Pad const p);
	/*! 指定したパッドの、右アナログスティック情報を返す。
	 * @brief		指定したパッドの、右アナログスティック情報を返す。
	 * @param p		パッドId
	 * @return		-1000 ～ 1000 の範囲で値を返す。
	 */
static	int		analogX_R(Pad const p);
static	int		analogY_R(Pad const p);


	/*! 指定したパッドを振動させる。
	 * @brief		指定したパッドを振動させる。
	 * @param power		強さ、0 ～ 1000
	 * @param time		振動させる時間(ミリ秒) -1 で stop までずっと
	 * @return		なし
	 */
static	void		startVibration(Pad const p, int const power, int const time);
static	void		stopVibration(Pad const p);


	/*! 接続されている、ジョイスティックの数を返す。
	 * @brief		接続されている、ジョイスティックの数を返す。
	 * @return		接続されているジョイスティックの数
	 */
static	int		getNum(void);


	/// 初期化。
static	void		reset(void);



	/*! キーマップの書き換え。
	 * @brief		キーマップの書き換え。
	 * @param urlFile	設定ファイル。
	 * @return		成功 true, 失敗 false
	 */
static	bool		loadKeyConfig(const char* const urlFile);


};


} } } // namespace aen::gl::input


#endif // __ATOM_ENGINE_AEN_GL_INPUT_H_INCLUDE_GUARD_65892143_1092761_122215492073926730911__

