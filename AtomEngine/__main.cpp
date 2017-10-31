/*
 *	AtomEngine
 *	Code name : aen
 *	極小エンジン。
 *	2012 / 03 / 09
 *	jskny
*/


#include	<iostream>
#include	"aen.h"


using namespace std;


// 確認は、EvenShooter でやる。
#if 0

// テスト。
class GTitle :public aen::scene::Scene
{
private:
	aen::scene::Scene* update(void)
	{
		aen::scene::Scene* next = this;


		{
			aen::profiler::Profile a(&this->m_manePro, "test");
			for (int i = 0; i < 3; i++) {
				aen::profiler::Profile a(&this->m_manePro, "for Y");
				for (int j = 0; j < 2; ++j) {
					aen::profiler::Profile a(&this->m_manePro, "for X");
				}

				Sleep(1);
			}
		}


		{
			aen::profiler::Profile a(&this->m_manePro, "test2");
			DrawBox(0, 0, 320, 240, GetColor(0xA0, 0xA0, 0xE0), TRUE);
		}


/*
		// プロファイル結果の出力。
		auto vec = this->m_manePro.getProfiles();
		for (auto itr = vec.begin(); itr != vec.end(); ++itr) {
			for (int i = 0; i < itr->getStep()+1; ++i) {
				if (i != 0) {
					cout << "-";
				}
				else {
					cout << ":";
				}
			}


			cout << itr->getName() << " [" << itr->getTime() << "]" << endl;
		}*/


		// プロファイルのリスタート。
		this->m_manePro.restart();
		return (next);
	}

	void init(void)
	{
		cout << "INIT" << endl;
		this->m_manePro.initialize();
	}


	~GTitle() {
		cout << "ENDE" << endl;
	}


private:
	aen::profiler::Manager m_manePro;


};


// aen::handle::Handle のテスト。
void HandleTest(void)
{
class MyData :public aen::handle::Handle
{
public:
	MyData(int n) { dat=n; cout << "コンストラクタ :" << dat << endl; }
	~MyData() { cout << "デストラクタ :" << dat << endl; }


	void		destroy(void)
	{
		cout << "完全破棄" << this->dat << endl;
	}


private:
	int dat;


};


	boost::shared_ptr<MyData> dat(new MyData(100), aen::handle::Deleter());
	boost::shared_ptr<MyData> dat2(new MyData(200), aen::handle::Deleter());
	dat2 = dat;
	{
		boost::shared_ptr<MyData> dat(new MyData(300), aen::handle::Deleter());
	}


	return;
}


int main(int argc, char* argv[])
{
try {
	aen::Initialize();
	aen::draw::Window window;
	window.initialize();
	window.open("Hello World !", 640, 480, false);
	window.setScene(new GTitle());


	HandleTest();


	// test hash
	cout << aen::hash::Adler((unsigned char*)"Hello C++ World !!") << endl;


	// 同じ。
cout << aen::hash::AdlerStringLower(string("hEllO").c_str()) << endl;
cout << aen::hash::Adler((unsigned char*)string("hello").c_str()) << endl;


window.setFps(30);
	// window loop
	while (window.windowLoop()) {
cout << "i am live : " << window.getFtime() << endl;
cout << window.getFps() << ":" << window.getFpsAverage() << endl;
	}


	window.finalize();
	aen::Finalize();
}
catch (aen::Exception exp) {
	exp.output(cout);
	aen::Finalize();
}


	return (0);
}


#endif // #if 0

