/*!
 *	AtomEngine
 *	Model
 *	@brief		モデル
 *	@date		2012 / 06 / 04
 *	@author		jskny
*/


#include	<sstream>


#include	"def.h"
#include	"aen_hash.h"
#include	"aen_gl.h"
#include	"aen_gl_model.h"


using namespace std;


namespace aen
{
namespace gl
{


/*! 3dモデル
 * @brief		モデルを読み込む
 * @param urlFile	ファイルへのアドレス
 * @warning		すでにセットされていた場合上書きとなる。
 * @return		読み込み成功 true 失敗 false
*/
bool Model::load(const char* const urlFile)
{
	if (urlFile == NULL) {
		AEN_PRINTF_ERROR("parameter urlFile is unjust. [ exp : 'urlFile == NULL' ]");
		return (false);
	}
	else if (!DxLib_IsInit()) {
		AEN_PRINTF_ERROR("DxLib は未初期化です。");
		return (false);
	}


	std::ostringstream oss;
	oss.str("");
	oss << urlFile;
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
// POINT: 低速	3D モデルのコピーハンドルの作成。
			boost::shared_ptr<handle::HandleDxLib> ptr = boost::shared_ptr<handle::HandleDxLib>(new handle::HandleDxLib(), aen::handle::Deleter());
			ptr->type = handle::HandleDxLib::HANDLE_TYPE_MODEL;
			ptr->handle = MV1DuplicateModel(a->getHandle());
			ptr->text = oss.str();
			this->m_handle = ptr;
// POINT: 高速	ハンドル表のハンドルコピーを使用。
//			this->m_handle = a;
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


	int buf = MV1LoadModel(urlFile);
	if (buf == -1) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Model::load -> ERROR >> Can not create model handle '" << urlFile << "'" << endl;
AEN_STREAM_LOG_ERROR() << "Model::load -> ERROR >> Can not create model handle '" << urlFile << "'" << endl;
		AEN_PRINTF_ERROR("can't create 'model handle'...");
		return (false);
	}


{
	boost::shared_ptr<aen::handle::HandleDxLib> handle(new aen::handle::HandleDxLib(), aen::handle::Deleter());
	handle->handle = buf;
	handle->type = handle::HandleDxLib::HANDLE_TYPE_MODEL;
	handle->text = oss.str();
	this->m_handle = handle;
	// 登録。
	aen::gl::GetHandleManager()->set(key, handle);
}


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << oss.str() << endl;
label_return_func:
	return (true);
}


} } // namespace aen::gl

