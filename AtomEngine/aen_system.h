/*!
 *	AtomEngine
 *	system
 *	@brief		ゲームシステムとか。
 *	@date		2012 / 04 / 09
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_SYSTEM_H_INCLUDE_GUARD_387925190345_2_329829630__
#define __ATOM_ENGINE_AEN_SYSTEM_H_INCLUDE_GUARD_387925190345_2_329829630__


#include	<vector>
#include	<list>
#include	<hash_map>


#include	<boost/shared_ptr.hpp>
#include	<boost/noncopyable.hpp>
#include	<boost/thread.hpp>


namespace aen
{
/// ゲームシステムとか楽に作りたいい人向け。
namespace system
{


/// タスクシステム
namespace task
{


/// プロセス
class Process;
/// タスク管理
class Manager;


typedef std::list<boost::shared_ptr<Process>> ProcessList;


/// プロセス
class Process
{
friend	class		Manager;

public:
	unsigned int	getType(void) const { return (this->m_processType); };
	void		setType(unsigned int type) { this->m_processType = type; };


	// 死ぬ～(^^)v : true の時、このプロセスは、マネージャーに消される。
	bool		isKill(void) const { return (this->m_flagKill); };
	void		setKill(bool flag = false) { this->m_flagKill = flag; };


	// タスクリストからこのタスクを消す。
virtual	void		kill(void) { this->m_flagKill = true; };


	// 一時停止中か、否か。
	bool		isPaused(void) const { return (this->m_flagPaused); };
	void		setPaused(bool flag) { this->m_flagPaused = flag; };
	// フラグを変更 true の時は、false、false の時は true にする。
	void		togglePause(void) { this->setPaused(!this->isPaused()); };


	// プロセスがマネージャーに管理されてるか否か、
	// マネージャが、フラグを立てる。
	bool		isAttached(void) const { return (this->m_flagManage); };
	void		setAttached(const bool flag) { this->m_flagManage = flag; };


	// プロセス終了時に実行するプロセスへのポインタを返す。
const	boost::shared_ptr<Process>
			getNext(void) const { return (this->m_ptrNext); };
	// プロセス終了時に実行するプロセスを設定。
	void		setNext(const boost::shared_ptr<Process>& next) { this->m_ptrNext = next; };


	bool		isInit(void) const { return (this->m_flagInit); };
	void		setFlagInit(bool flag) { this->m_flagInit = flag; };


	// これを継承して各タスクを作る。
virtual	void		update(void) = 0;
virtual	void		init(void) = 0;


	Process();
	Process(unsigned int type);
virtual	~Process() {};


protected:
boost::shared_ptr<Process>	m_ptrNext;


private:
	bool		m_flagKill; // 殺すか否か。 true くたばれ。
	bool		m_flagPaused; // 一時停止中か、否か true 停止

	bool		m_flagManage; // マネージャーに管理されてるか否か、 true 管理
	bool		m_flagInit; // 初期化した true


	unsigned int	m_processType; // プロセスの種類。


};


/// タスク管理
class Manager
{
public:
	// -1 の時全部実行。
virtual	void		update(int hNum = -1);
	// 指定したプロセスタイプのみ実行。
	void		update(unsigned int type) { this->update(type); };


	bool		isProcessActive(unsigned int type);
	void		attach(const boost::shared_ptr<Process>& pProcess);


	// 要素は空。
	bool		empty(void) { return (this->m_processList.empty()); };


	// プロセスリストの初期化
	void		reset(void) { this->m_processList.clear(); };
	void		clear(void) { this->reset(); };


	// プロセスリストのサイズ。
	unsigned int	size(void) const { return (this->m_processList.size()); };


	// プロセスリストを取得する
	ProcessList	getProcessList(void) { return (this->m_processList); };


virtual	~Manager() {};


protected:
	ProcessList	m_processList;


private:
	void		detach(boost::shared_ptr<Process> pProcess);


};


#pragma comment(lib, "R:/AtomEngine/trunk/AtomEngine/AtomEngine/3rdParty/boost/boost_thread-vc100-mt-1_47.lib")
/*!
 *	マルチスレッドタスク処理 管理
 *	@brief		タスクシステムのマルチスレッドバージョン 管理
 *	@date		2013 / 02 / 26
 *	@author		jskny
*/
class ManagerMT :public Manager
{
public:
	/// -1 の時全部実行。
	void		update(int hNum = -1);
	/// すべてのスレッドが停止していたら true を返す。
	bool		isStoped(void) const { return (this->m_flagStoped); };
	/// すべてのスレッド停止まで待つ。
	void		wait(void);


protected:
boost::thread_group	m_threads;
	bool		m_flagStoped;


};


inline Process::Process() :
	m_flagKill(false), m_flagPaused(false), m_flagManage(false), m_flagInit(false),
	m_processType(0),
	m_ptrNext()
{
}


inline Process::Process(unsigned int type) :
	m_flagKill(false), m_flagPaused(false), m_flagManage(false), m_flagInit(false),
	m_processType(type)
{
	this->m_ptrNext.reset();
}


} // namespace task


/// イベントシステム
namespace event
{


/// イベント
class Event;
/// イベント受信
class Listener;
/// イベントマネージャー管理
class Manager;


typedef unsigned int EventType;


/// イベント
class Event
{
public:
	// 内部でハッシュ化
	void		setType(const char* const type);
	void		setType(const EventType& type) { this->m_type = type; };
	EventType	getType(void) const { return (this->m_type); };


	// 数値の設定。
	void		setValue(double value) { this->m_value = value; };
	double		getValue(void) const { return (this->m_value); };


	// 文字列の設定。
	void		setStrValue(const std::string& str) { this->m_strValue = str; };
inline	void		setText(const std::string& str) { this->setStrValue(str); }
	std::string	getStrValue(void) const { return (this->m_strValue); };
inline	std::string	getText(void) const { return (this->getStrValue()); };


	// ポインタのセット。
	void		setPtr(void* ptr) { this->m_ptr = ptr; };
	void*		getPtr(void) { return (this->m_ptr); };


	Event() :
		m_value(0.0), m_strValue(""),
		m_ptr(NULL), m_type(0)
	{
	}


	Event(const EventType& t) :
		m_value(0.0), m_strValue(""),
		m_ptr(NULL), m_type(t)
	{
	}


	Event(const char* const hashString);
virtual	~Event() {};


private:
	EventType	m_type;
	double		m_value;
	std::string	m_strValue;
	void*		m_ptr;


};


/// リスナー
class Listener
{
public:
	// 受信機
	// 返却 false の時は、イベントを受け取らなかったことを示す。
	// イベントの伝達は続けられる。
	// 返却 true の時は、イベントを受け取ったことを示す。
	// イベント歩伝達は停止する。
virtual	bool		handleEvent(Event* event) = 0;


	// イベントリスナから、登録を解除するか否か、
	// 解除 true
	bool		isKill(void) const { return (this->m_flagListenerKill); };
	// イベントリスナから登録を解除する。
	void		kill(void) { this->m_flagListenerKill = true; };


	Listener() :
		m_flagListenerKill(false)
	{
	}
virtual	~Listener() {};


private:
	bool		m_flagListenerKill;


};


// 純粋仮想関数呼び出しにしないように。
inline bool Listener::handleEvent(Event* event)
{
	return (true);
}


/// リスナーマネージャー
class Manager :private boost::noncopyable
{
public:
virtual	~Manager() {};


	// リスナーを追加
	bool		addListener(const boost::shared_ptr<Listener>& handle, const EventType& type);
	bool		addListener(const EventType& type, const boost::shared_ptr<Listener>& handle) { return (this->addListener(handle, type)); };
	// リスナーを外す。
	bool		delListener(const boost::shared_ptr<Listener>& handle, const EventType& type);
	bool		delListener(const EventType& type, const boost::shared_ptr<Listener>& handle) { return (this->delListener(handle, type)); };

	// リスナーリストのクリア
	void		clear(void)
	{
		this->m_queue.clear();
		this->m_registry.clear();
	}


	// イベントの発信
	//
	// 強制的に受信させる。
	// イベントをリスナーが受診すれば true しなければ false
	bool		trigger(const boost::shared_ptr<Event>& event);
	bool		trigger(const Event& event)
	{
		auto p = boost::shared_ptr<Event>(new Event(event));
		return (this->trigger(p));
	}


	// イベントをイベントキューに追加。
	void		push(const Event& event)
	{
		auto p = boost::shared_ptr<Event>(new Event(event));
		this->m_queue.push_back(p);
	}


	// イベントをイベントキューに追加。
	void		push(const boost::shared_ptr<Event>& event)
	{
		if (event.get() == NULL) {
			// null は無視。
			return;
		}


		this->m_queue.push_back(event);
	}


	// 選択したイベントを
	// 送信待ちのキューから削除する。
	// allOfType が true なら、そのイベント型はすべてキューから削除される。
	bool		abortEvent(const EventType& eventType, bool allOfType = false);


	// イベント送信。
	// キューにあるイベントを送信する。
	//
	// maxNum :	指定した数回イベントを処理する。
	//		イベントの数が、指定した数より少なければ、スルー。
	//		イベントの数が、指定した数よりも多ければ、未処理のイベントが残る。
	//
	// 処理待ちのすべてのイベントを処理できれば true、
	// そうでなければ false を返す。
	bool		tick(unsigned long int maxNum = 0xFFFFFFFF);


	// イベントリスナーがそのイベントに登録されているか調べる。
	// 登録されている true, されていない false
	bool		isRegistry(const EventType& t, const boost::shared_ptr<Listener>& l);
	bool		isRegistry(const boost::shared_ptr<Listener>& l, const EventType& t) { return (this->isRegistry(t, l)); };
	bool		isReg(const EventType& t, const boost::shared_ptr<Listener>& l) { return (this->isRegistry(t, l)); };
	bool		isReg(const boost::shared_ptr<Listener>& l, const EventType& t) { return (this->isRegistry(t, l)); };


//---------------------------------------------------------

protected:
	// イベントリスナーリスト。
std::hash_map<EventType, std::list<boost::shared_ptr<Listener>>> m_registry;

	// 二重バッファのイベントキュー
std::vector<boost::shared_ptr<Event>>	m_queue;


};


} // namespace event


} } // namespace aen


#endif // __ATOM_ENGINE_AEN_SYSTEM_H_INCLUDE_GUARD_387925190345_2_329829630__

