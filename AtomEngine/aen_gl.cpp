/*!
 *	AtomEngine
 *	gl
 *	@brief		DxLib の機能を使うやつ。
 *	@date		2012 / 03 / 10
 *	@author		jskny
*/


#include	<iostream>
#include	"aen.h"
#include	"aen_gl.h"
#include	"aen_gl_input.h"
#include	"def.h"


using namespace std;


namespace aen
{
namespace gl
{


/*! ポリゴンを描画 2d
 * @brief		ポリゴンを出力
 * @param x		出力中心 x 座標
 * @param y		出力中心 y 座標
 * @param num		頂点数
 * @param angle		回転角度
 * @param size		大きさ
 * @param color		線の色
 * @param skipNum	頂点から結ぶ線分を何個分スキップするか -> 五芒星を書きたい時は、draw(0, 0, 5, 0, size, GetColor(0xFF, 0x00, 0x00), 2, 1)
 * @param lw		線の太さ
 * @return		なし
 */
void DrawPolygon2d(int x, int y, int num, float angle, float size, int color, int skipNum, int lw)
{
	double one = aen::util::GetRad(360) / num;


	for (int i = 1; i < num + 1; i++) {
		DrawLine(
			x + (cos(one * (i - skipNum) + angle) * size),
			y + (sin(one * (i - skipNum) + angle) * size),
			x + (cos(one * i + angle) * size),
			y + (sin(one * i + angle) * size),
			color,
			lw
		);
	}


	return;
}


/// 四角形の当たり判定領域を作り、マウスと当たっていれば true を返す。
bool IsHitMouseBox(int x, int y, int bx, int by, bool flagDebugDraw)
{
	input::Mouse a;
	aen::CollisionBox mouse(a.x(), a.y(), 1, 1);
	aen::CollisionBox button(x, y, bx, by);


	if (flagDebugDraw) {
		DrawBox(
			a.x(), a.y(),
			a.x() + 1, a.y()+1, GetColor(0xFF, 0x00, 0x00), TRUE);
		DrawBox(x, y, x+bx, y+by, GetColor(0x00, 0xFF, 0x00), TRUE);
	}


	if (aen::IsHitBox(mouse, button)) {
		return (true);
	}


	return (true);
}


/// gl ネームスペース内のオブジェクトが使用するハンドルマネージャーを返す。
handle::Manager* GetHandleManager(void)
{
static	handle::Manager s_handleManager;
	return (&s_handleManager);
}


bool Window::initialize(void) throw (aen::Exception)
{
	this->m_flagInitialize = true;

	this->m_count = 0;
	this->m_frameTime = 0.0;
	this->m_windowW = 0;
	this->m_windowH = 0;
	this->m_flagBreakWindowLoop = false;
	this->m_fpsAverage= 0.0;
	this->m_fpsSaveTime = 0;


	this->setFps(60);
	return (true);
}


bool Window::finalize(void) throw (aen::Exception)
{
	this->close();
	return (true);
}


// ウインドウを作成。
// title	:	ウインドウのタイトル。
// w		:	ウインドウの X サイズ。
// h		:	ウインドウの Y サイズ。
// flagFull	:	true なら、フルスクリーン。
bool Window::open(const std::string& title, int w, int h, bool flagFull) throw (aen::Exception)
{
	// すでに、DxLibは起動されている。
	if (DxLib_IsInit()) {
		AEN_PRINTF_ERROR("DxLib has been already initialized.");
AEN_STREAM_LOG_ERROR() << "DxLib has been already initialized." << endl;
		AEN_THROW_EXCEPTION("DxLib has been already initialized.");
		return (false);
	}


	// window モードの設定 TRUE 通常、 FALSE フルスクリーン
	ChangeWindowMode(flagFull ? FALSE : TRUE);
	SetMainWindowText(title.c_str());
	if (SetGraphMode(w, h, 16) != DX_CHANGESCREEN_OK) {
		// ウインドウサイズの変更に失敗した
		AEN_PRINTF_ERROR("I failed in a change of the window size.");
AEN_STREAM_LOG_ERROR() << "I failed in a change of the window size." << endl;
		AEN_THROW_EXCEPTION("I failed in a change of the window size.");
		return (false);
	}


	// ウインドウアイコン
	SetWindowIconID(1012);


	// ウインドウサイズを変更可能にするか否か 変更可能 TRUE 不可能 FALSE
	SetWindowSizeChangeEnableFlag(FALSE);
	// プログラムの二重起動を許可するか否か 許可 TRUE 非許可 FALSE
	SetDoubleStartValidFlag(TRUE);


	// 非アクティブ時に処理を続けるか否か (続ける TRUE)
	SetAlwaysRunFlag(TRUE);
	// 垂直同期信号を待つか待たないか (待つ TRUE)
	SetWaitVSyncFlag(FALSE); // wait_fps の邪魔をするので無視する。
	// ログを表示するか否か 表示 TRUE 非表示 FALSE
	SetOutApplicationLogValidFlag(TRUE);
	// DxLib を再起動可能にする。
	SetDxLibEndPostQuitMessageFlag(FALSE);


	// DirectDrawをマルチスレッド化するか
	// TRUE マルチ
	SetMultiThreadFlag(TRUE);
	// ウインドウのスタイル
	// 0 通常
	// 1 タイトルバー無 枠あり
	// 2 タイトルバー無 枠なし
	SetWindowStyleMode(0);
	// 描画対象の指定
	SetDrawScreen(DX_SCREEN_BACK);
	// 光度を設定
	SetDrawBright(255, 255, 255);
	// 透過色を設定
	SetTransColor(0, 0, 0);


	// Z バッファの使用。
	SetUseZBuffer3D(TRUE);
	// Z バッファへの書き込みを有効化
	SetWriteZBuffer3D(TRUE);
	// カメラの視える範囲をセット。
	SetCameraNearFar(50, 10000);


	if (DxLib_Init() == -1) {
		AEN_PRINTF_ERROR("I failed in initialization of DxLib.");
AEN_STREAM_LOG_ERROR() << "I failed in initialization of DxLib." << endl;
		AEN_THROW_EXCEPTION("I failed in initialization of DxLib.");
		return (false);
	}


	this->m_windowW = w;
	this->m_windowH = h;
	return (true);
}


bool Window::close(void) throw (aen::Exception)
{
	if (DxLib_IsInit()) {
		DxLib_End();
	}


	return (true);
}


/*! ウインドウループを回す。
 * @brief		ウインドウループを回す。
 * @return		通常 true 終了メッセージが来た時 false
 */
bool Window::windowLoop(void)
{
	if (DxLib_IsInit() == FALSE) {
AEN_STREAM_LOG_ERROR() << "DxLib is non active." << endl;
cerr << "Window::windowLoop >> DxLib is non active." << endl;
		return (false);
	}



	if (	ProcessMessage() == 0 &&
		CheckHitKey(KEY_INPUT_ESCAPE) == 0 &&
		!this->isBreakLoop())
	{
		unsigned int tempTime = GetNowCount();
		this->m_fpsSaveTime = tempTime;


		// 入力機器の更新
		input::UpdateInputDevice();


		ClsDrawScreen();
		this->m_sceneManager.update();
		ScreenFlip();


		this->m_count++;
		this->m_frameTime = GetNowCount() - tempTime;


		// FPS 固定化
		this->__waitFps();
		this->m_fpsSaveTime = GetNowCount();
	}
	else {
		// シーンを破棄。
		this->m_sceneManager.deleteScene();
		return (false);
	}


	return (true);
}


bool Window::windowLoopNonClearNonFlip(bool useScene)
{
	if (DxLib_IsInit() == FALSE) {
AEN_STREAM_LOG_ERROR() << "DxLib is non active." << endl;
cerr << "Window::windowLoopNonClearNonFlip >> DxLib is non active." << endl;
		return (false);
	}


	if (	ProcessMessage() == 0 &&
		CheckHitKey(KEY_INPUT_ESCAPE) == 0 &&
		!this->isBreakLoop())
	{
		unsigned int tempTime = GetNowCount();
		this->m_fpsSaveTime = tempTime;


		// 入力機器の更新
		input::UpdateInputDevice();


//		ClsDrawScreen();
		if (useScene) {
			this->m_sceneManager.update();
		}
//		ScreenFlip();


		this->m_count++;
		this->m_frameTime = GetNowCount() - tempTime;


		// FPS 固定化
		this->__waitFps();
		this->m_fpsSaveTime = GetNowCount();
	}
	else {
		// シーンを破棄。
		this->m_sceneManager.deleteScene();
		return (false);
	}


	return (true);
}


// 固定フレーム用 FPSをあわせる。
void Window::__waitFps(void)
{
	if (DxLib_IsInit() == FALSE) {
AEN_STREAM_LOG_ERROR() << "DxLib is non active." << endl;
cerr << "Window::__waitFps >> DxLib is non active." << endl;
		return;
	}


	if (	// fps が 0 以下もしくは、 MAX より多けりゃ不正だろ。
		this->getSetFps() <= 0 ||
		this->getSetFps() > MAX_FRAMERATE) {
		AEN_PRINTF_ERROR("FPS に設定された目標値が異常です。");
AEN_STREAM_LOG_ERROR() << "FPS に設定された目標値が異常です。" << endl;
		return;
	}


	// ここをの変数に絶対 unsigned を付けてはいけない。
	// なぜかというと、計算の過程で負の値になるから・・・ほら見たことか、泣けてきた。
	static __int64 FrameRateCount = 0;
	static long double FrameRateCountNowTime = GetNowCount();
	static long double FrameRateCountEndTime = FrameRateCountNowTime;
	static __int64 FrameRateStopTime = 0;
	static __int64 FrameRateLoopTimeCounter = 0;

	if (FrameRateCount == 0) {
		if (FrameRateLoopTimeCounter == 0) {
			FrameRateStopTime = 0;
		} else {
			FrameRateStopTime = static_cast<__int64>(FrameRateCountEndTime + 1000 - GetNowCount());
		}
	}
	else {
		FrameRateStopTime = static_cast<__int64>(FrameRateCountEndTime + FrameRateCount * (1000.0 / this->getSetFps()) - GetNowCount()); 
	}

	//待つ
	if (FrameRateStopTime > 0) {
		Sleep(static_cast<DWORD>(FrameRateStopTime));
	}

	FrameRateCountNowTime = GetNowCount();

	if (FrameRateCount == 0) {
		FrameRateCountEndTime = FrameRateCountNowTime;
	}
	else if (FrameRateCount < 0) {
		AEN_THROW_EXCEPTION("buffer over run.");
	}


	this->m_frameRateSave[static_cast<int>(FrameRateCount)] = FrameRateCountNowTime - FrameRateLoopTimeCounter;
	// long int <- long double 何かあったら、指数オーバーを疑え。
	FrameRateLoopTimeCounter = static_cast<__int64>(FrameRateCountNowTime);

	// 平均計算
	if (FrameRateCount == (this->getSetFps() - 1)) {
		this->m_fpsAverage = 0.0;
		for (int i = 0; i < this->getSetFps(); i++) {
			this->m_fpsAverage += this->m_frameRateSave[i];
		}
		this->m_fpsAverage /= this->getSetFps();
	}


	FrameRateCount++;
	FrameRateCount %= this->getSetFps();


	return;
}


// 目標値をセット
void Window::setFps(int fps)
{
	// 目標値の取得。
	unsigned int old = this->getSetFps();
	// 同じなら、無視。
	if (old == fps) {
		return;
	}


	// フレームレート目標値をセット。
	this->m_setFPS = fps;


	if (old >= fps) {
		// 古いのよりも、新たに設定する FPS が大きい時、( 10fps -> 60fps など、 )
		// 補間すると、あら不思議。
		// 意味不になる。
		// この魔法？の return のおかげで、そうはならない。
		return;
	}


	// 補間処理。
	// FPS の処理落ちをなくしたい。
	double frame_wait_time = 1000.0 / fps;
	for (int i = 0; i < MAX_FRAMERATE; i++) {
		this->m_frameRateSave[i] = frame_wait_time;
	}


	return;
}


//---------------------------------------------------------
// Blend
//---------------------------------------------------------


// 現在のブレンドモードを記録。
Blend::Element Blend::g_blendElement;
// スタックで、前のブレンドに戻しやすくする。
std::vector<Blend::Element> Blend::g_stackBlend;


void Blend::set(BLEND_TYPE type, int parameter)
{
	if (parameter < 0) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HIGH) << "Blend::set -> parameter is (obj < 0)." << endl;
		parameter = 0;
	}
	else if (parameter > 0xFF) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HIGH) << "Blend::set -> parameter is (obj > 0xFF)." << endl;
		parameter = 0xFF;
	}


	if (type == Blend::BT_NORMAL) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, parameter);
	}
	else if (type == Blend::BT_ALPHA) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, parameter);
	}
	else if (type == Blend::BT_ADD) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, parameter);
	}
	else if (type == Blend::BT_SUB) {
		SetDrawBlendMode(DX_BLENDMODE_SUB, parameter);
	}
	else if (type == Blend::BT_MULA) {
		SetDrawBlendMode(DX_BLENDMODE_MULA, parameter);
	}
	else if (type == Blend::BT_INVSRC) {
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, parameter);
	}
	else {
AEN_STREAM_LOG_ERROR() << "Blend::set -> type is unjust." << endl;
AEN_THROW_EXCEPTION("type is unjust.");
	}


	Blend::g_blendElement.reset();
	Blend::g_blendElement.type = type;
	Blend::g_blendElement.parameter = parameter;
	return;
}


// スタックに保存したブレンドモードに戻す。
void Blend::pop(void)
{
	// 空ならエラー
	if (Blend::g_stackBlend.empty()) {
AEN_STREAM_LOG_ERROR() << "Blend::pop -> stack size is empty." << endl;
AEN_THROW_EXCEPTION("stack overflow.");
		return;
	}


	Blend::Element temp = Blend::g_stackBlend.at(Blend::g_stackBlend.size()-1);
	Blend::set(temp.type, temp.parameter);
	Blend::g_stackBlend.pop_back();
}


//---------------------------------------------------------
// カメラ。
// らくしたい時用。
// これ使うのは、早めにやめたほうがいいのでは？
// 2012 / 09 / 19
// jskny
//---------------------------------------------------------
// カメラアングル
float		g_cameraAngleH, g_cameraAngleV;
float		g_cameraCircleR_MouseWheel = 0;


bool		g_flagMouseInput;
int		g_prevMouseX, g_prevMouseY;
int		g_mouseX, g_mouseY;
float		g_prevCameraH, g_prevCameraV;


VECTOR		g_posCamera;
VECTOR		g_posTarget;


#define		GetRad		aen::util::GetRad


void InitCamera(void)
{
	g_cameraAngleH = -PHI_F / 2.0f;
	g_cameraAngleV = 0.0f;

	g_prevCameraH = g_cameraAngleH;
	g_prevCameraV = g_cameraAngleV;

	g_cameraCircleR_MouseWheel = 0.0f;
	g_posCamera = VGet(0, 0, 0);
	g_posTarget = VGet(0, 0, 0);


	g_flagMouseInput = false;
	g_mouseX = 0;
	g_mouseY = 0;
	g_prevMouseX = g_mouseX;
	g_prevMouseY = g_mouseY;
}


// マウス操作
void CameraMove2Mouse(void)
{
	int bufX = 0, bufY = 0;
	float speed = 50;
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		speed *= 10;
	}


	auto a = aen::gl::input::Mouse();
	g_cameraCircleR_MouseWheel -= a.wheel() * speed;


	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 ||
	    (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		GetMousePoint(&g_mouseX, &g_mouseY);
		if (!g_flagMouseInput) {
			g_prevMouseX = g_mouseX;
			g_prevMouseY = g_mouseY;
		}


		g_cameraAngleH = g_prevCameraH + static_cast<float>(g_mouseX-g_prevMouseX) / 50.0f / PHI_F;
		g_cameraAngleV = g_prevCameraV + static_cast<float>(g_mouseY-g_prevMouseY) / 50.0f / PHI_F;


		// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
		if (g_cameraAngleH > PHI_F) {
			g_cameraAngleH -= TWO_PHI_F;
		}
		// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
		else if (g_cameraAngleH < -PHI_F) {
			g_cameraAngleH += TWO_PHI_F;
		}


		if ((static_cast<float>(g_mouseY-g_prevMouseY) / 50.0f / PHI_F) < -GetRad(3)) {
			if (g_cameraAngleV < -PHI_F / 2.0f + 0.6f) {
				g_cameraAngleV = -PHI_F / 2.0f + 0.6f;
			}
		}
		else if ((static_cast<float>(g_mouseY-g_prevMouseY) / 50.0f / PHI_F) > GetRad(3)) {
			if (g_cameraAngleV > PHI_F / 2.0f - 0.6f) {
				g_cameraAngleV = PHI_F / 2.0f - 0.6f;
			}
		}


		DrawCircle(g_prevMouseX, g_prevMouseY, 5, GetColor(0x90, 0, 0), TRUE);
		DrawCircle(g_mouseX, g_mouseY, 5, GetColor(0xFF, 0, 0), TRUE);

		g_flagMouseInput = true;
	}
	else {
		if (g_flagMouseInput) {
			g_prevCameraH = g_cameraAngleH;
			g_prevCameraV = g_cameraAngleV;
		}

		g_flagMouseInput = false;
	}


}


// カメラを動かす。
void UpdateCamera(const VECTOR& obj)
{
g_posTarget = obj;
	VECTOR eye, target;


	// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
	if (g_cameraAngleH > PHI_F) {
		g_cameraAngleH -= TWO_PHI_F;
	}
	// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
	else if (g_cameraAngleH < -PHI_F) {
		g_cameraAngleH += TWO_PHI_F;
	}


	CameraMove2Mouse();


	// プレーヤーより少し高い位置。
	float cameraPlayerLength = 10.0f;
	// カメラの半径
	float cameraR = 1000.0f + g_cameraCircleR_MouseWheel;
	target = VAdd(obj, VGet(0.0f, cameraPlayerLength, 0.0f));


	MATRIX rotateZ, rotateY;
	// 水平方向の回転はＹ軸回転
	rotateY = MGetRotY(g_cameraAngleH);
	// 垂直方向の回転はＺ軸回転 )
	rotateZ = MGetRotZ(-g_cameraAngleV);

	// カメラの座標を算出
	// Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	// 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
	// 注視点の座標を足したものがカメラの座標
	eye = VAdd(VTransform(VTransform(VGet(-cameraR, 0.0f, 0.0f), rotateZ), rotateY), target);
	// 更新
	SetCameraPositionAndTarget_UpVecY(eye, target);


	g_posCamera = eye;
}


/// カメラのパラメーターを変更せずに、もう一度セットするだけ。
void UpdateCamera__(void)
{
	VECTOR eye, target;


	// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
	if (g_cameraAngleH > PHI_F) {
		g_cameraAngleH -= TWO_PHI_F;
	}
	// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
	else if (g_cameraAngleH < -PHI_F) {
		g_cameraAngleH += TWO_PHI_F;
	}


//	CameraMove2Mouse();


	// プレーヤーより少し高い位置。
	float cameraPlayerLength = 10.0f;
	// カメラの半径
	float cameraR = 1000.0f + g_cameraCircleR_MouseWheel;
	target = VAdd(g_posTarget, VGet(0.0f, cameraPlayerLength, 0.0f));


	MATRIX rotateZ, rotateY;
	// 水平方向の回転はＹ軸回転
	rotateY = MGetRotY(g_cameraAngleH);
	// 垂直方向の回転はＺ軸回転 )
	rotateZ = MGetRotZ(-g_cameraAngleV);

	// カメラの座標を算出
	// Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	// 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
	// 注視点の座標を足したものがカメラの座標
	eye = VAdd(VTransform(VTransform(VGet(-cameraR, 0.0f, 0.0f), rotateZ), rotateY), target);
	// 更新
	SetCameraPositionAndTarget_UpVecY(eye, target);


	g_posCamera = eye;
}


// カメラの回転値を取得。
float GetCameraRotateH(void)
{
	return (g_cameraAngleH);
}


float GetCameraRotateV(void)
{
	return (g_cameraAngleV);
}


void SetCameraRotateH(float val)
{
	g_cameraAngleH = val;
}


void SetCameraRotateV(float val)
{
	g_cameraAngleV = val;
}


VECTOR GetCameraPos(void)
{
	return (g_posCamera);
}


#undef		GetRad


} } // namespace aen::draw


