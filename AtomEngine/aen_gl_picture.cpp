/*!
 *	AtomEngine
 *	Picture
 *	@brief		画像 ( 連番 )
 *	@date		2012 / 03 / 28
 *	@author		jskny
*/


#include	<sstream>
#include	"def.h"


#include	"aen_hash.h"
#include	"aen_gl.h"
#include	"aen_gl_picture.h"


using namespace std;


namespace aen
{
namespace gl
{


/*! 画像のセット
 * @brief		画像のセット
 * @param urlFile	画像へのパス
 * @param num_g		画像数
 * @param num_gx	横に何枚画像があるか
 * @param num_gy	縦に何枚画像があるか
 * @param imageOneW	画像一つの横幅
 * @param imageOneH	画像一つの縦幅
 * @warning		すでにセットされていた場合上書きとなる。
 * @return		成功 true, 失敗 false
 */
bool Picture::load(
	const char* const urlFile,
	const unsigned int num_g,
	const unsigned int num_xg,
	const unsigned int num_yg,
	const unsigned int imageOneW,
	const unsigned int imageOneH
)
{
	bool flagParamUnjust = false;
	if (!urlFile) {
		AEN_PRINTF_ERROR("parameter num_g is unjust. [ exp : 'url_file == 0' ]");
		flagParamUnjust = true;
	}
	else if (num_g < 1) {
		AEN_PRINTF_ERROR("parameter num_g is unjust. [ exp : 'num_g < 1' ]");
		flagParamUnjust = true;
	}
	else if (num_xg < 1) {
		AEN_PRINTF_ERROR("parameter num_xg is unjust. [ exp : 'num_xg < 1' ]");
		flagParamUnjust = true;
	}
	else if (num_yg < 1) {
		AEN_PRINTF_ERROR("parameter num_yg is unjust. [ exp : 'num_yg < 1' ]");
		flagParamUnjust = true;
	}
	else if (imageOneW < 1) {
		AEN_PRINTF_ERROR("parameter image_one_w is unjust. [ exp : 'image_one_w < 1' ]");
		flagParamUnjust = true;
	}
	else if (imageOneH < 1) {
		AEN_PRINTF_ERROR("parameter image_one_h is unjust. [ exp : 'image_one_h < 1' ]");
		flagParamUnjust = true;
	}


	if (flagParamUnjust) {
AEN_STREAM_LOG_ERROR() << "parameter is unjust." << endl;
		return (false);
	}


	if (!DxLib_IsInit()) {
		AEN_PRINTF_ERROR("AtomEngine GameLibrary は未初期化です。");
AEN_STREAM_LOG_ERROR() << "DxLib is non initialized." << endl;
		return (false);
	}


	std::ostringstream oss;


	// ハンドル表を検索、あればハンドルを取得し、
	// 終了。
	for (unsigned int i = 0; i < num_g; i++) {
		oss.str("");
		oss << "picture:" << urlFile << ":" << num_g << ":" << num_xg << ":" << num_yg;
		oss << ":" << imageOneW << ":" << imageOneH << "*" << i;


		int key = aen::hash::AdlerStringLower(oss.str().c_str());
		// oss.str() はハンドル表に登録する文字列。
		if (aen::gl::GetHandleManager()->search(key)) {
			// 発見、
			// だが本当にそのハンドルは生きているのか確認。
			// すげ。こんなキャストがあったんだ。
			boost::shared_ptr<aen::handle::HandleDxLib> a = boost::dynamic_pointer_cast<aen::handle::HandleDxLib>(aen::gl::GetHandleManager()->get(key).lock());


			if (a.get()) {
				if (i == 0) {
					// 一回目だけ初期化。
					this->del();
				}
				this->m_handle.push_back(a);
			}
			else {
				// ハンドルは不正。
				aen::gl::GetHandleManager()->del(key);
			}
		}
		else {
			break; // なけりゃ無いでしょ。
		}
	}


	// ハンドルがセットされた。
	if (!this->m_handle.empty()) {
		oss.str("");
		oss << "picture:" << urlFile << ":" << num_g << ":" << num_xg << ":" << num_yg;
		oss << ":" << imageOneW << ":" << imageOneH;

		// 終了処理を行って終了。
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "copy create. : " << oss.str() << endl;
		goto label_return_func;
	}


	int* buf = new int[num_g];
	if (!buf) {
		AEN_PRINTF_ERROR("can't new memory.");
		return (false);
	}


	if (LoadDivGraph(
		urlFile, num_g,
		num_xg, num_yg,
		imageOneW, imageOneH,
		buf) != 0)
	{
		AEN_SAFE_DELETE_ARRAY(buf);
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Picture::load -> ERROR >> NotFound File '" << urlFile << "'" << endl;
AEN_STREAM_LOG_ERROR() << "Picture::load -> ERROR >> NotFound File '" << urlFile << "'" << endl;
		AEN_PRINTF_ERROR("can't find graphic file.");
		return (false);
	}


	if (!this->m_handle.empty()) {
		this->del();
	}


	for (unsigned int i = 0; i < num_g; i++) {
		int key = aen::hash::AdlerStringLower(oss.str().c_str());
		boost::shared_ptr<aen::handle::HandleDxLib> handle(new aen::handle::HandleDxLib(), aen::handle::Deleter());


		// ハンドルが不正ではないか、
		if (!buf[i]) {
			AEN_PRINTF_ERROR("'graphic handle' is unjust.");
			AEN_SAFE_DELETE_ARRAY(buf);
			return (false);
		}


		oss.str("");
		oss << "picture:" << urlFile << ":" << num_g << ":" << num_xg << ":" << num_yg;
		oss << ":" << imageOneW << ":" << imageOneH << "*" << i;

		handle->handle = buf[i];
		handle->type = handle::HandleDxLib::HANDLE_TYPE_GRAPHIC;
		handle->text = oss.str();
		this->m_handle.push_back(handle);


		// 登録。
		aen::gl::GetHandleManager()->set(key, handle);
	}


	AEN_SAFE_DELETE_ARRAY(buf);


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << oss.str() << endl;
label_return_func: // ハンドル出力
	// 画像の縦横を記録
	this->m_imageW = imageOneW;
	this->m_imageH = imageOneH;


	this->m_flagLoad = true;
	this->m_flagTexture = false;
	return (true);
}


/*! 画像のセット
 * @brief		一枚絵として画像を読み込む
 * @param urlFile	画像へのパス
 * @warning		すでにセットされていた場合上書きとなる。
 * @return		成功 true, 失敗 false
 */
bool Picture::load(const char* const urlFile)
{
	if (!urlFile) {
		AEN_PRINTF_ERROR("parameter num_g is unjust. [ exp : 'url_file == 0' ]");
		return (false);
	}
	else if (!DxLib_IsInit()) {
		AEN_PRINTF_ERROR("AtomEngine GameLibrary は未初期化です。");
AEN_STREAM_LOG_ERROR() << "DxLib is non initialized." << endl;
		return (false);
	}


	std::ostringstream oss;
	oss << "picture to oneimage:" << urlFile;


	int key = aen::hash::AdlerStringLower(oss.str().c_str());
	// oss.str() はハンドル表に登録する文字列。
	if (aen::gl::GetHandleManager()->search(key)) {
		// 発見、
		// だが本当にそのハンドルは生きているのか確認。
		// すげ。こんなキャストがあったんだ。
		boost::shared_ptr<aen::handle::HandleDxLib> a = boost::dynamic_pointer_cast<aen::handle::HandleDxLib>(aen::gl::GetHandleManager()->get(key).lock());


		if (a.get()) {
			this->del();
			this->m_handle.push_back(a);
		}
		else {
			// ハンドルは不正。
			aen::gl::GetHandleManager()->del(key);
		}
	}


	// ハンドルがセットされた。
	if (!this->m_handle.empty()) {
		// 終了処理を行って終了。
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "copy create. : " << oss.str() << endl;
		goto label_return_func;
	}


	int buf = LoadGraph(urlFile);
	if (buf == -1) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Picture::load -> ERROR >> NotFound File '" << urlFile << "'" << endl;
AEN_STREAM_LOG_ERROR() << "Picture::load -> ERROR >> NotFound File '" << urlFile << "'" << endl;
		AEN_PRINTF_ERROR("can't find graphic file.");
		return (false);
	}


	this->del();
{
	boost::shared_ptr<aen::handle::HandleDxLib> handle(new aen::handle::HandleDxLib(), aen::handle::Deleter());
	handle->handle = buf;
	handle->type = handle::HandleDxLib::HANDLE_TYPE_GRAPHIC;
	handle->text = oss.str();
	this->m_handle.push_back(handle);
	// 登録。
	aen::gl::GetHandleManager()->set(key, handle);
}


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << oss.str() << endl;
label_return_func:
	// 画像の縦横を取得。
	int imageOneW = 0, imageOneH = 0;
	GetGraphSize(this->m_handle.at(0)->getHandle(), &imageOneW, &imageOneH);


	// 画像の縦横を記録
	this->m_imageW = imageOneW;
	this->m_imageH = imageOneH;
	
	this->m_flagLoad = true;
	this->m_flagTexture = true;
	return (true);
}


// ハンドルの開放。
void Picture::del(void)
{
	if (this->m_handle.empty()) {
		return;
	}


	auto itr = this->m_handle.begin();
	while (itr != this->m_handle.end()) {
		AEN_ASSERT_KILL(itr->get(), "null pointer.");
		itr->reset();
		itr++;
	}


	this->m_handle.clear();
	this->m_flagLoad = false;
	this->m_flagTexture = false;
}


// 画像の出力、
// index の値にハンドルが存在しければ、例外を発生させる。 ( vector アクセスは at )
// index	配列 i
void Picture::draw(const long int x, const long int y, const unsigned long int index) const
{
	// DxLib が未初期化です。
	AEN_ASSERT_KILL(DxLib_IsInit(), "DxLib が未初期化です。");


	if (index < this->m_handle.size()) {
		AEN_ASSERT_KILL(this->m_handle.at(index)->getHandle() != 0, "null pointer.");
		DrawGraph(x, y, this->m_handle.at(index)->getHandle(), TRUE);
	} else {
// cout << index << "," << this->m_handle.size() << endl;
		AEN_PRINTF_ERROR("can't draw 'graphic handle'. 'index num' are over the 'vector point'.");
	}
}


void Picture::draw(const long int x, const long int y, const unsigned long int index, const long double angle, const long double extend) const
{
	// DxLib が未初期化です。
	AEN_ASSERT_KILL(DxLib_IsInit(), "DxLib が未初期化です。");


	if (index < this->m_handle.size()) {
		AEN_ASSERT_KILL(this->m_handle.at(index)->getHandle() != 0, "null pointer.");
		DrawRotaGraph(x, y, extend, angle, this->m_handle.at(index)->getHandle(), TRUE, FALSE);
	} else {
		AEN_PRINTF_ERROR("can't draw 'graphic handle'. 'index num' are over the 'vector point'.");
	}
}


} } // namespace aen::gl

