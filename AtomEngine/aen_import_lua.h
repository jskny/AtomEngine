/*!
 *	AtomEngine
 *	@brief		LuaPlus を取り込む。
 *	@date		2012 / 05 / 26
 *	@author		jskny
*/


/*
 *	取り込むのは LuaPlus の DLL バージョンなので、
 *	3rdParty/LuaPlus/luaplus_1100.dll を実行ファイルのあるフォルダにコピーする必要がある。
 *	Static 版は jam の使い方わからず挫折・・・
 *	2012 / 05 / 26
*/


#pragma once

#ifndef __ATOM_ENGINE_3rdParty_IMPORT_LUA_H_INCLUDE_GUARD_70214893_4601_1__
#define __ATOM_ENGINE_3rdParty_IMPORT_LUA_H_INCLUDE_GUARD_70214893_4601_1__


#include	"3rdParty/LuaPlus/LuaPlus.h"
#include	"3rdParty/LuaPlus/LuaObject.h"


// DLL ver
#pragma comment(lib, "R:/AtomEngine/trunk/AtomEngine/AtomEngine/3rdParty/LuaPlus/luaplus_1100.lib")
// #pragma comment(lib, "3rdParty/LuaPlus/luaplus_1100.lib")
// Static Ver ・・・ いろいろめんどい。
// #pragma comment(lib, "luaplusstatic.lib")


#endif // __ATOM_ENGINE_3rdParty_IMPORT_LUA_H_INCLUDE_GUARD_70214893_4601_1__

