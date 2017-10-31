/*!
 *	AtomEngine
 *	Text
 *	@brief		テキスト
 *	@date		2012 / 04 / 03
 *	@author		jskny
*/


#include	<sstream>
#include	"def.h"


#include	"aen_hash.h"
#include	"aen_gl.h"
#include	"aen_gl_text.h"


using namespace std;


namespace aen
{
namespace gl
{


/*! フォントのセット。
 * @brief		フォントを読み込む。
 * @param fontName	フォント名
 * @param size		フォントの大きさ
 * @param thinck	フォントの太さ
 * @param t		フォントタイプ
 * @warning		すでにセットされていた場合上書きとなる。
 * @return		成功 true, 失敗 false
 */
bool Text::load(const char* const fontName, const unsigned int size, const unsigned int thinck, const TypeFont t)
{
	if (size < 0) {
		AEN_PRINTF_ERROR("parameter size is unjust. [ exp : 'size < 0' ]");
		return (false);
	}
	else if (thinck < 0) {
		AEN_PRINTF_ERROR("parameter thinck is unjust. [ exp : 'thinck < 0' ]");
		return (false);
	}
	else if (t < FT_N || t > FT_AE) {
		AEN_PRINTF_ERROR("parameter t is unjust. [ exp : 't < FT_N || t > FT_AE' ]");
		return (false);
	}
	else if (fontName == 0) {
		AEN_PRINTF_ERROR("parameter fontname is unjust. [ exp : 'fontname == 0' ]");
		return (false);
	}
	else if (!DxLib_IsInit()) {
		AEN_PRINTF_ERROR("AtomEngine GameLibrary は未初期化です。");
		return (false);
	}


	std::ostringstream oss;
	oss.str("");
	oss << "font:" << fontName << ":" << size << ":" << thinck << ":" << t;


	// ハンドル表を検索、あればハンドルを取得し、
	// 終了。
	int key = aen::hash::AdlerStringLower(oss.str().c_str());


	// oss.str() はハンドル表に登録する文字列。
	if (aen::gl::GetHandleManager()->search(key)) {
		// 発見、
		// だが本当にそのハンドルは生きているのか確認。
		// すげ。こんなキャストがあったんだ。
		boost::shared_ptr<aen::handle::HandleDxLib> a = boost::dynamic_pointer_cast<aen::handle::HandleDxLib>(aen::gl::GetHandleManager()->get(key).lock());


		if (a.get()) {
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


	int buf = 0, ftype = 0;
	if (t == FT_N) {
		ftype = DX_FONTTYPE_NORMAL;
	}
	else if (t == FT_E) {
		ftype = DX_FONTTYPE_EDGE;
	}
	else if (t == FT_A) {
		ftype = DX_FONTTYPE_ANTIALIASING;
	}
	else if (t == FT_AE) {
		ftype = DX_FONTTYPE_ANTIALIASING_EDGE;
	}
	else {
		AEN_ASSERT_KILL(0, "フォントハンドル初期化に使用するフォントタイプが不正です。");
	}


	buf = CreateFontToHandle(fontName, size, thinck, ftype);
	if (buf == -1) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Text::load -> ERROR >> Can not create font handle '" << fontName << "'" << endl;
AEN_STREAM_LOG_ERROR() << "Text::load -> ERROR >> Can not create font handle '" << fontName << "'" << endl;
		AEN_PRINTF_ERROR("can't create 'font handle'...");
		return (false);
	}


{
	boost::shared_ptr<aen::handle::HandleDxLib> handle(new aen::handle::HandleDxLib(), aen::handle::Deleter());
	handle->handle = buf;
	handle->type = handle::HandleDxLib::HANDLE_TYPE_FONT;
	handle->text = oss.str();
	this->m_handle = handle;
	// 登録。
	aen::gl::GetHandleManager()->set(key, handle);
}


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << oss.str() << endl;
label_return_func:
	return (true);
}


/*! 文字列の出力。
 * @brief		文字列を出力する。
 * @param x		出力 x 座標
 * @param y		出力 y 座標
 * @param color		出力色
 * @param str		出力文字列
 * @return		なし。
 */
void Text::draw(int x, int y, int color, const char* const str) const
{
	AEN_ASSERT_KILL(DxLib_IsInit(), "DxLib が未初期化です。");
	AEN_ASSERT_KILL(str, "null pointer.");
	AEN_ASSERT_KILL(this->m_handle.get(), "handle is unjust.");


	DrawStringToHandle(x, y, str, color, this->m_handle->getHandle());
}


/// ハンドルの開放。
void Text::del(void)
{
	if (!this->m_handle.get()) {
		return;
	}


	this->m_handle.reset();
	this->m_fontSize = 0;
}


} } // namespace aen::gl

