/*!
 *	AtomEngine
 *	profiler
 *	@brief		プロファイラー
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#include	<cassert>


// 仕方がないが、 Windows プログラミングだ。
// timeGetTime 関数の代わりになるものを見つけてくれ。
#include	<windows.h>
#include	<MMSystem.h>
#pragma comment(lib, "winmm.lib")


#include	"def.h"
#include	"aen_profiler.h"


using namespace std;


namespace aen
{
namespace profiler
{


bool Manager::initialize(void)
{
	this->clear();
	return (true);
}


bool Manager::finalize(void)
{
	this->clear();
	return (true);
}


void Manager::clear(void)
{
	this->m_vectorProfile.clear();
	this->m_stackProfile.clear();
}


/// 計測を開始する。
void Manager::push(const char* const profileName, int level/* = 0 */, int color/* = -1 */)
{
	Manager::Data a;


	a.m_level = level;
	a.m_name = string(profileName);
	a.m_time = timeGetTime();
	a.m_step = this->m_stackProfile.size();
	a.m_id = this->m_vectorProfile.size();


	if (color == -1) {
		a.m_color = GetColor(0xFF, 0x00, 0x00);
	}
	else {
		a.m_color = color;
	}


	this->m_vectorProfile.push_back(a);
	this->m_stackProfile.push_back(a);
}


/// 一番最後に開始した計測を終了する。
void Manager::pop(void)
{
	if (this->m_stackProfile.size() <= 0) {
		assert(!this->m_stackProfile.empty() && "stack over flow.");
AEN_STREAM_LOG_ERROR() << "stack over flow." << endl;
		return;
	}


	int index = this->m_stackProfile.at(this->m_stackProfile.size() - 1).m_id;
	unsigned long int t = this->m_vectorProfile.at(index).getTime();
	this->m_vectorProfile.at(index).m_time = timeGetTime() - t;
	this->m_stackProfile.pop_back();
}


/*! プロファイラ
 * @brief		プロファイラ
 * @param pManager	プロファイラマネージャーへのポインタ。
 * @param name		プロファイル名
 * @param level		プロファイル優先度
 * @return		なし
 */
Profile::Profile(Manager* pManager, const char* const name, int level) :
	m_flagPush(false),
	m_pManager(pManager)
{
	if (!pManager) {
AEN_ASSERT_KILL(pManager, "manager is null pointer.");
		return;
	}
	else if (!name) {
AEN_ASSERT_KILL(name, "name is null pointer.");
		return;
	}


	pManager->push(name, level);
	this->m_flagPush = true;

}


Profile::~Profile()
{
	if (this->m_flagPush && this->m_pManager) {
		this->m_pManager->pop();
	}


	this->m_flagPush = false;

}


} } // namespace aen::profiler

