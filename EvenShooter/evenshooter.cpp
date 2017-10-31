/*
 *	EvenShooter
 *	2012 / 03 / 24
 *	jskny
*/


#include	<iostream>
#include	<sstream>
#include	"evenshooter.h"
#include	"scripting.h"


using namespace std;


EvenLogic* EvenShooter = NULL;


EvenLogic::EvenLogic()
{
	this->getManagerEvent()->clear();
	this->getManagerProfile()->clear();
	this->getSystemSprite()->clear();

	this->m_mapObjectGame.clear();


	// クラッシュイベントの受信。
	auto listener = boost::shared_ptr<aen::system::event::Listener>(new EvenShooterEventListener());
	this->getManagerEvent()->addListener(listener, aen::hash::AdlerStringLower("crash"));
}


EvenLogic::~EvenLogic()
{
	this->getManagerEvent()->clear();
	this->getManagerProfile()->clear();
	this->getSystemSprite()->clear();

	this->m_mapObjectGame.clear();
}


// EvenShooter システムイベントリスナー
bool EvenShooterEventListener::handleEvent(aen::system::event::Event& event)
{
cout << "EvenShooterEventListener::handleEvent -> OutputEvent listener : ";
cout << event.getType() << "," << event.getValue() << "," << event.getStrValue() << endl;


	// イベントの中身、
	// type		種類
	// value	数値データ ( 座標など )
	// strValue	文字列データ ( マップ名とか、 )
const	int	eventClash = aen::hash::AdlerStringLower("crash");
	if (event.getType() == eventClash) {
		// Again 強制終了イベント
		EvenShooter->crash(event.getStrValue().c_str());
		return (true);

	}


	return (true);
}


bool EvenLogic::initialize(void)
{
	// 基底クラス呼び出し
aen::Logic::initialize();
	return (true);
}


bool EvenLogic::finalize(void)
{
	// 破棄。
	if (this->m_scripter.isLoad()) {
ScriptingPop();
	}


	// 基底クラス呼び出し
aen::Logic::finalize();
	return (true);
}


// スクリプト・シーン実行
void EvenLogic::run(void)
{
	this->m_scripter.run();
}


bool EvenLogic::setScene(const char* const name)
{
	std::ostringstream oss;
	oss << "./data/scene/" << name << "/scene.txt";


	if (this->m_scripter.isLoad()) {
ScriptingPop();
	}


ScriptingPush();
	SetEvenFunc(&this->m_scripter);
	this->m_scripter.load(oss.str().c_str());
	return (true);
}


// システムループ呼び出し
bool EvenLogic::systemLoop(void)
{
	if (!EvenShooter->getWindow()->windowLoopNonClearNonFlip()) {
		return (false);
	}


	// 入力系の更新。
	Keybord::update();
	// 描画せよ。
	EvenShooter->getSystemSprite()->run();
	// 回る～回るよ～
	EvenShooter->getManagerEvent()->tick(100);
	return (true);
}

