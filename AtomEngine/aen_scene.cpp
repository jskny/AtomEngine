/*!
 *	AtomEngine
 *	シーン
 *	@brief		シーン。
 *	@date		2012 / 03 / 20
 *	@author		jskny
*/


#include	"def.h"
#include	"aen_scene.h"


using namespace std;


namespace aen
{
namespace scene
{


// 画面変更開始前の一番最初の画面をセットする
void Manager::setSequence(Scene* ss)
{
	AEN_ASSERT_KILL(ss, "null pointer.");


	if (ss) {
		this->m_pSequence = boost::shared_ptr<Scene>(ss);
	}
	else {
		AEN_PRINTF_ERROR("シーケンスが不正です。");
AEN_STREAM_LOG_ERROR() << "設定しようとしたsequenceは不正です。" << endl;
	}


	return;
}


// 画面切り替えなど、
void Manager::update(void)
{
	AEN_ASSERT_KILL(this->m_pSequence.get(), "null pointer.");
	if (!this->m_pSequence.get()) {
AEN_STREAM_LOG_ERROR() << "scene is nullpointer." << endl;
AEN_THROW_EXCEPTION("scene is nullpointer.");
	}


	// 初期化していなければ初期化する。
	if (!this->m_flagInit) {
		this->m_pSequence->init();
		this->m_flagInit = true;
	}


	Scene* return_sequence = this->m_pSequence->update();


	/// 返却値が null は不正だろ。
	if (!return_sequence) {
		AEN_PRINTF_ERROR("null pointer.");
AEN_STREAM_LOG_ERROR() << "sequenceの返却値は null pointer です。" << endl;
		return;
	}


	// 返ってきたのが 前と同じでなければ delete 後書き換え。
	if (this->m_pSequence.get() != return_sequence) {
		this->m_pSequence = boost::shared_ptr<Scene>(return_sequence);
		this->m_flagInit = false;
	}


	return;
}



} } // namespace aen::scene


