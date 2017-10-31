/*!
 *	AtomEngine
 *	handle
 *	@brief		AtomEngine で使用するハンドルの基底クラスなど。
 *	@date		2012 / 03 / 10
 *	@author		jskny
*/


/*
 *	2012 / 03 / 12
 *	もういいや。
 *	boost::shared_ptr よすべてお前に任せた。
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_HANDLE_H_INCLUDE_GUARD_176832_571302_55_1093_2_32798_2__
#define __ATOM_ENGINE_AEN_HANDLE_H_INCLUDE_GUARD_176832_571302_55_1093_2_32798_2__


#include	<string>
#include	<boost/shared_ptr.hpp>


namespace aen
{
/// ハンドル関連
namespace handle
{


// ハンドルオブジェクト
// これを継承して作れ。
class Handle;
/// カスタムデリータ
class Deleter;


/// DxLib が作成したハンドル
class HandleDxLib;


class Handle
{
public:
	/// boost::shared_ptr の参照カウントが 0 になった時に呼ばれる。
virtual	void	destroy(void) = 0;


};


class Deleter
{
public:
	/// boost::shared_ptr にセットするカスタムデリータ
virtual	void operator ()(Handle* handle)
	{
		if (handle) {
			handle->destroy();
			delete (handle);
		}

	}


};


/// DxLib が作成したハンドル
class HandleDxLib :public Handle
{
public:
	// ハンドルの種類。
	enum TypeHandle {
		HANDLE_TYPE_UNKNOWN = 0,
		HANDLE_TYPE_GRAPHIC = 100,
		HANDLE_TYPE_SOUND = 200,
		HANDLE_TYPE_FONT = 300,
		HANDLE_TYPE_MODEL = 400
	};


	/// type を見て、破棄動作を行う。
virtual	void	destroy(void);


	HandleDxLib() :
		handle(0),
		type(HANDLE_TYPE_UNKNOWN)
	{
	}


inline	int		getHandle(void) const { return (this->handle); };
inline	TypeHandle	getType(void) const { return (this->type); };
inline	std::string	getText(void) const { return (this->text); };


	int		handle;
	TypeHandle	type;
	std::string	text;


};


//---------------------------------------------------------


// 純粋仮想関数の中身。
// boost::shared_ptr の参照カウントが 0 になった時に呼ばれる。
inline void Handle::destroy(void)
{
}


} } // namespace aen::handle


#endif // __ATOM_ENGINE_AEN_HANDLE_H_INCLUDE_GUARD_176832_571302_55_1093_2_32798_2__

