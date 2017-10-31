/*
 *	EvenShooter
 *	スクリプティング
 *	2012 / 04 / 15
 *	jskny
*/


#include	<iostream>
#include	<vector>
#include	<aen_hash.h>
#include	"scripting.h"
#include	"evenshooter.h"


using namespace std;
using aen::scripter::ScripterV8;


// ハンドル表
vector<vector<boost::shared_ptr<aen::gl::Picture>>> g_listPicture;
vector<vector<boost::shared_ptr<aen::gl::sound::Player>>> g_listSoundPlayer;
vector<vector<boost::shared_ptr<aen::gl::Text>>> g_listText;


// 似非ガベージコレクション
void ScriptingPush(void)
{
	// なんだこの汚いコードは。
	g_listPicture.push_back(vector<boost::shared_ptr<aen::gl::Picture>>());
	g_listSoundPlayer.push_back(vector<boost::shared_ptr<aen::gl::sound::Player>>());
	g_listText.push_back(vector<boost::shared_ptr<aen::gl::Text>>());
}


void ScriptingPop(void)
{
	g_listPicture.pop_back();
	g_listSoundPlayer.pop_back();
	g_listText.pop_back();
}


// EvenShooter の関数をスクリプターに登録
void SetEvenFunc(aen::scripter::ScripterV8* scripter)
{
AEN_NULL_TO_THROW_EXCEPTION_PARAM(scripter);


	scripter->setFunction("EvConfig", V8_EvConfig);
	scripter->setFunction("EvLoadGraph", V8_EvLoadGraph);
	scripter->setFunction("EvDrawGraph", V8_EvDrawGraph);
	scripter->setFunction("EvSystem", V8_EvSystem);
	scripter->setFunction("EvChangeScene", V8_EvChangeScene);
	scripter->setFunction("EvSleep", V8_EvSleep);
	scripter->setFunction("EvScreenClear", V8_EvScreenClear);
	scripter->setFunction("EvScreenFlip", V8_EvScreenFlip);
}


// EvenShooter の設定。
ScripterV8::FuncRet V8_EvConfig(ScripterV8::FuncParam args)
{
	// 引数の長さチェック
	if (args.size() < 2) {
		// とりあえず引数が足りないときは undefined にする。
		return (v8::Undefined());
	}


	string key = ScripterV8::GetString(args.at(0));
	int value = ScripterV8::GetInt32(args.at(1));


	if (key == "debug") {
cout << "debug flag :";
cout << (value ? "TRUE" : "FALSE") << endl;
		// ログ出力いらんー
		aen::SetStreamLogLevel(value ? 0 : 1000);
	}


	return (v8::Undefined());
}


// 画像ハンドルの読み込み
// EvLoadGraph(url)
// EvLoadGraph(url, imax, imaxX, imaxY, imageOneW, imageOneH)
aen::scripter::ScripterV8::FuncRet V8_EvLoadGraph(aen::scripter::ScripterV8::FuncParam args)
{
	ScripterV8::FuncRet ret = v8::Undefined();
	// 引数の長さチェック
	if (args.size() < 1) {
cerr << "error >> V8_EvLoadGraph -> parameter is unjust." << endl;
		return (ret);
	}
	else if (args.size() != 1 && args.size() != 6) {
cerr << "error >> V8_EvLoadGraph -> parameter is unjust." << endl;
cerr << "\t" << "is not" << endl;
cerr << "\t" << "EvLoadGraph(url) or EvLoadGraph(url, imax, imaxX, imaxY, imageOneW, imageOneH)" << endl;
		return (ret);
	}


	aen::gl::Picture a;
	if (args.size() == 1) {
		if (!a.load(ScripterV8::GetString(args.at(0)).c_str())) {
cerr << "error >> V8_EvLoadGraph -> can not load file." << endl;
			return (ret);
		}
	}
	else if (args.size() == 6) {
		if (!a.load(
			ScripterV8::GetString(args.at(0)).c_str(),
			ScripterV8::GetInt32(args.at(1)),
			ScripterV8::GetInt32(args.at(2)),
			ScripterV8::GetInt32(args.at(3)),
			ScripterV8::GetInt32(args.at(4)),
			ScripterV8::GetInt32(args.at(5)))
		) {
cerr << "error >> V8_EvLoadGraph -> can not load file." << endl;
			return (ret);
		}
	}
	else {
		// 私はこのエラーメッセージを出した人間です・・・orz
cerr << "error >> V8_EvLoadGraph -> parameter is unjust. why... this is impossible !!!!" << endl;
		return (ret);
	}


	// MEMO:
	// メモリ馬鹿食いになるが、
	// 保持しとかんと・・・
	// ガベージコレクションとか作りたい・・・
	boost::shared_ptr<aen::gl::Picture> handle(new aen::gl::Picture(a));
	g_listPicture.at(g_listPicture.size() - 1).push_back(handle);


// TODO:
// 2012 / 05 / 02
// jskny
// 描画可能にしろ。
// cout << "LOAD ED ;" << ScripterV8::GetString(args.at(0)) << endl;


	// 配列の初期化きもちわるいよ・・・
	v8::Local<v8::Array> buf = v8::Array::New(a.getSize());
	for (int i = 0; i < buf->Length(); ++i) {
		buf->Set(v8::Int32::New(i), v8::Int32::New(a.getHandle(i)));
	}


	ret = buf;
	return (ret);
}


// 描画！
// EvDrawGraph(x, y, handle)
// EvDrawGraph(x, y, handle, angle, extend)
aen::scripter::ScripterV8::FuncRet V8_EvDrawGraph(aen::scripter::ScripterV8::FuncParam args)
{
	ScripterV8::FuncRet ret = v8::Undefined();
	// 引数の長さチェック
	if (args.size() < 3) {
cerr << "error >> V8_EvDrawGraph -> parameter is unjust." << endl;
		return (ret);
	}
	else if (args.size() != 3 && args.size() != 5) {
cerr << "error >> V8_EvDrawGraph -> parameter is unjust." << endl;
cerr << "\t" << "is not" << endl;
cerr << "\t" << "V8_EvDrawGraph(x, y, handle) or V8_EvDrawGraph(x, y, handle, angle, extend)" << endl;
		return (ret);
	}


	if (args.size() == 3) {
		DrawGraph(ScripterV8::GetInt32(args.at(0)), ScripterV8::GetInt32(args.at(1)), ScripterV8::GetInt32(args.at(2)), TRUE);
	}
	else if (args.size() == 5) {
		DrawRotaGraph(
			ScripterV8::GetInt32(args.at(0)), ScripterV8::GetInt32(args.at(1)), ScripterV8::GetInt32(args.at(2)),
			ScripterV8::GetInt32(args.at(3)), ScripterV8::GetInt32(args.at(4)), TRUE);
	}


	return (ret);
}


// エンジン側に処理を戻す。
// EvSystem
aen::scripter::ScripterV8::FuncRet V8_EvSystem(aen::scripter::ScripterV8::FuncParam args)
{
	ScripterV8::FuncRet ret = v8::Undefined();
	// 引数の長さチェック
	if (args.size() != 0) {
cerr << "error >> V8_EvSystem -> parameter is unjust." << endl;
		return (ret);
	}


	ret = v8::Int32::New((EvenShooter->systemLoop() ? 1 : 0));
	return (ret);
}


// シーンを変える。
// EvChangeScene(name)
aen::scripter::ScripterV8::FuncRet V8_EvChangeScene(aen::scripter::ScripterV8::FuncParam args)
{
	ScripterV8::FuncRet ret = v8::Undefined();
	// 引数の長さチェック
	if (args.size() != 1) {
cerr << "error >> V8_EvChangeScene -> parameter is unjust." << endl;
		return (ret);
	}


	// 画面変更。
	EvenShooter->setScene(ScripterV8::GetString(args.at(0)).c_str());
	// のち実行。
	EvenShooter->run();
	return (ret);
}


// Sleep
// EvSleep(time)
aen::scripter::ScripterV8::FuncRet V8_EvSleep(aen::scripter::ScripterV8::FuncParam args)
{
	ScripterV8::FuncRet ret = v8::Undefined();
	// 引数の長さチェック
	if (args.size() != 1) {
cerr << "error >> V8_EvSystem -> parameter is unjust." << endl;
		return (ret);
	}


	Sleep(ScripterV8::GetInt32(args.at(0)));
	return (ret);
}


// 画面消去
aen::scripter::ScripterV8::FuncRet V8_EvScreenClear(aen::scripter::ScripterV8::FuncParam args)
{
	ClsDrawScreen();
	return (v8::Undefined());
}


// 画面更新
aen::scripter::ScripterV8::FuncRet V8_EvScreenFlip(aen::scripter::ScripterV8::FuncParam args)
{
	ScreenFlip();
	return (v8::Undefined());
}

