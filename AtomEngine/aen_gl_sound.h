/*!
 *	AtomEngine
 *	Sound
 *	@brief		音声関連
 *	@date		2012 / 04 / 30
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_SOUND_H_INCLUDE_GUARD_3992871_1793_1047_3361__
#define __ATOM_ENGINE_AEN_GL_SOUND_H_INCLUDE_GUARD_3992871_1793_1047_3361__


#include	<vector>
#include	"aen_handle.h"


namespace aen
{
namespace gl
{
/// 音声系
namespace sound
{


/*! 音声再生を使用しない。
 * @brief	aen::gl::sound 内の出力系関数が呼ばれたら、すぐに return するようになる。
 * @param flag	再生しない true, 再生する false
 * @warning	デフォルトでは再生するになっている。
 * @return	なし。
*/
extern	void		SetFlagNonUseSound(bool flag);
/*! 音声再生を使用するか否か。
 * @brief	音声再生をするか否か。
 * @return	再生しない true, 再生する false
*/
extern	bool		IsNonUseSound(void);


/// プレイヤー
class Player;


/// テキスト
class Player
{
public:
	enum ModePlay {
		MP_LOOP = 1,		/// バックグラウンドループ再生
		MP_NONLOOP,		/// バックグラウンド一回再生
		MP_STOP_ONE_SHOOT	/// フォアグラウンド一回再生 ゲームループを止めて再生する。
	};

	enum ModeSound {
		MS_STREAM = 1,	/// ストリーム再生 ( ファイルをそのまま転送。 処理負荷とても高い。 )
		MS_ARCHIVE,	/// 圧縮されている音源。
		MS_WAVE		/// wave 音源等無圧縮の音源。
	};


	/*! 音声の読み込み
	 * @brief		音声ファイルを読み込む。
	 * @param urlFile	音声ファイルへのアドレス
	 * @param sm		音声ファイルの種類
	 * @param volume	音量 (0 ～ 255)
	 * @param pan		パン (左 -10000 ～ +10000 右)
	 * @warning		すでにセットされていた場合上書きとなる。
	 * @return		成功 true, 失敗 false
	 */
	bool		load(const char* const urlFile, ModeSound sm = MS_WAVE, unsigned int volume = 0xFF, int pan = 0);


	/// ハンドルの開放。
	void		del(void);
inline	void		clear(void) { this->del(); };
inline	void		reset(void) { this->del(); };


	/*! 読み込まれていれば true を返す。
	 * @brief		読み込まれていれば true ただし、ハンドルの有効性までは判定しない。
	 * @warning		ハンドルの有効性までは判定しない。
	 * @return		読み込まれている true, 読み込まれていない false
	 */
	bool		isLoad(void) const { return (this->m_handle.get() ? true : false); };


	/// 再生中なら true を返す。
	bool		isPlay(void);
	/// 再生
	void		play(const ModePlay mode = MP_LOOP);
	/// 単発再生。
	void		playOne(void) { this->play(this->MP_NONLOOP); };
	/// 単発 Windows Loop 停止再生。
	void		playOneNormal(void) { this->play(this->MP_STOP_ONE_SHOOT); };
	/// 再生停止。
	void		stop(void);


	// ボリュームをセット
	// 0xFF == 最大値
	void		setVolume(unsigned int volume);
	// パンをセット、
	// -10000 ～ +10000
	// + は右寄りに、
	// - は左寄りに音声を再生させる。
	void		setPan(int pan);


	/// ハンドルの取得
	int		getHandle(void) const
	{
		if (this->isLoad()) {
			return (this->m_handle->getHandle());
		}


		return (-1);
	}


	Player() { this->del(); };
virtual	~Player() { this->del(); }


private:
	boost::shared_ptr<handle::HandleDxLib>	m_handle;


};


} } } // namespace aen::gl::sound


#endif // __ATOM_ENGINE_AEN_GL_SOUND_H_INCLUDE_GUARD_3992871_1793_1047_3361__

