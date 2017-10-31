/*!
 *	AtomEngine
 *	gl
 *	@brief		DxLib の機能を使うやつ。
 *	@date		2012 / 03 / 10
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_DRAW_H_INCLUDE_GUARD_84622_0_2_6728542_131_029__
#define __ATOM_ENGINE_AEN_DRAW_H_INCLUDE_GUARD_84622_0_2_6728542_131_029__


#include	<string>


#include	<boost/noncopyable.hpp>


#include	"def.h"
#include	"aen_scene.h"
#include	"aen_handle_manager.h"
#include	"aen_gl_input.h"
#include	"aen_gl_picture.h"
#include	"aen_gl_text.h"
#include	"aen_gl_screen.h"
#include	"aen_gl_sound.h"
#include	"aen_gl_model.h"


namespace aen
{
/// DxLib の機能を使うやつ。
namespace gl
{


/// ウインドウ管理クラス。
class Window;


/// gl ネームスペース内のオブジェクトが使用するハンドルマネージャーを返す。
extern	handle::Manager*	GetHandleManager(void);
/// 四角形の当たり判定領域を作り、マウスと当たっていれば true を返す。
extern	bool		IsHitMouseBox(int x, int y, int bx, int by, bool flagDebugDraw = false);
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
extern	void		DrawPolygon2d(int x, int y, int num, float angle, float size, int color, int skipNum, int lw);


/*! 手抜きカメラを使いたい時用。
 * @brief		手抜きカメラの初期化。
 * @return		なし。
*/
extern	void		InitCamera(void);
/*! 手抜きカメラを使いたい時用。
 * @brief		マウスで動かせるカメラ
 * @param obj		回転の中心となる座標。
 * @return		なし。
*/
extern	void		UpdateCamera(const VECTOR& obj);
/// カメラのパラメーターを変更せずに、もう一度セットするだけ。
extern	void		UpdateCamera__(void);

// カメラの回転値を取得。
extern	float		GetCameraRotateH(void);
extern	float		GetCameraRotateV(void);
extern	void		SetCameraRotateH(float val);
extern	void		SetCameraRotateV(float val);
extern	VECTOR		GetCameraPos(void);


class Window
{
	enum {
		MAX_FRAMERATE = 120
	};
public:
	/*! Window オブジェクトの初期化処理
	 * @brief		Window の初期化処理 open を呼ぶ前に呼ぶ必要がある。
	 * @return		成功 true 失敗 false
	 */
	bool		initialize(void) throw (aen::Exception);
	/*! Window オブジェクトの終了処理
	 * @brief		Window の終了処理。
	 * @return		成功 true 失敗 false
	 */
	bool		finalize(void)  throw (aen::Exception);
	/*! Window オブジェクトが initialize されているか否か
	 * @brief		Window オブジェクトが initialize されていれば true を返す。
	 * @return		成功 true 失敗 false
	 */
	bool		isInitialize(void) const { return (this->m_flagInitialize); };


	/*! DxLib の初期化
	 * @brief		DxLib の初期化
	 * @param title		ウインドウのタイトル。
	 * @param w		ウインドウの X サイズ。
	 * @param h		ウインドウの Y サイズ。
	 * @param flagFull	true なら、フルスクリーン。
	 * @return		成功 true 失敗 false
	 */
	bool		open(const std::string& title, int w, int h, bool flagFull) throw (aen::Exception);
	/*! DxLib の終了
	 * @brief		DxLib を終了させる。
	 * @return		成功 true 失敗 false
	 */
	bool		close(void) throw (aen::Exception);
	/*! DxLib が初期化されているのなら true を返す。
	 * @brief		DxLib が初期化されているのなら true を返す。
	 * @return		ウインドウがある true ない false
	 */
	bool		isOpen(void) const { return (DxLib_IsInit() == TRUE ? true : false); };


	/*! ウインドウループを回す。
	 * @brief		ウインドウループを回す。
	 * @return		通常 true 終了メッセージが来た時 false
	 */
	bool		windowLoop(void);
	bool		windowLoopNonClearNonFlip(bool useScene = false);
	/// ウインドウループを抜けたい時呼ぶ
	void		breakLoop(void) { this->m_flagBreakWindowLoop = true; };
	bool		isBreakLoop(void) const { return (this->m_flagBreakWindowLoop); };


	/*! 最初の画面をセット
	 * @brief		最初の画面をセット。
	 * @param s		aen::scene::Scene* へのポインタ。
	 * @return		なし
	 */
	void		setScene(scene::Scene* s) { this->m_sceneManager.setSequence(s); };


	/// ループカウンタの取得。
	long int	getCount(void) const { return (this->m_count); };
	/// １つ前のフレームでかかった時間の取得。
	double		getFtime(void) const { return (this->m_frameTime); };
	/// フレーム全体 ( FPS固定化も含む ) でかかった時間の取得。
	double		getFtimeAll(void) const { return (this->m_fpsSaveTime); };


	/*! 目標 FPS をセット
	 * @brief		目標 FPS をセット
	 * @param fps		目標値
	 * @return		なし
	 */
	void		setFps(int fps);
	/*! 目標 FPS を取得
	 * @brief		目標 FPS を取得
	 * @return		目標値
	 */
	int		getSetFps(void) const { return (this->m_setFPS); };
	/*! 現在の FPS を取得
	 * @brief		現在の FPS を取得
	 * @return		FPS
	 */
	double		getFps(void) const { return (1000.0 / this->getFpsAverage());  };
	/// 平均値を取得
	double		getFpsAverage(void) const { return (this->m_fpsAverage); };


	Window();
	~Window() {
		if (this->isInitialize()) {
			this->finalize();
		}

	}


private:
	bool		m_flagInitialize;
	// ループカウンタ
	long int	m_count;
	// 前のフレームでかかった時間
	double		m_frameTime;
	// ウインドウのサイズ
	int		m_windowW;
	int		m_windowH;
	// ウインドウループを抜けるとき true
	bool		m_flagBreakWindowLoop;


	// シーンマネージャー
	scene::Manager	m_sceneManager;


	// 固定フレーム用 FPSをあわせる。
	void		__waitFps(void);
	// 目標値に指定された FPS
	int		m_setFPS;
	double		m_frameRateSave[MAX_FRAMERATE];
	// FPS の平均値
	double		m_fpsAverage;
	// フレーム全体の経過時刻を保存
	int		m_fpsSaveTime;


};


inline Window::Window() :
	m_flagInitialize(false),
	m_count(0), m_frameTime(0.0),
	m_windowW(0), m_windowH(0),
	m_flagBreakWindowLoop(false),
	m_setFPS(60), m_fpsAverage(0.0),
	m_fpsSaveTime(0)
{
	this->setFps(60);

}


// ブレンド
class Blend :private boost::noncopyable
{
public:
	enum BLEND_TYPE {
		BT_NORMAL = 0xFF,	/// 何もしない。
		BT_ALPHA,		/// アルファブレンド・・・とか言うもの。
		BT_ADD,			/// 加算ブレンド
		BT_SUB,			/// 減算ブレンド
		BT_MULA,		/// 乗算ブレンド
		BT_INVSRC		/// 反転ブレンド
	};


	// ブレンドモード, ブレンドする強さ。 0xFF が最大値。
static	void		set(BLEND_TYPE type, int parameter);


	// ブレンドを破棄。
static	void		reset(void) { Blend::set(Blend::BT_NORMAL, 0); };


	// 現在のブレンドモードを記録。
static	void		push(void) { Blend::g_stackBlend.push_back(Blend::g_blendElement); };
	// スタックに保存したブレンドモードに戻す。
static	void		pop(void);


static	bool		stackEmpty(void) { return (Blend::g_stackBlend.empty()); };
static	int		stackSize(void) { return (Blend::g_stackBlend.size()); };


protected:
	class Element
	{
	public:
		int		parameter;
		BLEND_TYPE	type;


		void		reset(void)
		{
			this->parameter = 0;
			this->type = BT_NORMAL;
		}


		Element() :
			parameter(0),
			type(BT_NORMAL)
		{
		}
	};


private:
	// 現在のブレンドモードを記録。
static	Element			g_blendElement;
	// スタックで、前のブレンドに戻しやすくする。
static	std::vector<Element>	g_stackBlend;


};


} } // namespace aen::gl


#endif // __ATOM_ENGINE_AEN_DRAW_H_INCLUDE_GUARD_84622_0_2_6728542_131_029__

