/*!
 *	AtomEngine
 *	Input
 *	@brief		入力関連。
 *	@date		2012 / 04 / 03
 *	@author		jskny
*/


#include	<iostream>
#include	<vector>
#include	"def.h"
#include	"aen_gl_input.h"


using namespace std;


namespace aen
{
namespace gl
{
namespace input
{


// ジョイスティック関連の初期化
extern	void		InitJoystick(void);
// ジョイスティックの更新。
extern	void		UpdateJoystick(void);


// マウスデータ
class MouseData
{
public:
	int		keyCode;
	int		x, y;
	int		wheel;


	MouseData() :
		keyCode(0),
		x(0), y(0),
		wheel(0)
	{
	}


};


static	MouseData	g_mouseData;
static	MouseData	g_mouseDataOld;


// キーボード
static	char		g_keyMapOld[256] = { 0 };
static	char		g_keyMapNow[256] = { 0 };


// マウス情報のアップデート
void UpdateInputMouse(void)
{
	int x = 0, y = 0;
	int buf = 0;


	g_mouseDataOld = g_mouseData;


	// 座標を取得。
	GetMousePoint(&x, &y);
	g_mouseData.x = x;
	g_mouseData.y = y;


	g_mouseData.keyCode = GetMouseInput();
	g_mouseData.wheel = GetMouseWheelRotVol();


	return;
}


// キーボード情報を更新
void UpdateInputKeyboard(void)
{
	// 古いのを保存。
	for (int i = 0; i < 256; i++) {
		g_keyMapOld[i] = g_keyMapNow[i];
	}


	// 更新。
	GetHitKeyStateAll(g_keyMapNow);
}


/// 入力機器の状態を更新する。 ( AtomEngine が内部で呼ぶ )
void UpdateInputDevice(void)
{
	if (!DxLib_IsInit()) {
AEN_STREAM_LOG_ERROR() << "DxLib は未初期化です。" << endl;
		return;
	}


	UpdateInputMouse();
	UpdateInputKeyboard();
	UpdateJoystick();
}


// キーが押されているか、否か、
bool Mouse::isInput(KeyCode code)
{
	int c = g_mouseData.keyCode;
	bool retVal = false;

	if (code == Mouse::KEY_L) {
		retVal = ((c & MOUSE_INPUT_LEFT) != 0);
	}
	else if (code == Mouse::KEY_R) {
		retVal = ((c & MOUSE_INPUT_RIGHT) != 0);
	}
	else if (code == Mouse::KEY_M) {
		retVal = ((c & MOUSE_INPUT_MIDDLE) != 0);
	}
	else {
		AEN_ASSERT_KILL(0, "Key code is unjust.");
	}


	return (retVal);
}


// 現在のフレームで初めて押されていれば、
// true
bool Mouse::isNowInput(KeyCode code)
{
	int key = 0;
	if (code == Mouse::KEY_L) {
		key = MOUSE_INPUT_LEFT;
	}
	else if (code == Mouse::KEY_R) {
		key = MOUSE_INPUT_RIGHT;
	}
	else if (code == Mouse::KEY_M) {
		key = MOUSE_INPUT_MIDDLE;
	}
	else {
		AEN_ASSERT_KILL(0, "Key code is unjust.");
	}


	if (((g_mouseData.keyCode ^ g_mouseDataOld.keyCode) ^ key) != 0) {
		return (false);
	}


	return (this->isInput(code));
}


int Mouse::x(void) const
{
	return (g_mouseData.x);
}


int Mouse::y(void) const
{
	return (g_mouseData.y);
}


int Mouse::wheel(void) const
{
	return (g_mouseData.wheel);
}


int Mouse::vx(void) const
{
	return (abs(g_mouseData.x - g_mouseDataOld.x));
}


int Mouse::vy(void) const
{
	return (abs(g_mouseData.y - g_mouseDataOld.y));
}


// マウス座標を設定。
bool Mouse::set(int x, int y)
{
	bool retval = (SetMousePoint(x, y) != -1);
	return (true);
}


/*! キーが押されていれば true を返す。
 * @brief		キーが押されていれば true を返す。
 * @param code		キー
 * @return		押されていれば true, 押されていなければ false
 */
bool Keyboard::isInput(int code)
{
	if (code < 0 || code >= 256) {
AEN_PRINTF_ERROR("aen::gl::input::Keyboard::isInput -> parameter 'code' is unjust.");
throw std::out_of_range("aen::gl::input::Keyboard::isInput -> buffer over flow.");
		return (false);
	}


	bool ret = g_keyMapNow[code] == 1 ? true : false;
	return (ret);
}


/*! 現在のフレームで初めて押されていれば true を返す。
 * @brief		現在のフレームで初めて押されていれば true を返す。
 * @param code		キー
 * @return		押されていれば true, 押されていなければ false
 */
bool Keyboard::isNowInput(int code)
{
	if (code < 0 || code >= 256) {
AEN_PRINTF_ERROR("aen::gl::input::Keyboard::isNowInput -> parameter 'code' is unjust.");
throw std::out_of_range("aen::gl::input::Keyboard::isNowInput -> buffer over flow.");
		return (false);
	}


	if (g_keyMapNow[code] == g_keyMapOld[code]) {
		return (false);
	}


	return (Keyboard::isInput(code));
}


/*! 現在のキーマップを返す。
 * @brief		現在のキーマップを返す。
 * @param dest		データが入れられる配列
 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
 * @return		なし
 */
void Keyboard::getKey(char* dest)
{
AEN_NULL_TO_THROW_EXCEPTION_PARAM(dest);


	for (int i = 0; i < 0xFF; ++i) {
		dest[i] = g_keyMapNow[i];
	}
}


/*! 一フレーム前のキーマップを返す。
 * @brief		一フレーム前のキーマップを返す。
 * @param dest		データが入れられる配列
 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
 * @return		なし
 */
void Keyboard::getKeyOld(char* dest)
{
AEN_NULL_TO_THROW_EXCEPTION_PARAM(dest);


	for (int i = 0; i < 0xFF; ++i) {
		dest[i] = g_keyMapOld[i];
	}
}


/*! 現在のフレームで初めて押されたキーマップを返す。
 * @brief		現在のフレームで初めて押されたキーマップを返す。
 * @param dest		データが入れられる配列
 * @warning		dest のサイズは sizeof(char) * 0xFF 以上です。
 * @return		なし
 */
void Keyboard::getKeyNowInput(char* dest)
{
AEN_NULL_TO_THROW_EXCEPTION_PARAM(dest);


	for (int i = 0; i < 0xFF; ++i) {
		if (g_keyMapNow[i] == g_keyMapOld[i]) {
			dest[i] = 0;
			continue;
		}


		dest[i] = 1;
	}
}


//---------------------------------------------------------
// ジョイスティック
//---------------------------------------------------------


static	int		g_keyInput[Joystick::MAX] = { 0 }, g_keyInputOld[Joystick::MAX] = { 0 };
static	int		g_joypadAnalogX[Joystick::MAX] = { 0 }, g_joypadAnalogY[Joystick::MAX] = { 0 };
static	int		g_joypadAnalogX_R[Joystick::MAX] = { 0 }, g_joypadAnalogY_R[Joystick::MAX] = { 0 };


// PSL の狂気の技・・・確かに便利ですっ！
#define		K(n, v)	unsigned int Joystick::n = v
	K(UP, PAD_INPUT_UP); K(DOWN, PAD_INPUT_DOWN); K(LEFT, PAD_INPUT_LEFT); K(RIGHT, PAD_INPUT_RIGHT);
	K(A, PAD_INPUT_1); K(B, PAD_INPUT_2); K(X, PAD_INPUT_3); K(Y, PAD_INPUT_4);
	K(L, PAD_INPUT_5); K(R, PAD_INPUT_6); K(L2, PAD_INPUT_7); K(R2, PAD_INPUT_8); K(L3, PAD_INPUT_11); K(R3, PAD_INPUT_12);
	K(START, PAD_INPUT_10); K(SELECT, PAD_INPUT_11);
#undef K


// ジョイスティック関連の初期化
void InitJoystick(void)
{
	for (int i = 0; i < Joystick::MAX; ++i) {
		g_keyInputOld[i] = g_keyInput[i] = 0;
		g_keyInput[i] = 0;


		g_joypadAnalogX[i] = 0, g_joypadAnalogY[i] = 0;
		g_joypadAnalogX_R[i] = 0, g_joypadAnalogY_R[i] = 0;
	}


}


// ジョイスティックの更新。
void UpdateJoystick(void)
{
	for (int i = 0; i < Joystick::MAX; ++i) {
		int k = DX_INPUT_PAD1 + i;


		g_keyInputOld[i] = g_keyInput[i];
		g_keyInput[i] = GetJoypadInputState(k);


		GetJoypadAnalogInput(&g_joypadAnalogX[i], &g_joypadAnalogY[i], k);
		GetJoypadAnalogInputRight(&g_joypadAnalogX_R[i], &g_joypadAnalogY_R[i], k);
	}


}


// PAD1 のキー入力を取得
int GetKeyInput(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_keyInput[i]);
}


int GetKeyInputOld(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_keyInputOld[i]);
}


int GetKeyNowInput(int i)
{
	return (GetKeyInput(i) ^ GetKeyInputOld(i));
}


int GetKeyAnalogX(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_joypadAnalogX[i]);
}


int GetKeyAnalogY(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_joypadAnalogY[i]);
}


int GetKeyAnalogX_R(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_joypadAnalogX_R[i]);
}


int GetKeyAnalogY_R(int i)
{
AEN_ASSERT_KILL(i >= 0 && i < Joystick::MAX, "overflow.");
	return (g_joypadAnalogY_R[i]);
}


/*! キーが押されていれば true を返す。
 * @brief		キーが押されていれば true を返す。
 * @param p		パッドId
 * @param key		キーコード
 * @return		押されていれば true, 押されていなければ false
 */
bool Joystick::isInput(const Pad p, unsigned int const code)
{
	return ((GetKeyInput(p) & code) != 0);
}


/*! 現在のフレームで初めて押されていれば true を返す。
 * @brief		現在のフレームで初めて押されていれば true を返す。
 * @param p		パッドId
 * @param key		キーコード
 * @return		押されていれば true, 押されていなければ false
 */
bool Joystick::isNowInput(Pad const p, unsigned int const key)
{
	return ((GetKeyNowInput(p) & key) != 0);
}


/*! 指定したパッドの、左アナログスティック情報を返す。
 * @brief		指定したパッドの、左アナログスティック情報を返す。
 * @param p		パッドId
 * @return		-1000 ～ 1000 の範囲で値を返す。
 */
int Joystick::analogX(Pad const p)
{
	return (GetKeyAnalogX(static_cast<int>(p)));
}


int Joystick::analogY(Pad const p)
{
	return (GetKeyAnalogY(static_cast<int>(p)));
}


/*! 指定したパッドの、右アナログスティック情報を返す。
 * @brief		指定したパッドの、右アナログスティック情報を返す。
 * @param p		パッドId
 * @return		-1000 ～ 1000 の範囲で値を返す。
 */
int Joystick::analogX_R(Pad const p)
{
	return (GetKeyAnalogX_R(static_cast<int>(p)));
}


int Joystick::analogY_R(Pad const p)
{
	return (GetKeyAnalogY_R(static_cast<int>(p)));
}


/*! 指定したパッドを振動させる。
 * @brief		指定したパッドを振動させる。
 * @param power		強さ、0 ～ 1000
 * @param time		振動させる時間(ミリ秒) -1 で stop までずっと
 * @return		なし
 */
void Joystick::startVibration(Pad const p, int const power, int const time)
{
AEN_ASSERT_KILL(p >= 0 && p < Joystick::MAX, "overflow.");
	StartJoypadVibration(static_cast<int>(p) + DX_INPUT_PAD1, power, time);
}


void Joystick::stopVibration(Pad const p)
{
AEN_ASSERT_KILL(p >= 0 && p < Joystick::MAX, "overflow.");
	StopJoypadVibration(static_cast<int>(p) + DX_INPUT_PAD1);
}


/*! 接続されている、ジョイスティックの数を返す。
 * @brief		接続されている、ジョイスティックの数を返す。
 * @return		接続されているジョイスティックの数
 */
int Joystick::getNum(void)
{
	return (GetJoypadNum());
}


/// 初期化。
void Joystick::reset(void)
{
	InitJoystick();
}


static	unsigned int g_joystickDefaultKeyMap[16] = { 0 };
inline unsigned int GetJoystickKey(int i)
{
	if (g_joystickDefaultKeyMap[0] == 0) {
		// 値を代入。
	// 「##」はシンボル連結を表すマクロ記法
#define K(i) g_joystickDefaultKeyMap[i- 1] = PAD_INPUT_ ## i
	K(1); K(2); K(3); K(4);
	K(5); K(6); K(7); K(8);
	K(9); K(10); K(11); K(12);
	K(13); K(14); K(15); K(16);
#undef K
	}


	return (g_joystickDefaultKeyMap[i]);
}


/*! キーマップの書き換え。
 * @brief		キーマップの書き換え。
 * @param urlFile	設定ファイル。
 * @return		成功 true, 失敗 false
 */
bool Joystick::loadKeyConfig(const char* const urlFile)
{
	if (!urlFile) {
AEN_NULL_TO_THROW_EXCEPTION_PARAM(urlFile);
	}


	FILE* fp;
	fp = fopen(urlFile, "r");
	if (fp == NULL) {
		AEN_PRINTF_ERROR("Can not load JoystickConfigFile.");
AEN_STREAM_LOG_ERROR() << "Can not load JoystickConfigFile. : " << urlFile << endl;
		return (false);
	}


	int r = 0;
	char buf[300];
	int v = 0;
	int line = 0;
	while (true) {
		++line;
		r = fscanf(fp, "%s%d", buf, &v);
cout << "READ LINE : " << line << " into " << buf << "," << v << endl;
		if (r == EOF) {
			break;
		}
		else if (v < 0 || v > 15) {
			AEN_PRINTF_ERROR("JoystickConfigFile is unjust.");
AEN_STREAM_LOG_ERROR() << "Can not load JoystickConfigFile. : " << urlFile << " at " << line << endl;
			continue;
		}


		int t = 0;
		if (v == 0) {
			// 使わない。
		}
		else {
			t = GetJoystickKey(v-1);
		}


		// さぁやるか。
#define K(T) if (strcmp(buf, #T) == 0) { cout << "UDATE KEY * #T by" << v << endl; Joystick::T = t; }
		K(A); K(B); K(X); K(Y);
		K(L); K(R); K(L2); K(R2); K(L3); K(R3);
		K(START); K(SELECT);
#undef K
	}


	fclose(fp);
	fp = NULL;
	return (true);
}


} } } // namespace aen::gl::input


