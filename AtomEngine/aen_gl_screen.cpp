/*!
 *	AtomEngine
 *	Screen
 *	@brief		出力先画面
 *	@date		2012 / 04 / 28
 *	@author		jskny
*/


#include	<sstream>
#include	"def.h"


#include	"aen_gl.h"
#include	"aen_gl_screen.h"


using namespace std;


namespace aen
{
namespace gl
{


std::vector<boost::shared_ptr<aen::handle::HandleDxLib>> Screen::g_stack;


/// 画面作成。
bool Screen::create(int sizeX, int sizeY, bool useAlpha/* = false */)
{
	std::ostringstream oss;
	boost::shared_ptr<aen::handle::HandleDxLib> a(new aen::handle::HandleDxLib(), aen::handle::Deleter());
	a->type = a->HANDLE_TYPE_GRAPHIC;
	a->handle = MakeScreen(sizeX, sizeY, useAlpha);


	if (a->handle == -1) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "Screen::create -> ERROR >> Can not create screen handle." << endl;
AEN_STREAM_LOG_ERROR() << "Screen::create -> ERROR >> Can not create screen handle." << endl;
		AEN_PRINTF_ERROR("can't create 'screen handle'...");
		return (false);
	}


	if (this->isSet()) {
		this->destroy();
	}


	oss << "SCREEN HANDLE [" << sizeX << "," << sizeY << ":" << (useAlpha ? "TRUE" : "FALSE") << "@";
	oss << a->handle << "]";
	a->text = oss.str();
	Screen::g_stack.push_back(a);
	this->m_flagSet = true;


AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "new create. : " << a->handle << endl;
	return (true);
}


/// 終了
void Screen::destroy(void)
{
	if (this->empty()) {
AEN_THROW_EXCEPTION("buffer over flow.");
	}


	Screen::g_stack.pop_back();
	if (this->empty()) {
		// 通常画面。
		SetDrawScreen(DX_SCREEN_BACK);
	}
	else {
		// 描画対象を戻す。
		SetDrawScreen(this->get());
	}


	this->m_flagSet = false;
}


/// 描画対象をこの画面にする。
void Screen::start(void)
{
	// 描画対象画面を変更。
	SetDrawScreen(this->get());


	this->m_flagStart = true;
}


/// 描画対象を一つ前の状態に戻す。
void Screen::end(void)
{
	if (!this->isStart()) {
AEN_THROW_EXCEPTION("not started screen.");
	}



	if (Screen::g_stack.size() < 1) {
AEN_THROW_EXCEPTION("buffer over flow.");
	}
	else if (Screen::g_stack.size() < 2) {
		// 標準へ戻す。
		SetDrawScreen(DX_SCREEN_BACK);
		return;
	}


	// 描画対象を一つ前へ戻す。
	int o = Screen::g_stack.at(Screen::g_stack.size()-2)->getHandle();
	SetDrawScreen(o);
	this->m_flagStart = false;
}


/// 画面ハンドル取得
int Screen::get(void) const
{
	if (this->empty()) {
AEN_THROW_EXCEPTION("buffer over flow.");
	}


	int ret = Screen::g_stack.at(Screen::g_stack.size()-1)->getHandle();
	return (ret);
}


} } // namespace aen::gl

