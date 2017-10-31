/*!
 *	AtomEngine
 *	Screen
 *	@brief		出力先画面
 *	@date		2012 / 04 / 28
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_SCREEN_H_INCLUDE_GUARD_1673531_39684214897321__
#define __ATOM_ENGINE_AEN_GL_SCREEN_H_INCLUDE_GUARD_1673531_39684214897321__


#include	<boost/noncopyable.hpp>
#include	<vector>
#include	"aen_handle.h"


namespace aen
{
namespace gl
{


/// 出力画面
class Screen :private boost::noncopyable
{
public:
	/// 画面作成。
	bool		create(int sizeX, int sizeY, bool useAlpha = false);
	// 破棄。
	void		destroy(void);


	/// 描画対象をこの画面にする。
	void		start(void);
	/// 描画対象を一つ前の状態に戻す。
	void		end(void);
	/// 画面ハンドル取得
	int		get(void) const;
inline	int		getHandle(void) const { return (this->get()); };


	bool		empty(void) const { return (Screen::g_stack.empty()); };
	bool		isSet(void) const { return (this->m_flagSet); };
	bool		isStart(void) const { return (this->m_flagStart); };


	Screen() :
		m_flagStart(false), m_flagSet(false)
	{
	}


	~Screen() {
		if (this->isSet()) { this->destroy(); }
	}


private:
static	std::vector<boost::shared_ptr<aen::handle::HandleDxLib>>	g_stack;
	bool		m_flagSet, m_flagStart;


};


} } // namespace aen::gl


#endif // __ATOM_ENGINE_AEN_GL_SCREEN_H_INCLUDE_GUARD_1673531_39684214897321__

