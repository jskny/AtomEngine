/*!
 *	AtomEngine
 *	@brief		ハンドルマネージャー
 *	@date		2012 / 03 / 28
 *	@author		jskny
*/


#include	<iostream>
#include	<sstream>
#include	<string>


#include	<hash_map>


#include	"def.h"
#include	"aen_handle.h"
#include	"aen_handle_manager.h"


using namespace std;


namespace aen
{
namespace handle
{


//---------------------------------------------------------
// ハンドル管理
//---------------------------------------------------------


// ハンドルの追加、
// すでに登録されていたファイルの場合、
// 上書き
bool Manager::set(unsigned int key, const boost::shared_ptr<Handle>& handle)
{
	hash_map<unsigned int, boost::weak_ptr<Handle>>::iterator itr;


	if (!handle.get()) {
AEN_PRINTF_DEBUG("null pointer.");
		return (false);
	}


// DebugStreamGL_Start() << "HandleManager::set -> Search : '" << urlFile << "' [ hash : " << num << " ]" << endl;
	itr = this->m_mapHandle.find(key);
	if (itr != this->m_mapHandle.end()) {
AEN_STREAM_LOG_ERROR() << "HandleManager::set -> Find And Update [ hash : " << key << " ] " << this->m_mapHandle[key].use_count() << endl;
		this->m_mapHandle[key] = handle; // weak_ptr
		return (true);
	}


	// 書き込み。
	this->m_mapHandle[key] = handle;
	return (true);
}


// このファイルは登録されているか調べる。
// 登録されていれば true そうでなければ false
bool Manager::search(unsigned int key)
{
	hash_map<unsigned int, boost::weak_ptr<Handle>>::iterator itr;
	unsigned long int num = 0;


	itr = this->m_mapHandle.find(key);
	if (itr != this->m_mapHandle.end()) {
		return (true);
	}
	else {
		return (false);
	}


	return (true);
}


// ハンドルの取得
boost::weak_ptr<Handle> Manager::get(unsigned int key)
{
	hash_map<unsigned int, boost::weak_ptr<Handle>>::iterator itr;
	boost::shared_ptr<Handle> handle;


// DebugStreamGL_Start() << "HandleManager::get -> Search : '" << urlFile << "' [ hash : " << num << " ]" << endl;
	itr = this->m_mapHandle.find(key);
	if (itr != this->m_mapHandle.end()) {
// DebugStreamGL_Start() << "HandleManager::get -> Find [ hash : " << num << " ]" << endl;
		handle = itr->second.lock();
		if (!handle.get()) {
// DebugStreamGL_Start() << "HandleManager::get -> Find -> handle is unjust [ hash : " << num << " ]" << endl;
		}


		return (handle);
	}
	else {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "NotFound [ hash : " << key << " ]" << endl;
		AEN_PRINTF_DEBUG("ハンドルは存在しません。");
		return (handle);
	}


	return (handle);
}


void Manager::clear(void)
{
	for (auto itr = this->m_mapHandle.begin();
		itr != this->m_mapHandle.end();
		itr++) {
		// std::shared_ptr<HandleData> a =
		itr->second.reset();
	}


	this->m_mapHandle.clear();


	return;
}


// ハンドルを破棄。
bool Manager::del(unsigned int key)
{
	hash_map<unsigned int, boost::weak_ptr<Handle>>::iterator itr;


	itr = this->m_mapHandle.find(key);
	if (itr != this->m_mapHandle.end()) {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "HandleManager::del -> dec to ref counter. [ hash : " << key << " ] : rc(" << itr->second.use_count() << ")" << endl;
		this->m_mapHandle.erase(itr);
		return (true);
	}
	else {
AEN_STREAM_LOG_DEBUG(aen::DEBUG_LOG_LEVEL_HANDLE) << "HandleManager::del -> NotFound [ hash : " << key << " ]" << endl;
		AEN_PRINTF_DEBUG("削除しようとしたハンドルは存在しません。");
		return (false);
	}


	return (true);
}


} } // namespace aen::handle


