/*!
 *	AtomEngine
 *	Sound
 *	@brief		音声関連
 *	@date		2012 / 04 / 30
 *	@author		jskny
*/


#include	<sstream>
#include	"def.h"


#include	"aen_hash.h"
#include	"aen_gl.h"
#include	"aen_gl_sound.h"


using namespace std;


namespace aen
{
namespace gl
{
namespace sound
{


static bool g_flsgNonUseSound = false;
/*! 音声再生を使用しない。
 * @brief	aen::gl::sound 内の出力系関数が呼ばれたら、すぐに return するようになる。
 * @param flag	再生しない true, 再生する false
 * @warning	デフォルトでは再生するになっている。
 * @return	なし。
*/
void SetFlagNonUseSound(bool flag)
{
	if (g_flsgNonUseSound == flag) {
		return;
	}


	std::ostringstream oss;
	oss << "SetFlagNonUseSound : change mode. [";
	oss << (g_flsgNonUseSound ? "TRUE" : "FALSE");
	oss << " => " << (flag ? "TRUE" : "FALSE") << "]";


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_LOW) << oss.str() << endl;
	g_flsgNonUseSound = flag;
}


/*! 音声再生を使用するか否か。
 * @brief	音声再生をするか否か。
 * @return	再生しない true, 再生する false
*/
bool IsNonUseSound(void)
{
	return (g_flsgNonUseSound);
}


//---------------------------------------------------------
// Player
//---------------------------------------------------------


/*! 音声の読み込み
 * @brief		音声ファイルを読み込む。
 * @param urlFile	音声ファイルへのアドレス
 * @param sm		音声ファイルの種類
 * @param volume	音量 (0 ～ 255)
 * @param pan		パン (左 -10000 ～ +10000 右)
 * @warning		すでにセットされていた場合上書きとなる。
 * @return		成功 true, 失敗 false
 */
bool Player::load(const char* const urlFile, ModeSound sm, unsigned int volume, int pan)
{
	int buf = 0, ftype = 0;


	if (volume < 0) {
		AEN_PRINTF_ERROR("parameter size is unjust. [ exp : 'volume < 0' ]");
		return (false);
	}
	if (volume > 0xFF) {
		AEN_PRINTF_ERROR("parameter size is unjust. [ exp : 'volume > 0xFF' ]");
		return (false);
	}
	else if (sm < this->MS_STREAM || sm > this->MS_WAVE) {
		AEN_PRINTF_ERROR("parameter t is unjust. [ exp : 't < SM_STREAM || t > SM_WAVE' ]");
		return (false);
	}
	else if (urlFile == 0) {
		AEN_PRINTF_ERROR("parameter fontname is unjust. [ exp : 'urlFile == 0' ]");
		return (false);
	}
	else if (!DxLib_IsInit()) {
		AEN_PRINTF_ERROR("DxLib は未初期化です。");
		return (false);
	}


/*
DebugStreamGL_Sound_Start() << "Player::load -> load sound : '" << urlFile << "' ";
DebugStreamGL_Sound() << "info [" << urlFile << ":" << sm << "]" << endl;
*/
	std::ostringstream oss;
	oss.str("");
	oss << urlFile << ":" << sm;
	// ハンドル表を検索、あればハンドルを取得し、
	// 終了。
	int key = aen::hash::AdlerStringLower(oss.str().c_str());


	// ハンドル表検索。
	if (aen::gl::GetHandleManager()->search(key)) {
		// 発見、
		// だが本当にそのハンドルは生きているのか確認。
		// すげ。こんなキャストがあったんだ。
		boost::shared_ptr<aen::handle::HandleDxLib> a = boost::dynamic_pointer_cast<aen::handle::HandleDxLib>(aen::gl::GetHandleManager()->get(key).lock());


		if (a.get()) {
			// すでにハンドルがセットされているのなら、
			// 前のを破棄。
			this->del();
			this->m_handle = a;
		}
		else {
			// ハンドルは不正。
			aen::gl::GetHandleManager()->del(key);
		}
	}


	// ハンドルがセットされた。
	if (this->m_handle.get()) {
		// 終了処理を行って終了。
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "copy create. : " << oss.str() << endl;
		goto label_return_func;
	}


	if (sm == this->MS_STREAM) {
		ftype = DX_SOUNDDATATYPE_FILE;
	}
	else if (sm == this->MS_ARCHIVE) {
		ftype = DX_SOUNDDATATYPE_MEMPRESS;
	}
	else if (sm == this->MS_WAVE) {
		ftype = DX_SOUNDDATATYPE_MEMNOPRESS;
	}
	else {
		AEN_ASSERT_KILL(0, "サウンドハンドル初期化に使用するサウンドタイプが不正です。");
		ftype = DX_SOUNDDATATYPE_FILE;
	}


	// 音源の種類を設定。
SetCreateSoundDataType(ftype);
	buf = LoadSoundMem(urlFile);
	// デフォルトに戻す。
SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);


	if (buf == -1) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Player::load -> ERROR >> Can not create sound handle '" << urlFile << "'" << endl;
AEN_STREAM_LOG_ERROR() << "Player::load -> ERROR >> Can not create sound handle '" << urlFile << "'" << endl;
		AEN_PRINTF_ERROR("can't create 'sound handle'...");
		return (false);
	}


{
	boost::shared_ptr<aen::handle::HandleDxLib> handle(new aen::handle::HandleDxLib(), aen::handle::Deleter());
	handle->handle = buf;
	handle->type = handle::HandleDxLib::HANDLE_TYPE_SOUND;
	handle->text = oss.str();
	this->m_handle = handle;
	// 登録。
	aen::gl::GetHandleManager()->set(key, handle);
}


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << oss.str() << endl;


	this->setVolume(volume);
	this->setPan(pan);
label_return_func:
	return (true);
}


// 再生
void Player::play(const ModePlay mode)
{
	AEN_ASSERT_KILL(DxLib_IsInit(), "DxLib が未初期化です。");


	if (!this->m_handle.get()) {
AEN_PRINTF_ERROR("aen::gl::sound::Player::play -> sound handle is unjust.");
		return;
	}


	if (this->isPlay()) {
		this->stop();
	}


	// ミュート？
	if (IsNonUseSound()) {
		return;
	}


	if (mode == this->MP_LOOP) {
		PlaySoundMem(this->m_handle->getHandle(), DX_PLAYTYPE_LOOP, TRUE);
	}
	else if (mode == this->MP_NONLOOP) {
		PlaySoundMem(this->m_handle->getHandle(), DX_PLAYTYPE_BACK, TRUE);
	}
	else if (mode == this->MP_STOP_ONE_SHOOT) {
		PlaySoundMem(this->m_handle->getHandle(), DX_PLAYTYPE_NORMAL, TRUE);
	}
	else {
		AEN_PRINTF_ERROR("parameter 'mode' is unjust...");
	}


	return;
}


// 再生停止。
void Player::stop(void)
{
	if (!DxLib_IsInit()) {
// cerr << "aen::gl::sound::Player::stop -> DxLib は未初期化です。" << endl;
		return;
	}
	else if (!this->m_handle.get()) {
// AEN_ERROR("aen::gl::sound::Player::stop -> sound handle is unjust.");
		return;
	}


	if (this->isPlay()) {
		StopSoundMem(this->m_handle->getHandle());
	}


	return;
}


// ボリュームをセット
// 0xFF == 最大値
void Player::setVolume(unsigned int volume)
{
	if (!this->m_handle.get()) {
		AEN_PRINTF_ERROR("'sound handle' is unjust... [un set]");
		return;
	}


	if (volume > 0xFF) {
		AEN_PRINTF_ERROR("parameter volume is unjust. [ exp : 'volume > 0xFF' ]");
		return;
	}


	if (ChangeVolumeSoundMem(volume, this->m_handle->getHandle()) != 0) {
		// error
		AEN_PRINTF_ERROR("can't reset, 'sound volume'.");
	}


	return;
}


// パンをセット、
// -10000 ～ +10000
// + は右寄りに、
// - は左寄りに音声を再生させる。
void Player::setPan(int pan)
{
	if (!this->m_handle.get()) {
		AEN_PRINTF_ERROR("'sound handle' is unjust... [un set]");
		return;
	}


	if (pan < -10000) {
		AEN_PRINTF_ERROR("parameter volume is unjust. [ exp : 'pan < -10000' ]");
		return;
	}
	else if (pan > 10000) {
		AEN_PRINTF_ERROR("parameter volume is unjust. [ exp : 'volume > 10000' ]");
		return;
	}


	if (SetPanSoundMem(pan, this->m_handle->getHandle()) != 0) {
		// error
		AEN_PRINTF_ERROR("can't reset, 'sound pan'.");
	}


	return;
}


// 再生中か否か、
bool Player::isPlay(void)
{
	if (!this->m_handle.get()) {
		return (false);
	}


	int a = CheckSoundMem(this->m_handle->getHandle());
	if (a == 0) {
		// 再生していない
		return (false);
	}
	else if (a == 1) {
		// 再生中
		return (true);
	}
	else {
		// エラー
		AEN_PRINTF_ERROR("'sound handle' is unjust... [un set ?]");
		return (false);
	}


	return (true);
}


// ハンドルの破棄
void Player::del(void)
{
	if (!this->m_handle.get()) {
		return;
	}


	if (this->isPlay()) {
		this->stop();
	}


	this->m_handle.reset();
}


} } } // namespace aen::gl::sound

