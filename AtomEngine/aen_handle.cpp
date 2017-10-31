/*!
 *	AtomEngine
 *	handle
 *	@brief		AtomEngine で使用するハンドルの基底クラスなど。
 *	@date		2012 / 03 / 10
 *	@author		jskny
*/


#include	<iostream>


#include	"aen.h"
#include	"aen_handle.h"


using namespace std;


namespace aen
{
namespace handle
{


/// DxLib のハンドル。
/// type を見て、破棄動作を行う。
void HandleDxLib::destroy(void)
{
	bool rc = false;


	if (!::aen::IsInitialize()) {
		// Engine が落ちているんだ
		// もう手に負えない。
		// ハンドル自体が狂っている可能性もあるし、
		// 忘れろ。
		return;
	}
	else if (!DxLib_IsInit()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_HANDLE) << "DxLib is not active... skip to release finish." << endl;
		return;
	}


AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_HANDLE) << "DELETE DXLIB HANDLE >> ";
	if (this->type == HANDLE_TYPE_GRAPHIC) {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << "graphic : " << this->handle;
		rc = (DeleteGraph(this->handle) == 0);
	}
	else if (this->type == HANDLE_TYPE_SOUND) {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << "sound : " << this->handle;
		rc = (DeleteSoundMem(this->handle) == 0);
	}
	else if (this->type == HANDLE_TYPE_FONT) {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << "font : " << this->handle;
		rc = (DeleteFontToHandle(this->handle) == 0);
	}
	else if (this->type == HANDLE_TYPE_MODEL) {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << "model : " << this->handle;
		rc = (MV1DeleteModel(this->handle) == 0);
	}
	else {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << "unknown : " << this->handle << " [ ERROR: Can not delete 'handle'. memory leak ]";
AEN_STREAM_LOG_ERROR() << "unknown : " << this->handle << " [ ERROR: Can not delete 'handle'. memory leak ]" << endl;
	}


aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << " '" << this->text << "' : " << ((rc == 1) ? "true" : "false");
	if (!DxLib_IsInit()) {
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << " [ DxLib is non active. ]";
	}
	else {
		if (!rc) {
			// DxLib が健在で、
			// ハンドルの開放失敗はおかしいだろ。
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_HANDLE) << " ERROR: handle delete FAIL.";
AEN_STREAM_LOG_ERROR() << "handle dleete FAIL." << this->handle << ":" << this->text << endl;
		}
	}
aen::GetStreamLogDebug(aen::DEBUG_LOG_LEVEL_HANDLE) << endl;


	return;
}


} } // namespace aen::handle

