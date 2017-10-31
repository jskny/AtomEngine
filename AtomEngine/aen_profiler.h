/*!
 *	AtomEngine
 *	profiler
 *	@brief		プロファイラー
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#pragma once

#ifndef __ATOM_ENGINE_PROFILER_H_INCLUDE_GUARD_2959183_13_22_10__
#define __ATOM_ENGINE_PROFILER_H_INCLUDE_GUARD_2959183_13_22_10__


#include	<string>
#include	<vector>


#include	<boost/noncopyable.hpp>


namespace aen
{
/// プロファイラーなど
namespace profiler
{


/// プロファイルマネージャー
class Manager
{
public:
	class Data
	{
	friend	class		Manager;


	public:
		/// 経過時刻を取得。
	unsigned long int	getTime(void) const { return (this->m_time); };
		/// 階層を取得。
		int		getStep(void) const { return (this->m_step); };
		/// レベルを取得。
		int		getLevel(void) const { return (this->m_level); };


		/// プロファイル名を取得。
		std::string	getName(void) const { return (this->m_name); };


		/// 出力するときの色を取得
		int		getColor(void) const { return (this->m_color); };


		Data() :
			m_time(0), m_step(0), m_level(0), m_id(0), m_color(0)
		{
		}


	private:
	unsigned long int	m_time;
		int		m_step, m_level, m_id;
		int		m_color;
		std::string	m_name;


	};


	bool		initialize(void);
	bool		finalize(void);


	void		clear(void);
	void		reset(void) { this->clear(); };


	/// 計測を開始する。
	void		push(const char* const profileName, int level = 0, int color = -1);
	/// 一番最後に開始した計測を終了する。
	void		pop(void);
	/// プロファイルを開始する時に呼ぶ。
	void		restart(void) { this->clear(); };


	/// プロファイルリストを取得。
std::vector<Data>	getProfiles(void) const { return (this->m_vectorProfile); };


	Manager()
	{
		this->initialize();
	}
virtual	~Manager()
	{
		this->finalize();
	}


protected:
	// すべてを保存。
std::vector<Data>	m_vectorProfile;
	// スタックの状態のみ保存。
std::vector<Data>	m_stackProfile;


};


/// プロファイラ
class Profile :private boost::noncopyable
{
public:
	/*! プロファイラ
	 * @brief		プロファイラ
	 * @param pManager	プロファイラマネージャーへのポインタ。
	 * @param name		プロファイル名
	 * @param level		プロファイル優先度
	 * @warning		プロファイラマネージャーへのポインタを、
	 *			デストラクタで破棄するまで保持し続けるため、
	 *			最悪の場合 メモリ破壊を引き起こす。
	 *			そもそも、デストラクタで終了処理なので、極めて危険。
	 * @return		なし
	 */
	Profile(Manager* pManager, const char* const name, int level = 0);
	~Profile();


	/// マネージャーにプロファイル結果を報告。
	void		commit(void)
	{
		if (this->m_flagPush && this->m_pManager) {
			this->m_pManager->pop();
		}


		this->m_flagPush = false;
	}


	/// 通常のインスタンスの作成禁止。実態はない。
	Profile();


private:
	bool		m_flagPush;
	Manager*	m_pManager;


};


} } // namespace aen::profiler


#endif // __ATOM_ENGINE_PROFILER_H_INCLUDE_GUARD_2959183_13_22_10__

