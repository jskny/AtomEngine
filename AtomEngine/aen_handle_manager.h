/*!
 *	AtomEngine
 *	@brief		ハンドルマネージャー
 *	@date		2012 / 03 / 24
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_HANDLE_MANAGER_H_INCLUDE_GUARD_63_31_310__
#define __ATOM_ENGINE_HANDLE_MANAGER_H_INCLUDE_GUARD_63_31_310__


#include	<hash_map>


#include	<boost/noncopyable.hpp>
#include	<boost/shared_ptr.hpp>
#include	<boost/weak_ptr.hpp>


#include	"aen_handle.h"


namespace aen
{
namespace handle
{


/// ハンドルマネージャー
class Manager;


class Manager
{
public:
	// ハンドルの追加、
	// すでに登録されていたファイルの場合、
	// 上書き
	bool		set(unsigned int key, const boost::shared_ptr<Handle>& handle);


	// このファイルは登録されているか調べる。
	// 登録されていれば true そうでなければ false
	bool		search(unsigned int key);
	bool		isReg(unsigned int key) { return (this->search(key)); };


	// ハンドルの取得
	// handle = man.get("hash");
	// if (!handle.get()) { err; }
boost::weak_ptr<Handle>	get(unsigned int key);
	// ハンドルを破棄。
	bool		del(unsigned int key);


	void		clear(void);


private:
	/// ハッシュキー, ハンドル
std::hash_map<unsigned int, boost::weak_ptr<Handle>>	m_mapHandle;


};


} } // namespace aen::handle


#endif // __ATOM_ENGINE_HANDLE_MANAGER_H_INCLUDE_GUARD_63_31_310__

