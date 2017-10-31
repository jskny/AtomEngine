/*
 *	EvenShooter
 *	AtomEngine を使ったゲーム。
 *	ライブラリ作りって、虚しい。
 *	だからゲームと一緒に作る。
 *	2012 / 03 / 24
 *	jskny
*/


#include	<iostream>
#include	<aen.h>
#pragma	comment(lib, "AtomEngine.lib")


#include	"evenshooter.h"
#include	"g_title.h"
#include	"scripting.h"


using namespace std;


bool g_flagFullScreen = false;


// コマンドラインオプションを処理。
void RunCommandLineOption(int argc, char* argv[])
{
	vector<string> buf;
	// バッファに push
	for (int i = 0; i < argc; i++) {
		if (argv[i]) {
			buf.push_back(argv[i]);
		}
	}


	for (auto itr = buf.begin(); itr != buf.end(); itr++) {
		if (	// help コマンド
			aen::util::CompareStringA((*itr), "help") ||
			aen::util::CompareStringA((*itr), "--help") ||
			aen::util::CompareStringA((*itr), "-h") ||
			aen::util::CompareStringA((*itr), "--h") ||
			aen::util::CompareStringA((*itr), "/h") ||
			aen::util::CompareStringA((*itr), "/?")
		) {
//			OutputHelp();
		}
		else if (
			aen::util::CompareStringA((*itr), "full") ||
			aen::util::CompareStringA((*itr), "--full") ||
			aen::util::CompareStringA((*itr), "-full") ||
			aen::util::CompareStringA((*itr), "/full")
		) {
			// フルスクリーン。
			g_flagFullScreen = true;
cout << "option : full screen." << endl;
		}
		else if (
			aen::util::CompareStringA((*itr), "sound-off") ||
			aen::util::CompareStringA((*itr), "--sound-off") ||
			aen::util::CompareStringA((*itr), "-sound-off") ||
			aen::util::CompareStringA((*itr), "/sound-off")
		) {
			// サウンド再生禁止。
//			SoundPlayer::setFlagMute(true);
cout << "option : sound off." << endl;
		}
	}


	return;
}


int main(int argc, char* argv[])
{
	RunCommandLineOption(argc, argv);


try {
	// ロジック作成。
	EvenShooter = new EvenLogic();
	EvenShooter->initialize();
ScriptingPush();


	EvenShooter->openWindow("EvenShooter", WINDOW_W, WINDOW_H, g_flagFullScreen);


	// config.js 実行
{
	aen::scripter::ScripterV8 scripter;
	SetEvenFunc(&scripter);
	scripter.load("./data/config.js");
	scripter.run();

/*	{
		aen::scripter::ScripterV8 scripter;
		SetEvenFunc(&scripter);
		scripter.load("./data/config.js");
		scripter.run();
	}


	scripter.run();*/
}


	EvenShooter->getWindow()->setFps(60);
//	EvenShooter->getWindow()->setScene(new GTitle());
	EvenShooter->setScene("title");
	EvenShooter->run();


/*	while (EvenShooter->getWindow()->windowLoop()) {
		// 入力系の更新。
		Keybord::update();
		// 描画せよ。
		EvenShooter->getSystemSprite()->run();


		// 回る～回るよ～
		EvenShooter->getManagerEvent()->tick(100);
	}*/


ScriptingPop();
	EvenShooter->closeWindow();
	EvenShooter->finalize();


	// 破棄
	AEN_SAFE_DELETE(EvenShooter);
}
catch (aen::Exception exp) {
	exp.serialize(cerr);


	EvenShooter->finalize();
	AEN_SAFE_DELETE(EvenShooter);
	return (1);
}

	return (0);
}

