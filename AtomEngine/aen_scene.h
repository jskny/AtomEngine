/*!
 *	AtomEngine
 *	シーン
 *	@brief		シーン。
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_AEN_SCENE_H_INCLUDE_GUARD_910_277722_439183_3__
#define __ATOM_ENGINE_AEN_SCENE_H_INCLUDE_GUARD_910_277722_439183_3__


#include	<boost/shared_ptr.hpp>


namespace aen
{
/// シーン関連
namespace scene
{


/// 画面変化のシーケンス ( 画面出力、これを継承して各画面を作る。 )
class Scene;
/// シーンマネージャー
class Manager;


/// 画面変化のシーケンス ( 画面出力、これを継承して各画面を作る。 )
class Scene
{
public:
virtual		~Scene() = 0;

	/*! 各画面はこれを継承して作る。
	 * @brief		すべての画面の基底クラス。
	 * @return		通常 true, シーンが変わるときは、 false
	 */
virtual	Scene*	update(void) = 0;
	/// シーケンス起動時に呼ばれる。
virtual	void	init(void) = 0;


};


/// シーンマネージャー
class Manager
{
public:
	// 画面変更開始前の一番最初の画面をセットする
	void		setSequence(Scene* ss);
	// 画面切り替えなど、
	void		update(void);
	// シーンの消去。
	void		deleteScene(void) { this->m_pSequence.reset(); this->m_flagInit = false; }


	Manager() :
		m_flagInit(false)
	{
		this->m_pSequence.reset();
	}

virtual	~Manager(){};


private:
	// 現在のシーケンスを指す。
boost::shared_ptr<Scene>	m_pSequence;
	bool		m_flagInit;


};


//---------------------------------------------------------


inline Scene::~Scene()
{
}


inline Scene* Scene::update(void)
{
	return (this);
}


inline void Scene::init(void)
{
}



} } // namespace aen::scene


#endif // __ATOM_ENGINE_AEN_SCENE_H_INCLUDE_GUARD_910_277722_439183_3__

