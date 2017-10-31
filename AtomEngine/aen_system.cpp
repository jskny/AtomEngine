/*!
 *	AtomEngine
 *	system
 *	@brief		ゲームシステムとか。
 *	@date		2012 / 04 / 09
 *	@author		jskny
*/


#include	<iostream>
#include	"def.h"
#include	"aen_system.h"
#include	"aen_hash.h"


using namespace std;


namespace aen
{
/// ゲームシステムとか楽に作りたいい人向け。
namespace system
{


/// タスクシステム
namespace task
{


void Manager::update(int hNum)
{
	ProcessList::iterator i = this->m_processList.begin();
	boost::shared_ptr<Process> pNext;


	while (i != this->m_processList.end()) {
		if (!i->get()) {
			i = this->m_processList.erase(i);
			continue;
		}


		boost::shared_ptr<Process> p( *i );
		if (hNum != -1 && p->getType() != hNum) {
			i++;
			continue;
		}


		if (p->isKill()) {
			pNext = p->getNext();
			if (pNext.get()) {
				p->setNext(boost::shared_ptr<Process>(reinterpret_cast<Process *>(NULL)));
				// 追加。
				pNext->setAttached(true);
				i = this->m_processList.insert(i, pNext);
				continue;
			}


			// 破棄処理。
			p->setAttached(false);
			i = this->m_processList.erase(i);
			continue;
		}
		else if (!p->isPaused()) {
			// 起動中で、止まっていなけりゃ起動。
			if (!p->isInit()) {
				p->init();
				p->setFlagInit(true);
			}
			p->update();
		}


		// イテレータをすすめる。
		// 絶対に fo 文にするな。
		// 追加処理と、破棄処理で、イテレータを変更するが、
		// for (i = begin; i != end; i++) の
		// i++ で、あら不思議。(^_^)
		++i;
	}


	return;
}


bool Manager::isProcessActive(unsigned int type)
{
	for(ProcessList::iterator i = this->m_processList.begin(); i != this->m_processList.end(); i++) {
#ifdef ATOM_ENGINE_DEBUG
		AEN_ASSERT_KILL(i->get(), "null pointer.");
#else
		if (!i->get()) {
			i = this->m_processList.erase(i);
			continue;
		}
#endif

		// チェック
		if ((*i)->getType() == type && ( (*i)->isKill() == false || (*i)->getNext() )) {
			 return (true);
		}
	}


	return (false);
}


// 要素の追加
void Manager::attach(const boost::shared_ptr<Process>& pProcess)
{
	AEN_ASSERT_KILL(pProcess.get(), "null pointer.");
	if (!pProcess.get()) {
		return;
	}


	if (!pProcess->isInit()) {
		pProcess->init();
		pProcess->setFlagInit(true);
	}


	this->m_processList.push_back(pProcess);
	pProcess->setAttached(true);
}


// 要素の消去
void Manager::detach(boost::shared_ptr<Process> pProcess)
{
	AEN_ASSERT_KILL(pProcess.get(), "null pointer.");
	if (!pProcess.get()) {
		return;
	}


	this->m_processList.remove(pProcess);
	pProcess->setAttached(false);
}


//---------------------------------------------------------
// マルチスレッド化
//---------------------------------------------------------


class __DummyProcess
{
public:
	// コンストラクタ
	__DummyProcess(Process* ptr) :
		p(ptr)
	{
	}


	// 関数オブジェクトのoperator()
	inline void operator()()
	{
		p->update();
	}


private:
	Process*	p;

};


void ManagerMT::update(int hNum/* = -1*/)
{
	ProcessList::iterator i = this->m_processList.begin();
	boost::shared_ptr<Process> pNext;


	// スレッド停止。
	this->wait();
	this->m_flagStoped = false;


	while (i != this->m_processList.end()) {
		if (!i->get()) {
			i = this->m_processList.erase(i);
			continue;
		}


		boost::shared_ptr<Process> p( *i );
		if (hNum != -1 && p->getType() != hNum) {
			i++;
			continue;
		}


		if (p->isKill()) {
			pNext = p->getNext();
			if (pNext.get()) {
				p->setNext(boost::shared_ptr<Process>(reinterpret_cast<Process *>(NULL)));
				// 追加。
				pNext->setAttached(true);
				i = this->m_processList.insert(i, pNext);
				continue;
			}


			// 破棄処理。
			p->setAttached(false);
			i = this->m_processList.erase(i);
			continue;
		}
		else if (!p->isPaused()) {
			// 起動中で、止まっていなけりゃ起動。
			if (!p->isInit()) {
				p->init();
				p->setFlagInit(true);
			}
			// p->update();

			__DummyProcess __tmp(p.get());
			// これでマルチスレッドでける？
			this->m_threads.create_thread(__tmp);
		}


		++i;
	}


	return;
}


void ManagerMT::wait(void)
{
	this->m_threads.join_all();
	this->m_flagStoped = true;
}


} // namespace task


/// イベントシステム
namespace event
{


Event::Event(const char* const hashString) :
		m_value(0.0), m_strValue(""),
		m_ptr(NULL), m_type(0)
{
	if (hashString) {
		this->m_type = hash::AdlerStringLower(hashString);
	}


}


void Event::setType(const char* const type)
{
	if (type) {
		this->m_type = aen::hash::AdlerStringLower(type);
	}


}


// イベントを追加
bool Manager::addListener(const boost::shared_ptr<Listener>& handle, const EventType& type)
{
	if (!handle.get()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::addListener -> addListener is null pointer." << endl;
		return (false);
	}


	auto itr = this->m_registry.find(type);
	// そもそも、マップのキーが作成されていない場合。
	if (itr == this->m_registry.end()) {
		this->m_registry[type] = std::list<boost::shared_ptr<Listener>>();
		this->m_registry[type].push_back(handle);
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::addListener -> add listener finish. : " << handle.get() << endl;
		return (false);
	}


	auto itr2 = itr->second.begin();


	// 同じのがないか検索。
	// TODO:	hash と、 map の組み合わせのほうが絶対に早いのだろうが、
	//		さっさと作りたいので、それは、今後実装する。要は、後回し。
	while (itr2 != itr->second.end()) {
		// 二重登録か？
		if (handle.get() == itr2->get()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::addListener -> search to hit. double booking." << endl;
			return (false);
		}
		else {
			++itr2;
		}
	}


	if (itr2 == this->m_registry[type].end()) {
		this->m_registry[type].push_back(handle);
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::addListener -> add listener finish. : " << handle.get() << endl;
	}


	return (true);
}


// 内部テーブルから、リスナーとイベント型のペアを外す。
bool Manager::delListener(const boost::shared_ptr<Listener>& handle, const EventType& type)
{
	if (!handle.get()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::delListener -> addListener is null pointer." << endl;
		return (false);
	}


	// 返却値。
	bool rc = false;


	// リスナーをマップリストから消す。
	auto itr = this->m_registry.find(type);
	// そもそも、マップのキーが作成されていない場合。
	if (itr == this->m_registry.end()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::delListener -> can not find EventListener. " << type << endl;
		return (false);
	}


	auto itr2 = itr->second.begin();


	// 同じのがないか検索。
	// TODO:	hash と、 map の組み合わせのほうが絶対に早いのだろうが、
	//		さっさと作りたいので、それは、今後実装する。要は、後回し。
	while (itr2 != itr->second.end()) {
		// 消す。
		if (handle.get() == itr2->get()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::delListener -> search to hit. Let's delete !! ( rc" << itr2->use_count() << ", " << itr2->get() << " )" << endl;
			rc = true;
			itr2 = this->m_registry[type].erase(itr2);

			// なぜ、ここで、 return しないのか、
			// その理由は、add 絶対二重登録がないと 100% 保証できないから。
			continue;
		}


		++itr2;
	}


	return (rc);
}


// イベントの発信 ( 同期 )
// イベントをリスナーが受診すれば true しなければ false
bool Manager::trigger(const boost::shared_ptr<Event>& event)
{
AEN_NULL_TO_THROW_EXCEPTION_PARAM(event.get());


	auto itr = this->m_registry.find(event->getType());
	// そもそも、マップのキーが作成されていない場合。
	if (itr == this->m_registry.end()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::trigger -> can not find EventListener. " << event->getType() << endl;
		return (false);
	}


	auto itr2 = itr->second.begin();
	bool rc = false;


	// 同じのがないか検索。
	// TODO:	hash と、 map の組み合わせのほうが絶対に早いのだろうが、
	//		さっさと作りたいので、それは、今後実装する。要は、後回し。
	while (itr2 != itr->second.end()) {
		// 送信。
		if (itr2->get()) {
			bool brc = (*itr2)->handleEvent(event.get());

			// true なら、受信完了。
			if (brc) {
				// なぜ、 return しないのかというと、
				// この関数でイベントが発信される時って、
				// gameover とか全部のリスナーに一応伝えるための、
				// メッセージだと思うから。
				rc = true;
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::trigger -> transmission to event. : " << event->getType() << ", '" << event->getValue() << "'" << ", '" << event->getStrValue() << "'" << endl;
			}


			// 破棄するか？
			if ((*itr2)->isKill()) {
				itr2 = this->m_registry[event->getType()].erase(itr2);
				continue;
			}
		}
		else {
			// null pointer は気づかぬうちに、消えてゆく・・・
			itr2 = this->m_registry[event->getType()].erase(itr2);
			continue;
		}


		++itr2;
	}


	return (rc);
}


// 選択したイベントを
// 送信待ちのキューから削除する。
// allOfType が true なら、そのイベント型はすべてキューから削除される。
bool Manager::abortEvent(const EventType& eventType, bool allOfType)
{
	bool rc = false;


	auto itr = this->m_queue.begin();
	while (itr != this->m_queue.end()) {
		if (itr->get() == NULL) {
			itr = this->m_queue.erase(itr);
			continue;
		}


		if ((*itr)->getType() == eventType) {
			itr = this->m_queue.erase(itr);
			rc = true;
			if (!allOfType) {
				break;
			}
		}


		++itr;
	}


	return (rc);
}


// イベント送信。
// キューにあるイベントを送信する。
//
// maxNum :	指定した数回イベントを処理する。
//		イベントの数が、指定した数より少なければ、スルー。
//		イベントの数が、指定した数よりも多ければ、未処理のイベントが残る。
//
// 処理待ちのすべてのイベントを処理できれば true、
// そうでなければ false を返す。
bool Manager::tick(unsigned long int maxNum)
{
	if (this->m_registry.empty()) {
// DebugStream() << "EventManager::tick -> can not find EventListener." << endl;
		return (false);
	}


	auto itrQueue = this->m_queue.begin();
	bool rc = false;
	int counter = 0;


	while (itrQueue != this->m_queue.end()) {
		if (itrQueue->get() == NULL) {
			// null はスルー
			// どのみち下でクリアされるので erase は行わない。
			++itrQueue;
			continue;
		}


		auto itr = this->m_registry.find((*itrQueue)->getType());
		if (itr == this->m_registry.end()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::tick -> can not find EventListener. " << (*itrQueue)->getType() << endl;
			++itrQueue;
			continue;
		}


		auto itr2 = itr->second.begin();
		while (itr2 != itr->second.end()) {
			// 送信
			if (itr2->get()) {
				bool brc = (*itr2)->handleEvent(itrQueue->get());

				// true なら、受信完了。
				if (brc) { rc = true; }

				// 破棄するか？
				if ((*itr2)->isKill()) {
					itr2 = this->m_registry[(*itrQueue)->getType()].erase(itr2);
					continue;
				}
			}
			else {
				// null pointer は気づかぬうちに、消えてゆく・・・
				itr2 = this->m_registry[(*itrQueue)->getType()].erase(itr2);
				continue;
			}


// DebugStream() << "EventManager::trigger -> transmission to event. : ";
// 6DebugStream() << itrQueue->getType().getIdent() << ", '" << itrQueue->getType().getStr() << "'" << endl;
			++itr2;
		}


		++itrQueue;
		// 終了 OK ?
		if (counter > maxNum) {
			// 全部に送信されているか？
			if (itrQueue == this->m_queue.end()) {
				break;
			}

			return (false);
		}


		++counter;
	}


	// キューの初期化。
	this->m_queue.clear();
	return (rc);
}


// イベントリスナーがそのイベントに登録されているか調べる。
// 登録されている true, されていない false
bool Manager::isRegistry(const EventType& t, const boost::shared_ptr<Listener>& l)
{
	bool ret = false;
	// リスナーをマップリストから消す。
	auto itr = this->m_registry.find(t);


	// そもそも、マップのキーが作成されていない場合。
	if (itr == this->m_registry.end()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_GAMESYSTEM) << "EventManager::isRegistry -> can not find EventListener. " << t << endl;
		return (false);
	}


	// リスナーを逐一見ていってあれば消す。
	auto vec = itr->second;
	for (auto itrV = vec.begin(); itrV != vec.end(); ++itrV) {
		if ((*itrV) == l) {
			// 同じー消すー
			vec.erase(itrV);
			ret = true;
			break;
		}

	}


	// 書き戻し。
	if (ret) {
		this->m_registry[t] = vec;
	}


	return (ret);
}


} // namespace event


} } // namespace aen

