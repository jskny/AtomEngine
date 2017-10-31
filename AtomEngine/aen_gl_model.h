/*!
 *	AtomEngine
 *	Model
 *	@brief		モデル
 *	@date		2012 / 06 / 04
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_GL_MODEL_H_INCLUDE_GUARD_467291823_6214_1_
#define __ATOM_ENGINE_AEN_GL_MODEL_H_INCLUDE_GUARD_467291823_6214_1_


#include	"aen_handle.h"


namespace aen
{
namespace gl
{


/// 3dモデル
class Model;


/// 3dモデル
class Model
{
public:
	/*! 3dモデル
	 * @brief		モデルを読み込む
	 * @param urlFile	ファイルへのアドレス
	 * @warning		すでにセットされていた場合上書きとなる。
	 * @return		読み込み成功 true 失敗 false
	*/
	bool		load(const char* const urlFile);


	/// モデルがセットされていれば true を返す。
	bool		isLoad(void) const { return (this->m_handle.get() ? true : false); };


	/// モデルハンドルを返す。
	int		getHandle(void)
	{
		if (this->isLoad()) {
			return (this->m_handle->getHandle());
		}


		return (-1);
	}


	void		del(void)
	{
		this->m_handle.reset();
	}


	void		clear(void) { this->del(); };
inline	void		reset(void) { this->del(); };


private:
	boost::shared_ptr<handle::HandleDxLib>	m_handle;


};


} } // namespace aen::gl


#endif // __ATOM_ENGINE_AEN_GL_MODEL_H_INCLUDE_GUARD_467291823_6214_1_

