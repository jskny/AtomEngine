/*!
 *	AtomEngine
 *	スクリプター
 *	@brief		スクリプト関連。
 *	@date		2012 / 04 / 08
 *	@author		jskny
*/


#include	"def.h"


#ifdef AEN_USE_SCRIPTER_JAVASCRIPT


#include	<iostream>
#include	<sstream>
#include	<fstream>
#include	<string>


#include	<vector>
#include	<map>


#include	<v8.h>


#include	"aen_scripter.h"
#include	"aen_util.h"


using namespace std;


namespace aen
{
/// スクリプト言語関連
namespace scripter
{


// Call 関数で呼ぶ。
// あとから追加はできないから裏ワザかな・・・
class V8NativeFunc
{
public:
	ScripterV8::FuncRet(*ptr)(ScripterV8::FuncParam);


};


map<string, V8NativeFunc> g_mapNativeFunc;


// オブジェクトを UTF-8 文字列に変換する。
string ObjectToStringUTF8(v8::Local<v8::Value> value)
{
	v8::String::Utf8Value utf8_value(value);
	return (string(*utf8_value));
}


// オブジェクトを ASCII 文字列に変換する。
string ObjectToStringASCII(v8::Local<v8::Value> value)
{
	v8::String::AsciiValue ascii(value);
	// 元の文字列が UTF-16 なので
	// UTF-16 の一文字のバイト数が 4 バイトだから。
	int bufSize = ascii.length() * 4;
	if (bufSize == 0) {
		return (string(""));
	}


	char* buf = new char[bufSize];
	ZeroMemory(buf, bufSize);
	value->ToString()->WriteUtf8(buf, bufSize);


	// UTF-8 を SHiftJis に変換。
	int nSize = 0;
	util::ConvUtf8toSJis(buf, NULL, &nSize );
	char* pDist = new char[ nSize + 1 ];
	ZeroMemory( pDist, nSize + 1 );
	util::ConvUtf8toSJis(buf, pDist, &nSize );


	// 結果。
	string str = pDist;
	delete[] (pDist);
	delete[] (buf);


	return (str);
}


// http://www.codeproject.com/Articles/29109/Using-V8-Google-s-Chrome-JavaScript-Virtual-Machin
// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called. Prints its arguments on stdout separated by
// spaces and ending with a newline.
static v8::Handle<v8::Value> V8_Print(const v8::Arguments& args)
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope;
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}

		// convert the args[i] type to normal char* string
		string str = ObjectToStringASCII(args[i]);
		printf("%s", str.c_str());
	}


//	printf("\n");
	// returning Undefined is the same as returning void...
	return (v8::Undefined());
}


// ファイルを読み込む。
static std::vector<std::string> LoadFileASCII(const char* const urlFile)
{
	if (!urlFile) {
AEN_ERROR("null pointer.");
AEN_THROW_EXCEPTION("null pointer.");
	}


	std::vector <std::string> retVec;
	ifstream ifs(urlFile, ios::in);


	if (!ifs) {
cerr << "can not open '" << urlFile << "'." << endl;
AEN_STREAM_LOG_ERROR() << "can not open '" << urlFile << "'." << endl;
		return (retVec);
	}


const	int bufSize = 0xFF*2;
	// メモリの確保
	char* buf = new char[bufSize];
	while (!ifs.eof()) {
		ifs.getline(buf, bufSize);
		if (strlen(buf) != 0) {
			retVec.push_back(buf);
		}


		// V8 は改行がないとおかしくなる。
		retVec.push_back("\n");
	}


	ifs.close();
	delete[] buf;
	return (retVec);
}


// 表に登録された
// 関数ポインタを呼び出す。
static v8::Handle<v8::Value> V8_Call(const v8::Arguments& args)
{
	v8::HandleScope handle_scope;
	v8::Handle<v8::Value> ret = v8::Undefined();


	if (args.Length() < 1) {
cerr << "V8_Call >> parameter is unjust." << endl;
AEN_STREAM_LOG_ERROR() << "parameter is unjust." << endl;
		return (v8::Undefined());
	}


	string key = ObjectToStringASCII(args[0]);
	auto itr = g_mapNativeFunc.find(key);
	if (itr == g_mapNativeFunc.end()) {
AEN_STREAM_LOG_ERROR() << "Native function '" << key << "' is not registry." << endl;
cerr << "V8_Call >> Native function '" << key << "' is not registry." << endl;
		return (v8::Undefined());
	}


	std::vector<v8::Local<v8::Value>> vec;
	for (int i = 1; i < args.Length(); i++) {
		vec.push_back(args[i]);
	}


	// 行ってらっしゃい。
	ret = (*itr->second.ptr)(vec);
	return (ret);
}


//---------------------------------------------------------
// スクリプター本体
//---------------------------------------------------------


// static に一つ作る。
class V8EngineContext
{
public:
// root スコープ
v8::Handle<v8::ObjectTemplate> g_globalV8;
v8::Persistent<v8::Context>	g_context;
bool g_flagInitRootV8;


	V8EngineContext() :
		g_flagInitRootV8(false)
	{
	}


	~V8EngineContext()
	{
		if (this->g_flagInitRootV8) {
			this->g_context.Dispose();
		}
	}


private:
	// static に作成する以外受付ないわ。
	V8EngineContext(const V8EngineContext&);
	void operator=(const V8EngineContext&);
	void* operator new(size_t size);
	void operator delete(void*, size_t);


};


V8EngineContext g_v8EngineContext;


// スクリプトの読み込み
// 成功 true
bool ScripterV8::load(const char* const urlFile)
{
	if (!urlFile) {
AEN_THROW_EXCEPTION("null pointer.");
	}


	string strSource;


	// UTF-8 以外の形式で保存したファイルだと、
	// 日本語の文字列で文字化けが発生する。
	// 一行にまとめないと行けない。
	auto buf = LoadFileASCII(urlFile);
	if (buf.empty()) {
AEN_STREAM_LOG_DEBUG(DEBUG_LOG_LEVEL_SCRIPTER) << "script file '" << urlFile << "' is empty." << endl;
		return (false);
	}


	for (auto itr = buf.begin(); itr != buf.end(); ++itr) {
		strSource += (*itr);
	}


	// Create a string containing the JavaScript source code.
	v8::Handle<v8::String> source = v8::String::New(strSource.c_str());
	// Compile the source code.
	this->m_script = v8::Script::Compile(source);


	this->m_flagLoad = true;
	return (true);
}


// 関数の登録
// 成功 true
bool ScripterV8::setFunction(const std::string& funcName, const FuncNative callback)
{
	AEN_ASSERT_KILL(callback, "null pointer.");
	if (!callback) {
		return (false);
	}


	V8NativeFunc a;
	a.ptr = callback;

	g_mapNativeFunc[funcName] = a;
//	g_v8EngineContext.g_globalV8->Set(v8::String::New(funcName.c_str()), FunctionTemplate::New(callback));
	return (true);
}


// 実行。
bool ScripterV8::run(void)
{
	if (!this->isLoad()) {
AEN_STREAM_LOG_ERROR() << "script file is not loaded." << endl;
		return (false);
	}


	v8::HandleScope handle_scope;
	this->m_script->Run();
	return (true);
}


// 初期化
void ScripterV8::clear(void)
{
	this->m_flagLoad = false;
	this->m_flagCreateContext = false;
}


ScripterV8::ScripterV8() :
	m_flagLoad(false), m_flagCreateContext(false),
	m_handleScopeV8(), m_scope(), m_script()
{
	if (!g_v8EngineContext.g_flagInitRootV8) {
		// Aen オブジェクトを作成。
		// これを、アクセサとして機能の追加。
v8::Handle<v8::FunctionTemplate> aenTemplate = v8::FunctionTemplate::New();
aenTemplate->SetClassName(v8::String::New("Aen"));


v8::Handle<v8::ObjectTemplate> aenPrototype = aenTemplate->PrototypeTemplate();
aenPrototype->Set("Print", v8::FunctionTemplate::New(V8_Print));
aenPrototype->Set("Call", v8::FunctionTemplate::New(V8_Call));


		g_v8EngineContext.g_globalV8 = v8::ObjectTemplate::New();
		// Aen オブジェクト。
		g_v8EngineContext.g_globalV8->Set(v8::String::New("Aen"), aenPrototype);


		// まぁ、楽に使いたいから残す。
		g_v8EngineContext.g_globalV8->Set(v8::String::New("Print"), v8::FunctionTemplate::New(V8_Print));


		g_v8EngineContext.g_context = v8::Context::New(NULL, g_v8EngineContext.g_globalV8);
		// Aen オブジェクトの登録。
//		g_context->Global()->Set(v8::String::New("Aen"), obj);


		this->m_scope.reset();
		// 実態のコピーは禁止されている・・・ポインタしかないじゃん。
		this->m_scope = boost::shared_ptr<v8::Context::Scope>(new v8::Context::Scope(g_v8EngineContext.g_context));

// MEMO:
// 2012 / 05 / 03
// jskny
// ２つオブジェクトを作ったらなんか落ちた。
// この行コメントアウトで動いているみたい。
//		g_v8EngineContext.g_flagInitRootV8 = true;
	}


}


} } // namespace aen::scripter


#endif // AEN_USE_SCRIPTER_JAVASCRIPT

